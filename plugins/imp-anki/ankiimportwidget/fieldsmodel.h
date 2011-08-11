#ifndef FIELDSMODEL_H
#define FIELDSMODEL_H

#include <QtCore/QAbstractItemModel>
#include <QtSql/QSqlDatabase>

class FieldsModel : public QAbstractItemModel
{
    Q_ENUMS(eColumn)

    public:
        enum eColumn {
            ColumnName,
            ColumnMark,
            ColumnCount
        }; // eColumn

        FieldsModel(const QSqlDatabase *pAnki, QObject *pParent = NULL);

        virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
		const qlonglong GetFieldId(const int &pRow) const;
        virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
        virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
        const void SetModelId(const qlonglong &pModelId);

    private:
        enum eColumnPosition {
            ColumnPosition1
        }; // eColumnPosition

        qlonglong _qllModelId;
        const QSqlDatabase *_qsdAnki;

        virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
        virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
        virtual QModelIndex parent(const QModelIndex &index) const;
}; // FieldsModel

#endif // FIELDSMODEL_H