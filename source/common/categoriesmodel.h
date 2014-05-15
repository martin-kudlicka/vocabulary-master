#ifndef CATEGORIESMODEL_H
#define CATEGORIESMODEL_H

#include <QtCore/QAbstractItemModel>
#include "vocabulary.h"

class CategoriesModel : public QAbstractItemModel
{
    Q_OBJECT

    public:
        CategoriesModel(const Vocabulary *vocabulary, QObject *parent = NULL);

    private:
        enum Column
		{
            ColumnName,
            ColumnCount
        }; // Column

        const Vocabulary *_vocabulary;

        virtual int         columnCount(const QModelIndex &parent = QModelIndex())                            const;
        virtual QVariant    data       (const QModelIndex &index, int role = Qt::DisplayRole)                 const;
        virtual QVariant    headerData (int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
        virtual QModelIndex index      (int row, int column, const QModelIndex &parent = QModelIndex())       const;
        virtual QModelIndex parent     (const QModelIndex &index)                                             const;
        virtual int         rowCount   (const QModelIndex &parent = QModelIndex())                            const;
}; // CategoriesModel

#endif // CATEGORIESMODEL_H