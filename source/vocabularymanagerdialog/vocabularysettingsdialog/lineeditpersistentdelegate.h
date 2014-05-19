#ifndef LINEEDITPERSISTENTDELEGATE_H
#define LINEEDITPERSISTENTDELEGATE_H

#include <QtWidgets/QStyledItemDelegate>

class LineEditPersistentDelegate : public QStyledItemDelegate
{
    public:
                 LineEditPersistentDelegate(QObject *parent = NULL);
		virtual ~LineEditPersistentDelegate();

	private:
        virtual QWidget *createEditor (QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
        virtual void     setEditorData(QWidget *editor, const QModelIndex &index)                                     const;
		virtual void     setModelData (QWidget *editor, QAbstractItemModel *model, const QModelIndex &index)          const;
}; // LineEditPersistentDelegate

#endif // LINEEDITPERSISTENTDELEGATE_H