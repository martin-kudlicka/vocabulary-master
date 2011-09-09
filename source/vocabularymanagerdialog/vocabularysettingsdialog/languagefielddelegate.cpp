#include "vocabularymanagerdialog/vocabularysettingsdialog/languagefielddelegate.h"

#include <QtGui/QComboBox>

QWidget *LanguageFieldDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QComboBox *qcbEditor = new QComboBox(parent);

    foreach (int iLanguageId, _vVocabulary->GetLanguageIds()) {
        qcbEditor->addItem(_vVocabulary->GetLanguageName(iLanguageId));
    } // foreach

    return qcbEditor;
} // createEditor

LanguageFieldDelegate::LanguageFieldDelegate(const Vocabulary *pVocabulary, QObject *pParent /* NULL */) : QStyledItemDelegate(pParent)
{
    _vVocabulary = pVocabulary;
} // LanguageFieldDelegate

void LanguageFieldDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QComboBox *qcbLanguage = qobject_cast<QComboBox *>(editor);
    int iLanguageId = index.model()->data(index, Qt::EditRole).toInt();
    if (iLanguageId == VocabularyDatabase::FieldLanguageAll) {
        qcbLanguage->setCurrentIndex(qcbLanguage->count() - 1);
    } else {
        qcbLanguage->setCurrentIndex(iLanguageId - 1);
    } // if else
} // setEditorData

void LanguageFieldDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QComboBox *qcbLanguage = qobject_cast<QComboBox *>(editor);
    if (qcbLanguage->currentIndex() == qcbLanguage->count() - 1) {
        model->setData(index, VocabularyDatabase::FieldLanguageAll, Qt::EditRole);
    } else {
        model->setData(index, qcbLanguage->currentIndex() + 1, Qt::EditRole);
    } // if else
} // setModelData