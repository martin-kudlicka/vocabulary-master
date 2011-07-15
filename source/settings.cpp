#include "settings.h"

const QString APPLICATION = "Vocabulary Master";
const QString DEFAULT_COLORFLASH = "chartreuse";
const QString KEY_ALWAYSONTOP = "AlwaysOnTop";
const QString KEY_COLORFLASH = "ColorFlash";
const QString KEY_FONTSIZENOTE = "FontSizeNote";
const QString KEY_FONTSIZEWORD = "FontSizeWord";
const QString KEY_HORIZONTALLAYOUT = "HorizontalLayout";
const QString KEY_MUTE = "Mute";
const QString KEY_NEWWORDFLASH = "NewWordFlash";
const QString KEY_NEWWORDSCOUNT = "NewWordsCount";
const QString KEY_NEWWORDSFREQUENCY = "NewWordsFrequency";
const QString KEY_NEWWORDSOUND = "NewWordSound";
const QString KEY_REMEMBERWINDOWPOSITION = "RememberWindowPosition";
const QString KEY_STARTLEARNINGONSTARTUP = "StartLearningOnStartup";
const QString KEY_SWITCHLEARNINGDIRECTION = "SwitchLearningDirection";
const QString KEY_SYSTEMTRAYICON = "SystemTrayIcon";
const QString KEY_VOCABULARYFILE = "VocabularyFile";
const QString KEY_WAITFORANSWER = "WaitForAnswer";
const QString KEY_WINDOWHEIGHT = "WindowHeight";
const QString KEY_WINDOWWIDTH = "WindowWidth";
const QString KEY_WINDOWX = "WindowX";
const QString KEY_WINDOWY = "WindowY";
const QString KEY_WORDSFREQUENCY = "WordsFrequency";
const QString ORGANIZATION  = "Isshou";

const bool Settings::GetAlwaysOnTop() const
{
    return _qsSettings.value(KEY_ALWAYSONTOP, false).toBool();
} // GetAlwaysOnTop

const QString Settings::GetColorFlash() const
{
    return _qsSettings.value(KEY_COLORFLASH, DEFAULT_COLORFLASH).toString();
} // GetColorFlash

const int Settings::GetFontSizeNote() const
{
    return _qsSettings.value(KEY_FONTSIZENOTE, DEFAULT_FONTSIZENOTE).toInt();
} // GetFontSizeNote

const int Settings::GetFontSizeWord() const
{
    return _qsSettings.value(KEY_FONTSIZEWORD, DEFAULT_FONTSIZEWORD).toInt();
} // GetFontSizeWord

const bool Settings::GetHorizontalLayout() const
{
    return _qsSettings.value(KEY_HORIZONTALLAYOUT, false).toBool();
} // GetHorizontalLayout

const bool Settings::GetMute() const
{
    return _qsSettings.value(KEY_MUTE, false).toBool();
} // GetMute

const int Settings::GetNewWordsCount() const
{
	return _qsSettings.value(KEY_NEWWORDSCOUNT).toInt();
} // GetNewWordsCount

const int Settings::GetNewWordsFrequency() const
{
	return _qsSettings.value(KEY_NEWWORDSFREQUENCY, DEFAULT_NEWWORDSFREQUENCY).toInt();
} // GetNewWordsFrequency

const bool Settings::GetNewWordFlash() const
{
	return _qsSettings.value(KEY_NEWWORDFLASH, false).toBool();
} // GetNewWordFlash

const bool Settings::GetNewWordSound() const
{
	return _qsSettings.value(KEY_NEWWORDSOUND, true).toBool();
} // GetNewWordSound

const bool Settings::GetRememberWindowPosition() const
{
    return _qsSettings.value(KEY_REMEMBERWINDOWPOSITION, true).toBool();
} // GetRememberWindowPosition

const bool Settings::GetStartLearningOnStartup() const
{
	return _qsSettings.value(KEY_STARTLEARNINGONSTARTUP, false).toBool();
} // GetStartLearningOnStartup

const Qt::CheckState Settings::GetSwitchLearningDirection() const
{
	return static_cast<Qt::CheckState>(_qsSettings.value(KEY_SWITCHLEARNINGDIRECTION, false).toInt());
} // GetSwitchLearningDirection

const bool Settings::GetSystemTrayIcon() const
{
    return _qsSettings.value(KEY_SYSTEMTRAYICON, false).toBool();
} // GetSystemTrayIcon

const QString Settings::GetVocabularyFile() const
{
	return _qsSettings.value(KEY_VOCABULARYFILE).toString();
} // GetVocabularyFile

const int Settings::GetWaitForAnswer() const
{
	return _qsSettings.value(KEY_WAITFORANSWER, DEFAULT_WAIT).toInt();
} // GetWaitForAnswer

const int Settings::GetWindowHeight() const
{
	return _qsSettings.value(KEY_WINDOWHEIGHT, DEFAULT_DIMENSION).toInt();
} // GetWindowHeight

const int Settings::GetWindowWidth() const
{
	return _qsSettings.value(KEY_WINDOWWIDTH, DEFAULT_DIMENSION).toInt();
} // GetWindowWidth

const int Settings::GetWindowX() const
{
	return _qsSettings.value(KEY_WINDOWX, DEFAULT_DIMENSION).toInt();
} // GetWindowX

const int Settings::GetWindowY() const
{
	return _qsSettings.value(KEY_WINDOWY, DEFAULT_DIMENSION).toInt();
} // GetWindowY

const int Settings::GetWordsFrequency() const
{
    return _qsSettings.value(KEY_WORDSFREQUENCY, DEFAULT_FREQUENCY).toInt();
} // GetWordsFrequency

const void Settings::SetAlwaysOnTop(const bool &pEnable)
{
    _qsSettings.setValue(KEY_ALWAYSONTOP, pEnable);
} // SetAlwaysOnTop

const void Settings::SetColorFlash(const QString &pColor)
{
    _qsSettings.setValue(KEY_COLORFLASH, pColor);
} // SetColorFlash

const void Settings::SetFontSizeNote(const int &pSize)
{
    _qsSettings.setValue(KEY_FONTSIZENOTE, pSize);
} // SetFontSizeNote

const void Settings::SetFontSizeWord(const int &pSize)
{
    _qsSettings.setValue(KEY_FONTSIZEWORD, pSize);
} // SetFontSizeWord

const void Settings::SetHorizontalLayout(const bool &pEnable)
{
    _qsSettings.setValue(KEY_HORIZONTALLAYOUT, pEnable);
} // SetHorizontalLayout

const void Settings::SetMute(const bool &pEnable)
{
    _qsSettings.setValue(KEY_MUTE, pEnable);
} // SetMute

const void Settings::SetNewWordFlash(const bool &pEnable)
{
	_qsSettings.setValue(KEY_NEWWORDFLASH, pEnable);
} // SetNewWordFlash

const void Settings::SetNewWordSound(const bool &pEnable)
{
	_qsSettings.setValue(KEY_NEWWORDSOUND, pEnable);
} // SetNewWordSound

const void Settings::SetRememberWindowPosition(const bool &pEnable)
{
    _qsSettings.setValue(KEY_REMEMBERWINDOWPOSITION, pEnable);
} // SetRememberWindowPosition

const void Settings::SetStartLearningOnStartup(const bool &pEnable)
{
	_qsSettings.setValue(KEY_STARTLEARNINGONSTARTUP, pEnable);
} // SetStartLearningOnStartup

const void Settings::SetSwitchLearningDirection(const Qt::CheckState &pSwitch)
{
	_qsSettings.setValue(KEY_SWITCHLEARNINGDIRECTION, pSwitch);
} // SetSwitchLearningDirection

const void Settings::SetSystemTrayIcon(const bool &pEnable)
{
    _qsSettings.setValue(KEY_SYSTEMTRAYICON, pEnable);
} // SetSystemTrayIcon

Settings::Settings() : _qsSettings(ORGANIZATION, APPLICATION)
{
} // Settings

const void Settings::SetVocabularyFile(const QString &pFile)
{
	_qsSettings.setValue(KEY_VOCABULARYFILE, pFile);
} // SetVocabularyFile

const void Settings::SetWaitForAnswer(const int &pTime)
{
	_qsSettings.setValue(KEY_WAITFORANSWER, pTime);
} // SetWaitForAnswer

const void Settings::SetWindowHeight(const int &pHeight)
{
	_qsSettings.setValue(KEY_WINDOWHEIGHT, pHeight);
} // SetWindowHeight

const void Settings::SetWindowWidth(const int &pWidth)
{
	_qsSettings.setValue(KEY_WINDOWWIDTH, pWidth);
} // SetWindowWidth

const void Settings::SetWindowX(const int &pX)
{
	_qsSettings.setValue(KEY_WINDOWX, pX);
} // SetWindowX

const void Settings::SetWindowY(const int &pY)
{
	_qsSettings.setValue(KEY_WINDOWY, pY);
} // SetWindowY

const void Settings::SetWordsFrequency(const int &pFrequency)
{
    _qsSettings.setValue(KEY_WORDSFREQUENCY, pFrequency);
} // SetWordsFrequency