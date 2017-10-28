#include "ankiimportwidget/modelsmodel.h"

#include <QtSql/QSqlQuery>

const auto    COLUMN_DECKID = "deckId";
const QString COLUMN_ID     = "id";
const QString COLUMN_NAME   = "name";
const auto    TABLE_MODELS  = "models";

ModelsModel::ModelsModel(const QSqlDatabase *database) : _deckId(0), _database(database)
{
}

void ModelsModel::deckId(quintptr deckId)
{
  _deckId = deckId;
}

qlonglong ModelsModel::modelId(quintptr row) const
{
  auto query = _database->exec("SELECT " + COLUMN_ID + " FROM " + TABLE_MODELS + " WHERE " + COLUMN_DECKID + " = " + QString::number(_deckId));
  query.seek(row);
  return query.value(static_cast<int>(ColumnPosition::N1)).toLongLong();
}

int ModelsModel::columnCount(const QModelIndex &parent /* QModelIndex() */) const
{
  return static_cast<int>(Column::Count);
}

QVariant ModelsModel::data(const QModelIndex &index, int role /* Qt::DisplayRole */) const
{
  switch (role)
  {
    case Qt::DisplayRole:
    {
      auto query = _database->exec("SELECT " + COLUMN_NAME + " FROM " + TABLE_MODELS + " WHERE " + COLUMN_DECKID + " = " + QString::number(_deckId));
      query.seek(index.row());

      switch (index.column())
      {
        case Column::Name:
          return query.value(static_cast<int>(ColumnPosition::N1));
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
        case Column::Name:
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
    auto query = _database->exec("SELECT " + COLUMN_ID + " FROM " + TABLE_MODELS + " WHERE " + COLUMN_DECKID + " = " + QString::number(_deckId));
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