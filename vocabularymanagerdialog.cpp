#include "vocabularymanagerdialog.h"

#include <QtGui/QTableView>
#include <QtGui/QInputDialog>

const int VocabularyManagerDialog::AddTab(const QString &pName)
{
    return _qdvmVocabularyManager.qtwTabs->addTab(new QTableView(_qdvmVocabularyManager.qtwTabs), pName);
} // AddTab

const void VocabularyManagerDialog::InitTabs()
{
    Vocabulary::tCategoryIdList tcilCategories = _vVocabulary->GetCategoryIds();

    Vocabulary::tCategoryIdList::const_iterator ciCategoryId;
    for (ciCategoryId = tcilCategories.constBegin(); ciCategoryId != tcilCategories.constEnd(); ciCategoryId++) {
        _qhTabCategory.insert(AddTab(_vVocabulary->GetCategoryName(*ciCategoryId)), *ciCategoryId);
    } // for
} // InitTabs

const void VocabularyManagerDialog::on_qpbCategoryAdd_clicked(bool checked /* false */)
{
    QString qsCategory = QInputDialog::getText(this, tr("Add category"), tr("New category name"));
    if (!qsCategory.isEmpty()) {
        _qhTabCategory.insert(AddTab(qsCategory), _vVocabulary->AddCategory(qsCategory));
    } // if
} // on_qpbCategoryAdd_clicked

VocabularyManagerDialog::VocabularyManagerDialog(const Vocabulary *pVocabulary, QWidget *parent /* 0 */, Qt::WindowFlags f /* 0 */) : QDialog(parent, f)
{
    _vVocabulary = pVocabulary;

    _qdvmVocabularyManager.setupUi(this);

    InitTabs();
} // VocabularyManagerDialog