#include "marklineeditpersistentdelegate.h"

#include "marklineedit.h"

MarkLineEditPersistentDelegate::MarkLineEditPersistentDelegate(QObject *pParent /* NULL */) : QStyledItemDelegate(pParent)
{
} // MarkLineEditPersistentDelegate

MarkLineEditPersistentDelegate::~MarkLineEditPersistentDelegate()
{
} // ~MarkLineEditPersistentDelegate

QWidget *MarkLineEditPersistentDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return new MarkLineEdit(parent);
} // createEditor

void MarkLineEditPersistentDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    MarkLineEdit *markEditor = qobject_cast<MarkLineEdit *>(editor);
    markEditor->setText(index.model()->data(index).toString());
} // setEditorData