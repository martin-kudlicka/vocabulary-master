#include "ankiimportwidget/decksmodel.h"

#include <QtSql/QSqlQuery>

const QString COLUMN_ID   = "id";
const QString TABLE_DECKS = "decks";

DecksModel::DecksModel(const QSqlDatabase *database, QObject *parent /* NULL */) : QAbstractItemModel(parent), _database(database)
{
} // DecksModel

DecksModel::~DecksModel()
{
} // ~DecksModel

quint8 DecksModel::deckId(quint8 row) const
{
	QSqlQuery query = _database->exec("SELECT " + COLUMN_ID + " FROM " + TABLE_DECKS);
	query.seek(row);
	return query.value(ColumnPosition1).toUInt();
} // deckId

int DecksModel::columnCount(const QModelIndex &parent /* QModelIndex() */) const
{
    return ColumnCount;
} // columnCount

QVariant DecksModel::data(const QModelIndex &index, int role /* Qt::DisplayRole */) const
{
    switch (role)
	{
        case Qt::DisplayRole:
            {
                QSqlQuery query = _database->exec("SELECT " + COLUMN_ID + " FROM " + TABLE_DECKS);
                query.seek(index.row());

                switch (index.column())
				{
                    case ColumnId:
                        return query.value(ColumnPosition1);
                } // switch
            }
        default:
            return QVariant();
    } // switch
} // data

QVariant DecksModel::headerData(int section, Qt::Orientation orientation, int role /* Qt::DisplayRole */) const
{
    switch (role)
	{
        case Qt::DisplayRole:
            switch (section)
			{
                case ColumnId:
                    return tr("Deck");
            } // switch
        default:
            return QVariant();
    } // switch
} // headerData

QModelIndex DecksModel::index(int row, int column, const QModelIndex &parent /* QModelIndex() */) const
{
    return createIndex(row, column);
} // index

QModelIndex DecksModel::parent(const QModelIndex &index) const
{
    return QModelIndex();
} // parent

int DecksModel::rowCount(const QModelIndex &parent /* QModelIndex() */) const
{
    if (parent == QModelIndex())
	{
        QSqlQuery query = _database->exec("SELECT " + COLUMN_ID + " FROM " + TABLE_DECKS);
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