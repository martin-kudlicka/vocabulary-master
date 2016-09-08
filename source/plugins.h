#ifndef PLUGINS_H
#define PLUGINS_H

#include <QtCore/QHash>
#include "../plugins/common/tts-interface.h"

class ExpInterface;
class ImpInterface;

class Plugins
{
  public:
    struct PluginInfo
    {
      QString libraryName;
    };
    struct ExpPlugin
    {
      PluginInfo    info;
      ExpInterface *expInterface;
    };
    struct ImpPlugin
    {
      PluginInfo    info;
      ImpInterface *impInterface;
    };
    struct TTSPlugin
    {
      PluginInfo    info;
      TTSInterface *ttsInterface;
    };

    typedef QList<ExpPlugin> ExpPluginList;
    typedef QList<ImpPlugin> ImpPluginList;
    typedef QList<TTSPlugin> TTSPluginList;

    const ExpPluginList &explugins   ()                                 const;
    const ImpPluginList &impPlugins  ()                                 const;
          void           initialize  ();
          void           load        ();
          void           setLanguage (const QString &language)          const;
          TTSInterface  *ttsPlugin   (TTSInterface::TTSPlugin pluginId) const;
    const TTSPluginList  ttsPlugins  ()                                 const;
          void           uninitialize();

  private:
    ExpPluginList              _expPlugins;
    ImpPluginList              _impPlugins;
    QHash<quintptr, TTSPlugin> _ttsPlugins;
};

#endif