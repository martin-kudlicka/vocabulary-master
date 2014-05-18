#ifndef MARKLINEEDITPERSISTENTDELEGATE_H
#define MARKLINEEDITPERSISTENTDELEGATE_H

#include <QtWidgets/QStyledItemDelegate>

class MarkLineEditPersistentDelegate : public QStyledItemDelegate
{
    public:
                 MarkLineEditPersistentDelegate(QObject *parent = NULL);
		virtual ~MarkLineEditPersistentDelegate();

    private:
        virtual QWidget *createEditor (QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
        virtual void     setEditorData(QWidget *editor, const QModelIndex &index)                                     const;
}; // MarkLineEditPersistentDelegate

#endif // MARKLINEEDITPERSISTENTDELEGATE_H