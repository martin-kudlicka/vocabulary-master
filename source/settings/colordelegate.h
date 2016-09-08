#ifndef COLORDELEGATE_H
#define COLORDELEGATE_H

#include <QtWidgets/QStyledItemDelegate>

class ColorDelegate : public QStyledItemDelegate
{
  public:
    ColorDelegate(QObject *parent = nullptr);

  private:
    virtual ~ColorDelegate() override;

    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

#endif