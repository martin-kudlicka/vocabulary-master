#include "vocabularymanagerdialog/vocabularysettingsdialog/languagefielddelegate.h"

#include <QtWidgets/QComboBox>

LanguageFieldDelegate::LanguageFieldDelegate(const Vocabulary *vocabulary, QObject *parent /* nullptr */) : QStyledItemDelegate(parent), _vocabulary(vocabulary)
{
} // LanguageFieldDelegate

LanguageFieldDelegate::~LanguageFieldDelegate()
{
} // ~LanguageFieldDelegate

QWidget *LanguageFieldDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	const qint8 fieldId = _vocabulary->fieldId(index.row());

	VocabularyDatabase::LanguageIdList languageIds;
	if (_vocabulary->fieldHasAttribute(fieldId, VocabularyDatabase::FieldAttributeBuiltIn))
	{
		languageIds = _vocabulary->languageIds(VocabularyDatabase::LanguageIdsAllOnly);
	}
	else
	{
		languageIds = _vocabulary->languageIds(VocabularyDatabase::LanguageIdsUserDefined);
	} // if else

	QComboBox *editorBox = new QComboBox(parent);
    foreach (quint8 languageId, languageIds)
	{
        editorBox->addItem(_vocabulary->languageName(languageId), languageId);
    } // foreach

    return editorBox;
} // createEditor

void LanguageFieldDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    const quint8 languageId = index.model()->data(index, Qt::EditRole).toUInt();

	QComboBox *languageBox = qobject_cast<QComboBox *>(editor);
	for (quint8 languageIndex = 0; languageIndex < languageBox->count(); languageIndex++)
	{
		const quint8 itemLanguageId = languageBox->itemData(languageIndex).toUInt();
		if (itemLanguageId == languageId)
		{
			languageBox->setCurrentIndex(languageIndex);
			break;
		} // if
	} // for
} // setEditorData

void LanguageFieldDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    const QComboBox *languageBox = qobject_cast<QComboBox *>(editor);
    model->setData(index, languageBox->itemData(languageBox->currentIndex()), Qt::EditRole);
} // setModelData