#include "vocabularymanagerdialog/vocabularysettingsdialog/lineeditpersistentdelegate.h"

#include <QtGui/QLineEdit>

QWidget *LineEditPersistentDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return new QLineEdit(parent);
} // createEditor

LineEditPersistentDelegate::LineEditPersistentDelegate(QObject *pParent /* NULL */) : QStyledItemDelegate(pParent)
{
} // LineEditPersistentDelegate

void LineEditPersistentDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QLineEdit *qleEditor = qobject_cast<QLineEdit *>(editor);
    qleEditor->setText(index.model()->data(index).toString());
} // setEditorData