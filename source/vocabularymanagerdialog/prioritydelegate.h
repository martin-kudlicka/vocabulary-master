#ifndef PRIORITYDELEGATE_H
#define PRIORITYDELEGATE_H

#include <QtWidgets/QStyledItemDelegate>

class PriorityDelegate : public QStyledItemDelegate
{
	public:
        static const quint8 RECORD_PRIORITY_MAX = 9;
		static const quint8 RECORD_PRIORITY_MIN = 1;

		PriorityDelegate(QObject *parent = nullptr);

	private:
        static const quint8 SPINBOX_BUTTONS_WIDTH = 16;

		virtual ~PriorityDelegate() override;

		virtual QWidget *createEditor (QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index)   const override;
		virtual void     paint        (QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
		virtual void     setEditorData(QWidget *editor, const QModelIndex &index)                                       const override;
}; // PriorityDelegate

#endif // PRIORITYDELEGATE_H