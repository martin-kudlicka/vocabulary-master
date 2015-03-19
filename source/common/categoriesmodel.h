#ifndef CATEGORIESMODEL_H
#define CATEGORIESMODEL_H

#include <QtCore/QAbstractItemModel>
#include "vocabulary.h"

class CategoriesModel : public QAbstractItemModel
{
    Q_OBJECT

    public:
                 CategoriesModel(const Vocabulary *vocabulary, QObject *parent = NULL);
		virtual ~CategoriesModel() override;

    private:
        enum Column
		{
            ColumnName,
            ColumnCount
        }; // Column

        const Vocabulary *_vocabulary;

		virtual int         columnCount(const QModelIndex &parent = QModelIndex())                            const override;
        virtual QVariant    data       (const QModelIndex &index, int role = Qt::DisplayRole)                 const override;
        virtual QVariant    headerData (int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
        virtual QModelIndex index      (int row, int column, const QModelIndex &parent = QModelIndex())       const override;
        virtual QModelIndex parent     (const QModelIndex &index)                                             const override;
        virtual int         rowCount   (const QModelIndex &parent = QModelIndex())                            const override;
}; // CategoriesModel

#endif // CATEGORIESMODEL_H