#ifndef COLORDELEGATE_H
#define COLORDELEGATE_H

#include <QtWidgets/QStyledItemDelegate>

class ColorDelegate : public QStyledItemDelegate
{
	public:
		ColorDelegate(QObject *pParent = NULL);

	private:
		static const int BORDER_WIDTH = 2;

		virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index ) const;
}; // ColorDelegate

#endif // COLORDELEGATE_H