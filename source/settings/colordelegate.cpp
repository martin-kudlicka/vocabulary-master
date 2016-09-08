#include "settings/colordelegate.h"

#include <QtGui/QPainter>

const quint8 BORDER_WIDTH = 2;

ColorDelegate::ColorDelegate(QObject *parent /* nullptr */) : QStyledItemDelegate(parent)
{
}

ColorDelegate::~ColorDelegate()
{
}

void ColorDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	QStyledItemDelegate::paint(painter, option, index);
	painter->fillRect(option.rect.adjusted(BORDER_WIDTH, BORDER_WIDTH, - BORDER_WIDTH, - BORDER_WIDTH), QColor(index.data(Qt::UserRole).toString()));
}