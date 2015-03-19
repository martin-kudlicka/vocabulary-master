#ifndef EXPPLUGINSMODEL_H
#define EXPPLUGINSMODEL_H

#include <QtCore/QAbstractItemModel>
#include "plugins.h"

class ExpPluginsModel : public QAbstractItemModel
{
    Q_OBJECT

    public:
                 ExpPluginsModel(const Plugins::ExpPluginList *expPlugins, QObject *parent = NULL);
		virtual ~ExpPluginsModel() override;

    private:
		enum Column
		{
			ColumnName,
			ColumnCount
		}; // Column

        const Plugins::ExpPluginList *_expPlugins;

		virtual int         columnCount(const QModelIndex &parent = QModelIndex())                            const override;
        virtual QVariant    data       (const QModelIndex &index, int role = Qt::DisplayRole)                 const override;
        virtual QVariant    headerData (int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
		virtual QModelIndex index      (int row, int column, const QModelIndex &parent = QModelIndex())       const override;
        virtual QModelIndex parent     (const QModelIndex &index)                                             const override;
		virtual int         rowCount   (const QModelIndex &parent = QModelIndex())                            const override;
}; // ExpPluginsModel

#endif // EXPPLUGINSMODEL_H