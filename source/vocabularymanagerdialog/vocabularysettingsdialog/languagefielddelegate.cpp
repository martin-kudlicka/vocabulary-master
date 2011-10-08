#include "vocabularymanagerdialog/vocabularysettingsdialog/languagefielddelegate.h"

#include <QtGui/QComboBox>

QWidget *LanguageFieldDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	int iFieldId = _vVocabulary->GetFieldId(index.row());

	VocabularyDatabase::tLanguageIdList tlilIds;
	if (_vVocabulary->FieldHasAttribute(iFieldId, VocabularyDatabase::FieldAttributeBuiltIn)) {
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
    qcbLanguage->setCurrentIndex(iLanguageId - 1);
} // setEditorData

void LanguageFieldDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QComboBox *qcbLanguage = qobject_cast<QComboBox *>(editor);
    model->setData(index, qcbLanguage->currentIndex() + 1, Qt::EditRole);
} // setModelData