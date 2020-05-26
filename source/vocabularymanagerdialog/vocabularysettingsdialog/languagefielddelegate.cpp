#include "pch.h"
#include "vocabularymanagerdialog/vocabularysettingsdialog/languagefielddelegate.h"

#include "vocabulary.h"

LanguageFieldDelegate::LanguageFieldDelegate(const QSharedPointer<Vocabulary> &vocabulary) : _vocabulary(vocabulary)
{
}

QWidget *LanguageFieldDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
  Q_UNUSED(option);

  const auto fieldId = _vocabulary->fieldId(index.row());

  VocabularyDatabase::LanguageIdList languageIds;
  if (_vocabulary->fieldHasAttribute(fieldId, VocabularyDatabase::FieldAttribute::BuiltIn))
  {
    languageIds = _vocabulary->languageIds(VocabularyDatabase::LanguageId::AllOnly);
  }
  else
  {
    languageIds = _vocabulary->languageIds(VocabularyDatabase::LanguageId::UserDefined);
  }

  auto editorBox = new QComboBox(parent);
  for (auto languageId : languageIds)
  {
    editorBox->addItem(_vocabulary->languageName(languageId), static_cast<quintptr>(languageId));
  }

  return editorBox;
}

void LanguageFieldDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
  const auto languageId = index.model()->data(index, Qt::EditRole).toUInt();

  auto languageBox = qobject_cast<QComboBox *>(editor);
  for (auto languageIndex = 0; languageIndex < languageBox->count(); ++languageIndex)
  {
    const auto itemLanguageId = languageBox->itemData(languageIndex).toUInt();
    if (itemLanguageId == languageId)
    {
      languageBox->setCurrentIndex(languageIndex);
      break;
    }
  }
}

void LanguageFieldDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
  const auto languageBox = qobject_cast<QComboBox *>(editor);
  model->setData(index, languageBox->itemData(languageBox->currentIndex()), Qt::EditRole);
}