#include "vocabularymanagerdialog.h"

#include <QtGui/QInputDialog>
#include "vocabularymanagerdialog/vocabularymodel.h"
#include "vocabularymanagerdialog/vocabularyview.h"
#include "vocabularymanagerdialog/vocabularysettingsdialog.h"
#ifndef FREE
# include "vocabularymanagerdialog/wordsimportdialog.h"
#endif
#include <QtGui/QFileDialog>

const void VocabularyManagerDialog::AddTab(const int &pCategoryId)
{
    VocabularyView *vvTableView = new VocabularyView(_qdvmVocabularyManager.vtwTabs);
    vvTableView->setModel(new VocabularyModel(_vVocabulary, pCategoryId, vvTableView));

    for (int iColumn = 0; iColumn < VocabularyModel::ColumnCount; iColumn++) {
	    vvTableView->horizontalHeader()->setResizeMode(iColumn, QHeaderView::Stretch);
    } // for

    VocabularyTabWidget *vtwTabs = _qdvmVocabularyManager.vtwTabs;
    int iTab = vtwTabs->addTab(vvTableView, _vVocabulary->GetCategoryName(pCategoryId)
#ifndef FREE
        , _vVocabulary->GetCategoryEnabled(pCategoryId)
#endif
        );
#ifndef FREE
    vtwTabs->setTabEnabled(iTab, _vVocabulary->GetCategoryEnabled(pCategoryId));
#endif
} // AddTab

const void VocabularyManagerDialog::EnableControls()
{
    _qdvmVocabularyManager.qpbWordAdd->setEnabled(_qdvmVocabularyManager.vtwTabs->currentWidget());
} // EnableControls

const void VocabularyManagerDialog::InitTabs()
{
    Vocabulary::tCategoryIdList tcilCategories = _vVocabulary->GetCategoryIds();

    Vocabulary::tCategoryIdList::const_iterator ciCategoryId;
    for (ciCategoryId = tcilCategories.constBegin(); ciCategoryId != tcilCategories.constEnd(); ciCategoryId++) {
        AddTab(*ciCategoryId);
        _qlCategories.append(*ciCategoryId);
    } // for
} // InitTabs

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
		EnableControls();
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
    VocabularyView *vvVocabularyView = static_cast<VocabularyView *>(_qdvmVocabularyManager.vtwTabs->currentWidget());
    VocabularyModel *vmVocabularyModel = static_cast<VocabularyModel *>(vvVocabularyView->model());
    QItemSelectionModel *qismSelection = vvVocabularyView->selectionModel();
    int iCurrentWord = _vVocabulary->GetWordId(_qlCategories.at(_qdvmVocabularyManager.vtwTabs->currentIndex()), qismSelection->currentIndex().row());

    // search for next word
    int iWord = _vVocabulary->Search(_qdvmVocabularyManager.qleSearch->text(), iCurrentWord + 1);
    if (iWord == Vocabulary::SEARCH_NOT_FOUND) {
        return;
    } // if

    // get found word category
    int iCategory = _vVocabulary->GetWordCategory(iWord);

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
    vvVocabularyView = static_cast<VocabularyView *>(_qdvmVocabularyManager.vtwTabs->currentWidget());
    vmVocabularyModel = static_cast<VocabularyModel *>(vvVocabularyView->model());
    vvVocabularyView->setCurrentIndex(vmVocabularyModel->index(_vVocabulary->GetRow(iWord, iCategory), VocabularyModel::ColumnLang1));
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
	VocabularyView *vvVocabularyView = static_cast<VocabularyView *>(_qdvmVocabularyManager.vtwTabs->currentWidget());
    VocabularyModel *vmVocabularyModel = static_cast<VocabularyModel *>(vvVocabularyView->model());
    vmVocabularyModel->InsertRow(vmVocabularyModel->rowCount());

	vvVocabularyView->setCurrentIndex(vmVocabularyModel->index(vmVocabularyModel->rowCount() - 1, VocabularyModel::ColumnLang1));
	vvVocabularyView->setFocus(Qt::OtherFocusReason);
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
    VocabularyView *vvVocabularyView = static_cast<VocabularyView *>(_qdvmVocabularyManager.vtwTabs->currentWidget());
    VocabularyModel *vmVocabularyModel = static_cast<VocabularyModel *>(vvVocabularyView->model());
    QItemSelectionModel *qismSelection = vvVocabularyView->selectionModel();
    vmVocabularyModel->RemoveRow(qismSelection->currentIndex().row());
} // on_qpbWordRemove_clicked

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
    EnableControls();
#ifndef FREE
    SelectFirstEnabledTab();
#endif

    connect(_qdvmVocabularyManager.vtwTabs, SIGNAL(TabEnableChanged(const int &, const Qt::CheckState &)), SLOT(on_vtwTabs_TabEnableChanged(const int &, const Qt::CheckState &)));
} // VocabularyManagerDialog