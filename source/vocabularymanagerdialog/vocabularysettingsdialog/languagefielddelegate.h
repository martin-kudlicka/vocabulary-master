#ifndef LANGUAGEFIELDDELEGATE_H
#define LANGUAGEFIELDDELEGATE_H

#include <QtWidgets/QStyledItemDelegate>

class Vocabulary;

class LanguageFieldDelegate : public QStyledItemDelegate
{
  public:
             LanguageFieldDelegate(const QSharedPointer<Vocabulary> &vocabulary, QObject *parent = Q_NULLPTR);
    virtual ~LanguageFieldDelegate() Q_DECL_OVERRIDE;

  private:
    const QSharedPointer<Vocabulary> _vocabulary;

    virtual QWidget *createEditor (QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;
    virtual void     setEditorData(QWidget *editor, const QModelIndex &index)                                     const Q_DECL_OVERRIDE;
    virtual void     setModelData (QWidget *editor, QAbstractItemModel *model, const QModelIndex &index)          const Q_DECL_OVERRIDE;
};

#endif