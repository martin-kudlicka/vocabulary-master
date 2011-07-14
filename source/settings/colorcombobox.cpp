#include "settings/colorcombobox.h"

#include <QtGui/QPainter>
#include <QtGui/QPaintEvent>

ColorComboBox::ColorComboBox(QWidget *pParent /* NULL */) : QComboBox(pParent)
{
} // ColorComboBox

void ColorComboBox::paintEvent(QPaintEvent * e)
{
	QComboBox::paintEvent(e);

	QPainter qpPainter(this);
	qpPainter.fillRect(e->rect().adjusted(BORDER_WIDTH, BORDER_WIDTH, - BORDER_WIDTH, - BORDER_WIDTH), QColor(itemData(currentIndex(), Qt::UserRole).toString()));
} // paintEvent