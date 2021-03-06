#ifndef LINEEDITPERSISTENTDELEGATE_H
#define LINEEDITPERSISTENTDELEGATE_H

class LineEditPersistentDelegate : public QStyledItemDelegate
{
  public:
    virtual ~LineEditPersistentDelegate() Q_DECL_OVERRIDE Q_DECL_EQ_DEFAULT;

  private:
    virtual QWidget *createEditor (QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;
    virtual void     setEditorData(QWidget *editor, const QModelIndex &index)                                     const Q_DECL_OVERRIDE;
    virtual void     setModelData (QWidget *editor, QAbstractItemModel *model, const QModelIndex &index)          const Q_DECL_OVERRIDE;
};

#endif