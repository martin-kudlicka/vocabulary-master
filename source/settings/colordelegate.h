#ifndef COLORDELEGATE_H
#define COLORDELEGATE_H

class ColorDelegate : public QStyledItemDelegate
{
  public:
    ColorDelegate(QObject *parent);

  private:
    virtual ~ColorDelegate() Q_DECL_OVERRIDE Q_DECL_EQ_DEFAULT;

    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;
};

#endif