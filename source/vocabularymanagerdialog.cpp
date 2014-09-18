#include "vocabularymanagerdialog.h"

#include <QtWidgets/QInputDialog>
#include "vocabularymanagerdialog/vocabularymodel.h"
#include "vocabularymanagerdialog/vocabularysettingsdialog.h"
#ifndef EDITION_FREE
# include "vocabularymanagerdialog/wordsimportdialog.h"
#endif
#include <QtWidgets/QFileDialog>
#ifndef EDITION_FREE
# include "common/vocabularyopenprogressdialog.h"
# include "vocabularymanagerdialog/wordsexportdialog.h"
# include "vocabularymanagerdialog/prioritydelegate.h"
# include "vocabularymanagerdialog/wordcopymovedialog.h"
#endif

const char *PROPERTY_COLUMN = "Column";

VocabularyManagerDialog::~VocabularyManagerDialog()
{
    _vVocabulary->endEdit();
} // ~VocabularyManagerDialog

const void VocabularyManagerDialog::AddTab(const int &pCategoryId)
{
    VocabularyView *vvVocabularyView = new VocabularyView(
#ifndef EDITION_FREE
		_vVocabulary,
#endif
		_qdvmVocabularyManager.vtwTabs);
	vvVocabularyView->setSelectionBehavior(QAbstractItemView::SelectRows);
    vvVocabularyView->setModel(new VocabularyModel(_vVocabulary, pCategoryId, vvVocabularyView));
    HideColumns(vvVocabularyView);
#ifndef EDITION_FREE
	SetPriorityDelegate(vvVocabularyView);
    vvVocabularyView->setEditTriggers(QAbstractItemView::AllEditTriggers);
#endif
	connect(vvVocabularyView->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)), SLOT(on_vvVocabularyViewSelectionModel_selectionChanged(const QItemSelection &, const QItemSelection &)));

    StretchColumns(vvVocabularyView);

    VocabularyTabWidget *vtwTabs = _qdvmVocabularyManager.vtwTabs;
    int iTab = vtwTabs->addTab(vvVocabularyView, _vVocabulary->GetCategoryName(pCategoryId)
#ifndef EDITION_FREE
        , _vVocabulary->GetCategoryEnabled(pCategoryId), _vVocabulary->GetCategoryPriority(pCategoryId)
#endif
        );
#ifndef EDITION_FREE
    vtwTabs->setTabEnabled(iTab, _vVocabulary->GetCategoryEnabled(pCategoryId));
#endif
} // AddTab

const void VocabularyManagerDialog::EnableTabControls() const
{
	_qdvmVocabularyManager.qpbCategoryRemove->setEnabled(_qdvmVocabularyManager.vtwTabs->currentWidget());
	_qdvmVocabularyManager.qpbWordAdd->setEnabled(_qdvmVocabularyManager.vtwTabs->currentWidget() && _qdvmVocabularyManager.vtwTabs->isTabEnabled(_qdvmVocabularyManager.vtwTabs->currentIndex()));

#ifndef EDITION_FREE
    _qdvmVocabularyManager.qpbWordImport->setEnabled(_qdvmVocabularyManager.vtwTabs->currentWidget());
    _qdvmVocabularyManager.qpbWordExport->setEnabled(_qdvmVocabularyManager.vtwTabs->currentWidget());
#endif
} // EnableTabControls

const void VocabularyManagerDialog::EnableWordControls() const
{
	const VocabularyView *qtvVocabularyView = qobject_cast<const VocabularyView *>(_qdvmVocabularyManager.vtwTabs->currentWidget());
	const QItemSelectionModel *qismSelection;
	if (qtvVocabularyView) {
		qismSelection = qtvVocabularyView->selectionModel();
	} else {
		qismSelection = NULL;
	} // if else

	_qdvmVocabularyManager.qpbWordRemove->setEnabled(qismSelection && qtvVocabularyView->isEnabled() && qismSelection->hasSelection());
#ifndef EDITION_FREE
	_qdvmVocabularyManager.qpbWordCopyMove->setEnabled(qismSelection && qtvVocabularyView->isEnabled() && qismSelection->hasSelection());
#endif
} // EnableWordControls

#ifndef EDITION_FREE
int VocabularyManagerDialog::ExecOnRecord(const int &pRecordId)
{
    FocusOnRecord(pRecordId);
    return exec();
} // ExecOnRecord
#endif

const void VocabularyManagerDialog::FocusOnRecord(const int &pRecordId) const
{
    // get found word category
    int iCategory = _vVocabulary->recordCategory(pRecordId);

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
    VocabularyView *qtvVocabularyView = qobject_cast<VocabularyView *>(_qdvmVocabularyManager.vtwTabs->currentWidget());
    const VocabularyModel *vmVocabularyModel = qobject_cast<const VocabularyModel *>(qtvVocabularyView->model());
    qtvVocabularyView->setCurrentIndex(vmVocabularyModel->index(_vVocabulary->GetRow(pRecordId, iCategory), 0));
} // FocusOnRecord

#ifndef EDITION_FREE
const void VocabularyManagerDialog::HideColumns() const
{
	for (int iTab = 0; iTab < _qdvmVocabularyManager.vtwTabs->count(); iTab++) {
		VocabularyView *qtvVocabularyView = qobject_cast<VocabularyView *>(_qdvmVocabularyManager.vtwTabs->widget(iTab));
		HideColumns(qtvVocabularyView);
	} // for
} // HideColumns
#endif

const void VocabularyManagerDialog::HideColumns(VocabularyView *pTableView) const
{
	int iColumn = 0;
	foreach (int iFieldId, _vVocabulary->fieldIds()) {
#ifdef EDITION_FREE
		if (!_vVocabulary->fieldHasAttribute(iFieldId, VocabularyDatabase::FieldAttributeBuiltIn)) {
#else
		if (_vVocabulary->fieldHasAttribute(iFieldId, VocabularyDatabase::FieldAttributeShow)) {
#endif
			pTableView->showColumn(iColumn);
		} else {
			pTableView->hideColumn(iColumn);
		} // if else

		iColumn++;
	} // foreach
} // HideColumns

const void VocabularyManagerDialog::InitEditor()
{
	int iFieldsLeft = 0;
	int iFieldsRight = 0;
    int iPosLeft = 0;
    int iPosRight = 0;

	foreach (int iFieldId, _vVocabulary->fieldIds()) {
		int iColumn, iRow;

        // update field count
        if (_vVocabulary->fieldLanguage(iFieldId) == VocabularyDatabase::FieldLanguageLeft) {
            iFieldsLeft++;
        } else {
            iFieldsRight++;
        } // if else

		// check if visible or builtin field
		VocabularyDatabase::FieldAttributes qfaAttributes = _vVocabulary->fieldAttributes(iFieldId);
        if (
#ifndef EDITION_FREE
			!(qfaAttributes & VocabularyDatabase::FieldAttributeShow) ||
#endif
			qfaAttributes & VocabularyDatabase::FieldAttributeBuiltIn) {
			continue;
		} // if

        // get field language
        if (_vVocabulary->fieldLanguage(iFieldId) == VocabularyDatabase::FieldLanguageLeft) {
            iRow = iPosLeft++;
            iColumn = EditorColumnLeftLabel;
        } else {
            iRow = iPosRight++;
            iColumn = EditorColumnRightLabel;
        } // if else

		// label
		QLabel *qlLabel = new QLabel(_vVocabulary->fieldName(iFieldId) + ':', _qdvmVocabularyManager.qgbEditor);
		_qdvmVocabularyManager.qglEditor->addWidget(qlLabel, iRow, iColumn);

		// control
		QLineEdit *qleControl = new QLineEdit(_qdvmVocabularyManager.qgbEditor);
		qleControl->setProperty(PROPERTY_COLUMN, iFieldsLeft + iFieldsRight - 1);
		connect(qleControl, SIGNAL(textEdited(const QString &)), SLOT(on_qleControl_textEdited(const QString &)));
		_qdvmVocabularyManager.qglEditor->addWidget(qleControl, iRow, iColumn + 1);
	} // foreach
} // InitEditor

const void VocabularyManagerDialog::InitTabs()
{
    VocabularyTabWidget *vtwTabs = _qdvmVocabularyManager.vtwTabs;
#ifndef EDITION_FREE
    vtwTabs->SetShowEnabled(_sSettings->canEnableCategories());
	vtwTabs->SetShowPriorities(_sSettings->canChangeCategoryPriority());
#endif

    VocabularyDatabase::CategoryIdList tcilCategories = _vVocabulary->categoryIds();
    VocabularyDatabase::CategoryIdList::const_iterator ciCategoryId;
    for (ciCategoryId = tcilCategories.constBegin(); ciCategoryId != tcilCategories.constEnd(); ciCategoryId++) {
        AddTab(*ciCategoryId);
        _qlCategories.append(*ciCategoryId);
    } // for
} // InitTabs

const void VocabularyManagerDialog::on_qleControl_textEdited(const QString &pText) const
{
	int iColumn = sender()->property(PROPERTY_COLUMN).toInt();

	const VocabularyView *qtvVocabularyView = qobject_cast<const VocabularyView *>(_qdvmVocabularyManager.vtwTabs->currentWidget());
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
        int iCategory = _vVocabulary->addCategory(qsCategory);
		AddTab(iCategory);
        _qlCategories.append(iCategory);

		_qdvmVocabularyManager.vtwTabs->setCurrentIndex(_qdvmVocabularyManager.vtwTabs->count() - 1);
		EnableTabControls();
    } // if
} // on_qpbCategoryAdd_clicked

const void VocabularyManagerDialog::on_qpbCategoryRemove_clicked(bool checked /* false */)
{
    int iTab = _qdvmVocabularyManager.vtwTabs->currentIndex();
    _qdvmVocabularyManager.vtwTabs->removeTab(iTab);
    _vVocabulary->removeCategory(_qlCategories.takeAt(iTab));

	EnableTabControls();
} // on_qpbCategoryRemove_clicked

const void VocabularyManagerDialog::on_qpbSearch_clicked(bool checked /* false */) const
{
    // get current word id
    VocabularyView *qtvVocabularyView = qobject_cast<VocabularyView *>(_qdvmVocabularyManager.vtwTabs->currentWidget());
    const VocabularyModel *vmVocabularyModel = qobject_cast<const VocabularyModel *>(qtvVocabularyView->model());
    const QItemSelectionModel *qismSelection = qtvVocabularyView->selectionModel();
    int iCurrentRecord = _vVocabulary->recordId(_qlCategories.at(_qdvmVocabularyManager.vtwTabs->currentIndex()), qismSelection->currentIndex().row());

    // search for next word
    int iRecordId = _vVocabulary->Search(_qdvmVocabularyManager.qleSearch->text(), iCurrentRecord + 1);
    if (iRecordId == VocabularyDatabase::NOT_FOUND) {
        return;
    } // if

    FocusOnRecord(iRecordId);
} // on_qpbSearch_clicked

const void VocabularyManagerDialog::on_qpbVocabularySettings_clicked(bool checked /* false */)
{
	VocabularySettingsDialog vsdSettings(_vVocabulary,
#ifndef EDITION_FREE
        _pPlugins,
#endif
        this);

#ifndef EDITION_FREE
	int iOldColumnCount = _vVocabulary->fieldCount();
#endif

    _vVocabulary->endEdit();
    _vVocabulary->beginEdit();

    if (vsdSettings.exec() == QDialog::Accepted) {
#ifndef EDITION_FREE
		if (iOldColumnCount != _vVocabulary->fieldCount()) {
			ReassignModels();
		} // if
        SetPriorityDelegate();
        StretchColumns();
        UninitEditor();
        InitEditor();
        UpdateEditor();
		HideColumns();
#endif
	} else {
        _vVocabulary->endEdit(false);
        _vVocabulary->beginEdit();
    } // if else
} // on_qpbVocabularySettings_clicked

const void VocabularyManagerDialog::on_qpbWordAdd_clicked(bool checked /* false */)
{
	VocabularyView *qtvVocabularyView = qobject_cast<VocabularyView *>(_qdvmVocabularyManager.vtwTabs->currentWidget());
    VocabularyModel *vmVocabularyModel = qobject_cast<VocabularyModel *>(qtvVocabularyView->model());
    vmVocabularyModel->AddRow();

	qtvVocabularyView->setCurrentIndex(vmVocabularyModel->index(vmVocabularyModel->rowCount() - 1, 0));
	qtvVocabularyView->setFocus(Qt::OtherFocusReason);

    if (!_qdvmVocabularyManager.qglEditor->isEmpty()) {
        QLayoutItem *qliControl = _qdvmVocabularyManager.qglEditor->itemAtPosition(0, EditorColumnLeftControl);
        qliControl->widget()->setFocus(Qt::OtherFocusReason);
    } // if
} // on_qpbWordAdd_clicked

#ifndef EDITION_FREE
const void VocabularyManagerDialog::on_qpbWordCopyMove_clicked(bool checked /* false */)
{
	// get selected records
	WordsCopyMoveDialog::tRowNumList trnlRowNums;
	const VocabularyView *qtvVocabularyView = qobject_cast<const VocabularyView *>(_qdvmVocabularyManager.vtwTabs->currentWidget());
	const VocabularyModel *vmVocabularyModel = qobject_cast<const VocabularyModel *>(qtvVocabularyView->model());
	const QItemSelectionModel *qismSelection = qtvVocabularyView->selectionModel();
	foreach (QModelIndex qmiIndex, qismSelection->selectedRows()) {
		trnlRowNums.append(qmiIndex.row());
	} // foreach

	// copy/move dialog
	WordsCopyMoveDialog wcmdCopyMove(_qlCategories.at(_qdvmVocabularyManager.vtwTabs->currentIndex()), trnlRowNums, _vVocabulary, this);
    if (wcmdCopyMove.exec() == QDialog::Accepted) {
        ReassignModels();
        StretchColumns();
        HideColumns();
    } // if
} // on_qpbWordCopyMove_clicked

const void VocabularyManagerDialog::on_qpbWordExport_clicked(bool checked /* false */)
{
	WordsExportDialog wedExport(_vVocabulary, _pPlugins->explugins(), this);
	wedExport.exec();
} // on_qpbWordExport_clicked

const void VocabularyManagerDialog::on_qpbWordImport_clicked(bool checked /* false */)
{
	// prepare filter
	QStringList qslFilters;
    foreach (Plugins::ImpPlugin sipPlugin, _pPlugins->impPlugins()) {
        const ImpInterface *iiPlugin = sipPlugin.impInterface;
		qslFilters.append(iiPlugin->filter());
	} // foreach

	QString qsFilter;
	QString qsFile = QFileDialog::getOpenFileName(this, tr("Import words"), QString(), qslFilters.join(";;"), &qsFilter);
	if (!qsFile.isEmpty()) {
        int iFilter = qslFilters.indexOf(qsFilter);
        ImpInterface *iiPlugin = _pPlugins->impPlugins().at(iFilter).impInterface;
		WordsImportDialog widImport(qsFile, _vVocabulary, iiPlugin, this);

		_vVocabulary->endEdit();
		_vVocabulary->beginEdit();

        if (widImport.exec() == QDialog::Accepted) {
            _vVocabulary->endEdit();

            VocabularyOpenProgressDialog vopdOpenProgress(_vVocabulary, this);
            vopdOpenProgress.show();
			_vVocabulary->close();
# ifdef EDITION_TRY
            _vVocabulary->openMemory();
# else
            _vVocabulary->open(_vVocabulary->GetVocabularyFile());
# endif
            vopdOpenProgress.hide();

            _vVocabulary->beginEdit();
            ReassignModels();
            StretchColumns();
			HideColumns();
		} else {
			_vVocabulary->endEdit(false);
			_vVocabulary->beginEdit();
		} // if else
	} // if
} // on_qpbWordImport_clicked
#endif

const void VocabularyManagerDialog::on_qpbWordRemove_clicked(bool checked /* false */)
{
    const VocabularyView *qtvVocabularyView = qobject_cast<const VocabularyView *>(_qdvmVocabularyManager.vtwTabs->currentWidget());
    VocabularyModel *vmVocabularyModel = qobject_cast<VocabularyModel *>(qtvVocabularyView->model());
    const QItemSelectionModel *qismSelection = qtvVocabularyView->selectionModel();
    vmVocabularyModel->RemoveRow(qismSelection->currentIndex().row());
} // on_qpbWordRemove_clicked

const void VocabularyManagerDialog::on_vtwTabs_currentChanged(int index) const
{
	UpdateEditor();
	EnableWordControls();
} // on_vtwTabs_currentChanged

#ifndef EDITION_FREE
const void VocabularyManagerDialog::on_vtwTabs_TabEnableChanged(const int &pIndex, const Qt::CheckState &pState) const
{
    _vVocabulary->SetCategoryEnabled(_qlCategories.at(pIndex), pState);
    _qdvmVocabularyManager.vtwTabs->setTabEnabled(pIndex, pState);

	EnableTabControls();
	EnableWordControls();
} // on_vtwTabs_TabEnableChanged

const void VocabularyManagerDialog::on_vtwTabs_TabPriorityChanged(const int &pIndex, const int &pValue) const
{
	_vVocabulary->SetCategoryPriority(_qlCategories.at(pIndex), pValue);
} // on_vtwTabs_TabPriorityChanged
#endif

const void VocabularyManagerDialog::on_vvVocabularyViewSelectionModel_selectionChanged(const QItemSelection &selected, const QItemSelection &deselected) const
{
	const VocabularyView *qtvCurrentTabView = qobject_cast<const VocabularyView *>(_qdvmVocabularyManager.vtwTabs->widget(_qdvmVocabularyManager.vtwTabs->currentIndex()));
	const VocabularyView *qtvVocabularyView = qobject_cast<const VocabularyView *>(_qdvmVocabularyManager.vtwTabs->currentWidget());
	if (qtvCurrentTabView != qtvVocabularyView) {
		// tab switching in progress
		return;
	} // if

    UpdateEditor();
    EnableWordControls();
} // on_vvVocabularyViewSelectionModel_selectionChanged

#ifndef EDITION_FREE
const void VocabularyManagerDialog::ReassignModels() const
{
    for (int iTab = 0; iTab < _qdvmVocabularyManager.vtwTabs->count(); iTab++) {
        VocabularyView *qtvVocabularyView = qobject_cast<VocabularyView *>(_qdvmVocabularyManager.vtwTabs->widget(iTab));
        VocabularyModel *vmVocabularyModel = qobject_cast<VocabularyModel *>(qtvVocabularyView->model());
        qtvVocabularyView->setModel(NULL);
        qtvVocabularyView->setModel(vmVocabularyModel);
        connect(qtvVocabularyView->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)), SLOT(on_vvVocabularyViewSelectionModel_selectionChanged(const QItemSelection &, const QItemSelection &)));
    } // for
} // ReassignModels

const void VocabularyManagerDialog::SelectFirstEnabledTab()
{
    for (int iI = 0; iI < _qdvmVocabularyManager.vtwTabs->count(); iI++) {
        if (_qdvmVocabularyManager.vtwTabs->isTabEnabled(iI)) {
            _qdvmVocabularyManager.vtwTabs->setCurrentIndex(iI);
            return;
        } // if
    } // for
} // SelectFirstEnabledTab

const void VocabularyManagerDialog::SetPriorityDelegate()
{
	for (int iTab = 0; iTab < _qdvmVocabularyManager.vtwTabs->count(); iTab++) {
		VocabularyView *qtvVocabularyView = qobject_cast<VocabularyView *>(_qdvmVocabularyManager.vtwTabs->widget(iTab));
		SetPriorityDelegate(qtvVocabularyView);
	} // for
} // SetPriorityDelegate

const void VocabularyManagerDialog::SetPriorityDelegate(VocabularyView *pTableView)
{
	for (int iColumn = 0; iColumn < pTableView->horizontalHeader()->count(); iColumn++) {
		int iFieldId = _vVocabulary->fieldId(iColumn);
		if (_vVocabulary->fieldHasAttribute(iFieldId, VocabularyDatabase::FieldAttributeBuiltIn)) {
			VocabularyDatabase::FieldBuiltIn efbBuiltIn = _vVocabulary->fieldBuiltIn(iFieldId);
			if (efbBuiltIn == VocabularyDatabase::FieldBuiltInPriority) {
				PriorityDelegate *sbpdPriorityDelegate = new PriorityDelegate(pTableView);
				pTableView->setItemDelegateForColumn(iColumn, sbpdPriorityDelegate);
            } else {
                pTableView->setItemDelegateForColumn(iColumn, NULL);
            } // if else
        } else {
            pTableView->setItemDelegateForColumn(iColumn, NULL);
        } // if else
	} // for
} // SetPriorityDelegate

const void VocabularyManagerDialog::StretchColumns() const
{
    for (int iTab = 0; iTab < _qdvmVocabularyManager.vtwTabs->count(); iTab++) {
        const VocabularyView *qtvVocabularyView = qobject_cast<const VocabularyView *>(_qdvmVocabularyManager.vtwTabs->widget(iTab));
        StretchColumns(qtvVocabularyView);
    } // for
} // StretchColumns
#endif

const void VocabularyManagerDialog::StretchColumns(const VocabularyView *pTableView) const
{
	for (int iColumn = 0; iColumn < pTableView->horizontalHeader()->count(); iColumn++) {
#ifndef EDITION_FREE
		int iFieldId = _vVocabulary->fieldId(iColumn);
		if (_vVocabulary->fieldHasAttribute(iFieldId, VocabularyDatabase::FieldAttributeBuiltIn)) {
			pTableView->horizontalHeader()->setSectionResizeMode(iColumn, QHeaderView::Fixed);
            pTableView->horizontalHeader()->resizeSection(iColumn, BUILTIN_COLUMN_SIZE);
		} else {
#endif
			pTableView->horizontalHeader()->setSectionResizeMode(iColumn, QHeaderView::Stretch);
#ifndef EDITION_FREE
		} // if else
#endif
	} // for
} // StretchColumns

#ifndef EDITION_FREE
const void VocabularyManagerDialog::UninitEditor() const
{
    QLayoutItem *qliItem;
    while ((qliItem = _qdvmVocabularyManager.qglEditor->takeAt(0)) != NULL) {
        qliItem->widget()->deleteLater();
    } // while
} // UninitEditor
#endif

const void VocabularyManagerDialog::UpdateEditor() const
{
	const VocabularyView *qtvVocabularyView = qobject_cast<const VocabularyView *>(_qdvmVocabularyManager.vtwTabs->currentWidget());
	const QItemSelectionModel *qismSelection;
	if (qtvVocabularyView) {
		qismSelection = qtvVocabularyView->selectionModel();
	} else {
		qismSelection = NULL;
	} // if else
	_qdvmVocabularyManager.qgbEditor->setEnabled(qismSelection && qismSelection->hasSelection());

	UpdateEditor(EditorColumnLeftControl);
	UpdateEditor(EditorColumnRightControl);
} // UpdateEditor

const void VocabularyManagerDialog::UpdateEditor(const eEditorColumn &pControlsColumn) const
{
	for (int iI = 0; iI <  _qdvmVocabularyManager.qglEditor->rowCount(); iI++) {
		QLayoutItem *qliItem = _qdvmVocabularyManager.qglEditor->itemAtPosition(iI, pControlsColumn);
		if (qliItem) {
			QWidget *qwWidget = qliItem->widget();
			QLineEdit *qleControl = qobject_cast<QLineEdit *>(qwWidget);

			const VocabularyView *qtvVocabularyView = qobject_cast<const VocabularyView *>(_qdvmVocabularyManager.vtwTabs->currentWidget());
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
				qleControl->setText(_vVocabulary->dataText(_qlCategories.at(iTab), qmiIndex.row(), _vVocabulary->fieldId(iColumn)));
			} else {
				qleControl->clear();
			} // if else
		} // if
	} // for
} // UpdateEditor

VocabularyManagerDialog::VocabularyManagerDialog(Vocabulary *pVocabulary,
#ifndef EDITION_FREE
    const Settings *pSettings,
    const Plugins *pPlugins,
#endif
    QWidget *pParent /* NULL */, Qt::WindowFlags pFlags /* 0 */) : QDialog(pParent, pFlags | Qt::WindowMaximizeButtonHint)
{
    _vVocabulary = pVocabulary;
#ifndef EDITION_FREE
    _sSettings = pSettings;
	_pPlugins = pPlugins;
#endif

    _qdvmVocabularyManager.setupUi(this);
#ifdef EDITION_FREE
	delete _qdvmVocabularyManager.qpbWordCopyMove;
    delete _qdvmVocabularyManager.qpbWordImport;
	delete _qdvmVocabularyManager.qpbWordExport;
#endif

	setWindowTitle(windowTitle() + " - " + pVocabulary->GetName());

    InitTabs();
	InitEditor();
#ifndef EDITION_FREE
    SelectFirstEnabledTab();
#endif

	EnableTabControls();

#ifndef EDITION_FREE
    connect(_qdvmVocabularyManager.vtwTabs, SIGNAL(TabEnableChanged(const int &, const Qt::CheckState &)), SLOT(on_vtwTabs_TabEnableChanged(const int &, const Qt::CheckState &)));
	connect(_qdvmVocabularyManager.vtwTabs, SIGNAL(TabPriorityChanged(const int &, const int &)), SLOT(on_vtwTabs_TabPriorityChanged(const int &, const int &)));
#endif

    pVocabulary->beginEdit();
} // VocabularyManagerDialog