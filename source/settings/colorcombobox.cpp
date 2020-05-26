#include "pch.h"
#include "settings/colorcombobox.h"

const auto BORDER_WIDTH = 2;

ColorComboBox::ColorComboBox(QWidget *parent) : QComboBox(parent)
{
}

void ColorComboBox::paintEvent(QPaintEvent * e)
{
  QComboBox::paintEvent(e);

  QStyleOptionComboBox styleOptionComboBox;
  initStyleOption(&styleOptionComboBox);

  QStylePainter stylePainter(this);
  auto rect = stylePainter.style()->subElementRect(QStyle::SE_ComboBoxFocusRect, &styleOptionComboBox, this);
  rect.adjust(BORDER_WIDTH, BORDER_WIDTH, -BORDER_WIDTH, -BORDER_WIDTH);

  stylePainter.fillRect(rect, QColor(itemData(currentIndex(), Qt::UserRole).toString()));
}