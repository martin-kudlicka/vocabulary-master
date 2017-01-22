#ifndef SETTINGS_H
#define SETTINGS_H

#include <QtCore/QSettings>
#include <QtNetwork/QNetworkProxy>

const auto DIR_LANG = "lang";

class Settings
{
  public:
#ifdef Q_OS_WIN
    enum class Hotkey
    {
      Answer,
      Minimize,
      Next,
      Restore,
      Count
    };
#endif
    enum class NewWordSoundType
    {
      System,
      Custom
    };

#ifdef Q_OS_WIN
    struct HotkeyInfo
    {
      QString  text;
      quintptr virtualKey;
    };
#endif
    struct VocabularyInfo
    {
      QString filePath;
      bool    enabled;
    };

    static const auto DEFAULT_DIMENSION             = 0;

    Settings();

    bool                     alwaysOnTop                 ()            const;
    bool                     cacheVocabulary             ()            const;
    bool                     canChangeCategoryPriority   ()            const;
    bool                     canEnableCategories         ()            const;
    QString                  colorFlash                  ()            const;
    bool                     horizontalLayout            ()            const;
#ifdef Q_OS_WIN
    HotkeyInfo               hotkey                      (Hotkey type) const;
#endif
    bool                     learnDisabledWords          ()            const;
    QByteArray               license                     ()            const;
    bool                     minimizeToTray              ()            const;
    bool                     mute                        ()            const;
    bool                     newWordFlash                ()            const;
    bool                     newWordSound                ()            const;
    QString                  newWordSoundFile            ()            const;
    NewWordSoundType         newWordSoundType            ()            const;
    QString                  proxyHostname               ()            const;
    QString                  proxyPassword               ()            const;
    quintptr                 proxyPort                   ()            const;
    QNetworkProxy::ProxyType proxyType                   ()            const;
    QString                  proxyUsername               ()            const;
    quintptr                 recordsToCache              ()            const;
    bool                     rememberWindowPosition      ()            const;
    void                     setAlwaysOnTop              (bool enable);
    void                     setCacheVocabulary          (bool enable);
    void                     setCanChangeCategoryPriority(bool enable);
    void                     setCanEnableCategories      (bool enable);
    void                     setColorFlash               (const QString &color);
    void                     setHorizontalLayout         (bool enable);
#ifdef Q_OS_WIN
    void                     setHotkey                   (Hotkey type, const HotkeyInfo &hotkeyInfo);
#endif
    void                     setLearnDisabledWords       (bool enable);
    void                     setLicense                  (const QByteArray &licenseData);
    void                     setMinimizeToTray           (bool enable);
    void                     setMute                     (bool enable);
    void                     setNewWordFlash             (bool enable);
    void                     setNewWordSound             (bool enable);
    void                     setNewWordSoundFile         (const QString &file);
    void                     setNewWordSoundType         (NewWordSoundType type);
    void                     setProxyHostname            (const QString &hostname);
    void                     setProxyPassword            (const QString &password);
    void                     setProxyPort                (quintptr port);
    void                     setProxyType                (QNetworkProxy::ProxyType type);
    void                     setProxyUsername            (const QString &username);
    void                     setRecordsToCache           (quintptr count);
    void                     setRememberWindowPosition   (bool enable);
    void                     setShowCategoryName         (bool show);
    void                     setShowLanguageNames        (bool show);
    void                     setShowRecordControls       (bool show);
    void                     setShowStatusBar            (bool show);
    void                     setShowToolBar              (bool show);
    void                     setShowWordsInTrayBalloon   (bool enable);
    void                     setStartLearningOnStartup   (bool enable);
    void                     setSwitchLearningDirection  (Qt::CheckState switchDirection);
    void                     setSystemTrayIcon           (bool enable);
    void                     setTranslation              (const QString &translationName);
    void                     setUpdateCheck              (bool check);
    void                     setUseProxy                 (bool use);
    void                     setVocabularyCount          (quintptr count);
    void                     setVocabularyInfo           (quintptr index, const VocabularyInfo &info);
    void                     setWaitForAnswer            (quintptr time);
    void                     setWindowHeight             (quintptr height);
    void                     setWindowWidth              (quintptr width);
    void                     setWindowX                  (quintptr x);
    void                     setWindowY                  (quintptr y);
    void                     setWordsFrequency           (quintptr frequency);
    bool                     showCategoryName            ()            const;
    bool                     showLanguageNames           ()            const;
    bool                     showRecordControls          ()            const;
    bool                     showStatusBar               ()            const;
    bool                     showToolBar                 ()            const;
    bool                     showWordsInTrayBalloon      ()            const;
    bool                     startLearningOnStartup      ()            const;
    Qt::CheckState           switchLearningDirection     ()            const;
    bool                     systemTrayIcon              ()            const;
    QString                  translation                 ()            const;
    bool                     updateCheck                 ()            const;
    bool                     useProxy                    ()            const;
    quintptr                 vocabularyCount             ();
    VocabularyInfo           vocabularyInfo              (quintptr index);
    quintptr                 waitForAnswer               ()            const;
    quintptr                 windowHeight                ()            const;
    quintptr                 windowWidth                 ()            const;
    quintptr                 windowX                     ()            const;
    quintptr                 windowY                     ()            const;
    quintptr                 wordsFrequency              ()            const;

  private:
    enum class Version
    {
      N1,
      N2,
    };
    enum class VocabularyPosition
    {
      N1
    };

    QSettings _settings;

#ifdef Q_OS_WIN
    QString hotkeyKey          (Hotkey type) const;
    QString hotkeyKeyText      (Hotkey type) const;
    QString hotkeyKeyVirtualKey(Hotkey type) const;
#endif
    void    updateSettings();
};

#endif