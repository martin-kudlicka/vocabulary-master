#ifndef PLUGINSMODEL_H
#define PLUGINSMODEL_H

#include <QtCore/QAbstractItemModel>
#include "plugins.h"

class PluginsModel : public QAbstractItemModel
{
  Q_OBJECT

  public:
    enum class Column
    {
      Name,
      License,
      Count
    };
    enum class PluginType
    {
      Imp,
      Exp,
      TTS
    };

             PluginsModel(const Plugins *plugins, PluginType type, QObject *parent = nullptr);
    virtual ~PluginsModel() override;

    virtual QVariant    data    (const QModelIndex &index, int role = Qt::DisplayRole)           const override;
    virtual QModelIndex index   (int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    virtual int         rowCount(const QModelIndex &parent = QModelIndex())                      const override;

  private:
          PluginType _type;
    const Plugins   *_plugins;

    Plugins::PluginInfo pluginInfo(quintptr number) const;

    virtual int         columnCount(const QModelIndex &parent = QModelIndex())                            const override;
    virtual QVariant    headerData (int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    virtual QModelIndex parent     (const QModelIndex &index)                                             const override;
};

#endif