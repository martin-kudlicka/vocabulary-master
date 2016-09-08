#include "translationhandler.h"

#include <QtCore/QLocale>
#include <QtCore/QCoreApplication>
#ifdef Q_OS_WIN
# include <QtCore/QDir>
#endif

HINSTANCE TranslationHandler::_instance = nullptr;

const auto DIR_LANG = "lang";

#ifdef Q_OS_WIN
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
  if (fdwReason == DLL_PROCESS_ATTACH)
  {
    TranslationHandler::_instance = hinstDLL;
  }

  return TRUE;
}
#endif

TranslationHandler::TranslationHandler()
{
  QCoreApplication::installTranslator(&_translator);

#ifdef Q_OS_WIN
  TCHAR fileName[MAX_PATH];
  GetModuleFileName(_instance, fileName, _countof(fileName));
  const auto pluginPath = QString::fromWCharArray(fileName);

  _pluginDir = pluginPath.left(pluginPath.lastIndexOf(QDir::separator()) + 1);
#endif
}

void TranslationHandler::setLanguage(const QString &language)
{
  if (!_translator.load(language,
#ifdef Q_OS_WIN
    _pluginDir +
#endif
    DIR_LANG))
  {
    if (language.isEmpty())
    {
      _translator.load(QLocale::system().name(), DIR_LANG);
    }
  }
}