#include "vocabularymanagerdialog.h"

#include <QtGui/QTableView>
#include <QtGui/QInputDialog>
#include "vocabularymanagerdialog/vocabularymodel.h"

const int VocabularyManagerDialog::AddTab(const int &pCategoryId)
{
    QTableView *qtvTableView = new QTableView(_qdvmVocabularyManager.qtwTabs);
    qtvTableView->setModel(new VocabularyModel(_vVocabulary, pCategoryId, qtvTableView));

    return _qdvmVocabularyManager.qtwTabs->addTab(qtvTableView, _vVocabulary->GetCategoryName(pCategoryId));
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