#ifndef MODELSMODEL_H
#define MODELSMODEL_H

#include <QtCore/QAbstractItemModel>
#include <QtSql/QSqlDatabase>

class ModelsModel : public QAbstractItemModel
{
    Q_OBJECT

    public:
                 ModelsModel(const QSqlDatabase *database, QObject *parent = NULL);
		virtual ~ModelsModel();

        void      deckId (quint8 deckId);
		qlonglong modelId(quint8 row) const;

    private:
        enum Column
		{
            ColumnName,
            ColumnCount
        }; // Column
        enum ColumnPosition
		{
            ColumnPosition1
        }; // ColumnPosition

              quint8        _deckId;
        const QSqlDatabase *_database;

        virtual int         columnCount(const QModelIndex &parent = QModelIndex())                            const;
        virtual QVariant    data       (const QModelIndex &index, int role = Qt::DisplayRole)                 const;
        virtual QVariant    headerData (int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
        virtual QModelIndex index      (int row, int column, const QModelIndex &parent = QModelIndex())       const;
        virtual QModelIndex parent     (const QModelIndex &index)                                             const;
        virtual int         rowCount   (const QModelIndex &parent = QModelIndex())                            const;
}; // ModelsModel

#endif // MODELSMODEL_H