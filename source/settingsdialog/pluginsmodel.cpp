#include "settingsdialog/pluginsmodel.h"

PluginsModel::PluginsModel(const Plugins *plugins, PluginType type) : _plugins(plugins), _type(type)
{
}

QVariant PluginsModel::data(const QModelIndex &index, int role /* Qt::DisplayRole */) const
{
  switch (role)
  {
    case Qt::DisplayRole:
      switch (index.column())
      {
        case Column::Name:
          return pluginInfo(index.row()).libraryName;
        case Column::License:
          return QVariant();
      }
    default:
      return QVariant();
  }
}

QModelIndex PluginsModel::index(int row, int column, const QModelIndex &parent /* QModelIndex() */) const
{
  return createIndex(row, column);
}

int PluginsModel::rowCount(const QModelIndex &parent /* QModelIndex() */) const
{
  if (parent.isValid())
  {
    return 0;
  }
  else
  {
    switch (_type)
    {
      case PluginType::Imp:
        return _plugins->impPlugins().count();
      case PluginType::Exp:
        return _plugins->explugins().count();
      case PluginType::TTS:
        return _plugins->ttsPlugins().count();
      default:
        return 0;
    }
  }
}

Plugins::PluginInfo PluginsModel::pluginInfo(quintptr number) const
{
  switch (_type)
  {
    case PluginType::Imp:
      return _plugins->impPlugins().at(number).info;
    case PluginType::Exp:
      return _plugins->explugins().at(number).info;
    case PluginType::TTS:
      return _plugins->ttsPlugins().at(number).info;
    default:
      return Plugins::PluginInfo();
  }
}

int PluginsModel::columnCount(const QModelIndex &parent /* QModelIndex() */) const
{
  return static_cast<int>(Column::Count);
}

QVariant PluginsModel::headerData(int section, Qt::Orientation orientation, int role /* Qt::DisplayRole */) const
{
  switch (role)
  {
    case Qt::DisplayRole:
      switch (section)
      {
        case Column::Name:
          return tr("Name");
        case Column::License:
          return tr("License");
      }
    default:
      return QVariant();
  }
}

QModelIndex PluginsModel::parent(const QModelIndex &index) const
{
  return QModelIndex();
}