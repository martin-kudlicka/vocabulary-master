#ifndef MARKLINEEDITPERSISTENTDELEGATE_H
#define MARKLINEEDITPERSISTENTDELEGATE_H

class MarkLineEditPersistentDelegate : public QStyledItemDelegate
{
  public:
    virtual ~MarkLineEditPersistentDelegate() Q_DECL_OVERRIDE Q_DECL_EQ_DEFAULT;

  private:
    virtual QWidget *createEditor (QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;
    virtual void     setEditorData(QWidget *editor, const QModelIndex &index)                                     const Q_DECL_OVERRIDE;
};

#endif