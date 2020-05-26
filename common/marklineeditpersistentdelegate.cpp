#include "pch.h"
#include "marklineeditpersistentdelegate.h"

#include "marklineedit.h"

QWidget *MarkLineEditPersistentDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
  Q_UNUSED(option);
  Q_UNUSED(index);

  return new MarkLineEdit(parent);
}

void MarkLineEditPersistentDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
  auto markEditor = qobject_cast<MarkLineEdit *>(editor);
  markEditor->setText(index.model()->data(index).toString());
}