#ifndef PRIORITYDELEGATE_H
#define PRIORITYDELEGATE_H

#include <QtWidgets/QStyledItemDelegate>

class PriorityDelegate : public QStyledItemDelegate
{
	public:
        static const int RECORD_PRIORITY_MAX = 9;
		static const int RECORD_PRIORITY_MIN = 1;

		PriorityDelegate(QObject *pParent = NULL);

	private:
        static const int SPINBOX_BUTTONS_WIDTH = 16;

		virtual QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
		virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
		virtual void setEditorData(QWidget *editor, const QModelIndex &index) const;
}; // PriorityDelegate

#endif // PRIORITYDELEGATE_H