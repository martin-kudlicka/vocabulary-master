#ifndef PLUGINSMODEL_H
#define PLUGINSMODEL_H

#include <QtCore/QAbstractItemModel>
#include "plugins.h"

class PluginsModel : public QAbstractItemModel
{
    Q_OBJECT

    public:
        enum Column
		{
            ColumnName,
            ColumnLicense,
            ColumnCount
        }; // Column
        enum PluginType
		{
            PluginTypeImp,
            PluginTypeExp,
            PluginTypeTTS
        }; // PluginType

                 PluginsModel(const Plugins *plugins, const PluginType &type, QObject *parent = NULL);
		virtual ~PluginsModel();

        virtual QVariant    data    (const QModelIndex &index, int role = Qt::DisplayRole)           const;
        virtual QModelIndex index   (int row, int column, const QModelIndex &parent = QModelIndex()) const;
        virtual int         rowCount(const QModelIndex &parent = QModelIndex())                      const;

    private:
              PluginType _type;
        const Plugins   *_plugins;

        virtual       int                 columnCount(const QModelIndex &parent = QModelIndex())                            const;
        virtual       QVariant            headerData (int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
        virtual       QModelIndex         parent     (const QModelIndex &index)                                             const;
		        const Plugins::PluginInfo pluginInfo (const quint8 &number)                                                 const;
}; // PluginsModel

#endif // PLUGINSMODEL_H