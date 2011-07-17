#include "plugins.h"

#include <QtCore/QDir>
#include <QtCore/QCoreApplication>
#include <QtCore/QPluginLoader>

TTSInterface *Plugins::GetTTSPlugin(const TTSInterface::eTTSPlugin &pPluginId) const
{
	return _qhTTSPlugins.value(pPluginId);
} // GetTTSPlugin

const Plugins::tImpPluginList &Plugins::GetImpPlugins() const
{
	return _tiplImpPlugins;
} // GetImpPlugins

const Plugins::tTTSPluginList Plugins::GetTTSPlugins() const
{
	return _qhTTSPlugins.values();
} // GetTTSPlugins

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
			if (qsFileName.startsWith("tts-")) {
				TTSInterface *tiPlugin = qobject_cast<TTSInterface *>(qplLoader.instance());
				if (tiPlugin) {
					_qhTTSPlugins.insert(tiPlugin->GetPluginId(), tiPlugin);
				} // if
			} else {
				ImpInterface *iiPlugin = qobject_cast<ImpInterface *>(qplLoader.instance());
				if (iiPlugin) {
					_tiplImpPlugins.append(iiPlugin);
				} // if
			} // if else
		} // if
	} // foreach
} // Load

const void Plugins::Uninitialize()
{
	foreach (TTSInterface *tiPlugin, _qhTTSPlugins.values()) {
		tiPlugin->Uninitialize();
	} // foreach
} // Uninitialize