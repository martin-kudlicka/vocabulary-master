#include "plugins.h"

#include <QtCore/QDir>
#include <QtCore/QCoreApplication>
#include <QtCore/QPluginLoader>

#ifndef _DEBUG
const QString DIR_PLUGINS = "plugins";
#endif

TTSInterface *Plugins::GetPlugin(const TTSInterface::eTTSPlugin &pPluginId) const
{
	return _qhTTSPlugins.value(pPluginId);
} // GetPlugin

const Plugins::tKeyList Plugins::GetPluginList() const
{
	return _qhTTSPlugins.keys();
} // GetPluginList

const void Plugins::Initialize()
{
	foreach (TTSInterface *tiPlugin, _qhTTSPlugins.values()) {
		tiPlugin->Initialize();
	} // foreach
} // Initialize

const void Plugins::Load()
{
	QDir qdPlugins(QCoreApplication::applicationDirPath());
#ifndef _DEBUG
	qdPlugins.cd(DIR_PLUGINS);
#endif
	foreach (QString qsFileName, qdPlugins.entryList(QDir::Files)) {
		QPluginLoader qplLoader(qdPlugins.absoluteFilePath(qsFileName));
		if (qplLoader.instance()) {
			TTSInterface *tiPlugin = qobject_cast<TTSInterface *>(qplLoader.instance());
			if (tiPlugin) {
				_qhTTSPlugins.insert(tiPlugin->GetPluginId(), tiPlugin);
			} // if
		} // if
	} // foreach
} // Load

const void Plugins::Uninitialize()
{
	foreach (TTSInterface *tiPlugin, _qhTTSPlugins.values()) {
		tiPlugin->Uninitialize();
	} // foreach
} // Uninitialize