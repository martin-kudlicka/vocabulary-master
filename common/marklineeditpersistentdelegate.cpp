#include "marklineeditpersistentdelegate.h"

#include "marklineedit.h"

MarkLineEditPersistentDelegate::MarkLineEditPersistentDelegate(QObject *pParent /* Q_NULLPTR */) : QStyledItemDelegate(pParent)
{
}

QWidget *MarkLineEditPersistentDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
  return new MarkLineEdit(parent);
}

void MarkLineEditPersistentDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
  auto markEditor = qobject_cast<MarkLineEdit *>(editor);
  markEditor->setText(index.model()->data(index).toString());
}