#include "pch.h"
#include "codecsmodel.h"

CodecsModel::CodecsModel() : _codecs(QTextCodec::availableCodecs())
{
}

QVariant CodecsModel::data(const QModelIndex &index, int role /* Qt::DisplayRole */) const
{
  QVariant indexData;

  if (role == Qt::DisplayRole)
  {
    indexData.setValue(_codecs.at(index.row()));
  }

  return indexData;
}

QModelIndex CodecsModel::index(int row, int column, const QModelIndex &parent /* QModelIndex() */) const
{
  Q_UNUSED(parent);

  return createIndex(row, column);
}

int CodecsModel::rowCount(const QModelIndex &parent /* QModelIndex() */) const
{
  if (parent.isValid())
  {
    return 0;
  }
  else
  {
    return _codecs.count();
  }
}

int CodecsModel::columnCount(const QModelIndex &parent /* QModelIndex() */) const
{
  Q_UNUSED(parent);

  return static_cast<int>(Column::Count);
}

QVariant CodecsModel::headerData(int section, Qt::Orientation orientation, int role /* Qt::DisplayRole */) const
{
  Q_UNUSED(section);
  Q_UNUSED(orientation);

  QVariant sectionHeader;

  if (role == Qt::DisplayRole)
  {
    sectionHeader.setValue(tr("Codec"));
  }

  return sectionHeader;
}

QModelIndex CodecsModel::parent(const QModelIndex &index) const
{
  Q_UNUSED(index);

  return QModelIndex();
}