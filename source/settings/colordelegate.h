#ifndef COLORDELEGATE_H
#define COLORDELEGATE_H

#include <QtWidgets/QStyledItemDelegate>

class ColorDelegate : public QStyledItemDelegate
{
  public:
    ColorDelegate(QObject *parent = Q_NULLPTR);

  private:
    virtual ~ColorDelegate() Q_DECL_OVERRIDE;

    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;
};

#endif