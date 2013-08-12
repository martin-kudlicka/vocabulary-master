#ifndef TRANSLATIONHANDLER_H
#define TRANSLATIONHANDLER_H

#include <QtCore/QString>
#include <QtCore/QTranslator>
#ifdef Q_OS_WIN
# include <qt_windows.h>
#endif

class TranslationHandler
{
	public:
#ifdef Q_OS_WIN
		static HINSTANCE _hiInstance;
#endif

		TranslationHandler();

		virtual const void SetLanguage(const QString &pLanguage);

	private:
#ifdef Q_OS_WIN
		QString _qsPluginDir;
#endif
		QTranslator _qtTranslator;
}; // TranslationHandler

#endif // TRANSLATIONHANDLER_H