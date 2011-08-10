#ifndef DECKSMODEL_H
#define DECKSMODEL_H

#include <QtCore/QAbstractItemModel>
#include <QtSql/QSqlDatabase>

class DecksModel : public QAbstractItemModel
{
    public:
        DecksModel(const QSqlDatabase *pAnki, QObject *pParent = NULL);

        const int GetDeckId(const int &pRow) const;

    private:
        enum eColumn {
            ColumnId,
            ColumnCount
        }; // eColumn

        enum eColumnPosition {
            ColumnPosition1
        }; // eColumnPosition

        const QSqlDatabase *_qsdAnki;

        virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
        virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
        virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
        virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
        virtual QModelIndex parent(const QModelIndex &index) const;
        virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
}; // DecksModel

#endif // DECKSMODEL_H