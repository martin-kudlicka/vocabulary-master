#ifndef PLUGINS_H
#define PLUGINS_H

#include <QtCore/QHash>
#include "../plugins/common/tts-interface.h"
#include "../plugins/common/imp-interface.h"
#include "../plugins/common/exp-interface.h"

class Plugins
{
	public:
        struct sPluginInfo {
            QString qsLibraryName;
        }; // sPluginInfo
        struct sExpPlugin {
            sPluginInfo spiInfo;
            ExpInterface *eiInterface;
        }; // sExpPlugin
        struct sImpPlugin {
            sPluginInfo spiInfo;
            ImpInterface *iiInterface;
        }; // sImpPlugin
        struct sTTSPlugin {
            sPluginInfo spiInfo;
            TTSInterface *tiInterface;
        }; // sTTSPlugin

		typedef QList<sExpPlugin> ExpPluginList;
		typedef QList<sImpPlugin> tImpPluginList;
		typedef QList<sTTSPlugin> tTTSPluginList;

		TTSInterface *GetTTSPlugin(const TTSInterface::TTSPlugin &pPluginId) const;
		const ExpPluginList &GetExpPlugins() const;
		const tImpPluginList &GetImpPlugins() const;
		const tTTSPluginList GetTTSPlugins() const;
		const void Initialize();
		const void Load();
		const void SetLanguage(const QString &pLanguage) const;
		const void Uninitialize();

	private:
		QHash<TTSInterface::TTSPlugin, sTTSPlugin> _qhTTSPlugins;
		ExpPluginList _teplExpPlugins;
		tImpPluginList _tiplImpPlugins;
}; // Plugins

#endif // PLUGINS_H