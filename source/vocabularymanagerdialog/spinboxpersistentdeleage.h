#ifndef SPINBOXPERSISTENTDELEGATE_H
#define SPINBOXPERSISTENTDELEGATE_H

#include <QtGui/QStyledItemDelegate>

class SpinBoxPersistentDelegate : public QStyledItemDelegate
{
	public:
		SpinBoxPersistentDelegate(QObject *pParent = NULL);

	private:
		static const int RECORD_PRIORITY_MAX = 9;
		static const int RECORD_PRIORITY_MIN = 1;

		virtual QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
		virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
		virtual void setEditorData(QWidget *editor, const QModelIndex &index) const;
		//virtual void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
}; // SpinBoxPersistentDelegate

#endif // SPINBOXPERSISTENTDELEGATE_H