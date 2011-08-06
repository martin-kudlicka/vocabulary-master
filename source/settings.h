#pragma once

#include <QtCore/QSettings>

const QString DIR_LANG = "lang";

class Settings
{
#if !defined(FREE) && defined(Q_WS_WIN)
    Q_ENUMS(eHotkey)
#endif

	public:
#if !defined(FREE) && defined(Q_WS_WIN)
        enum eHotkey {
            HotkeyAnswer,
            HotkeyMinimize,
            HotkeyNext,
            HotkeyRestore,
			HotkeyCount
        }; // eHotkey

		struct sHotKeyInfo {
			QString qsText;
			quint32 qui32VirtualKey;
		}; // sHotKeyInfo
#endif

		static const int DEFAULT_DIMENSION = -1;

		Settings();

        const bool GetAlwaysOnTop() const;
#ifndef FREE
        const QString GetColorFlash() const;
        const bool GetHorizontalLayout() const;
# ifdef Q_WS_WIN
        const sHotKeyInfo GetHotkey(const eHotkey &pType) const;
# endif
        const bool GetMinimizeToTray() const;
        const bool GetMute() const;
		const bool GetNewWordFlash() const;
		const bool GetNewWordSound() const;
        const bool GetRememberWindowPosition() const;
		const bool GetShowWordsInTrayBalloon() const;
		const bool GetStartLearningOnStartup() const;
#endif
		const Qt::CheckState GetSwitchLearningDirection() const;
#ifndef FREE
        const bool GetSystemTrayIcon() const;
#endif
        const QString GetTranslation() const;
		const QString GetVocabularyFile() const;
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
        const void SetColorFlash(const QString &pColor);
        const void SetHorizontalLayout(const bool &pEnable);
# ifdef Q_WS_WIN
        const void SetHotkey(const eHotkey &pType, const sHotKeyInfo &pHotkey);
# endif
		const void SetMinimizeToTray(const bool &pEnable);
        const void SetMute(const bool &pEnable);
		const void SetNewWordFlash(const bool &pEnable);
		const void SetNewWordSound(const bool &pEnable);
        const void SetRememberWindowPosition(const bool &pEnable);
		const void SetShowWordsInTrayBalloon(const bool &pEnable);
		const void SetStartLearningOnStartup(const bool &pEnable);
#endif
		const void SetSwitchLearningDirection(const Qt::CheckState &pSwitch);
#ifndef FREE
        const void SetSystemTrayIcon(const bool &pEnable);
#endif
        const void SetTranslation(const QString &pTranslation);
		const void SetVocabularyFile(const QString &pFile);
#ifndef FREE
		const void SetWaitForAnswer(const int &pTime);
		const void SetWindowHeight(const int &pHeight);
		const void SetWindowWidth(const int &pWidth);
		const void SetWindowX(const int &pX);
		const void SetWindowY(const int &pY);
#endif
        const void SetWordsFrequency(const int &pFrequency);

	private:
		static const int DEFAULT_FREQUENCY = 60 * 1000;
		static const int DEFAULT_WAIT = 0;

		QSettings _qsSettings;

#if !defined(FREE) && defined(Q_WS_WIN)
        const QString GetHotkeyKey(const eHotkey &pType) const;
		const QString GetHotkeyKeyText(const eHotkey &pType) const;
		const QString GetHotkeyKeyVirtualKey(const eHotkey &pType) const;
#endif
}; // Settings