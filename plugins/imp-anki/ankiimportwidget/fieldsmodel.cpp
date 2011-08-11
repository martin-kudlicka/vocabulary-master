#include "ankiimportwidget/fieldsmodel.h"

#include <QtSql/QSqlQuery>

const QString COLUMN_ID = "id";
const QString COLUMN_MODELID = "modelId";
const QString COLUMN_NAME = "name";
const QString COLUMN_ORDINAL = "ordinal";
const QString TABLE_FIELDMODELS = "fieldModels";

int FieldsModel::columnCount(const QModelIndex &parent /* QModelIndex() */) const
{
    return ColumnCount;
} // columnCount

QVariant FieldsModel::data(const QModelIndex &index, int role /* Qt::DisplayRole */) const
{
    switch (role) {
        case Qt::DisplayRole:
            switch (index.column()) {
                case ColumnName:
                    {
                        QSqlQuery qsqQuery = _qsdAnki->exec("SELECT " + COLUMN_NAME + " FROM " + TABLE_FIELDMODELS + " WHERE " + COLUMN_MODELID + " = " + QString::number(_qllModelId) + " ORDER BY " + COLUMN_ORDINAL);
                        qsqQuery.seek(index.row());

                        return qsqQuery.value(ColumnPosition1);
                    }
                default:
                    return QVariant();
            } // switch
        default:
            return QVariant();
    } // switch
} // data

FieldsModel::FieldsModel(const QSqlDatabase *pAnki, QObject *pParent /* NULL */) : QAbstractItemModel(pParent)
{
    _qsdAnki = pAnki;

    _qllModelId = 0;
} // DecksModel

const qlonglong FieldsModel::GetFieldId(const int &pRow) const
{
	QSqlQuery qsqQuery = _qsdAnki->exec("SELECT " + COLUMN_ID + " FROM " + TABLE_FIELDMODELS + " ORDER BY " + COLUMN_ORDINAL);
	qsqQuery.seek(pRow);
	return qsqQuery.value(ColumnPosition1).toLongLong();
} // GetFieldId

QVariant FieldsModel::headerData(int section, Qt::Orientation orientation, int role /* Qt::DisplayRole */) const
{
    switch (role) {
        case Qt::DisplayRole:
            switch (section) {
                case ColumnName:
                    return tr("Field");
                case ColumnMark:
                    return tr("Mark");
            } // switch
        default:
            return QVariant();
    } // switch
} // headerData

QModelIndex FieldsModel::index(int row, int column, const QModelIndex &parent /* QModelIndex() */) const
{
    return createIndex(row, column);
} // index

QModelIndex FieldsModel::parent(const QModelIndex &index) const
{
    return QModelIndex();
} // parent

int FieldsModel::rowCount(const QModelIndex &parent /* QModelIndex() */) const
{
    if (parent == QModelIndex()) {
        QSqlQuery qsqQuery = _qsdAnki->exec("SELECT " + COLUMN_ID + " FROM " + TABLE_FIELDMODELS + " WHERE " + COLUMN_MODELID + " = " + QString::number(_qllModelId));
        if (qsqQuery.last()) {
            return qsqQuery.at() + 1;
        } else {
            return 0;
        } // if else
    } else {
        return 0;
    } // if else
} // rowCount

const void FieldsModel::SetModelId(const qlonglong &pModelId)
{
    _qllModelId = pModelId;
} // SetModelId