#include "vocabularymanagerdialog/wordsexportdialog/exppluginsmodel.h"

ExpPluginsModel::ExpPluginsModel(const Plugins::ExpPluginList *expPlugins, QObject *parent /* nullptr */) : QAbstractItemModel(parent), _expPlugins(expPlugins)
{
}

ExpPluginsModel::~ExpPluginsModel()
{
}

int ExpPluginsModel::columnCount(const QModelIndex &parent /* QModelIndex() */) const
{
    return ColumnCount;
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
    return createIndex(row, column);
}

QModelIndex ExpPluginsModel::parent(const QModelIndex &index) const
{
    return QModelIndex();
}

int ExpPluginsModel::rowCount(const QModelIndex &parent /* QModelIndex() */) const
{
    if (parent == QModelIndex())
	{
        return _expPlugins->size();
    }
	else
	{
        return 0;
    }
}