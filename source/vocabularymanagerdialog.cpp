#include "vocabularymanagerdialog.h"

#include <QtGui/QInputDialog>
#include "vocabularymanagerdialog/vocabularymodel.h"
#include "vocabularymanagerdialog/vocabularyview.h"
#include "vocabularymanagerdialog/vocabularysettingsdialog.h"

const int VocabularyManagerDialog::AddTab(const int &pCategoryId)
{
    VocabularyView *vvTableView = new VocabularyView(_qdvmVocabularyManager.qtwTabs);
    vvTableView->setModel(new VocabularyModel(_vVocabulary, pCategoryId, vvTableView));

    for (int iColumn = 0; iColumn < VocabularyModel::ColumnCount; iColumn++) {
	    vvTableView->horizontalHeader()->setResizeMode(iColumn, QHeaderView::Stretch);
    } // for

    return _qdvmVocabularyManager.qtwTabs->addTab(vvTableView, _vVocabulary->GetCategoryName(pCategoryId));
} // AddTab

const void VocabularyManagerDialog::EnableControls()
{
    _qdvmVocabularyManager.qpbWordAdd->setEnabled(_qdvmVocabularyManager.qtwTabs->currentWidget());
} // EnableControls

const void VocabularyManagerDialog::InitTabs()
{
    Vocabulary::tCategoryIdList tcilCategories = _vVocabulary->GetCategoryIds();

    Vocabulary::tCategoryIdList::const_iterator ciCategoryId;
    for (ciCategoryId = tcilCategories.constBegin(); ciCategoryId != tcilCategories.constEnd(); ciCategoryId++) {
        _qhTabCategory.insert(AddTab(*ciCategoryId), *ciCategoryId);
    } // for
} // InitTabs

const void VocabularyManagerDialog::on_qpbCategoryAdd_clicked(bool checked /* false */)
{
    QString qsCategory = QInputDialog::getText(this, tr("Add category"), tr("New category name"));
    if (!qsCategory.isEmpty()) {
        int iCategory = _vVocabulary->AddCategory(qsCategory);
		int iTab = AddTab(iCategory);
        _qhTabCategory.insert(iTab, iCategory);

		_qdvmVocabularyManager.qtwTabs->setCurrentIndex(iTab);
		EnableControls();
    } // if
} // on_qpbCategoryAdd_clicked

const void VocabularyManagerDialog::on_qpbVocabularySettings_clicked(bool checked /* false */)
{
	VocabularySettingsDialog vsdSettings(_vVocabulary, _pPlugins, this);
	vsdSettings.exec();
} // on_qpbVocabularySettings_clicked

const void VocabularyManagerDialog::on_qpbWordAdd_clicked(bool checked /* false */)
{
	VocabularyView *vvVocabularyView = static_cast<VocabularyView *>(_qdvmVocabularyManager.qtwTabs->currentWidget());
    VocabularyModel *vmVocabularyModel = static_cast<VocabularyModel *>(vvVocabularyView->model());
    vmVocabularyModel->InsertRow(vmVocabularyModel->rowCount());

	vvVocabularyView->setCurrentIndex(vmVocabularyModel->index(vmVocabularyModel->rowCount() - 1, 0));
	vvVocabularyView->setFocus(Qt::OtherFocusReason);
} // on_qpbWordAdd_clicked

const void VocabularyManagerDialog::on_qpbWordRemove_clicked(bool checked /* false */)
{
    VocabularyView *vvVocabularyView = static_cast<VocabularyView *>(_qdvmVocabularyManager.qtwTabs->currentWidget());
    VocabularyModel *vmVocabularyModel = static_cast<VocabularyModel *>(vvVocabularyView->model());
    QItemSelectionModel *qismSelection = vvVocabularyView->selectionModel();
    vmVocabularyModel->RemoveRow(qismSelection->currentIndex().row());
} // on_qpbWordRemove_clicked

VocabularyManagerDialog::VocabularyManagerDialog(const Vocabulary *pVocabulary, const Plugins *pPlugins, QWidget *pParent /* NULL */, Qt::WindowFlags pFlags /* 0 */) : QDialog(pParent, pFlags)
{
    _vVocabulary = pVocabulary;
	_pPlugins = pPlugins;

    _qdvmVocabularyManager.setupUi(this);

    InitTabs();
    EnableControls();
} // VocabularyManagerDialog