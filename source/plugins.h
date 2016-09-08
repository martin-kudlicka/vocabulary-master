#ifndef PLUGINS_H
#define PLUGINS_H

#include <QtCore/QHash>
#include "../plugins/common/tts-interface.h"
#include "../plugins/common/imp-interface.h"
#include "../plugins/common/exp-interface.h"

class Plugins
{
	public:
        struct PluginInfo
		{
            QString libraryName;
        }; // PluginInfo
        struct ExpPlugin
		{
            PluginInfo    info;
            ExpInterface *expInterface;
        }; // ExpPlugin
        struct ImpPlugin
		{
            PluginInfo    info;
            ImpInterface *impInterface;
        }; // ImpPlugin
        struct TTSPlugin
		{
            PluginInfo    info;
            TTSInterface *ttsInterface;
        }; // TTSPlugin

		typedef QList<ExpPlugin> ExpPluginList;
		typedef QList<ImpPlugin> ImpPluginList;
		typedef QList<TTSPlugin> TTSPluginList;

		const ExpPluginList &explugins   ()                                        const;
		const ImpPluginList &impPlugins  ()                                        const;
		      void           initialize  ();
		      void           load        ();
		      void           setLanguage (const QString &language)                 const;
		      TTSInterface  *ttsPlugin   (const TTSInterface::TTSPlugin &pluginId) const;
		const TTSPluginList  ttsPlugins  ()                                        const;
		      void           uninitialize();

	private:
		ExpPluginList              _expPlugins;
		ImpPluginList              _impPlugins;
		QHash<quintptr, TTSPlugin> _ttsPlugins;
}; // Plugins

#endif // PLUGINS_H