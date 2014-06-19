#pragma once

#include <QtCore/QSettings>
#include <QtNetwork/QNetworkProxy>

const QString DIR_LANG = "lang";

class Settings
{
	public:
#ifndef EDITION_FREE
# ifdef Q_OS_WIN
        enum Hotkey
		{
            HotkeyAnswer,
            HotkeyMinimize,
            HotkeyNext,
            HotkeyRestore,
			HotkeyCount
        }; // Hotkey
# endif
        enum NewWordSoundType
		{
            NewWordSoundTypeSystem,
            NewWordSoundTypeCustom
        }; // NewWordSoundType
#endif

#ifndef EDITION_FREE
# ifdef Q_OS_WIN
		struct HotkeyInfo
		{
			QString text;
			quint32 virtualKey;
		}; // HotkeyInfo
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
		}; // VocabularyInfo

		static const qint8  DEFAULT_DIMENSION             = -1;
#ifdef EDITION_FREE
		static const quint8 EDITION_FREE_VOCABULARIES_MAX = 1;
#endif

		Settings();

        const bool                     alwaysOnTop                 ()                   const;
#ifndef EDITION_FREE
        const bool                     canChangeCategoryPriority   ()                   const;
        const bool                     canEnableCategories         ()                   const;
        const QString                  colorFlash                  ()                   const;
        const bool                     horizontalLayout            ()                   const;
# ifdef Q_OS_WIN
        const HotkeyInfo               hotkey                      (const Hotkey &type) const;
# endif
		const bool                     learnDisabledWords          ()                   const;
        const QByteArray               license                     ()                   const;
        const bool                     minimizeToTray              ()                   const;
        const bool                     mute                        ()                   const;
		const bool                     newWordFlash                ()                   const;
		const bool                     newWordSound                ()                   const;
        const QString                  newWordSoundFile            ()                   const;
        const NewWordSoundType         newWordSoundType            ()                   const;
#endif
		const QString                  proxyHostname               ()                   const;
		const QString                  proxyPassword               ()                   const;
		const quint16                  proxyPort                   ()                   const;
		const QNetworkProxy::ProxyType proxyType                   ()                   const;
		const QString                  proxyUsername               ()                   const;
#ifndef EDITION_FREE
        const bool                     rememberWindowPosition      ()                   const;
#endif
		const void                     setAlwaysOnTop              (const bool &enable);
#ifndef EDITION_FREE
		const void                     setCanChangeCategoryPriority(const bool &enable);
		const void                     setCanEnableCategories      (const bool &enable);
		const void                     setColorFlash               (const QString &color);
		const void                     setHorizontalLayout         (const bool &enable);
# ifdef Q_OS_WIN
		const void                     setHotkey                   (const Hotkey &type, const HotkeyInfo &hotkeyInfo);
# endif
		const void                     setLearnDisabledWords       (const bool &enable);
		const void                     setLicense                  (const QByteArray &licenseData);
		const void                     setMinimizeToTray           (const bool &enable);
		const void                     setMute                     (const bool &enable);
		const void                     setNewWordFlash             (const bool &enable);
		const void                     setNewWordSound             (const bool &enable);
		const void                     setNewWordSoundFile         (const QString &file);
		const void                     setNewWordSoundType         (const NewWordSoundType &type);
#endif
		const void                     setProxyHostname            (const QString &hostname);
		const void                     setProxyPassword            (const QString &password);
		const void                     setProxyPort                (const quint16 &port);
		const void                     setProxyType                (const QNetworkProxy::ProxyType &type);
		const void                     setProxyUsername            (const QString &username);
#ifndef EDITION_FREE
        const void                     setRememberWindowPosition   (const bool &enable);
		const void                     setShowCategoryName         (const bool &show);
		const void                     setShowLanguageNames        (const bool &show);
		const void                     setShowRecordControls       (const bool &show);
		const void                     setShowStatusBar            (const bool &show);
		const void                     setShowToolBar              (const bool &show);
		const void                     setShowWordsInTrayBalloon   (const bool &enable);
		const void                     setStartLearningOnStartup   (const bool &enable);
#endif
		const void                     setSwitchLearningDirection  (const Qt::CheckState &switchDirection);
#ifndef EDITION_FREE
        const void                     setSystemTrayIcon           (const bool &enable);
#endif
        const void                     setTranslation              (const QString &translationName);
		const void                     setUpdateCheck              (const bool &check);
		const void                     setUseProxy                 (const bool &use);
#ifndef EDITION_TRY
		const void                     setVocabularyCount          (const quint8 &count);
		const void                     setVocabularyInfo           (const quint8 &index, const VocabularyInfo &info);
#endif
#ifndef EDITION_FREE
		const void                     setWaitForAnswer            (const quint8 &time);
		const void                     setWindowHeight             (const quint16 &height);
		const void                     setWindowWidth              (const quint16 &width);
		const void                     setWindowX                  (const quint16 &x);
		const void                     setWindowY                  (const quint16 &y);
#endif
        const void                     setWordsFrequency           (const quint8 &frequency);
#ifndef EDITION_FREE
		const bool                     showCategoryName            ()                   const;
		const bool                     showLanguageNames           ()                   const;
		const bool                     showRecordControls          ()                   const;
		const bool                     showStatusBar               ()                   const;
		const bool                     showToolBar                 ()                   const;
		const bool                     showWordsInTrayBalloon      ()                   const;
		const bool                     startLearningOnStartup      ()                   const;
#endif
		const Qt::CheckState           switchLearningDirection     ()                   const;
#ifndef EDITION_FREE
        const bool                     systemTrayIcon              ()                   const;
#endif
        const QString                  translation                 ()                   const;
		const bool                     updateCheck                 ()                   const;
		const bool                     useProxy                    ()                   const;
#ifndef EDITION_TRY
		const quint8                   vocabularyCount             ();
		const VocabularyInfo           vocabularyInfo              (const quint8 &index);
#endif
		const quint8                   waitForAnswer               ()                   const;
#ifndef EDITION_FREE
		const quint16                  windowHeight                ()                   const;
		const quint16                  windowWidth                 ()                   const;
		const quint16                  windowX                     ()                   const;
		const quint16                  windowY                     ()                   const;
#endif
        const quint8                   wordsFrequency              ()                   const;
#ifndef EDITION_FREE
#endif

	private:
		static const quint8 DEFAULT_FREQUENCY = 60;
		static const quint8 DEFAULT_WAIT      = 0;

#ifndef EDITION_TRY
		enum Version
		{
			Version1,
			Version2,
		}; // Version
		enum VocabularyPosition
		{
			VocabularyPosition1
		}; // VocabularyPosition
#endif

		QSettings _settings;

#if !defined(EDITION_FREE) && defined(Q_OS_WIN)
        const QString hotkeyKey          (const Hotkey &pType) const;
		const QString hotkeyKeyText      (const Hotkey &pType) const;
		const QString hotkeyKeyVirtualKey(const Hotkey &pType) const;
#endif
#ifndef EDITION_TRY
		const void    updateSettings     ();
#endif
}; // Settings