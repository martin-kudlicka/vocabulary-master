#ifndef FIELDSMODEL_H
#define FIELDSMODEL_H

#include <QtCore/QAbstractItemModel>
#include <QtSql/QSqlDatabase>

class FieldsModel : public QAbstractItemModel
{
    Q_OBJECT

    public:
        enum Column
		{
            ColumnName,
            ColumnMark,
            ColumnCount
        }; // Column

                 FieldsModel(const QSqlDatabase *database, QObject *parent = NULL);
		virtual ~FieldsModel();

        virtual       QVariant    data      (const QModelIndex &index, int role = Qt::DisplayRole)           const;
		        const qlonglong   fieldId   (const quint8 &row)                                              const;
        virtual       QModelIndex index     (int row, int column, const QModelIndex &parent = QModelIndex()) const;
        virtual       int         rowCount  (const QModelIndex &parent = QModelIndex())                      const;
                const void        setModelId(const qlonglong &pModelId);

    private:
        enum ColumnPosition
		{
            ColumnPosition1
        }; // ColumnPosition

              qlonglong     _modelId;
        const QSqlDatabase *_database;

        virtual int         columnCount(const QModelIndex &parent = QModelIndex())                           const;
        virtual QVariant    headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
        virtual QModelIndex parent(const QModelIndex &index)                                                 const;
}; // FieldsModel

#endif // FIELDSMODEL_H