#ifndef LINEEDITPERSISTENTDELEGATE_H
#define LINEEDITPERSISTENTDELEGATE_H

#include <QtWidgets/QStyledItemDelegate>

class LineEditPersistentDelegate : public QStyledItemDelegate
{
  public:
             LineEditPersistentDelegate(QObject *parent = Q_NULLPTR);
    virtual ~LineEditPersistentDelegate() Q_DECL_OVERRIDE;

  private:
    virtual QWidget *createEditor (QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;
    virtual void     setEditorData(QWidget *editor, const QModelIndex &index)                                     const Q_DECL_OVERRIDE;
    virtual void     setModelData (QWidget *editor, QAbstractItemModel *model, const QModelIndex &index)          const Q_DECL_OVERRIDE;
};

#endif