#include "vocabularymanagerdialog/wordsimportdialog/wordsimporteditordelegate.h"

#include "vocabulary.h"
#include "vocabularymanagerdialog/prioritydelegate.h"
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QLineEdit>

WordsImportEditorDelegate::WordsImportEditorDelegate(const QSharedPointer<Vocabulary> &vocabulary) : _vocabulary(vocabulary)
{
}

QWidget *WordsImportEditorDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
  const auto fieldId = _vocabulary->fieldId(index.row());
  if (_vocabulary->fieldHasAttribute(fieldId, VocabularyDatabase::FieldAttribute::BuiltIn))
  {
    const auto builtIn = _vocabulary->fieldBuiltIn(fieldId);
    if (builtIn == VocabularyDatabase::FieldBuiltIn::Priority)
    {
      auto editor = new QSpinBox(parent);
      editor->setMinimum(PriorityDelegate::RECORD_PRIORITY_MIN);
      editor->setMaximum(PriorityDelegate::RECORD_PRIORITY_MAX);
      return editor;
    }
  }
  else
  {
    auto editor = new QLineEdit(parent);
    return editor;
  }

  return Q_NULLPTR;
}