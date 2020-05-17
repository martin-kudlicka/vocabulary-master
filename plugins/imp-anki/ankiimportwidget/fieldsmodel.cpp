#include "ankiimportwidget/fieldsmodel.h"

#include <QtSql/QSqlQuery>

const QString COLUMN_ID         = "id";
const auto    COLUMN_MODELID    = "modelId";
const QString COLUMN_NAME       = "name";
const auto    COLUMN_ORDINAL    = "ordinal";
const auto    TABLE_FIELDMODELS = "fieldModels";
const QString TEMPLATE_MARK     = "${%1}";

FieldsModel::FieldsModel(const QSqlDatabase *database) : _modelId(0), _database(database)
{
}

qlonglong FieldsModel::fieldId(quintptr row) const
{
  auto query = _database->exec("SELECT " + COLUMN_ID + " FROM " + TABLE_FIELDMODELS + " WHERE " + COLUMN_MODELID + " = " + QString::number(_modelId) + " ORDER BY " + COLUMN_ORDINAL);
  query.seek(row);
  return query.value(static_cast<int>(ColumnPosition::N1)).toLongLong();
}

void FieldsModel::setModelId(qlonglong modelId)
{
  _modelId = modelId;
}

QVariant FieldsModel::data(const QModelIndex &index, int role /* Qt::DisplayRole */) const
{
  switch (role)
  {
    case Qt::DisplayRole:
      switch (index.column())
      {
        case Column::Name:
        {
          auto query = _database->exec("SELECT " + COLUMN_NAME + " FROM " + TABLE_FIELDMODELS + " WHERE " + COLUMN_MODELID + " = " + QString::number(_modelId) + " ORDER BY " + COLUMN_ORDINAL);
          query.seek(index.row());

          return query.value(static_cast<int>(ColumnPosition::N1));
        }
        case Column::Mark:
          const auto nameIndex = this->index(index.row(), Column::Name);
          const auto name      = data(nameIndex).toString();
          return TEMPLATE_MARK.arg(name);
      }
    default:
      return QVariant();
  }
}

QModelIndex FieldsModel::index(int row, int column, const QModelIndex &parent /* QModelIndex() */) const
{
  Q_UNUSED(parent);

  return createIndex(row, column);
}

int FieldsModel::rowCount(const QModelIndex &parent /* QModelIndex() */) const
{
  if (parent.isValid())
  {
    return 0;
  }
  else
  {
    auto query = _database->exec("SELECT " + COLUMN_ID + " FROM " + TABLE_FIELDMODELS + " WHERE " + COLUMN_MODELID + " = " + QString::number(_modelId));
    if (query.last())
    {
      return query.at() + 1;
    }
    else
    {
      return 0;
    }
  }
}

int FieldsModel::columnCount(const QModelIndex &parent /* QModelIndex() */) const
{
  Q_UNUSED(parent);

  return Column::Count;
}

QVariant FieldsModel::headerData(int section, Qt::Orientation orientation, int role /* Qt::DisplayRole */) const
{
  Q_UNUSED(orientation);

  switch (role)
  {
    case Qt::DisplayRole:
      switch (section)
      {
        case Column::Name:
          return tr("Field");
        case Column::Mark:
          return tr("Mark");
      }
    default:
      return QVariant();
  }
}

QModelIndex FieldsModel::parent(const QModelIndex &index) const
{
  Q_UNUSED(index);

  return QModelIndex();
}