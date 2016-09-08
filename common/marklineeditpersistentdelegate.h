#ifndef MARKLINEEDITPERSISTENTDELEGATE_H
#define MARKLINEEDITPERSISTENTDELEGATE_H

#include <QtWidgets/QStyledItemDelegate>

class MarkLineEditPersistentDelegate : public QStyledItemDelegate
{
  public:
             MarkLineEditPersistentDelegate(QObject *parent = nullptr);
    virtual ~MarkLineEditPersistentDelegate() override;

  private:
    virtual QWidget *createEditor (QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    virtual void     setEditorData(QWidget *editor, const QModelIndex &index)                                     const override;
};

#endif