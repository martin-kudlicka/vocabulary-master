#include "vocabularymanagerdialog.h"

#include <QtGui/QInputDialog>
#include "vocabularymanagerdialog/vocabularymodel.h"
#include <QtGui/QTableView>
#include "vocabularymanagerdialog/vocabularysettingsdialog.h"
#ifndef FREE
# include "vocabularymanagerdialog/wordsimportdialog.h"
#endif
#include <QtGui/QFileDialog>

const char *PROPERTY_COLUMN = "FieldId";

const void VocabularyManagerDialog::AddTab(const int &pCategoryId)
{
    QTableView *qtvTableView = new QTableView(_qdvmVocabularyManager.vtwTabs);
	qtvTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    qtvTableView->setModel(new VocabularyModel(_vVocabulary, pCategoryId, qtvTableView));
	connect(qtvTableView->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)), SLOT(on_qtvTableViewSelectionModel_selectionChanged(const QItemSelection &, const QItemSelection &)));

    for (int iColumn = 0; iColumn < qtvTableView->horizontalHeader()->count(); iColumn++) {
	    qtvTableView->horizontalHeader()->setResizeMode(iColumn, QHeaderView::Stretch);
    } // for

    VocabularyTabWidget *vtwTabs = _qdvmVocabularyManager.vtwTabs;
    int iTab = vtwTabs->addTab(qtvTableView, _vVocabulary->GetCategoryName(pCategoryId)
#ifndef FREE
        , _vVocabulary->GetCategoryEnabled(pCategoryId)
#endif
        );
#ifndef FREE
    vtwTabs->setTabEnabled(iTab, _vVocabulary->GetCategoryEnabled(pCategoryId));
#endif
} // AddTab

const void VocabularyManagerDialog::InitEditor()
{
	int iFieldsLeft = 0;
	int iFieldsRight = 0;

	for (int iI = 0; iI < _vVocabulary->GetFieldCount(); iI++) {
		int iColumn, iRow;

		// get field language
		QGridLayout *qglGrid;
		if (_vVocabulary->GetFieldLanguage(iI) == Vocabulary::FieldLanguageLeft) {
			qglGrid = _qdvmVocabularyManager.qglLeft;
			iRow = iFieldsLeft++;
			iColumn = EditorColumnLeftLabel;
		} else {
			qglGrid = _qdvmVocabularyManager.qglRight;
			iRow = iFieldsRight++;
			iColumn = EditorColumnRightLabel;
		} // if else

		// label
		QLabel *qlLabel = new QLabel(_vVocabulary->GetFieldName(iI) + ':', _qdvmVocabularyManager.qgbEditor);
		qglGrid->addWidget(qlLabel, iRow, iColumn);

		// control
		QLineEdit *qleControl = new QLineEdit(_qdvmVocabularyManager.qgbEditor);
		qleControl->setProperty(PROPERTY_COLUMN, iI);
		connect(qleControl, SIGNAL(textEdited(const QString &)), SLOT(on_qleControl_textEdited(const QString &)));
		qglGrid->addWidget(qleControl, iRow, iColumn + 1);
	} // for
} // InitEditor

const void VocabularyManagerDialog::InitTabs()
{
    Vocabulary::tCategoryIdList tcilCategories = _vVocabulary->GetCategoryIds();

    Vocabulary::tCategoryIdList::const_iterator ciCategoryId;
    for (ciCategoryId = tcilCategories.constBegin(); ciCategoryId != tcilCategories.constEnd(); ciCategoryId++) {
        AddTab(*ciCategoryId);
        _qlCategories.append(*ciCategoryId);
    } // for
} // InitTabs

const void VocabularyManagerDialog::on_qleControl_textEdited(const QString &pText) const
{
	int iColumn = sender()->property(PROPERTY_COLUMN).toInt();

	const QTableView *qtvVocabularyView = qobject_cast<const QTableView *>(_qdvmVocabularyManager.vtwTabs->currentWidget());
	QModelIndex qmiCurrent = qtvVocabularyView->currentIndex();

	VocabularyModel *vmVocabularyModel = qobject_cast<VocabularyModel *>(qtvVocabularyView->model());
	vmVocabularyModel->setData(vmVocabularyModel->index(qmiCurrent.row(), iColumn), pText);
} // on_qleControl_textEdited

const void VocabularyManagerDialog::on_qleSearch_textChanged(const QString &text) const
{
    _qdvmVocabularyManager.qpbSearch->setEnabled(!text.isEmpty());
} // on_qleSearch_textChanged

const void VocabularyManagerDialog::on_qpbCategoryAdd_clicked(bool checked /* false */)
{
    QString qsCategory = QInputDialog::getText(this, tr("Add category"), tr("New category name"));
    if (!qsCategory.isEmpty()) {
        int iCategory = _vVocabulary->AddCategory(qsCategory);
		AddTab(iCategory);
        _qlCategories.append(iCategory);

		_qdvmVocabularyManager.vtwTabs->setCurrentIndex(_qdvmVocabularyManager.vtwTabs->count() - 1);
		_qdvmVocabularyManager.qpbWordAdd->setEnabled(true);
    } // if
} // on_qpbCategoryAdd_clicked

const void VocabularyManagerDialog::on_qpbCategoryRemove_clicked(bool checked /* false */)
{
    int iTab = _qdvmVocabularyManager.vtwTabs->currentIndex();
    _qdvmVocabularyManager.vtwTabs->removeTab(iTab);
    _vVocabulary->RemoveCategory(_qlCategories.takeAt(iTab));
} // on_qpbCategoryRemove_clicked

const void VocabularyManagerDialog::on_qpbSearch_clicked(bool checked /* false */) const
{
    // get current word id
    QTableView *qtvVocabularyView = qobject_cast<QTableView *>(_qdvmVocabularyManager.vtwTabs->currentWidget());
    const VocabularyModel *vmVocabularyModel = qobject_cast<const VocabularyModel *>(qtvVocabularyView->model());
    const QItemSelectionModel *qismSelection = qtvVocabularyView->selectionModel();
    int iCurrentRecord = _vVocabulary->GetRecordId(_qlCategories.at(_qdvmVocabularyManager.vtwTabs->currentIndex()), qismSelection->currentIndex().row());

    // search for next word
    int iRecordId = _vVocabulary->Search(_qdvmVocabularyManager.qleSearch->text(), iCurrentRecord + 1);
    if (iRecordId == Vocabulary::NOT_FOUND) {
        return;
    } // if

    // get found word category
    int iCategory = _vVocabulary->GetRecordCategory(iRecordId);

    // get tab for category
    int iTab;
    for (iTab = 0; iTab < _qdvmVocabularyManager.vtwTabs->count(); iTab++) {
        if (_qlCategories.at(iTab) == iCategory) {
            break;
        } // if
    } // for

    // switch tabs
    _qdvmVocabularyManager.vtwTabs->setCurrentIndex(iTab);

    // focus on word
    qtvVocabularyView = qobject_cast<QTableView *>(_qdvmVocabularyManager.vtwTabs->currentWidget());
    vmVocabularyModel = qobject_cast<const VocabularyModel *>(qtvVocabularyView->model());
    qtvVocabularyView->setCurrentIndex(vmVocabularyModel->index(_vVocabulary->GetRow(iRecordId, iCategory), 0));
} // on_qpbSearch_clicked

const void VocabularyManagerDialog::on_qpbVocabularySettings_clicked(bool checked /* false */)
{
	VocabularySettingsDialog vsdSettings(_vVocabulary,
#ifndef FREE
        _pPlugins,
#endif
        this);
	vsdSettings.exec();
} // on_qpbVocabularySettings_clicked

const void VocabularyManagerDialog::on_qpbWordAdd_clicked(bool checked /* false */)
{
	QTableView *qtvVocabularyView = qobject_cast<QTableView *>(_qdvmVocabularyManager.vtwTabs->currentWidget());
    VocabularyModel *vmVocabularyModel = qobject_cast<VocabularyModel *>(qtvVocabularyView->model());
    vmVocabularyModel->InsertRow(vmVocabularyModel->rowCount());

	qtvVocabularyView->setCurrentIndex(vmVocabularyModel->index(vmVocabularyModel->rowCount() - 1, 0));
	qtvVocabularyView->setFocus(Qt::OtherFocusReason);
} // on_qpbWordAdd_clicked

#ifndef FREE
const void VocabularyManagerDialog::on_qpbWordImport_clicked(bool checked /* false */)
{
	// prepare filter
	QStringList qslFilters;
	foreach (const ImpInterface *iiPlugin, _pPlugins->GetImpPlugins()) {
		qslFilters.append(iiPlugin->GetFilter());
	} // foreach

	QString qsFilter;
	QString qsFile = QFileDialog::getOpenFileName(this, tr("Import words"), QString(), qslFilters.join(";;"), &qsFilter);
	if (!qsFile.isEmpty()) {
		WordsImportDialog wiImport(qsFile, qsFilter, _vVocabulary, _pPlugins, this);
		wiImport.exec();
	} // if
} // on_qpbWordImport_clicked
#endif

const void VocabularyManagerDialog::on_qpbWordRemove_clicked(bool checked /* false */)
{
    const QTableView *qtvVocabularyView = qobject_cast<const QTableView *>(_qdvmVocabularyManager.vtwTabs->currentWidget());
    VocabularyModel *vmVocabularyModel = qobject_cast<VocabularyModel *>(qtvVocabularyView->model());
    const QItemSelectionModel *qismSelection = qtvVocabularyView->selectionModel();
    vmVocabularyModel->RemoveRow(qismSelection->currentIndex().row());
} // on_qpbWordRemove_clicked

const void VocabularyManagerDialog::on_qtvTableViewSelectionModel_selectionChanged(const QItemSelection &selected, const QItemSelection &deselected) const
{
	UpdateEditor();
} // on_qtvTableViewSelectionModel_selectionChanged

const void VocabularyManagerDialog::on_vtwTabs_currentChanged(int index) const
{
	UpdateEditor();
} // on_vtwTabs_currentChanged

#ifndef FREE
const void VocabularyManagerDialog::on_vtwTabs_TabEnableChanged(const int &pIndex, const Qt::CheckState &pState) const
{
    _vVocabulary->SetCategoryEnabled(_qlCategories.at(pIndex), pState);
    _qdvmVocabularyManager.vtwTabs->setTabEnabled(pIndex, pState);
} // on_vtwTabs_TabEnableChanged

const void VocabularyManagerDialog::SelectFirstEnabledTab()
{
    for (int iI = 0; iI < _qdvmVocabularyManager.vtwTabs->count(); iI++) {
        if (_qdvmVocabularyManager.vtwTabs->isTabEnabled(iI)) {
            _qdvmVocabularyManager.vtwTabs->setCurrentIndex(iI);
            return;
        } // if
    } // for
} // SelectFirstEnabledTab
#endif

const void VocabularyManagerDialog::UpdateEditor() const
{
	const QTableView *qtvVocabularyView = qobject_cast<const QTableView *>(_qdvmVocabularyManager.vtwTabs->currentWidget());
	const QItemSelectionModel *qismSelection;
	if (qtvVocabularyView) {
		qismSelection = qtvVocabularyView->selectionModel();
	} else {
		qismSelection = NULL;
	} // if else
	_qdvmVocabularyManager.qgbEditor->setEnabled(qismSelection && qismSelection->hasSelection());

	UpdateEditor(_qdvmVocabularyManager.qglLeft, EditorColumnLeftControl);
	UpdateEditor(_qdvmVocabularyManager.qglRight, EditorColumnRightControl);
} // UpdateEditor

const void VocabularyManagerDialog::UpdateEditor(const QGridLayout *pGridLayout, const eEditorColumn &pControlsColumn) const
{
	for (int iI = 0; iI <  pGridLayout->rowCount(); iI++) {
		QLayoutItem *qliItem = pGridLayout->itemAtPosition(iI, pControlsColumn);
		if (qliItem) {
			QWidget *qwWidget = qliItem->widget();
			QLineEdit *qleControl = qobject_cast<QLineEdit *>(qwWidget);

			const QTableView *qtvVocabularyView = qobject_cast<const QTableView *>(_qdvmVocabularyManager.vtwTabs->currentWidget());
			const QItemSelectionModel *qismSelection;
			if (qtvVocabularyView) {
				qismSelection = qtvVocabularyView->selectionModel();
			} else {
				qismSelection = NULL;
			} // if else

			if (qismSelection && qismSelection->hasSelection()) {
				int iTab = _qdvmVocabularyManager.vtwTabs->currentIndex();
				QModelIndex qmiIndex = qtvVocabularyView->currentIndex();
				int iColumn = qleControl->property(PROPERTY_COLUMN).toInt();
				qleControl->setText(_vVocabulary->GetDataText(_qlCategories.at(iTab), qmiIndex.row(), _vVocabulary->GetFieldId(iColumn)));
			} else {
				qleControl->clear();
			} // if else
		} // if
	} // for
} // UpdateEditor

VocabularyManagerDialog::VocabularyManagerDialog(const Vocabulary *pVocabulary,
#ifndef FREE
    const Plugins *pPlugins,
#endif
    QWidget *pParent /* NULL */, Qt::WindowFlags pFlags /* 0 */) : QDialog(pParent, pFlags)
{
    _vVocabulary = pVocabulary;
#ifndef FREE
	_pPlugins = pPlugins;
#endif

    _qdvmVocabularyManager.setupUi(this);
#ifdef FREE
    delete _qdvmVocabularyManager.qpbWordImport;
#endif

    InitTabs();
	InitEditor();
#ifndef FREE
    SelectFirstEnabledTab();
#endif

	_qdvmVocabularyManager.qpbWordAdd->setEnabled(_qdvmVocabularyManager.vtwTabs->currentWidget());

    connect(_qdvmVocabularyManager.vtwTabs, SIGNAL(TabEnableChanged(const int &, const Qt::CheckState &)), SLOT(on_vtwTabs_TabEnableChanged(const int &, const Qt::CheckState &)));
} // VocabularyManagerDialog