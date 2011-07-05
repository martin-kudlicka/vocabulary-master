#ifndef PLUGINS_H
#define PLUGINS_H

#include <QtCore/QHash>
#include "../plugins/common/tts-interface.h"

class Plugins
{
	public:
		const void Initialize();
		const void Load();
		const void Uninitialize();

	private:
		QHash<TTSInterface::eTTSPlugin, TTSInterface *> _qhTTSPlugins;
}; // Plugins

#endif // PLUGINS_H