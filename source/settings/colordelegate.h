#ifndef COLORDELEGATE_H
#define COLORDELEGATE_H

#include <QtWidgets/QStyledItemDelegate>

class ColorDelegate : public QStyledItemDelegate
{
	public:
		ColorDelegate(QObject *parent = NULL);

	private:
		static const quint8 BORDER_WIDTH = 2;

		virtual ~ColorDelegate();

		virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index ) const;
}; // ColorDelegate

#endif // COLORDELEGATE_H