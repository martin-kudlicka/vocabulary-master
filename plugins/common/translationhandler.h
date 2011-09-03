#pragma once

#include <QtCore/QString>
#include <QtCore/QTranslator>
#ifdef Q_WS_WIN
# include <Windows.h>
#endif

class TranslationHandler
{
	public:
#ifdef Q_WS_WIN
		static HINSTANCE _hiInstance;
#endif

		TranslationHandler();

		virtual const void SetLanguage(const QString &pLanguage);

	private:
#ifdef Q_WS_WIN
		QString _qsPluginDir;
#endif
		QTranslator _qtTranslator;
}; // TranslationHandler