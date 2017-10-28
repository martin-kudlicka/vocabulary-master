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

             PluginsModel(const Plugins *plugins, PluginType type);
    virtual ~PluginsModel() Q_DECL_OVERRIDE Q_DECL_EQ_DEFAULT;

    virtual QVariant    data    (const QModelIndex &index, int role = Qt::DisplayRole)           const Q_DECL_OVERRIDE;
    virtual QModelIndex index   (int row, int column, const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    virtual int         rowCount(const QModelIndex &parent = QModelIndex())                      const Q_DECL_OVERRIDE;

  private:
          PluginType _type;
    const Plugins   *_plugins;

    Plugins::PluginInfo pluginInfo(quintptr number) const;

    virtual int         columnCount(const QModelIndex &parent = QModelIndex())                            const Q_DECL_OVERRIDE;
    virtual QVariant    headerData (int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    virtual QModelIndex parent     (const QModelIndex &index)                                             const Q_DECL_OVERRIDE;
};

#endif