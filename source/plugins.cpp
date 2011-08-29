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
                    sTTSPlugin stpPlugin;
                    stpPlugin.spiInfo.qsLibraryName = QFileInfo(qplLoader.fileName()).fileName();
                    stpPlugin.tiInterface = tiPlugin;
					_qhTTSPlugins.insert(tiPlugin->GetPluginId(), stpPlugin);
				} // if
			} else {
				if (qsFileName.startsWith("imp-")) {
					ImpInterface *iiPlugin = qobject_cast<ImpInterface *>(qplLoader.instance());
					if (iiPlugin) {
                        sImpPlugin sipPlugin;
                        sipPlugin.spiInfo.qsLibraryName = QFileInfo(qplLoader.fileName()).fileName();
                        sipPlugin.iiInterface = iiPlugin;
						_tiplImpPlugins.append(sipPlugin);
					} // if
				} else {
					ExpInterface *eiPlugin = qobject_cast<ExpInterface *>(qplLoader.instance());
					if (eiPlugin) {
                        sExpPlugin sepPlugin;
                        sepPlugin.spiInfo.qsLibraryName = QFileInfo(qplLoader.fileName()).fileName();
                        sepPlugin.eiInterface = eiPlugin;
						_teplExpPlugins.append(sepPlugin);
					} // if
				} // if else
			} // if else
		} // if
	} // foreach
} // Load

const void Plugins::Uninitialize()
{
	foreach (sTTSPlugin stpPlugin, _qhTTSPlugins.values()) {
        TTSInterface *tiPlugin = stpPlugin.tiInterface;
		tiPlugin->Uninitialize();
	} // foreach
} // Uninitialize