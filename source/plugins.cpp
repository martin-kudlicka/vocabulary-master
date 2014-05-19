#include "plugins.h"

#include <QtCore/QDir>
#include <QtCore/QCoreApplication>
#include <QtCore/QPluginLoader>

const Plugins::ExpPluginList &Plugins::explugins() const
{
	return _expPlugins;
} // explugins

const Plugins::ImpPluginList &Plugins::impPlugins() const
{
	return _impPlugins;
} // impPlugins

const void Plugins::initialize()
{
	foreach (const TTSPlugin &ttsPlugin, _ttsPlugins.values())
	{
        TTSInterface *ttsInterface = ttsPlugin.ttsInterface;
		ttsInterface->initialize();
	} // foreach
} // initialize

const void Plugins::load()
{
#ifdef _DEBUG
	const QDir pluginDir(QCoreApplication::applicationDirPath());
#else
	const QDir pluginsDir(QCoreApplication::applicationDirPath());
	pluginsDir.cd(DIR_PLUGINS);

	foreach (const QFileInfo &pluginDir, pluginsDir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot))
	{
		const QDir pluginDir(pluginDir.filePath());
#endif
		foreach (const QFileInfo &plugin, pluginDir.entryInfoList(QDir::Files))
		{
			QPluginLoader pluginLoader(plugin.filePath());
			if (pluginLoader.instance())
			{
				if (plugin.fileName().startsWith("tts-"))
				{
					TTSInterface *ttsInterface = qobject_cast<TTSInterface *>(pluginLoader.instance());
					if (ttsInterface)
					{
						TTSPlugin ttsPlugin;
						ttsPlugin.info.libraryName = plugin.fileName();
						ttsPlugin.ttsInterface     = ttsInterface;
						_ttsPlugins.insert(ttsInterface->pluginId(), ttsPlugin);
					} // if
				}
				else
				{
					if (plugin.fileName().startsWith("imp-"))
					{
						ImpInterface *impInterface = qobject_cast<ImpInterface *>(pluginLoader.instance());
						if (impInterface)
						{
							ImpPlugin impPlugin;
							impPlugin.info.libraryName = plugin.fileName();
							impPlugin.impInterface     = impInterface;
							_impPlugins.append(impPlugin);
						} // if
					} else {
						ExpInterface *expInterface = qobject_cast<ExpInterface *>(pluginLoader.instance());
						if (expInterface)
						{
							ExpPlugin expPlugin;
							expPlugin.info.libraryName = plugin.fileName();
							expPlugin.expInterface     = expInterface;
							_expPlugins.append(expPlugin);
						} // if
					} // if else
				} // if else
			} // if
		} // foreach
#ifndef _DEBUG
	} // foreach
#endif
} // load

const void Plugins::setLanguage(const QString &language) const
{
	foreach (const ImpPlugin &impPlugin, _impPlugins)
	{
		impPlugin.impInterface->setLanguage(language);
	} // foreach
	foreach (const ExpPlugin &expPlugin, _expPlugins)
	{
		expPlugin.expInterface->setLanguage(language);
	} // foreach
} // setLanguage

TTSInterface *Plugins::ttsPlugin(const TTSInterface::TTSPlugin &pPluginId) const
{
	return _ttsPlugins.value(pPluginId).ttsInterface;
} // ttsPlugin

const Plugins::TTSPluginList Plugins::ttsPlugins() const
{
	return _ttsPlugins.values();
} // ttsPlugins

const void Plugins::uninitialize()
{
	foreach (const TTSPlugin &ttsPlugin, _ttsPlugins.values())
	{
        TTSInterface *ttsInterface = ttsPlugin.ttsInterface;
		ttsInterface->uninitialize();
	} // foreach
} // uninitialize