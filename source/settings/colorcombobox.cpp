#include "settings/colorcombobox.h"

#include <QtGui/QStylePainter>
#include <QtGui/QPaintEvent>

ColorComboBox::ColorComboBox(QWidget *pParent /* NULL */) : QComboBox(pParent)
{
} // ColorComboBox

void ColorComboBox::paintEvent(QPaintEvent * e)
{
	QComboBox::paintEvent(e);

    QStyleOptionComboBox qsocbComboBox;
    initStyleOption(&qsocbComboBox);

    QStylePainter qspPainter(this);
    QRect qrRect = qspPainter.style()->subElementRect(QStyle::SE_ComboBoxFocusRect, &qsocbComboBox, this);
    qrRect.adjust(BORDER_WIDTH, BORDER_WIDTH, - BORDER_WIDTH, - BORDER_WIDTH);

    qspPainter.fillRect(qrRect, QColor(itemData(currentIndex(), Qt::UserRole).toString()));
} // paintEvent