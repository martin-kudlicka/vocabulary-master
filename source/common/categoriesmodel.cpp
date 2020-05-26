#include "pch.h"
#include "common/categoriesmodel.h"

#include "vocabulary.h"

CategoriesModel::CategoriesModel(const QSharedPointer<Vocabulary> &vocabulary) : _vocabulary(vocabulary)
{
}

int CategoriesModel::columnCount(const QModelIndex &parent /* QModelIndex() */) const
{
  Q_UNUSED(parent);

  return static_cast<int>(Column::Count);
}

QVariant CategoriesModel::data(const QModelIndex &index, int role /* Qt::DisplayRole */) const
{
  switch (role)
  {
    case Qt::DisplayRole:
    {
      const auto categoryId = _vocabulary->categoryId(index.row());
      return _vocabulary->categoryName(categoryId);
    }
    default:
      return QVariant();
    }
}

QVariant CategoriesModel::headerData(int section, Qt::Orientation orientation, int role /* Qt::DisplayRole */) const
{
  Q_UNUSED(section);
  Q_UNUSED(orientation);

  switch (role)
  {
    case Qt::DisplayRole:
      return tr("Category");
    default:
      return QVariant();
  }
}

QModelIndex CategoriesModel::index(int row, int column, const QModelIndex &parent /* QModelIndex() */) const
{
  Q_UNUSED(parent);

  return createIndex(row, column);
}

QModelIndex CategoriesModel::parent(const QModelIndex &index) const
{
  Q_UNUSED(index);

  return QModelIndex();
}

int CategoriesModel::rowCount(const QModelIndex &parent /* QModelIndex() */) const
{
  if (parent.isValid())
  {
    return 0;
  }

  return _vocabulary->categoryCount();
}