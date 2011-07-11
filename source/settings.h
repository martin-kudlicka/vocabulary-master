#pragma once

#include <QtCore/QSettings>

class Settings
{
	public:
		static const int DEFAULT_DIMENSION = -1;

		Settings();

        const bool GetAlwaysOnTop() const;
        const bool GetHorizontalLayout() const;
		const int GetNewWordsCount() const;
		const int GetNewWordsFrequency() const;
        const bool GetRememberWindowPosition() const;
		const bool GetNewWordSound() const;
		const bool GetStartLearningOnStartup() const;
		const Qt::CheckState GetSwitchLearningDirection() const;
		const QString GetVocabularyFile() const;
		const int GetWaitForAnswer() const;
		const int GetWindowHeight() const;
		const int GetWindowWidth() const;
		const int GetWindowX() const;
		const int GetWindowY() const;
        const int GetWordsFrequency() const;
        const void SetAlwaysOnTop(const bool &pEnable);
        const void SetHorizontalLayout(const bool &pEnable);
        const void SetRememberWindowPosition(const bool &pEnable);
		const void SetNewWordSound(const bool &pEnable);
		const void SetStartLearningOnStartup(const bool &pEnable);
		const void SetSwitchLearningDirection(const Qt::CheckState &pSwitch);
		const void SetVocabularyFile(const QString &pFile);
		const void SetWaitForAnswer(const int &pTime);
		const void SetWindowHeight(const int &pHeight);
		const void SetWindowWidth(const int &pWidth);
		const void SetWindowX(const int &pX);
		const void SetWindowY(const int &pY);
        const void SetWordsFrequency(const int &pFrequency);

	private:
		static const int DEFAULT_FREQUENCY = 60 * 1000;
		static const int DEFAULT_NEWWORDSFREQUENCY = 5;
		static const int DEFAULT_WAIT = 0;

		QSettings _qsSettings;
}; // Settings