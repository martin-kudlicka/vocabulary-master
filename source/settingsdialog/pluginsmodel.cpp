#include "settingsdialog/pluginsmodel.h"

int PluginsModel::columnCount(const QModelIndex &parent /* QModelIndex() */) const
{
    return ColumnCount;
} // columnCount

QVariant PluginsModel::data(const QModelIndex &index, int role /* Qt::DisplayRole */) const
{
    switch (role) {
        case Qt::DisplayRole:
            switch (index.column()) {
                case ColumnName:
                    return GetPluginInfo(index.row()).qsLibraryName;
                case ColumnLicense:
                    return QVariant();
            } // switch
        default:
            return QVariant();
    } // switch
} // data

const Plugins::sPluginInfo PluginsModel::GetPluginInfo(const int &pNum) const
{
    switch (_eptType) {
        case PluginTypeImp:
            return _pPlugins->GetImpPlugins().at(pNum).spiInfo;
        case PluginTypeExp:
            return _pPlugins->GetExpPlugins().at(pNum).spiInfo;
        case PluginTypeTTS:
            return _pPlugins->GetTTSPlugins().at(pNum).spiInfo;
        default:
            return Plugins::sPluginInfo();
    } // switch
} // GetPluginInfo

QVariant PluginsModel::headerData(int section, Qt::Orientation orientation, int role /* Qt::DisplayRole */) const
{
    switch (role) {
        case Qt::DisplayRole:
            switch (section) {
                case ColumnName:
                    return tr("Name");
                case ColumnLicense:
                    return tr("License");
            } // switch
        default:
            return QVariant();
    } // switch
} // headerData

QModelIndex PluginsModel::index(int row, int column, const QModelIndex &parent /* QModelIndex() */) const
{
    return createIndex(row, column);
} // index

QModelIndex PluginsModel::parent(const QModelIndex &index) const
{
    return QModelIndex();
} // parent

PluginsModel::PluginsModel(const Plugins *pPlugins, const ePluginType &pType, QObject *pParent /* NULL */) : QAbstractItemModel(pParent)
{
    _pPlugins = pPlugins;
    _eptType = pType;
} // PluginsModel

int PluginsModel::rowCount(const QModelIndex &parent /* QModelIndex() */) const
{
    if (parent == QModelIndex()) {
        switch (_eptType) {
            case PluginTypeImp:
                return _pPlugins->GetImpPlugins().size();
            case PluginTypeExp:
                return _pPlugins->GetExpPlugins().size();
            case PluginTypeTTS:
                return _pPlugins->GetTTSPlugins().size();
            default:
                return 0;
        } // switch
    } else {
        return 0;
    } // if else
} // rowCount