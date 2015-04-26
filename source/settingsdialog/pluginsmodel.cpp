#include "settingsdialog/pluginsmodel.h"

PluginsModel::PluginsModel(const Plugins *plugins, PluginType type, QObject *parent /* nullptr */) : QAbstractItemModel(parent), _plugins(plugins), _type(type)
{
} // PluginsModel

PluginsModel::~PluginsModel()
{
} // ~PluginsModel

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
		} // switch
	default:
		return QVariant();
	} // switch
} // data

QModelIndex PluginsModel::index(int row, int column, const QModelIndex &parent /* QModelIndex() */) const
{
	return createIndex(row, column);
} // index

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
		} // switch
	}
	else
	{
		return 0;
	} // if else
} // rowCount

int PluginsModel::columnCount(const QModelIndex &parent /* QModelIndex() */) const
{
    return ColumnCount;
} // columnCount

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
    } // switch
} // pluginInfo

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
            } // switch
        default:
            return QVariant();
    } // switch
} // headerData

QModelIndex PluginsModel::parent(const QModelIndex &index) const
{
    return QModelIndex();
} // parent