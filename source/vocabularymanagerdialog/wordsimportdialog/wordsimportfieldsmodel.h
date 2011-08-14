#ifndef WORDSIMPORTFIELDSMODEL_H
#define WORDSIMPORTFIELDSMODEL_H

#include <QtCore/QAbstractItemModel>
#include "vocabulary.h"

class WordsImportFieldsModel : public QAbstractItemModel
{
    Q_ENUMS(eColumn)

    public:
        enum eColumn {
            ColumnName,
			ColumnLanguage,
            ColumnEditor,
            ColumnCount
        }; // eColumn

        WordsImportFieldsModel(const Vocabulary *pVocabulary, QObject *pParent = NULL);

        virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
        virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;

    private:
        const Vocabulary *_vVocabulary;

        virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
        virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
        virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
        virtual QModelIndex parent(const QModelIndex &index) const;
}; // WordsImportFieldsModel

#endif // WORDSIMPORTFIELDSMODEL_H