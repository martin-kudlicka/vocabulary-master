#include "plugins.h"

#include <QtCore/QDir>
#include <QtCore/QCoreApplication>
#include <QtCore/QPluginLoader>

TTSInterface *Plugins::GetTTSPlugin(const TTSInterface::eTTSPlugin &pPluginId) const
{
	return _qhTTSPlugins.value(pPluginId).tiInterface;
} // GetTTSPlugin

const Plugins::tExpPluginList &Plugins::GetExpPlugins() const
{
	return _teplExpPlugins;
} // GetExpPlugins

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
	foreach (sTTSPlugin stpPlugin, _qhTTSPlugins.values()) {
        TTSInterface *tiPlugin = stpPlugin.tiInterface;
		tiPlugin->Initialize();
	} // foreach
} // Initialize

const void Plugins::Load()
{
#ifdef _DEBUG
	QDir qdPluginDir(QCoreApplication::applicationDirPath());
#else
	QDir qdPlugins(QCoreApplication::applicationDirPath());
	qdPlugins.cd(DIR_PLUGINS);

	foreach (QFileInfo qfiPluginDir, qdPlugins.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot)) {
		QDir qdPluginDir(qfiPluginDir.filePath());
#endif
		foreach (QFileInfo qfiPlugin, qdPluginDir.entryInfoList(QDir::Files)) {
			QPluginLoader qplLoader(qfiPlugin.filePath());
			if (qplLoader.instance()) {
				if (qfiPlugin.fileName().startsWith("tts-")) {
					TTSInterface *tiPlugin = qobject_cast<TTSInterface *>(qplLoader.instance());
					if (tiPlugin) {
						sTTSPlugin stpPlugin;
						stpPlugin.spiInfo.qsLibraryName = qfiPlugin.fileName();
						stpPlugin.tiInterface = tiPlugin;
						_qhTTSPlugins.insert(tiPlugin->GetPluginId(), stpPlugin);
					} // if
				} else {
					if (qfiPlugin.fileName().startsWith("imp-")) {
						ImpInterface *iiPlugin = qobject_cast<ImpInterface *>(qplLoader.instance());
						if (iiPlugin) {
							sImpPlugin sipPlugin;
							sipPlugin.spiInfo.qsLibraryName = qfiPlugin.fileName();
							sipPlugin.iiInterface = iiPlugin;
							_tiplImpPlugins.append(sipPlugin);
						} // if
					} else {
						ExpInterface *eiPlugin = qobject_cast<ExpInterface *>(qplLoader.instance());
						if (eiPlugin) {
							sExpPlugin sepPlugin;
							sepPlugin.spiInfo.qsLibraryName = qfiPlugin.fileName();
							sepPlugin.eiInterface = eiPlugin;
							_teplExpPlugins.append(sepPlugin);
						} // if
					} // if else
				} // if else
			} // if
		} // foreach
#ifndef _DEBUG
	} // foreach
#endif
} // Load

const void Plugins::SetLanguage(const QString &pLanguage) const
{
	foreach (sImpPlugin sipPlugin, _tiplImpPlugins) {
		sipPlugin.iiInterface->SetLanguage(pLanguage);
	} // foreach
	foreach (sExpPlugin sepPlugin, _teplExpPlugins) {
		sepPlugin.eiInterface->SetLanguage(pLanguage);
	} // foreach
} // SetLanguage

const void Plugins::Uninitialize()
{
	foreach (sTTSPlugin stpPlugin, _qhTTSPlugins.values()) {
        TTSInterface *tiPlugin = stpPlugin.tiInterface;
		tiPlugin->Uninitialize();
	} // foreach
} // Uninitialize