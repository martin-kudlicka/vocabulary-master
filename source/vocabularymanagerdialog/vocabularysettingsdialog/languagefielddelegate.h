#ifndef LANGUAGEFIELDDELEGATE_H
#define LANGUAGEFIELDDELEGATE_H

#include <QtGui/QItemDelegate>
#include "vocabulary.h"

class LanguageFieldDelegate : public QItemDelegate
{
    public:
        LanguageFieldDelegate(const Vocabulary *pVocabulary, QObject *pParent = NULL);

    private:
        const Vocabulary *_vVocabulary;

        virtual QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
        virtual void setEditorData(QWidget *editor, const QModelIndex &index) const;
        virtual void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
}; // LanguageFieldDelegate

#endif // LANGUAGEFIELDDELEGATE_H