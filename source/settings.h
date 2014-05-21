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
		struct HotKeyInfo
		{
			QString text;
			quint32 virtualKey;
		}; // HotKeyInfo
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

        const bool alwaysOnTop() const;
#ifndef EDITION_FREE
        const bool canChangeCategoryPriority() const;
        const bool canEnableCategories() const;
        const QString colorFlash() const;
        const bool horizontalLayout() const;
# ifdef Q_OS_WIN
        const HotKeyInfo hotkey(const Hotkey &pType) const;
# endif
		const bool learnDisabledWords() const;
        const QByteArray license() const;
        const bool minimizeToTray() const;
        const bool mute() const;
		const bool newWordFlash() const;
		const bool newWordSound() const;
        const QString newWordSoundFile() const;
        const NewWordSoundType newWordSoundType() const;
#endif
		const QString proxyHostname() const;
		const QString proxyPassword() const;
		const quint16 proxyPort() const;
		const QNetworkProxy::ProxyType proxyType() const;
		const QString proxyUsername() const;
#ifndef EDITION_FREE
        const bool rememberWindowPosition() const;
		const bool showCategoryName() const;
		const bool showLanguageNames() const;
		const bool showRecordControls() const;
		const bool showStatusBar() const;
		const bool showToolBar() const;
		const bool showWordsInTrayBalloon() const;
		const bool startLearningOnStartup() const;
#endif
		const Qt::CheckState switchLearningDirection() const;
#ifndef EDITION_FREE
        const bool systemTrayIcon() const;
#endif
        const QString translation() const;
		const bool updateCheck() const;
		const bool useProxy() const;
#ifndef EDITION_TRY
		const int vocabularyCount();
		const VocabularyInfo vocabularyInfo(const int &pIndex);
#endif
		const int waitForAnswer() const;
#ifndef EDITION_FREE
		const int windowHeight() const;
		const int windowWidth() const;
		const int windowX() const;
		const int windowY() const;
#endif
        const int wordsFrequency() const;
        const void setAlwaysOnTop(const bool &pEnable);
#ifndef EDITION_FREE
        const void setCanChangeCategoryPriority(const bool &pEnable);
        const void setCanEnableCategories(const bool &pEnable);
        const void setColorFlash(const QString &pColor);
        const void setHorizontalLayout(const bool &pEnable);
# ifdef Q_OS_WIN
        const void setHotkey(const Hotkey &pType, const HotKeyInfo &pHotkey);
# endif
		const void setLearnDisabledWords(const bool &pEnable);
		const void setLicense(const QByteArray &pLicense);
		const void setMinimizeToTray(const bool &pEnable);
        const void setMute(const bool &pEnable);
		const void setNewWordFlash(const bool &pEnable);
		const void setNewWordSound(const bool &pEnable);
        const void setNewWordSoundFile(const QString &pFile);
        const void setNewWordSoundType(const NewWordSoundType &pType);
#endif
		const void setProxyHostname(const QString &pHostname);
		const void setProxyPassword(const QString &pPassword);
		const void setProxyPort(const quint16 &pPort);
		const void setProxyType(const QNetworkProxy::ProxyType &pType);
		const void setProxyUsername(const QString &pUsername);
#ifndef EDITION_FREE
        const void setRememberWindowPosition(const bool &pEnable);
		const void setShowCategoryName(const bool &pShow);
		const void setShowLanguageNames(const bool &pShow);
		const void setShowRecordControls(const bool &pShow);
		const void setShowStatusBar(const bool &pShow);
		const void setShowToolBar(const bool &pShow);
		const void setShowWordsInTrayBalloon(const bool &pEnable);
		const void setStartLearningOnStartup(const bool &pEnable);
#endif
		const void setSwitchLearningDirection(const Qt::CheckState &pSwitch);
#ifndef EDITION_FREE
        const void setSystemTrayIcon(const bool &pEnable);
#endif
        const void setTranslation(const QString &pTranslation);
		const void setUpdateCheck(const bool &pCheck);
		const void setUseProxy(const bool &pUse);
#ifndef EDITION_TRY
		const void setVocabularyCount(const int &pCount);
		const void setVocabularyInfo(const int &pIndex, const VocabularyInfo &pInfo);
#endif
#ifndef EDITION_FREE
		const void setWaitForAnswer(const int &pTime);
		const void setWindowHeight(const int &pHeight);
		const void setWindowWidth(const int &pWidth);
		const void setWindowX(const int &pX);
		const void setWindowY(const int &pY);
#endif
        const void setWordsFrequency(const int &pFrequency);

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