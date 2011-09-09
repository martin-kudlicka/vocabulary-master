#include "vocabularymanagerdialog/vocabularysettingsdialog/languagefielddelegate.h"

#include <QtGui/QComboBox>

QWidget *LanguageFieldDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	int iFieldId = _vVocabulary->GetFieldId(index.row());
	VocabularyDatabase::qfFieldAttributes qfaAttributes = _vVocabulary->GetFieldAttributes(iFieldId);

	VocabularyDatabase::tLanguageIdList tlilIds;
	if (qfaAttributes & VocabularyDatabase::FieldAttributeBuiltIn) {
		tlilIds = _vVocabulary->GetLanguageIds(VocabularyDatabase::LanguageIdsAllOnly);
	} else {
		tlilIds = _vVocabulary->GetLanguageIds(VocabularyDatabase::LanguageIdsUserDefined);
	} // if else

	QComboBox *qcbEditor = new QComboBox(parent);
    foreach (int iLanguageId, tlilIds) {
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