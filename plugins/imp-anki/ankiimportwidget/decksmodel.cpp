#include "ankiimportwidget/decksmodel.h"

#include <QtSql/QSqlQuery>

const QString COLUMN_ID   = "id";
const auto    TABLE_DECKS = "decks";

DecksModel::DecksModel(const QSqlDatabase *database, QObject *parent /* nullptr */) : QAbstractItemModel(parent), _database(database)
{
}

DecksModel::~DecksModel()
{
}

quintptr DecksModel::deckId(quintptr row) const
{
  auto query = _database->exec("SELECT " + COLUMN_ID + " FROM " + TABLE_DECKS);
  query.seek(row);
  return query.value(static_cast<int>(ColumnPosition::N1)).toUInt();
}

int DecksModel::columnCount(const QModelIndex &parent /* QModelIndex() */) const
{
  return static_cast<int>(Column::Count);
}

QVariant DecksModel::data(const QModelIndex &index, int role /* Qt::DisplayRole */) const
{
  switch (role)
  {
    case Qt::DisplayRole:
    {
      auto query = _database->exec("SELECT " + COLUMN_ID + " FROM " + TABLE_DECKS);
      query.seek(index.row());

      switch (index.column())
      {
        case Column::Id:
          return query.value(static_cast<int>(ColumnPosition::N1));
      }
    }
    default:
      return QVariant();
  }
}

QVariant DecksModel::headerData(int section, Qt::Orientation orientation, int role /* Qt::DisplayRole */) const
{
  switch (role)
  {
    case Qt::DisplayRole:
      switch (section)
      {
        case Column::Id:
          return tr("Deck");
      }
    default:
      return QVariant();
  }
}

QModelIndex DecksModel::index(int row, int column, const QModelIndex &parent /* QModelIndex() */) const
{
  return createIndex(row, column);
}

QModelIndex DecksModel::parent(const QModelIndex &index) const
{
  return QModelIndex();
}

int DecksModel::rowCount(const QModelIndex &parent /* QModelIndex() */) const
{
  if (parent == QModelIndex())
  {
    auto query = _database->exec("SELECT " + COLUMN_ID + " FROM " + TABLE_DECKS);
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