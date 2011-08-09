#ifndef CATEGORIESMODEL_H
#define CATEGORIESMODEL_H

#include <QtCore/QAbstractItemModel>
#include "vocabulary.h"

class CategoriesModel : public QAbstractItemModel
{
    public:
        CategoriesModel(const Vocabulary *pVocabulary, QObject *pParent = NULL);

    private:
        enum eColumn {
            ColumnName,
            ColumnCount
        }; // eColumn

        const Vocabulary *_vVocabulary;

        virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
        virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
        virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
        virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
        virtual QModelIndex parent(const QModelIndex &index) const;
        virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
}; // CategoriesModel

#endif // CATEGORIESMODEL_H