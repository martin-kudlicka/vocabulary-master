#include "ankiimportwidget/fieldsmodel.h"

#include <QtSql/QSqlQuery>

const QString COLUMN_ID         = "id";
const QString COLUMN_MODELID    = "modelId";
const QString COLUMN_NAME       = "name";
const QString COLUMN_ORDINAL    = "ordinal";
const QString TABLE_FIELDMODELS = "fieldModels";
const QString TEMPLATE_MARK     = "${%1}";

FieldsModel::FieldsModel(const QSqlDatabase *database, QObject *parent /* NULL */) : QAbstractItemModel(parent), _modelId(0), _database(database)
{
} // DecksModel

FieldsModel::~FieldsModel()
{
} // ~FieldsModel

QVariant FieldsModel::data(const QModelIndex &index, int role /* Qt::DisplayRole */) const
{
	switch (role)
	{
	case Qt::DisplayRole:
		switch (index.column())
		{
		case ColumnName:
			{
				QSqlQuery query = _database->exec("SELECT " + COLUMN_NAME + " FROM " + TABLE_FIELDMODELS + " WHERE " + COLUMN_MODELID + " = " + QString::number(_modelId) + " ORDER BY " + COLUMN_ORDINAL);
				query.seek(index.row());

				return query.value(ColumnPosition1);
			}
		case ColumnMark:
			const QModelIndex nameIndex = this->index(index.row(), ColumnName);
			const QString name          = data(nameIndex).toString();
			return TEMPLATE_MARK.arg(name);
		} // switch
	default:
		return QVariant();
	} // switch
} // data

const qlonglong FieldsModel::fieldId(const quint8 &row) const
{
	QSqlQuery query = _database->exec("SELECT " + COLUMN_ID + " FROM " + TABLE_FIELDMODELS + " WHERE " + COLUMN_MODELID + " = " + QString::number(_modelId) + " ORDER BY " + COLUMN_ORDINAL);
	query.seek(row);
	return query.value(ColumnPosition1).toLongLong();
} // fieldId

QModelIndex FieldsModel::index(int row, int column, const QModelIndex &parent /* QModelIndex() */) const
{
	return createIndex(row, column);
} // index

int FieldsModel::rowCount(const QModelIndex &parent /* QModelIndex() */) const
{
	if (parent == QModelIndex())
	{
		QSqlQuery query = _database->exec("SELECT " + COLUMN_ID + " FROM " + TABLE_FIELDMODELS + " WHERE " + COLUMN_MODELID + " = " + QString::number(_modelId));
		if (query.last())
		{
			return query.at() + 1;
		}
		else
		{
			return 0;
		} // if else
	}
	else
	{
		return 0;
	} // if else
} // rowCount

const void FieldsModel::setModelId(const qlonglong &modelId)
{
	_modelId = modelId;
} // setModelId

int FieldsModel::columnCount(const QModelIndex &parent /* QModelIndex() */) const
{
    return ColumnCount;
} // columnCount

QVariant FieldsModel::headerData(int section, Qt::Orientation orientation, int role /* Qt::DisplayRole */) const
{
    switch (role)
	{
        case Qt::DisplayRole:
            switch (section)
			{
                case ColumnName:
                    return tr("Field");
                case ColumnMark:
                    return tr("Mark");
            } // switch
        default:
            return QVariant();
    } // switch
} // headerData

QModelIndex FieldsModel::parent(const QModelIndex &index) const
{
    return QModelIndex();
} // parent