#include "vocabularymanagerdialog/vocabularysettingsdialog/lineeditpersistentdelegate.h"

#include "vocabularymanagerdialog/vocabularysettingsdialog/lineeditpersistentdelegate/fieldlineedit.h"
#include "vocabularymanagerdialog/vocabularysettingsdialog/lineeditpersistentdelegate/fieldidentifiervalidator.h"
#include "vocabularymanagerdialog/vocabularysettingsdialog/fieldsmodel.h"

QWidget *LineEditPersistentDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
  Q_UNUSED(option);

  auto fieldLineEditor = new FieldLineEdit(parent);
  if (index.column() == static_cast<int>(FieldsModel::Column::TemplateName))
  {
    fieldLineEditor->setValidator(new FieldIdentifierValidator(index.row(), qobject_cast<const FieldsModel *>(index.model()), fieldLineEditor));
  }

  return fieldLineEditor;
}

void LineEditPersistentDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
  auto fieldLineEditor = qobject_cast<FieldLineEdit *>(editor);
  fieldLineEditor->setText(index.model()->data(index).toString());
}

void LineEditPersistentDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
  const auto fieldLineEditor = qobject_cast<FieldLineEdit *>(editor);
  if (fieldLineEditor->hasAcceptableInput())
  {
    QStyledItemDelegate::setModelData(editor, model, index);
  }
}