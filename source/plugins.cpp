#include "plugins.h"

#include "../plugins/common/imp-interface.h"
#include "../plugins/common/exp-interface.h"
#include <QtCore/QDir>
#include <QtCore/QCoreApplication>
#include <QtCore/QPluginLoader>

const Plugins::ExpPluginList &Plugins::explugins() const
{
  return _expPlugins;
}

const Plugins::ImpPluginList &Plugins::impPlugins() const
{
  return _impPlugins;
}

void Plugins::initialize()
{
  for (const TTSPlugin &ttsPlugin : _ttsPlugins.values())
  {
    auto ttsInterface = ttsPlugin.ttsInterface;
    ttsInterface->initialize();
  }
}

void Plugins::load()
{
#ifdef _DEBUG
  const QDir pluginDir(QCoreApplication::applicationDirPath());
#else
  const QDir pluginsDir(QCoreApplication::applicationDirPath());
  pluginsDir.cd(DIR_PLUGINS);

  for (const auto &pluginDir : pluginsDir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot))
  {
    const QDir pluginDir(pluginDir.filePath());
#endif
    for (const auto &plugin : pluginDir.entryInfoList(QDir::Files))
    {
      QPluginLoader pluginLoader(plugin.filePath());
      if (pluginLoader.instance())
      {
        if (plugin.fileName().startsWith("tts-"))
        {
          auto ttsInterface = qobject_cast<TTSInterface *>(pluginLoader.instance());
          if (ttsInterface)
          {
            TTSPlugin ttsPlugin;
            ttsPlugin.info.libraryName = plugin.fileName();
            ttsPlugin.ttsInterface     = ttsInterface;
            _ttsPlugins.insert(static_cast<quintptr>(ttsInterface->pluginId()), ttsPlugin);
          }
        }
        else
        {
          if (plugin.fileName().startsWith("imp-"))
          {
            auto impInterface = qobject_cast<ImpInterface *>(pluginLoader.instance());
            if (impInterface)
            {
              ImpPlugin impPlugin;
              impPlugin.info.libraryName = plugin.fileName();
              impPlugin.impInterface     = impInterface;
              _impPlugins.append(impPlugin);
            }
          }
          else
          {
            auto expInterface = qobject_cast<ExpInterface *>(pluginLoader.instance());
            if (expInterface)
            {
              ExpPlugin expPlugin;
              expPlugin.info.libraryName = plugin.fileName();
              expPlugin.expInterface     = expInterface;
              _expPlugins.append(expPlugin);
            }
          }
        }
      }
    }
#ifndef _DEBUG
  }
#endif
}

void Plugins::setLanguage(const QString &language) const
{
  for (const auto &impPlugin : _impPlugins)
  {
    impPlugin.impInterface->setLanguage(language);
  }
  for (const auto &expPlugin : _expPlugins)
  {
    expPlugin.expInterface->setLanguage(language);
  }
}

TTSInterface *Plugins::ttsPlugin(TTSInterface::TTSPlugin pluginId) const
{
  return _ttsPlugins.value(static_cast<quintptr>(pluginId)).ttsInterface;
}

const Plugins::TTSPluginList Plugins::ttsPlugins() const
{
  return _ttsPlugins.values();
}

void Plugins::uninitialize()
{
  for (const auto &ttsPlugin : _ttsPlugins.values())
  {
    auto ttsInterface = ttsPlugin.ttsInterface;
    ttsInterface->uninitialize();
  }
}