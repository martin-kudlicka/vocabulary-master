#include "codecsmodel.h"

#include <QtCore/QTextCodec>

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
  return createIndex(row, column);
}

int CodecsModel::rowCount(const QModelIndex &parent /* QModelIndex() */) const
{
  if (parent == QModelIndex())
  {
    return _codecs.count();
  }
  else
  {
    return 0;
  }
}

int CodecsModel::columnCount(const QModelIndex &parent /* QModelIndex() */) const
{
  return static_cast<int>(Column::Count);
}

QVariant CodecsModel::headerData(int section, Qt::Orientation orientation, int role /* Qt::DisplayRole */) const
{
  QVariant sectionHeader;

  if (role == Qt::DisplayRole)
  {
    sectionHeader.setValue(tr("Codec"));
  }

  return sectionHeader;
}

QModelIndex CodecsModel::parent(const QModelIndex &index) const
{
  return QModelIndex();
}