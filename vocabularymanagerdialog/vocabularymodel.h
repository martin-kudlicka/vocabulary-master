#ifndef VOCABULARYMODEL_H
#define VOCABULARYMODEL_H

#include <QtCore/QAbstractTableModel>
#include "vocabulary.h"

class VocabularyModel : public QAbstractTableModel
{
    Q_ENUMS(eColumn)

    public:
        VocabularyModel(const Vocabulary *pVocabulary, const int &pCategoryId, QObject *pParent = NULL);

		const void InsertRow(const int &pRow);
        int rowCount(const QModelIndex &parent = QModelIndex()) const;

    private:
        enum eColumn {
            ColumnLang1,
            ColumnLang2,
            ColumnPriority,
            ColumnCount
        }; // eColumn

        int _iCategoryId;
        const Vocabulary *_vVocabulary;

        int columnCount(const QModelIndex &parent = QModelIndex()) const;
        QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
}; // VocabularyModel

#endif // VOCABULARYMODEL_H