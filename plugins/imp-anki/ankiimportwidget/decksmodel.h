#ifndef DECKSMODEL_H
#define DECKSMODEL_H

#include <QtCore/QAbstractItemModel>
#include <QtSql/QSqlDatabase>

class DecksModel : public QAbstractItemModel
{
    Q_OBJECT

    public:
                 DecksModel(const QSqlDatabase *database, QObject *parent = NULL);
		virtual ~DecksModel();

        const quint8 deckId(const quint8 &row) const;

    private:
        enum Column
		{
            ColumnId,
            ColumnCount
        }; // Column
        enum ColumnPosition
		{
            ColumnPosition1
        }; // ColumnPosition

        const QSqlDatabase *_database;

        virtual int         columnCount(const QModelIndex &parent = QModelIndex())                            const;
        virtual QVariant    data       (const QModelIndex &index, int role = Qt::DisplayRole)                 const;
        virtual QVariant    headerData (int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
        virtual QModelIndex index      (int row, int column, const QModelIndex &parent = QModelIndex())       const;
        virtual QModelIndex parent     (const QModelIndex &index)                                             const;
        virtual int         rowCount   (const QModelIndex &parent = QModelIndex())                            const;
}; // DecksModel

#endif // DECKSMODEL_H