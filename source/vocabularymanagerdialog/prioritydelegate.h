#ifndef PRIORITYDELEGATE_H
#define PRIORITYDELEGATE_H

#include <QtWidgets/QStyledItemDelegate>

class PriorityDelegate : public QStyledItemDelegate
{
  public:
    static const auto RECORD_PRIORITY_MAX = 9;
    static const auto RECORD_PRIORITY_MIN = 1;

    PriorityDelegate(QObject *parent);

  private:
    virtual ~PriorityDelegate() Q_DECL_OVERRIDE Q_DECL_EQ_DEFAULT;

    virtual QWidget *createEditor (QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index)   const Q_DECL_OVERRIDE;
    virtual void     paint        (QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;
    virtual void     setEditorData(QWidget *editor, const QModelIndex &index)                                       const Q_DECL_OVERRIDE;
};

#endif