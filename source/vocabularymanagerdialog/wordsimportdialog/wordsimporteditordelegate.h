#ifndef WORDSIMPORTEDITORDELEGATE_H
#define WORDSIMPORTEDITORDELEGATE_H

#include <QtWidgets/QStyledItemDelegate>

class Vocabulary;

class WordsImportEditorDelegate : public QStyledItemDelegate
{
  public:
             WordsImportEditorDelegate(const Vocabulary *vocabulary, QObject *parent = Q_NULLPTR);
    virtual ~WordsImportEditorDelegate() Q_DECL_OVERRIDE;

  private:
    const Vocabulary *_vocabulary;

    virtual QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;
};

#endif