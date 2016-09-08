#include "settingsdialog/pluginsmodel.h"

PluginsModel::PluginsModel(const Plugins *plugins, PluginType type, QObject *parent /* nullptr */) : QAbstractItemModel(parent), _plugins(plugins), _type(type)
{
}

PluginsModel::~PluginsModel()
{
}

QVariant PluginsModel::data(const QModelIndex &index, int role /* Qt::DisplayRole */) const
{
	switch (role)
	{
	case Qt::DisplayRole:
		switch (index.column())
		{
		case ColumnName:
			return pluginInfo(index.row()).libraryName;
		case ColumnLicense:
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
	if (parent == QModelIndex())
	{
		switch (_type)
		{
		case PluginTypeImp:
			return _plugins->impPlugins().size();
		case PluginTypeExp:
			return _plugins->explugins().size();
		case PluginTypeTTS:
			return _plugins->ttsPlugins().size();
		default:
			return 0;
		}
	}
	else
	{
		return 0;
	}
}

int PluginsModel::columnCount(const QModelIndex &parent /* QModelIndex() */) const
{
    return ColumnCount;
}

Plugins::PluginInfo PluginsModel::pluginInfo(quint8 number) const
{
    switch (_type)
	{
        case PluginTypeImp:
            return _plugins->impPlugins().at(number).info;
        case PluginTypeExp:
            return _plugins->explugins().at(number).info;
        case PluginTypeTTS:
            return _plugins->ttsPlugins().at(number).info;
        default:
            return Plugins::PluginInfo();
    }
}

QVariant PluginsModel::headerData(int section, Qt::Orientation orientation, int role /* Qt::DisplayRole */) const
{
    switch (role)
	{
        case Qt::DisplayRole:
            switch (section)
			{
                case ColumnName:
                    return tr("Name");
                case ColumnLicense:
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