#ifndef EXPPLUGINSMODEL_H
#define EXPPLUGINSMODEL_H

#include <QtCore/QAbstractItemModel>
#include "plugins.h"

class ExpPluginsModel : public QAbstractItemModel
{
    Q_OBJECT

    public:
        ExpPluginsModel(const Plugins::tExpPluginList *pExpPlugins, QObject *pParent = NULL);

    private:
		enum eColumn {
			ColumnName,
			ColumnCount
		}; // eColumn

        const Plugins::tExpPluginList *_teplExpPlugins;

        virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
        virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
        virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
		virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
        virtual QModelIndex parent(const QModelIndex &index) const;
		virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
}; // ExpPluginsModel

#endif // EXPPLUGINSMODEL_H