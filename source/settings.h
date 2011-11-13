#pragma once

#include <QtCore/QSettings>
#include <QtNetwork/QNetworkProxy>

const QString DIR_LANG = "lang";

class Settings
{
#ifndef FREE
# ifdef Q_WS_WIN
    Q_ENUMS(eHotkey)
# endif
	Q_ENUMS(eNewWordSoundType)
#endif

	public:
#ifndef FREE
# ifdef Q_WS_WIN
        enum eHotkey {
            HotkeyAnswer,
            HotkeyMinimize,
            HotkeyNext,
            HotkeyRestore,
			HotkeyCount
        }; // eHotkey
# endif
        enum eNewWordSoundType {
            NewWordSoundTypeSystem,
            NewWordSoundTypeCustom
        }; // eNewWordSoundType
#endif

#ifndef FREE
# ifdef Q_WS_WIN
		struct sHotKeyInfo {
			QString qsText;
			quint32 qui32VirtualKey;
		}; // sHotKeyInfo
# endif
#endif

		static const int DEFAULT_DIMENSION = -1;

		Settings();

        const bool GetAlwaysOnTop() const;
#ifndef FREE
        const bool GetCanChangeCategoryPriority() const;
        const bool GetCanEnableCategories() const;
        const QString GetColorFlash() const;
        const bool GetHorizontalLayout() const;
# ifdef Q_WS_WIN
        const sHotKeyInfo GetHotkey(const eHotkey &pType) const;
# endif
		const bool GetLearnDisabledWords() const;
        const QByteArray GetLicense() const;
        const bool GetMinimizeToTray() const;
        const bool GetMute() const;
		const bool GetNewWordFlash() const;
		const bool GetNewWordSound() const;
        const QString GetNewWordSoundFile() const;
        const eNewWordSoundType GetNewWordSoundType() const;
#endif
		const QString GetProxyHostname() const;
		const QString GetProxyPassword() const;
		const quint16 GetProxyPort() const;
		const QNetworkProxy::ProxyType GetProxyType() const;
		const QString GetProxyUsername() const;
#ifndef FREE
        const bool GetRememberWindowPosition() const;
		const bool GetShowCategoryName() const;
		const bool GetShowLanguageNames() const;
		const bool GetShowRecordControls() const;
		const bool GetShowStatusBar() const;
		const bool GetShowToolBar() const;
		const bool GetShowWordsInTrayBalloon() const;
		const bool GetStartLearningOnStartup() const;
#endif
		const Qt::CheckState GetSwitchLearningDirection() const;
#ifndef FREE
        const bool GetSystemTrayIcon() const;
#endif
        const QString GetTranslation() const;
		const bool GetUpdateCheck() const;
		const bool GetUseProxy() const;
#ifndef TRY
		const QString GetVocabularyFile() const;
#endif
		const int GetWaitForAnswer() const;
#ifndef FREE
		const int GetWindowHeight() const;
		const int GetWindowWidth() const;
		const int GetWindowX() const;
		const int GetWindowY() const;
#endif
        const int GetWordsFrequency() const;
        const void SetAlwaysOnTop(const bool &pEnable);
#ifndef FREE
        const void SetCanChangeCategoryPriority(const bool &pEnable);
        const void SetCanEnableCategories(const bool &pEnable);
        const void SetColorFlash(const QString &pColor);
        const void SetHorizontalLayout(const bool &pEnable);
# ifdef Q_WS_WIN
        const void SetHotkey(const eHotkey &pType, const sHotKeyInfo &pHotkey);
# endif
		const void SetLearnDisabledWords(const bool &pEnable);
		const void SetLicense(const QByteArray &pLicense);
		const void SetMinimizeToTray(const bool &pEnable);
        const void SetMute(const bool &pEnable);
		const void SetNewWordFlash(const bool &pEnable);
		const void SetNewWordSound(const bool &pEnable);
        const void SetNewWordSoundFile(const QString &pFile);
        const void SetNewWordSoundType(const eNewWordSoundType &pType);
#endif
		const void SetProxyHostname(const QString &pHostname);
		const void SetProxyPassword(const QString &pPassword);
		const void SetProxyPort(const quint16 &pPort);
		const void SetProxyType(const QNetworkProxy::ProxyType &pType);
		const void SetProxyUsername(const QString &pUsername);
#ifndef FREE
        const void SetRememberWindowPosition(const bool &pEnable);
		const void SetShowCategoryName(const bool &pShow);
		const void SetShowLanguageNames(const bool &pShow);
		const void SetShowRecordControls(const bool &pShow);
		const void SetShowStatusBar(const bool &pShow);
		const void SetShowToolBar(const bool &pShow);
		const void SetShowWordsInTrayBalloon(const bool &pEnable);
		const void SetStartLearningOnStartup(const bool &pEnable);
#endif
		const void SetSwitchLearningDirection(const Qt::CheckState &pSwitch);
#ifndef FREE
        const void SetSystemTrayIcon(const bool &pEnable);
#endif
        const void SetTranslation(const QString &pTranslation);
		const void SetUpdateCheck(const bool &pCheck);
		const void SetUseProxy(const bool &pUse);
#ifndef TRY
		const void SetVocabularyFile(const QString &pFile);
#endif
#ifndef FREE
		const void SetWaitForAnswer(const int &pTime);
		const void SetWindowHeight(const int &pHeight);
		const void SetWindowWidth(const int &pWidth);
		const void SetWindowX(const int &pX);
		const void SetWindowY(const int &pY);
#endif
        const void SetWordsFrequency(const int &pFrequency);

	private:
		static const int DEFAULT_FREQUENCY = 60;
		static const int DEFAULT_WAIT = 0;

#ifndef TRY
		enum eVersion {
			Version1,
			Version2,
		}; // eVersion
		enum eVocabularyPosition {
			VocabularyPosition1
		}; // eVocabularyPosition
#endif

		QSettings _qsSettings;

#if !defined(FREE) && defined(Q_WS_WIN)
        const QString GetHotkeyKey(const eHotkey &pType) const;
		const QString GetHotkeyKeyText(const eHotkey &pType) const;
		const QString GetHotkeyKeyVirtualKey(const eHotkey &pType) const;
#endif
#ifndef TRY
		const void UpdateSettings();
#endif
}; // Settings