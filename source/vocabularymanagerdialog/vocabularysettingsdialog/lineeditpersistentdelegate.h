#ifndef LINEEDITPERSISTENTDELEGATE_H
#define LINEEDITPERSISTENTDELEGATE_H

#include <QtWidgets/QStyledItemDelegate>

class LineEditPersistentDelegate : public QStyledItemDelegate
{
    public:
                 LineEditPersistentDelegate(QObject *parent = NULL);
		virtual ~LineEditPersistentDelegate() override;

	private:
        virtual QWidget *createEditor (QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
        virtual void     setEditorData(QWidget *editor, const QModelIndex &index)                                     const override;
		virtual void     setModelData (QWidget *editor, QAbstractItemModel *model, const QModelIndex &index)          const override;
}; // LineEditPersistentDelegate

#endif // LINEEDITPERSISTENTDELEGATE_H