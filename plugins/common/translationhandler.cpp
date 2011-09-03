#include "translationhandler.h"

#include <QtCore/QLocale>
#include <QtCore/QCoreApplication>
#ifdef Q_WS_WIN
# include <QtCore/QDir>
#endif

HINSTANCE TranslationHandler::_hiInstance;

const QString DIR_LANG = "lang";

#ifdef Q_WS_WIN
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	if (fdwReason == DLL_PROCESS_ATTACH) {
		TranslationHandler::_hiInstance = hinstDLL;
	} // if

	return TRUE;
} // DllMain
#endif

const void TranslationHandler::SetLanguage(const QString &pLanguage)
{
	if (!_qtTranslator.load(pLanguage,
#ifdef Q_WS_WIN
		_qsPluginDir +
#endif
		DIR_LANG)) {
		if (pLanguage.isEmpty()) {
			_qtTranslator.load(QLocale::system().name(), DIR_LANG);
		} // if
	} // if
} // SetLanguage

TranslationHandler::TranslationHandler()
{
	QCoreApplication::installTranslator(&_qtTranslator);

#ifdef Q_WS_WIN
	TCHAR tcFileName[MAX_PATH + 1];
	GetModuleFileName(_hiInstance, tcFileName, sizeof(tcFileName) / sizeof(TCHAR));
	QString qsPluginPath = QString::fromWCharArray(tcFileName);

	_qsPluginDir = qsPluginPath.left(qsPluginPath.lastIndexOf(QDir::separator()) + 1);
#endif
} // TranslationHandler