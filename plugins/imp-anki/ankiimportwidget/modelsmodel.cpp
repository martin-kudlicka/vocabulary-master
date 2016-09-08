#include "ankiimportwidget/modelsmodel.h"

#include <QtSql/QSqlQuery>

const QString COLUMN_DECKID = "deckId";
const QString COLUMN_ID     = "id";
const QString COLUMN_NAME   = "name";
const QString TABLE_MODELS  = "models";

ModelsModel::ModelsModel(const QSqlDatabase *database, QObject *parent /* nullptr */) : QAbstractItemModel(parent), _deckId(0), _database(database)
{
}

ModelsModel::~ModelsModel()
{
}

void ModelsModel::deckId(quint8 deckId)
{
	_deckId = deckId;
}

qlonglong ModelsModel::modelId(quint8 row) const
{
	QSqlQuery query = _database->exec("SELECT " + COLUMN_ID + " FROM " + TABLE_MODELS + " WHERE " + COLUMN_DECKID + " = " + QString::number(_deckId));
	query.seek(row);
	return query.value(ColumnPosition1).toLongLong();
}

int ModelsModel::columnCount(const QModelIndex &parent /* QModelIndex() */) const
{
    return ColumnCount;
}

QVariant ModelsModel::data(const QModelIndex &index, int role /* Qt::DisplayRole */) const
{
    switch (role)
	{
        case Qt::DisplayRole:
            {
                QSqlQuery query = _database->exec("SELECT " + COLUMN_NAME + " FROM " + TABLE_MODELS + " WHERE " + COLUMN_DECKID + " = " + QString::number(_deckId));
                query.seek(index.row());

                switch (index.column())
				{
                    case ColumnName:
                        return query.value(ColumnPosition1);
                }
            }
        default:
            return QVariant();
    }
}

QVariant ModelsModel::headerData(int section, Qt::Orientation orientation, int role /* Qt::DisplayRole */) const
{
    switch (role)
	{
        case Qt::DisplayRole:
            switch (section)
			{
                case ColumnName:
                    return tr("Model");
            }
        default:
            return QVariant();
    }
}

QModelIndex ModelsModel::index(int row, int column, const QModelIndex &parent /* QModelIndex() */) const
{
    return createIndex(row, column);
}

QModelIndex ModelsModel::parent(const QModelIndex &index) const
{
    return QModelIndex();
}

int ModelsModel::rowCount(const QModelIndex &parent /* QModelIndex() */) const
{
    if (parent == QModelIndex())
	{
        QSqlQuery query = _database->exec("SELECT " + COLUMN_ID + " FROM " + TABLE_MODELS + " WHERE " + COLUMN_DECKID + " = " + QString::number(_deckId));
        if (query.last())
		{
            return query.at() + 1;
        }
		else
		{
            return 0;
        }
    }
	else
	{
        return 0;
    }
}