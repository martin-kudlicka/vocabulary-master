#ifndef WORDSIMPORTEDITORDELEGATE_H
#define WORDSIMPORTEDITORDELEGATE_H

class Vocabulary;

class WordsImportEditorDelegate : public QStyledItemDelegate
{
  public:
             WordsImportEditorDelegate(const QSharedPointer<Vocabulary> &vocabulary);
    virtual ~WordsImportEditorDelegate() Q_DECL_OVERRIDE Q_DECL_EQ_DEFAULT;

  private:
    const QSharedPointer<Vocabulary> _vocabulary;

    virtual QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;
};

#endif