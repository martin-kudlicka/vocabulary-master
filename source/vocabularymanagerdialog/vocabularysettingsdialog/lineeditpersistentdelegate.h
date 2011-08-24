#ifndef LINEEDITPERSISTENTDELEGATE_H
#define LINEEDITPERSISTENTDELEGATE_H

#include <QtGui/QStyledItemDelegate>

class LineEditPersistentDelegate : public QStyledItemDelegate
{
    public:
        LineEditPersistentDelegate(QObject *pParent = NULL);

    private:
        virtual QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
        virtual void setEditorData(QWidget *editor, const QModelIndex &index) const;
}; // LineEditPersistentDelegate

#endif // LINEEDITPERSISTENTDELEGATE_H