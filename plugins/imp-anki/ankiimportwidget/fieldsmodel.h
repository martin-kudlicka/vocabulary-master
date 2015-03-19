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
		virtual ~FieldsModel() override;

        virtual QVariant    data      (const QModelIndex &index, int role = Qt::DisplayRole)           const override;
		        qlonglong   fieldId   (quint8 row)                                                     const;
        virtual QModelIndex index     (int row, int column, const QModelIndex &parent = QModelIndex()) const override;
        virtual int         rowCount  (const QModelIndex &parent = QModelIndex())                      const override;
                void        setModelId(qlonglong pModelId);

    private:
        enum ColumnPosition
		{
            ColumnPosition1
        }; // ColumnPosition

              qlonglong     _modelId;
        const QSqlDatabase *_database;

		virtual int         columnCount(const QModelIndex &parent = QModelIndex())                            const override;
		virtual QVariant    headerData (int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
        virtual QModelIndex parent     (const QModelIndex &index)                                             const override;
}; // FieldsModel

#endif // FIELDSMODEL_H