#pragma once

#include <QtCore/QSettings>

class Settings
{
	public:
		static const int DEFAULT_DIMENSION = -1;

		Settings();

#ifndef FREE
        const bool GetAlwaysOnTop() const;
        const QString GetColorFlash() const;
#endif
        const int GetFontSizeNote() const;
        const int GetFontSizeWord() const;
#ifndef FREE
        const bool GetHorizontalLayout() const;
        const bool GetMinimizeToTray() const;
        const bool GetMute() const;
#endif
		const int GetNewWordsCount() const;
		const int GetNewWordsFrequency() const;
#ifndef FREE
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
		const QString GetVocabularyFile() const;
		const int GetWaitForAnswer() const;
#ifndef FREE
		const int GetWindowHeight() const;
		const int GetWindowWidth() const;
		const int GetWindowX() const;
		const int GetWindowY() const;
#endif
        const int GetWordsFrequency() const;
#ifndef FREE
        const void SetAlwaysOnTop(const bool &pEnable);
        const void SetColorFlash(const QString &pColor);
#endif
        const void SetFontSizeNote(const int &pSize);
        const void SetFontSizeWord(const int &pSize);
#ifndef FREE
        const void SetHorizontalLayout(const bool &pEnable);
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
        static const int DEFAULT_FONTSIZENOTE = 10;
        static const int DEFAULT_FONTSIZEWORD = 20;
		static const int DEFAULT_FREQUENCY = 60 * 1000;
		static const int DEFAULT_NEWWORDSFREQUENCY = 5;
		static const int DEFAULT_WAIT = 0;

		QSettings _qsSettings;
}; // Settings