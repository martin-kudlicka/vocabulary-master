#ifndef FIELDSMODEL_H
#define FIELDSMODEL_H

#include <QtCore/QAbstractItemModel>
#include <QtSql/QSqlDatabase>

class FieldsModel : public QAbstractItemModel
{
    public:
        FieldsModel(const QSqlDatabase *pAnki, QObject *pParent = NULL);

        const void SetModelId(const qlonglong &pModelId);

    private:
        enum eColumn {
            ColumnName,
            ColumnCount
        }; // eColumn

        enum eColumnPosition {
            ColumnPosition1
        }; // eColumnPosition

        qlonglong _qllModelId;
        const QSqlDatabase *_qsdAnki;

        virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
        virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
        virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
        virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
        virtual QModelIndex parent(const QModelIndex &index) const;
        virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
}; // FieldsModel

#endif // FIELDSMODEL_H