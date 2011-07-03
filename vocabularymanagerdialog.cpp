#include "vocabularymanagerdialog.h"

#include <QtGui/QTableView>
#include <QtGui/QInputDialog>
#include "vocabularymanagerdialog/vocabularymodel.h"
#include "vocabularymanagerdialog/vocabularyview.h"

const int VocabularyManagerDialog::AddTab(const int &pCategoryId)
{
    VocabularyView *vvTableView = new VocabularyView(_qdvmVocabularyManager.qtwTabs);
    vvTableView->setModel(new VocabularyModel(_vVocabulary, pCategoryId, vvTableView));
	vvTableView->horizontalHeader()->setResizeMode(VocabularyModel::ColumnLang1, QHeaderView::Stretch);
	vvTableView->horizontalHeader()->setResizeMode(VocabularyModel::ColumnLang2, QHeaderView::Stretch);

    int iTab = _qdvmVocabularyManager.qtwTabs->addTab(vvTableView, _vVocabulary->GetCategoryName(pCategoryId));
	EnableControls();
	return iTab;
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
        _qhTabCategory.insert(AddTab(iCategory), iCategory);
    } // if
} // on_qpbCategoryAdd_clicked

const void VocabularyManagerDialog::on_qpbWordAdd_clicked(bool checked /* false */)
{
    VocabularyModel *vmVocabularyModel = static_cast<VocabularyModel *>(static_cast<QTableView *>(_qdvmVocabularyManager.qtwTabs->currentWidget())->model());
    vmVocabularyModel->InsertRow(vmVocabularyModel->rowCount());
} // on_qpbWordAdd_clicked

VocabularyManagerDialog::VocabularyManagerDialog(const Vocabulary *pVocabulary, QWidget *pParent /* NULL */, Qt::WindowFlags pFlags /* 0 */) : QDialog(pParent, pFlags)
{
    _vVocabulary = pVocabulary;

    _qdvmVocabularyManager.setupUi(this);

    InitTabs();
    EnableControls();
} // VocabularyManagerDialog