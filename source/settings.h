#ifndef SETTINGS_H
#define SETTINGS_H

#include <QtCore/QSettings>
#include <QtNetwork/QNetworkProxy>

const auto DIR_LANG = "lang";

class Settings
{
  public:
#ifndef EDITION_FREE
# ifdef Q_OS_WIN
    enum class Hotkey
    {
      Answer,
      Minimize,
      Next,
      Restore,
      Count
    };
# endif
    enum class NewWordSoundType
    {
      System,
      Custom
    };
#endif

#ifndef EDITION_FREE
# ifdef Q_OS_WIN
    struct HotkeyInfo
    {
      QString  text;
      quintptr virtualKey;
    };
# endif
#endif
    struct VocabularyInfo
    {
#ifndef EDITION_TRY
      QString filePath;
#endif
#if !defined(EDITION_FREE) && !defined(EDITION_TRY)
      bool    enabled;
#endif
    };

    static const auto DEFAULT_DIMENSION             = 0;
#ifdef EDITION_FREE
    static const auto EDITION_FREE_VOCABULARIES_MAX = 1;
#endif

    Settings();

    bool                     alwaysOnTop                 ()            const;
#ifndef EDITION_TRY
    bool                     cacheVocabulary             ()            const;
#endif
#ifndef EDITION_FREE
    bool                     canChangeCategoryPriority   ()            const;
    bool                     canEnableCategories         ()            const;
    QString                  colorFlash                  ()            const;
    bool                     horizontalLayout            ()            const;
# ifdef Q_OS_WIN
    HotkeyInfo               hotkey                      (Hotkey type) const;
# endif
    bool                     learnDisabledWords          ()            const;
    QByteArray               license                     ()            const;
    bool                     minimizeToTray              ()            const;
    bool                     mute                        ()            const;
    bool                     newWordFlash                ()            const;
    bool                     newWordSound                ()            const;
    QString                  newWordSoundFile            ()            const;
    NewWordSoundType         newWordSoundType            ()            const;
#endif
    QString                  proxyHostname               ()            const;
    QString                  proxyPassword               ()            const;
    quintptr                 proxyPort                   ()            const;
    QNetworkProxy::ProxyType proxyType                   ()            const;
    QString                  proxyUsername               ()            const;
#ifndef EDITION_TRY
    quintptr                 recordsToCache              ()            const;
#endif
#ifndef EDITION_FREE
    bool                     rememberWindowPosition      ()            const;
#endif
    void                     setAlwaysOnTop              (bool enable);
#ifndef EDITION_TRY
    void                     setCacheVocabulary          (bool enable);
#endif
#ifndef EDITION_FREE
    void                     setCanChangeCategoryPriority(bool enable);
    void                     setCanEnableCategories      (bool enable);
    void                     setColorFlash               (const QString &color);
    void                     setHorizontalLayout         (bool enable);
# ifdef Q_OS_WIN
    void                     setHotkey                   (Hotkey type, const HotkeyInfo &hotkeyInfo);
# endif
    void                     setLearnDisabledWords       (bool enable);
    void                     setLicense                  (const QByteArray &licenseData);
    void                     setMinimizeToTray           (bool enable);
    void                     setMute                     (bool enable);
    void                     setNewWordFlash             (bool enable);
    void                     setNewWordSound             (bool enable);
    void                     setNewWordSoundFile         (const QString &file);
    void                     setNewWordSoundType         (NewWordSoundType type);
#endif
    void                     setProxyHostname            (const QString &hostname);
    void                     setProxyPassword            (const QString &password);
    void                     setProxyPort                (quintptr port);
    void                     setProxyType                (QNetworkProxy::ProxyType type);
    void                     setProxyUsername            (const QString &username);
#ifndef EDITION_TRY
    void                     setRecordsToCache           (quintptr count);
#endif
#ifndef EDITION_FREE
    void                     setRememberWindowPosition   (bool enable);
    void                     setShowCategoryName         (bool show);
    void                     setShowLanguageNames        (bool show);
    void                     setShowRecordControls       (bool show);
    void                     setShowStatusBar            (bool show);
    void                     setShowToolBar              (bool show);
    void                     setShowWordsInTrayBalloon   (bool enable);
    void                     setStartLearningOnStartup   (bool enable);
#endif
    void                     setSwitchLearningDirection  (Qt::CheckState switchDirection);
#ifndef EDITION_FREE
    void                     setSystemTrayIcon           (bool enable);
#endif
    void                     setTranslation              (const QString &translationName);
    void                     setUpdateCheck              (bool check);
    void                     setUseProxy                 (bool use);
#ifndef EDITION_TRY
    void                     setVocabularyCount          (quintptr count);
    void                     setVocabularyInfo           (quintptr index, const VocabularyInfo &info);
#endif
#ifndef EDITION_FREE
    void                     setWaitForAnswer            (quintptr time);
    void                     setWindowHeight             (quintptr height);
    void                     setWindowWidth              (quintptr width);
    void                     setWindowX                  (quintptr x);
    void                     setWindowY                  (quintptr y);
#endif
    void                     setWordsFrequency           (quintptr frequency);
#ifndef EDITION_FREE
    bool                     showCategoryName            ()            const;
    bool                     showLanguageNames           ()            const;
    bool                     showRecordControls          ()            const;
    bool                     showStatusBar               ()            const;
    bool                     showToolBar                 ()            const;
    bool                     showWordsInTrayBalloon      ()            const;
    bool                     startLearningOnStartup      ()            const;
#endif
    Qt::CheckState           switchLearningDirection     ()            const;
#ifndef EDITION_FREE
    bool                     systemTrayIcon              ()            const;
#endif
    QString                  translation                 ()            const;
    bool                     updateCheck                 ()            const;
    bool                     useProxy                    ()            const;
#ifndef EDITION_TRY
    quintptr                 vocabularyCount             ();
    VocabularyInfo           vocabularyInfo              (quintptr index);
#endif
    quintptr                 waitForAnswer               ()            const;
#ifndef EDITION_FREE
    quintptr                 windowHeight                ()            const;
    quintptr                 windowWidth                 ()            const;
    quintptr                 windowX                     ()            const;
    quintptr                 windowY                     ()            const;
#endif
    quintptr                 wordsFrequency              ()            const;
#ifndef EDITION_FREE
#endif

  private:
    static const auto DEFAULT_FREQUENCY = 60;
    static const auto DEFAULT_WAIT      = 0;

#ifndef EDITION_TRY
    enum class Version
    {
      N1,
      N2,
    };
    enum class VocabularyPosition
    {
      N1
    };
#endif

    QSettings _settings;

#if !defined(EDITION_FREE) && defined(Q_OS_WIN)
    QString hotkeyKey          (Hotkey type) const;
    QString hotkeyKeyText      (Hotkey type) const;
    QString hotkeyKeyVirtualKey(Hotkey type) const;
#endif
#ifndef EDITION_TRY
    void    updateSettings();
#endif
};

#endif