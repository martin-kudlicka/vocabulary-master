#ifndef PLUGINS_H
#define PLUGINS_H

#include <QtCore/QHash>
#include "../plugins/common/tts-interface.h"
#include "../plugins/common/imp-interface.h"

class Plugins
{
	public:
		typedef QList<ImpInterface *> tImpPluginList;
		typedef QList<TTSInterface *> tTTSPluginList;

		TTSInterface *GetTTSPlugin(const TTSInterface::eTTSPlugin &pPluginId) const;
		const tImpPluginList &GetImpPlugins() const;
		const tTTSPluginList GetTTSPlugins() const;
		const void Initialize();
		const void Load();
		const void Uninitialize();

	private:
		QHash<TTSInterface::eTTSPlugin, TTSInterface *> _qhTTSPlugins;
		tImpPluginList _tiplImpPlugins;
}; // Plugins

#endif // PLUGINS_H