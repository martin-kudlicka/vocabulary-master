#ifndef EXPPLUGINSMODEL_H
#define EXPPLUGINSMODEL_H

#include <QtCore/QAbstractItemModel>
#include "plugins.h"

class ExpPluginsModel : public QAbstractItemModel
{
  Q_OBJECT

  public:
             ExpPluginsModel(const Plugins::ExpPluginList *expPlugins, QObject *parent = Q_NULLPTR);
    virtual ~ExpPluginsModel() Q_DECL_OVERRIDE;

  private:
    enum class Column
    {
      Name,
      Count
    };

    const Plugins::ExpPluginList *_expPlugins;

    virtual int         columnCount(const QModelIndex &parent = QModelIndex())                            const Q_DECL_OVERRIDE;
    virtual QVariant    data       (const QModelIndex &index, int role = Qt::DisplayRole)                 const Q_DECL_OVERRIDE;
    virtual QVariant    headerData (int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    virtual QModelIndex index      (int row, int column, const QModelIndex &parent = QModelIndex())       const Q_DECL_OVERRIDE;
    virtual QModelIndex parent     (const QModelIndex &index)                                             const Q_DECL_OVERRIDE;
    virtual int         rowCount   (const QModelIndex &parent = QModelIndex())                            const Q_DECL_OVERRIDE;
};

#endif