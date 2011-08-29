#ifndef PLUGINSMODEL_H
#define PLUGINSMODEL_H

#include <QtCore/QAbstractItemModel>
#include "plugins.h"

class PluginsModel : public QAbstractItemModel
{
    Q_OBJECT
    Q_ENUMS(ePluginType)

    public:
        enum eColumn {
            ColumnName,
            ColumnLicense,
            ColumnCount
        }; // eColumn
        enum ePluginType {
            PluginTypeImp,
            PluginTypeExp,
            PluginTypeTTS
        }; // ePluginType

        PluginsModel(const Plugins *pPlugins, const ePluginType &pType, QObject *pParent = NULL);

        virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
        virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
        virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;

    private:
        ePluginType _eptType;
        const Plugins *_pPlugins;

        virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
        const Plugins::sPluginInfo GetPluginInfo(const int &pNum) const;
        virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
        virtual QModelIndex parent(const QModelIndex &index) const;
}; // PluginsModel

#endif // PLUGINSMODEL_H