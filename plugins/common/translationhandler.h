#ifndef TRANSLATIONHANDLER_H
#define TRANSLATIONHANDLER_H

#include <QtCore/QTranslator>
#ifdef Q_OS_WIN
# include <qt_windows.h>
#endif

class TranslationHandler
{
  public:
#ifdef Q_OS_WIN
    static HINSTANCE _instance;
#endif

    TranslationHandler();

    virtual void setLanguage(const QString &language);

  private:
#ifdef Q_OS_WIN
    QString     _pluginDir;
#endif
    QTranslator _translator;
};

#endif