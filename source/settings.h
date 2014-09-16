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

        bool                     alwaysOnTop                 ()            const;
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
		quint16                  proxyPort                   ()            const;
		QNetworkProxy::ProxyType proxyType                   ()            const;
		QString                  proxyUsername               ()            const;
#ifndef EDITION_FREE
        bool                     rememberWindowPosition      ()            const;
#endif
		void                     setAlwaysOnTop              (bool enable);
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
		void                     setProxyPort                (quint16 port);
		void                     setProxyType                (QNetworkProxy::ProxyType type);
		void                     setProxyUsername            (const QString &username);
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
		void                     setVocabularyCount          (quint8 count);
		void                     setVocabularyInfo           (quint8 index, const VocabularyInfo &info);
#endif
#ifndef EDITION_FREE
		void                     setWaitForAnswer            (quint8 time);
		void                     setWindowHeight             (quint16 height);
		void                     setWindowWidth              (quint16 width);
		void                     setWindowX                  (quint16 x);
		void                     setWindowY                  (quint16 y);
#endif
        void                     setWordsFrequency           (quint8 frequency);
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
		quint8                   vocabularyCount             ();
		VocabularyInfo           vocabularyInfo              (quint8 index);
#endif
		quint8                   waitForAnswer               ()            const;
#ifndef EDITION_FREE
		quint16                  windowHeight                ()            const;
		quint16                  windowWidth                 ()            const;
		quint16                  windowX                     ()            const;
		quint16                  windowY                     ()            const;
#endif
        quint8                   wordsFrequency              ()            const;
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
        QString hotkeyKey          (Hotkey pType) const;
		QString hotkeyKeyText      (Hotkey pType) const;
		QString hotkeyKeyVirtualKey(Hotkey pType) const;
#endif
#ifndef EDITION_TRY
		void    updateSettings     ();
#endif
}; // Settings