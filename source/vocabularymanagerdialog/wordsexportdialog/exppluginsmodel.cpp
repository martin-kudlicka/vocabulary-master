#include "pch.h"
#include "vocabularymanagerdialog/wordsexportdialog/exppluginsmodel.h"

#include "../../../plugins/common/exp-interface.h"

ExpPluginsModel::ExpPluginsModel(const Plugins::ExpPluginList *expPlugins) : _expPlugins(expPlugins)
{
}

int ExpPluginsModel::columnCount(const QModelIndex &parent /* QModelIndex() */) const
{
  Q_UNUSED(parent);

  return static_cast<int>(Column::Count);
}

QVariant ExpPluginsModel::data(const QModelIndex &index, int role /* Qt::DisplayRole */) const
{
  switch (role)
  {
    case Qt::DisplayRole:
      return _expPlugins->at(index.row()).expInterface->pluginName();
    default:
      return QVariant();
  }
}

QVariant ExpPluginsModel::headerData(int section, Qt::Orientation orientation, int role /* Qt::DisplayRole */) const
{
  Q_UNUSED(section);
  Q_UNUSED(orientation);

  switch (role)
  {
    case Qt::DisplayRole:
      return tr("Plugin");
    default:
      return QVariant();
  }
}

QModelIndex ExpPluginsModel::index(int row, int column, const QModelIndex &parent /* QModelIndex() */) const
{
  Q_UNUSED(parent);

  return createIndex(row, column);
}

QModelIndex ExpPluginsModel::parent(const QModelIndex &index) const
{
  Q_UNUSED(index);

  return QModelIndex();
}

int ExpPluginsModel::rowCount(const QModelIndex &parent /* QModelIndex() */) const
{
  if (parent.isValid())
  {
    return 0;
  }
  else
  {
    return _expPlugins->count();
  }
}