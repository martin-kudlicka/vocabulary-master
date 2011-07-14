#include "vocabularymanagerdialog.h"

#include <QtGui/QInputDialog>
#include "vocabularymanagerdialog/vocabularymodel.h"
#include "vocabularymanagerdialog/vocabularyview.h"
#include "vocabularymanagerdialog/vocabularysettingsdialog.h"

const void VocabularyManagerDialog::AddTab(const int &pCategoryId)
{
    VocabularyView *vvTableView = new VocabularyView(_qdvmVocabularyManager.vtwTabs);
    vvTableView->setModel(new VocabularyModel(_vVocabulary, pCategoryId, vvTableView));

    for (int iColumn = 0; iColumn < VocabularyModel::ColumnCount; iColumn++) {
	    vvTableView->horizontalHeader()->setResizeMode(iColumn, QHeaderView::Stretch);
    } // for

    VocabularyTabWidget *vtwTabs = _qdvmVocabularyManager.vtwTabs;
    int iTab = vtwTabs->addTab(vvTableView, _vVocabulary->GetCategoryName(pCategoryId), _vVocabulary->GetCategoryEnabled(pCategoryId));
    vtwTabs->setTabEnabled(iTab, _vVocabulary->GetCategoryEnabled(pCategoryId));
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

const void VocabularyManagerDialog::on_qpbVocabularySettings_clicked(bool checked /* false */)
{
	VocabularySettingsDialog vsdSettings(_vVocabulary, _pPlugins, this);
	vsdSettings.exec();
} // on_qpbVocabularySettings_clicked

const void VocabularyManagerDialog::on_qpbWordAdd_clicked(bool checked /* false */)
{
	VocabularyView *vvVocabularyView = static_cast<VocabularyView *>(_qdvmVocabularyManager.vtwTabs->currentWidget());
    VocabularyModel *vmVocabularyModel = static_cast<VocabularyModel *>(vvVocabularyView->model());
    vmVocabularyModel->InsertRow(vmVocabularyModel->rowCount());

	vvVocabularyView->setCurrentIndex(vmVocabularyModel->index(vmVocabularyModel->rowCount() - 1, 0));
	vvVocabularyView->setFocus(Qt::OtherFocusReason);
} // on_qpbWordAdd_clicked

const void VocabularyManagerDialog::on_qpbWordRemove_clicked(bool checked /* false */)
{
    VocabularyView *vvVocabularyView = static_cast<VocabularyView *>(_qdvmVocabularyManager.vtwTabs->currentWidget());
    VocabularyModel *vmVocabularyModel = static_cast<VocabularyModel *>(vvVocabularyView->model());
    QItemSelectionModel *qismSelection = vvVocabularyView->selectionModel();
    vmVocabularyModel->RemoveRow(qismSelection->currentIndex().row());
} // on_qpbWordRemove_clicked

const void VocabularyManagerDialog::on_vtwTabs_TabEnableChanged(const int &pIndex, const Qt::CheckState &pState) const
{
    _vVocabulary->SetCategoryEnabled(_qlCategories.at(pIndex), pState);
    _qdvmVocabularyManager.vtwTabs->setTabEnabled(pIndex, pState);
} // on_vtwTabs_TabEnableChanged

VocabularyManagerDialog::VocabularyManagerDialog(const Vocabulary *pVocabulary, const Plugins *pPlugins, QWidget *pParent /* NULL */, Qt::WindowFlags pFlags /* 0 */) : QDialog(pParent, pFlags)
{
    _vVocabulary = pVocabulary;
	_pPlugins = pPlugins;

    _qdvmVocabularyManager.setupUi(this);

    InitTabs();
    EnableControls();

    connect(_qdvmVocabularyManager.vtwTabs, SIGNAL(TabEnableChanged(const int &, const Qt::CheckState &)), SLOT(on_vtwTabs_TabEnableChanged(const int &, const Qt::CheckState &)));
} // VocabularyManagerDialog