#ifndef LANGUAGEFIELDDELEGATE_H
#define LANGUAGEFIELDDELEGATE_H

#include <QtWidgets/QStyledItemDelegate>
#include "vocabulary.h"

class LanguageFieldDelegate : public QStyledItemDelegate
{
    public:
        LanguageFieldDelegate(const Vocabulary *vocabulary, QObject *parent = NULL);

    private:
        const Vocabulary *_vocabulary;

        virtual QWidget *createEditor (QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
        virtual void     setEditorData(QWidget *editor, const QModelIndex &index)                                     const;
        virtual void     setModelData (QWidget *editor, QAbstractItemModel *model, const QModelIndex &index)          const;
}; // LanguageFieldDelegate

#endif // LANGUAGEFIELDDELEGATE_H