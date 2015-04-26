#include "settings/colorcombobox.h"

#include <QtWidgets/QStylePainter>
#include <QtGui/QPaintEvent>

ColorComboBox::ColorComboBox(QWidget *parent /* nullptr */) : QComboBox(parent)
{
} // ColorComboBox

ColorComboBox::~ColorComboBox()
{
} // ~ColorComboBox

void ColorComboBox::paintEvent(QPaintEvent * e)
{
	QComboBox::paintEvent(e);

    QStyleOptionComboBox styleOptionComboBox;
    initStyleOption(&styleOptionComboBox);

    QStylePainter stylePainter(this);
    QRect rect = stylePainter.style()->subElementRect(QStyle::SE_ComboBoxFocusRect, &styleOptionComboBox, this);
    rect.adjust(BORDER_WIDTH, BORDER_WIDTH, - BORDER_WIDTH, - BORDER_WIDTH);

    stylePainter.fillRect(rect, QColor(itemData(currentIndex(), Qt::UserRole).toString()));
} // paintEvent