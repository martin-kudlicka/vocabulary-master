#include "vocabularymanagerdialog/vocabularysettingsdialog/languagefielddelegate.h"

#include "vocabulary.h"
#include <QtWidgets/QComboBox>

LanguageFieldDelegate::LanguageFieldDelegate(const Vocabulary *vocabulary, QObject *parent /* Q_NULLPTR */) : QStyledItemDelegate(parent), _vocabulary(vocabulary)
{
}

LanguageFieldDelegate::~LanguageFieldDelegate()
{
}

QWidget *LanguageFieldDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	const qint8 fieldId = _vocabulary->fieldId(index.row());

	VocabularyDatabase::LanguageIdList languageIds;
	if (_vocabulary->fieldHasAttribute(fieldId, VocabularyDatabase::FieldAttribute::BuiltIn))
	{
		languageIds = _vocabulary->languageIds(VocabularyDatabase::LanguageId::AllOnly);
	}
	else
	{
		languageIds = _vocabulary->languageIds(VocabularyDatabase::LanguageId::UserDefined);
	}

	QComboBox *editorBox = new QComboBox(parent);
    for (auto languageId : languageIds)
	{
        editorBox->addItem(_vocabulary->languageName(languageId), static_cast<quintptr>(languageId));
    }

    return editorBox;
}

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
		}
	}
}

void LanguageFieldDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    const QComboBox *languageBox = qobject_cast<QComboBox *>(editor);
    model->setData(index, languageBox->itemData(languageBox->currentIndex()), Qt::EditRole);
}