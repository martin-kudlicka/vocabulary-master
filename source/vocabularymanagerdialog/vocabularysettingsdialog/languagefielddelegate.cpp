#include "vocabularymanagerdialog/vocabularysettingsdialog/languagefielddelegate.h"

#include <QtGui/QComboBox>

QWidget *LanguageFieldDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QComboBox *qcbEditor = new QComboBox(parent);

    for (int iI = Vocabulary::FieldLanguageLeft; iI < Vocabulary::FieldLanguageCount; iI++) {
        qcbEditor->addItem(_vVocabulary->GetLanguageName(static_cast<Vocabulary::eFieldLanguage>(iI)));
    } // for

    return qcbEditor;
} // createEditor

LanguageFieldDelegate::LanguageFieldDelegate(const Vocabulary *pVocabulary, QObject *pParent /* NULL */) : QItemDelegate(pParent)
{
    _vVocabulary = pVocabulary;
} // LanguageFieldDelegate

void LanguageFieldDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QComboBox *qcbLanguage = qobject_cast<QComboBox *>(editor);
    qcbLanguage->setCurrentIndex(index.model()->data(index, Qt::EditRole).toInt() - 1);
} // setEditorData

void LanguageFieldDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QComboBox *qcbLanguage = qobject_cast<QComboBox *>(editor);
    model->setData(index, qcbLanguage->currentIndex() + 1, Qt::EditRole);
} // setModelData