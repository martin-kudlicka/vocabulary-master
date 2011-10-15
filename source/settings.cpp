#include "settings.h"

const QString APPLICATION = "Vocabulary Master";
#ifndef FREE
const QString DEFAULT_COLORFLASH = "chartreuse";
#endif
const QString KEY_ALWAYSONTOP = "AlwaysOnTop";
#ifndef FREE
const QString KEY_CANCHANGECATEGORYPRIORITY = "CanChangeCategoryPriority";
const QString KEY_CANENABLECATEGORIES = "CanEnableCategories";
const QString KEY_COLORFLASH = "ColorFlash";
const QString KEY_HORIZONTALLAYOUT = "HorizontalLayout";
# ifdef Q_WS_WIN
const QString KEY_HOTKEY = "Hotkey";
# endif
const QString KEY_LICENSE = "License";
const QString KEY_MINIMIZETOTRAY = "MinimizeToTray";
const QString KEY_MUTE = "Mute";
const QString KEY_NEWWORDFLASH = "NewWordFlash";
const QString KEY_NEWWORDSOUND = "NewWordSound";
const QString KEY_NEWWORDSOUNDFILE = "NewWordSoundFile";
const QString KEY_NEWWORDSOUNDTYPE = "NewWordSoundType";
const QString KEY_REMEMBERWINDOWPOSITION = "RememberWindowPosition";
const QString KEY_SHOWCATEGORYNAME = "ShowCategoryName";
const QString KEY_SHOWLANGUAGENAMES = "ShowLanguageNames";
const QString KEY_SHOWSTATUSBAR= "ShowStatusBar";
const QString KEY_SHOWTOOLBAR = "ShowToolBar";
const QString KEY_SHOWWORDSINTRAYBALLOON = "ShowWordsInTrayBalloon";
const QString KEY_STARTLEARNINGONSTARTUP = "StartLearningOnStartup";
#endif
const QString KEY_SWITCHLEARNINGDIRECTION = "SwitchLearningDirection";
#ifndef FREE
const QString KEY_SYSTEMTRAYICON = "SystemTrayIcon";
#endif
const QString KEY_TRANSLATION = "Translation";
const QString KEY_VOCABULARYFILE = "VocabularyFile";
#ifndef FREE
const QString KEY_WAITFORANSWER = "WaitForAnswer";
const QString KEY_WINDOWHEIGHT = "WindowHeight";
const QString KEY_WINDOWWIDTH = "WindowWidth";
const QString KEY_WINDOWX = "WindowX";
const QString KEY_WINDOWY = "WindowY";
#endif
const QString KEY_WORDSFREQUENCY = "WordsFrequency";
const QString ORGANIZATION  = "Isshou";
#if !defined(FREE) && defined(Q_WS_WIN)
const QString SHORTCUT_ANSWER = "Answer";
const QString SHORTCUT_MINIMIZE = "Minimize";
const QString SHORTCUT_NEXT = "Next";
const QString SHORTCUT_RESTORE = "Restore";
#endif

const bool Settings::GetAlwaysOnTop() const
{
    return _qsSettings.value(KEY_ALWAYSONTOP, false).toBool();
} // GetAlwaysOnTop

#ifndef FREE
const bool Settings::GetCanChangeCategoryPriority() const
{
    return _qsSettings.value(KEY_CANCHANGECATEGORYPRIORITY, true).toBool();
} // GetCanChangeCategoryPriority

const bool Settings::GetCanEnableCategories() const
{
    return _qsSettings.value(KEY_CANENABLECATEGORIES, true).toBool();
} // GetCanEnableCategories

const QString Settings::GetColorFlash() const
{
    return _qsSettings.value(KEY_COLORFLASH, DEFAULT_COLORFLASH).toString();
} // GetColorFlash

const bool Settings::GetHorizontalLayout() const
{
    return _qsSettings.value(KEY_HORIZONTALLAYOUT, false).toBool();
} // GetHorizontalLayout

# ifdef Q_WS_WIN
const Settings::sHotKeyInfo Settings::GetHotkey(const eHotkey &pType) const
{
	sHotKeyInfo shkiHotKey;

	shkiHotKey.qsText = _qsSettings.value(GetHotkeyKeyText(pType)).toString();
	shkiHotKey.qui32VirtualKey = _qsSettings.value(GetHotkeyKeyVirtualKey(pType)).toUInt();

    return shkiHotKey;
} // GetHotkey

const QString Settings::GetHotkeyKey(const eHotkey &pType) const
{
    QString qsKey = KEY_HOTKEY;

    switch (pType) {
        case HotkeyAnswer:
            qsKey += SHORTCUT_ANSWER;
            break;
        case HotkeyMinimize:
            qsKey += SHORTCUT_MINIMIZE;
            break;
        case HotkeyNext:
            qsKey += SHORTCUT_NEXT;
            break;
        case HotkeyRestore:
            qsKey += SHORTCUT_RESTORE;
            break;
    } // switch

    return qsKey;
} // GetHotkeyKey

const QString Settings::GetHotkeyKeyText(const eHotkey &pType) const
{
	return GetHotkeyKey(pType) + "Text";
} // GetHotkeyKeyText

const QString Settings::GetHotkeyKeyVirtualKey(const eHotkey &pType) const
{
	return GetHotkeyKey(pType) + "VirtualKey";
} // GetHotkeyKeyVirtualKey
# endif

const QByteArray Settings::GetLicense() const
{
    return _qsSettings.value(KEY_LICENSE).toByteArray();
} // GetLicense

const bool Settings::GetMinimizeToTray() const
{
	return _qsSettings.value(KEY_MINIMIZETOTRAY, false).toBool();
} // GetMinimizeToTray

const bool Settings::GetMute() const
{
    return _qsSettings.value(KEY_MUTE, false).toBool();
} // GetMute

const bool Settings::GetNewWordFlash() const
{
	return _qsSettings.value(KEY_NEWWORDFLASH, false).toBool();
} // GetNewWordFlash

const bool Settings::GetNewWordSound() const
{
	return _qsSettings.value(KEY_NEWWORDSOUND, true).toBool();
} // GetNewWordSound

const QString Settings::GetNewWordSoundFile() const
{
    return _qsSettings.value(KEY_NEWWORDSOUNDFILE).toString();
} // GetNewWordSoundFile

const Settings::eNewWordSoundType Settings::GetNewWordSoundType() const
{
    return static_cast<eNewWordSoundType>(_qsSettings.value(KEY_NEWWORDSOUNDTYPE, NewWordSoundTypeSystem).toInt());
} // GetNewWordSoundType

const bool Settings::GetRememberWindowPosition() const
{
    return _qsSettings.value(KEY_REMEMBERWINDOWPOSITION, true).toBool();
} // GetRememberWindowPosition

const bool Settings::GetShowCategoryName() const
{
	return _qsSettings.value(KEY_SHOWCATEGORYNAME, true).toBool();
} // GetShowCategoryName

const bool Settings::GetShowLanguageNames() const
{
	return _qsSettings.value(KEY_SHOWLANGUAGENAMES, true).toBool();
} // GetShowLanguageNames

const bool Settings::GetShowStatusBar() const
{
	return _qsSettings.value(KEY_SHOWSTATUSBAR, true).toBool();
} // GetShowStatusBar

const bool Settings::GetShowToolBar() const
{
	return _qsSettings.value(KEY_SHOWTOOLBAR, true).toBool();
} // GetShowToolBar

const bool Settings::GetShowWordsInTrayBalloon() const
{
	return _qsSettings.value(KEY_SHOWWORDSINTRAYBALLOON, false).toBool();
} // GetShowWordsInTrayBalloon

const bool Settings::GetStartLearningOnStartup() const
{
	return _qsSettings.value(KEY_STARTLEARNINGONSTARTUP, false).toBool();
} // GetStartLearningOnStartup
#endif

const Qt::CheckState Settings::GetSwitchLearningDirection() const
{
	return static_cast<Qt::CheckState>(_qsSettings.value(KEY_SWITCHLEARNINGDIRECTION, false).toInt());
} // GetSwitchLearningDirection

#ifndef FREE
const bool Settings::GetSystemTrayIcon() const
{
    return _qsSettings.value(KEY_SYSTEMTRAYICON, false).toBool();
} // GetSystemTrayIcon
#endif

const QString Settings::GetTranslation() const
{
    return _qsSettings.value(KEY_TRANSLATION).toString();
} // GetTranslation

#ifndef TRY
const QString Settings::GetVocabularyFile() const
{
	return _qsSettings.value(KEY_VOCABULARYFILE).toString();
} // GetVocabularyFile
#endif

const int Settings::GetWaitForAnswer() const
{
#ifndef FREE
	return _qsSettings.value(KEY_WAITFORANSWER, DEFAULT_WAIT).toInt();
#else
    return DEFAULT_WAIT;
#endif
} // GetWaitForAnswer

#ifndef FREE
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
#endif

const int Settings::GetWordsFrequency() const
{
    return _qsSettings.value(KEY_WORDSFREQUENCY, DEFAULT_FREQUENCY).toInt();
} // GetWordsFrequency

const void Settings::SetAlwaysOnTop(const bool &pEnable)
{
    _qsSettings.setValue(KEY_ALWAYSONTOP, pEnable);
} // SetAlwaysOnTop

#ifndef FREE
const void Settings::SetCanChangeCategoryPriority(const bool &pEnable)
{
    _qsSettings.setValue(KEY_CANCHANGECATEGORYPRIORITY, pEnable);
} // SetCanChangeCategoryPriority

const void Settings::SetCanEnableCategories(const bool &pEnable)
{
    _qsSettings.setValue(KEY_CANENABLECATEGORIES, pEnable);
} // SetCanEnableCategories

const void Settings::SetColorFlash(const QString &pColor)
{
    _qsSettings.setValue(KEY_COLORFLASH, pColor);
} // SetColorFlash

const void Settings::SetHorizontalLayout(const bool &pEnable)
{
    _qsSettings.setValue(KEY_HORIZONTALLAYOUT, pEnable);
} // SetHorizontalLayout

# ifdef Q_WS_WIN
const void Settings::SetHotkey(const eHotkey &pType, const sHotKeyInfo &pHotkey)
{
    _qsSettings.setValue(GetHotkeyKeyText(pType), pHotkey.qsText);
	_qsSettings.setValue(GetHotkeyKeyVirtualKey(pType), pHotkey.qui32VirtualKey);
} // SetHotkey
# endif

const void Settings::SetLicense(const QByteArray &pLicense)
{
	_qsSettings.setValue(KEY_LICENSE, pLicense);
} // SetLicense

const void Settings::SetMinimizeToTray(const bool &pEnable)
{
	_qsSettings.setValue(KEY_MINIMIZETOTRAY, pEnable);
} // SetMinimizeToTray

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

const void Settings::SetNewWordSoundFile(const QString &pFile)
{
    _qsSettings.setValue(KEY_NEWWORDSOUNDFILE, pFile);
} // SetNewWordSoundFile

const void Settings::SetNewWordSoundType(const eNewWordSoundType &pType)
{
    _qsSettings.setValue(KEY_NEWWORDSOUNDTYPE, pType);
} // SetNewWordSoundType

const void Settings::SetRememberWindowPosition(const bool &pEnable)
{
    _qsSettings.setValue(KEY_REMEMBERWINDOWPOSITION, pEnable);
} // SetRememberWindowPosition

const void Settings::SetShowCategoryName(const bool &pShow)
{
	_qsSettings.setValue(KEY_SHOWCATEGORYNAME, pShow);
} // SetShowCategoryName

const void Settings::SetShowLanguageNames(const bool &pShow)
{
	_qsSettings.setValue(KEY_SHOWLANGUAGENAMES, pShow);
} // SetShowLanguageNames

const void Settings::SetShowStatusBar(const bool &pShow)
{
	_qsSettings.setValue(KEY_SHOWSTATUSBAR, pShow);
} // SetShowStatusBar

const void Settings::SetShowToolBar(const bool &pShow)
{
	_qsSettings.setValue(KEY_SHOWTOOLBAR, pShow);
} // SetShowToolBar

const void Settings::SetShowWordsInTrayBalloon(const bool &pEnable)
{
	_qsSettings.setValue(KEY_SHOWWORDSINTRAYBALLOON, pEnable);
} // SetShowWordsInTrayBalloon

const void Settings::SetStartLearningOnStartup(const bool &pEnable)
{
	_qsSettings.setValue(KEY_STARTLEARNINGONSTARTUP, pEnable);
} // SetStartLearningOnStartup
#endif

const void Settings::SetSwitchLearningDirection(const Qt::CheckState &pSwitch)
{
	_qsSettings.setValue(KEY_SWITCHLEARNINGDIRECTION, pSwitch);
} // SetSwitchLearningDirection

#ifndef FREE
const void Settings::SetSystemTrayIcon(const bool &pEnable)
{
    _qsSettings.setValue(KEY_SYSTEMTRAYICON, pEnable);
} // SetSystemTrayIcon
#endif

Settings::Settings() : _qsSettings(ORGANIZATION, APPLICATION)
{
} // Settings

const void Settings::SetTranslation(const QString &pTranslation)
{
    _qsSettings.setValue(KEY_TRANSLATION, pTranslation);
} // SetTranslation

#ifndef TRY
const void Settings::SetVocabularyFile(const QString &pFile)
{
	_qsSettings.setValue(KEY_VOCABULARYFILE, pFile);
} // SetVocabularyFile
#endif

#ifndef FREE
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
#endif

const void Settings::SetWordsFrequency(const int &pFrequency)
{
    _qsSettings.setValue(KEY_WORDSFREQUENCY, pFrequency);
} // SetWordsFrequency