#ifndef MODELSMODEL_H
#define MODELSMODEL_H

#include <QtCore/QAbstractItemModel>
#include <QtSql/QSqlDatabase>

class ModelsModel : public QAbstractItemModel
{
    public:
        ModelsModel(const QSqlDatabase *pAnki, QObject *pParent = NULL);

        const void SetDeckId(const int &pDeckId);

    private:
        enum eColumn {
            ColumnTags,
            ColumnName,
            ColumnDescription,
            ColumnCount
        }; // eColumn

        enum eColumnPosition {
            ColumnPosition1,
            ColumnPosition2,
            ColumnPosition3
        }; // eColumnPosition

        int _iDeckId;
        const QSqlDatabase *_qsdAnki;

        virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
        virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
        virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
        virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
        virtual QModelIndex parent(const QModelIndex &index) const;
        virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
}; // ModelsModel

#endif // MODELSMODEL_H