#ifndef LANGUAGEFIELDDELEGATE_H
#define LANGUAGEFIELDDELEGATE_H

#include <QtWidgets/QStyledItemDelegate>

class Vocabulary;

class LanguageFieldDelegate : public QStyledItemDelegate
{
  public:
             LanguageFieldDelegate(const Vocabulary *vocabulary, QObject *parent = nullptr);
    virtual ~LanguageFieldDelegate() override;

  private:
    const Vocabulary *_vocabulary;

    virtual QWidget *createEditor (QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    virtual void     setEditorData(QWidget *editor, const QModelIndex &index)                                     const override;
    virtual void     setModelData (QWidget *editor, QAbstractItemModel *model, const QModelIndex &index)          const override;
};

#endif