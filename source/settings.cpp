#include "settings.h"

const QString APPLICATION = "Vocabulary Master";
#ifndef EDITION_TRY
const QString ARRAY_VOCABULARIES = "Vocabularies";
#endif
#ifndef EDITION_FREE
const QString DEFAULT_COLORFLASH = "chartreuse";
#endif
const QString KEY_ALWAYSONTOP = "AlwaysOnTop";
#ifndef EDITION_FREE
const QString KEY_CANCHANGECATEGORYPRIORITY = "CanChangeCategoryPriority";
const QString KEY_CANENABLECATEGORIES = "CanEnableCategories";
const QString KEY_COLORFLASH = "ColorFlash";
const QString KEY_ENABLED = "Enabled";
const QString KEY_HORIZONTALLAYOUT = "HorizontalLayout";
# ifdef Q_OS_WIN
const QString KEY_HOTKEY = "Hotkey";
# endif
const QString KEY_LEARNDISABLEDWORDS = "LearnDisabledWords";
const QString KEY_LICENSE = "License";
const QString KEY_MINIMIZETOTRAY = "MinimizeToTray";
const QString KEY_MUTE = "Mute";
const QString KEY_NEWWORDFLASH = "NewWordFlash";
const QString KEY_NEWWORDSOUND = "NewWordSound";
const QString KEY_NEWWORDSOUNDFILE = "NewWordSoundFile";
const QString KEY_NEWWORDSOUNDTYPE = "NewWordSoundType";
#endif
const QString KEY_PROXYHOSTNAME = "ProxyHostname";
const QString KEY_PROXYPASSWORD = "ProxyPassword";
const QString KEY_PROXYPORT = "ProxyPort";
const QString KEY_PROXYTYPE = "ProxyType";
const QString KEY_PROXYUSERNAME = "ProxyUsername";
#ifndef EDITION_FREE
const QString KEY_REMEMBERWINDOWPOSITION = "RememberWindowPosition";
const QString KEY_SHOWCATEGORYNAME = "ShowCategoryName";
const QString KEY_SHOWLANGUAGENAMES = "ShowLanguageNames";
const QString KEY_SHOWRECORDCONTROLS = "ShowRecordControls";
const QString KEY_SHOWSTATUSBAR= "ShowStatusBar";
const QString KEY_SHOWTOOLBAR = "ShowToolBar";
const QString KEY_SHOWWORDSINTRAYBALLOON = "ShowWordsInTrayBalloon";
const QString KEY_STARTLEARNINGONSTARTUP = "StartLearningOnStartup";
#endif
const QString KEY_SWITCHLEARNINGDIRECTION = "SwitchLearningDirection";
#ifndef EDITION_FREE
const QString KEY_SYSTEMTRAYICON = "SystemTrayIcon";
#endif
const QString KEY_TRANSLATION = "Translation";
const QString KEY_UPDATECHECK = "UpdateCheck";
const QString KEY_USEPROXY = "UseProxy";
#ifndef EDITION_TRY
const QString KEY_VERSION = "Version";
#endif
const QString KEY_VOCABULARYFILE = "VocabularyFile";
#ifndef EDITION_FREE
const QString KEY_WAITFORANSWER = "WaitForAnswer";
const QString KEY_WINDOWHEIGHT = "WindowHeight";
const QString KEY_WINDOWWIDTH = "WindowWidth";
const QString KEY_WINDOWX = "WindowX";
const QString KEY_WINDOWY = "WindowY";
#endif
const QString KEY_WORDSFREQUENCY = "WordsFrequency";
const QString ORGANIZATION  = "Isshou";
#if !defined(EDITION_FREE) && defined(Q_OS_WIN)
const QString SHORTCUT_ANSWER = "Answer";
const QString SHORTCUT_MINIMIZE = "Minimize";
const QString SHORTCUT_NEXT = "Next";
const QString SHORTCUT_RESTORE = "Restore";
#endif

const bool Settings::alwaysOnTop() const
{
    return _settings.value(KEY_ALWAYSONTOP, false).toBool();
} // alwaysOnTop

#ifndef EDITION_FREE
const bool Settings::canChangeCategoryPriority() const
{
    return _settings.value(KEY_CANCHANGECATEGORYPRIORITY, true).toBool();
} // canChangeCategoryPriority

const bool Settings::canEnableCategories() const
{
    return _settings.value(KEY_CANENABLECATEGORIES, true).toBool();
} // canEnableCategories

const QString Settings::colorFlash() const
{
    return _settings.value(KEY_COLORFLASH, DEFAULT_COLORFLASH).toString();
} // colorFlash

const bool Settings::horizontalLayout() const
{
    return _settings.value(KEY_HORIZONTALLAYOUT, false).toBool();
} // horizontalLayout

# ifdef Q_OS_WIN
const Settings::HotKeyInfo Settings::hotkey(const Hotkey &pType) const
{
	HotKeyInfo shkiHotKey;

	shkiHotKey.text = _settings.value(hotkeyKeyText(pType)).toString();
	shkiHotKey.virtualKey = _settings.value(hotkeyKeyVirtualKey(pType)).toUInt();

    return shkiHotKey;
} // hotkey

const QString Settings::hotkeyKey(const Hotkey &pType) const
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
} // hotkeyKey

const QString Settings::hotkeyKeyText(const Hotkey &pType) const
{
	return hotkeyKey(pType) + "Text";
} // hotkeyKeyText

const QString Settings::hotkeyKeyVirtualKey(const Hotkey &pType) const
{
	return hotkeyKey(pType) + "VirtualKey";
} // hotkeyKeyVirtualKey
# endif

const bool Settings::learnDisabledWords() const
{
	return _settings.value(KEY_LEARNDISABLEDWORDS, false).toBool();
} // learnDisabledWords

const QByteArray Settings::license() const
{
    return _settings.value(KEY_LICENSE).toByteArray();
} // license

const bool Settings::minimizeToTray() const
{
	return _settings.value(KEY_MINIMIZETOTRAY, false).toBool();
} // minimizeToTray

const bool Settings::mute() const
{
    return _settings.value(KEY_MUTE, false).toBool();
} // mute

const bool Settings::newWordFlash() const
{
	return _settings.value(KEY_NEWWORDFLASH, false).toBool();
} // newWordFlash

const bool Settings::newWordSound() const
{
	return _settings.value(KEY_NEWWORDSOUND, true).toBool();
} // newWordSound

const QString Settings::newWordSoundFile() const
{
    return _settings.value(KEY_NEWWORDSOUNDFILE).toString();
} // newWordSoundFile

const Settings::NewWordSoundType Settings::newWordSoundType() const
{
    return static_cast<NewWordSoundType>(_settings.value(KEY_NEWWORDSOUNDTYPE, NewWordSoundTypeSystem).toInt());
} // newWordSoundType
#endif

const QString Settings::proxyHostname() const
{
	return _settings.value(KEY_PROXYHOSTNAME).toString();
} // proxyHostname

const QString Settings::proxyPassword() const
{
	return _settings.value(KEY_PROXYPASSWORD).toString();
} // proxyPassword

const quint16 Settings::proxyPort() const
{
	return _settings.value(KEY_PROXYPORT, 8080).toInt();
} // proxyPort

const QNetworkProxy::ProxyType Settings::proxyType() const
{
	return static_cast<QNetworkProxy::ProxyType>(_settings.value(KEY_PROXYTYPE, QNetworkProxy::HttpProxy).toInt());
} // proxyType

const QString Settings::proxyUsername() const
{
	return _settings.value(KEY_PROXYUSERNAME).toString();
} // proxyUsername

#ifndef EDITION_FREE
const bool Settings::rememberWindowPosition() const
{
    return _settings.value(KEY_REMEMBERWINDOWPOSITION, true).toBool();
} // rememberWindowPosition

const bool Settings::showCategoryName() const
{
	return _settings.value(KEY_SHOWCATEGORYNAME, true).toBool();
} // showCategoryName

const bool Settings::showLanguageNames() const
{
	return _settings.value(KEY_SHOWLANGUAGENAMES, true).toBool();
} // showLanguageNames

const bool Settings::showRecordControls() const
{
	return _settings.value(KEY_SHOWRECORDCONTROLS, true).toBool();
} // showRecordControls

const bool Settings::showStatusBar() const
{
	return _settings.value(KEY_SHOWSTATUSBAR, true).toBool();
} // showStatusBar

const bool Settings::showToolBar() const
{
	return _settings.value(KEY_SHOWTOOLBAR, true).toBool();
} // showToolBar

const bool Settings::showWordsInTrayBalloon() const
{
	return _settings.value(KEY_SHOWWORDSINTRAYBALLOON, false).toBool();
} // showWordsInTrayBalloon

const bool Settings::startLearningOnStartup() const
{
	return _settings.value(KEY_STARTLEARNINGONSTARTUP, false).toBool();
} // startLearningOnStartup
#endif

const Qt::CheckState Settings::switchLearningDirection() const
{
	return static_cast<Qt::CheckState>(_settings.value(KEY_SWITCHLEARNINGDIRECTION, false).toInt());
} // switchLearningDirection

#ifndef EDITION_FREE
const bool Settings::systemTrayIcon() const
{
    return _settings.value(KEY_SYSTEMTRAYICON, false).toBool();
} // systemTrayIcon
#endif

const QString Settings::translation() const
{
    return _settings.value(KEY_TRANSLATION).toString();
} // translation

const bool Settings::updateCheck() const
{
	return _settings.value(KEY_UPDATECHECK, true).toBool();
} // updateCheck

const bool Settings::useProxy() const
{
	return _settings.value(KEY_USEPROXY, false).toBool();
} // useProxy

#ifndef EDITION_TRY
const int Settings::vocabularyCount()
{
	int iCount = _settings.beginReadArray(ARRAY_VOCABULARIES);
	_settings.endArray();

#ifdef EDITION_FREE
	return qMin(iCount, EDITION_FREE_VOCABULARIES_MAX);
#else
	return iCount;
#endif
} // vocabularyCount

const Settings::VocabularyInfo Settings::vocabularyInfo(const int &pIndex)
{
	VocabularyInfo sviVocabularyInfo;

	_settings.beginReadArray(ARRAY_VOCABULARIES);
	_settings.setArrayIndex(pIndex);
	sviVocabularyInfo.filePath = _settings.value(KEY_VOCABULARYFILE).toString();
#if !defined(EDITION_FREE) && !defined(EDITION_TRY)
	sviVocabularyInfo.enabled = _settings.value(KEY_ENABLED, true).toBool();
#endif
	_settings.endArray();

	return sviVocabularyInfo;
} // vocabularyInfo
#endif

const int Settings::waitForAnswer() const
{
#ifndef EDITION_FREE
	return _settings.value(KEY_WAITFORANSWER, DEFAULT_WAIT).toInt();
#else
    return DEFAULT_WAIT;
#endif
} // waitForAnswer

#ifndef EDITION_FREE
const int Settings::windowHeight() const
{
	return _settings.value(KEY_WINDOWHEIGHT, DEFAULT_DIMENSION).toInt();
} // windowHeight

const int Settings::windowWidth() const
{
	return _settings.value(KEY_WINDOWWIDTH, DEFAULT_DIMENSION).toInt();
} // windowWidth

const int Settings::windowX() const
{
	return _settings.value(KEY_WINDOWX, DEFAULT_DIMENSION).toInt();
} // windowX

const int Settings::windowY() const
{
	return _settings.value(KEY_WINDOWY, DEFAULT_DIMENSION).toInt();
} // windowY
#endif

const int Settings::wordsFrequency() const
{
    return _settings.value(KEY_WORDSFREQUENCY, DEFAULT_FREQUENCY).toInt();
} // wordsFrequency

const void Settings::setAlwaysOnTop(const bool &pEnable)
{
    _settings.setValue(KEY_ALWAYSONTOP, pEnable);
} // setAlwaysOnTop

#ifndef EDITION_FREE
const void Settings::setCanChangeCategoryPriority(const bool &pEnable)
{
    _settings.setValue(KEY_CANCHANGECATEGORYPRIORITY, pEnable);
} // setCanChangeCategoryPriority

const void Settings::setCanEnableCategories(const bool &pEnable)
{
    _settings.setValue(KEY_CANENABLECATEGORIES, pEnable);
} // setCanEnableCategories

const void Settings::setColorFlash(const QString &pColor)
{
    _settings.setValue(KEY_COLORFLASH, pColor);
} // setColorFlash

const void Settings::setHorizontalLayout(const bool &pEnable)
{
    _settings.setValue(KEY_HORIZONTALLAYOUT, pEnable);
} // setHorizontalLayout

# ifdef Q_OS_WIN
const void Settings::setHotkey(const Hotkey &pType, const HotKeyInfo &pHotkey)
{
    _settings.setValue(hotkeyKeyText(pType), pHotkey.text);
	_settings.setValue(hotkeyKeyVirtualKey(pType), pHotkey.virtualKey);
} // setHotkey
# endif

const void Settings::setLearnDisabledWords(const bool &pEnable)
{
	_settings.setValue(KEY_LEARNDISABLEDWORDS, pEnable);
} // setLearnDisabledWords

const void Settings::setLicense(const QByteArray &pLicense)
{
	_settings.setValue(KEY_LICENSE, pLicense);
} // setLicense

const void Settings::setMinimizeToTray(const bool &pEnable)
{
	_settings.setValue(KEY_MINIMIZETOTRAY, pEnable);
} // setMinimizeToTray

const void Settings::setMute(const bool &pEnable)
{
    _settings.setValue(KEY_MUTE, pEnable);
} // setMute

const void Settings::setNewWordFlash(const bool &pEnable)
{
	_settings.setValue(KEY_NEWWORDFLASH, pEnable);
} // setNewWordFlash

const void Settings::setNewWordSound(const bool &pEnable)
{
	_settings.setValue(KEY_NEWWORDSOUND, pEnable);
} // setNewWordSound

const void Settings::setNewWordSoundFile(const QString &pFile)
{
    _settings.setValue(KEY_NEWWORDSOUNDFILE, pFile);
} // setNewWordSoundFile

const void Settings::setNewWordSoundType(const NewWordSoundType &pType)
{
    _settings.setValue(KEY_NEWWORDSOUNDTYPE, pType);
} // setNewWordSoundType
#endif

const void Settings::setProxyHostname(const QString &pHostname)
{
	_settings.setValue(KEY_PROXYHOSTNAME, pHostname);
} // setProxyHostname

const void Settings::setProxyPassword(const QString &pPassword)
{
	_settings.setValue(KEY_PROXYPASSWORD, pPassword);
} // setProxyPassword

const void Settings::setProxyPort(const quint16 &pPort)
{
	_settings.setValue(KEY_PROXYPORT, pPort);
} // setProxyPort

const void Settings::setProxyType(const QNetworkProxy::ProxyType &pType)
{
	_settings.setValue(KEY_PROXYTYPE, pType);
} // setProxyType

const void Settings::setProxyUsername(const QString &pUsername)
{
	_settings.setValue(KEY_PROXYUSERNAME, pUsername);
} // setProxyUsername

#ifndef EDITION_FREE
const void Settings::setRememberWindowPosition(const bool &pEnable)
{
    _settings.setValue(KEY_REMEMBERWINDOWPOSITION, pEnable);
} // setRememberWindowPosition

const void Settings::setShowCategoryName(const bool &pShow)
{
	_settings.setValue(KEY_SHOWCATEGORYNAME, pShow);
} // setShowCategoryName

const void Settings::setShowLanguageNames(const bool &pShow)
{
	_settings.setValue(KEY_SHOWLANGUAGENAMES, pShow);
} // setShowLanguageNames

const void Settings::setShowRecordControls(const bool &pShow)
{
	_settings.setValue(KEY_SHOWRECORDCONTROLS, pShow);
} // setShowRecordControls

const void Settings::setShowStatusBar(const bool &pShow)
{
	_settings.setValue(KEY_SHOWSTATUSBAR, pShow);
} // setShowStatusBar

const void Settings::setShowToolBar(const bool &pShow)
{
	_settings.setValue(KEY_SHOWTOOLBAR, pShow);
} // setShowToolBar

const void Settings::setShowWordsInTrayBalloon(const bool &pEnable)
{
	_settings.setValue(KEY_SHOWWORDSINTRAYBALLOON, pEnable);
} // setShowWordsInTrayBalloon

const void Settings::setStartLearningOnStartup(const bool &pEnable)
{
	_settings.setValue(KEY_STARTLEARNINGONSTARTUP, pEnable);
} // setStartLearningOnStartup
#endif

const void Settings::setSwitchLearningDirection(const Qt::CheckState &pSwitch)
{
	_settings.setValue(KEY_SWITCHLEARNINGDIRECTION, pSwitch);
} // setSwitchLearningDirection

#ifndef EDITION_FREE
const void Settings::setSystemTrayIcon(const bool &pEnable)
{
    _settings.setValue(KEY_SYSTEMTRAYICON, pEnable);
} // setSystemTrayIcon
#endif

Settings::Settings() : _settings(ORGANIZATION, APPLICATION)
{
#ifndef EDITION_TRY
	updateSettings();
#endif
} // Settings

const void Settings::setTranslation(const QString &pTranslation)
{
    _settings.setValue(KEY_TRANSLATION, pTranslation);
} // setTranslation

const void Settings::setUpdateCheck(const bool &pCheck)
{
	_settings.setValue(KEY_UPDATECHECK, pCheck);
} // setUpdateCheck

const void Settings::setUseProxy(const bool &pUse)
{
	_settings.setValue(KEY_USEPROXY, pUse);
} // setUseProxy

#ifndef EDITION_TRY
const void Settings::setVocabularyCount(const int &pCount)
{
	_settings.beginWriteArray(ARRAY_VOCABULARIES, pCount);
	_settings.endArray();
} // setVocabularyCount

const void Settings::setVocabularyInfo(const int &pIndex, const VocabularyInfo &pInfo)
{
	_settings.beginWriteArray(ARRAY_VOCABULARIES);
	_settings.setArrayIndex(pIndex);
	_settings.setValue(KEY_VOCABULARYFILE, pInfo.filePath);
#if !defined(EDITION_FREE) && !defined(EDITION_TRY)
	_settings.setValue(KEY_ENABLED, pInfo.enabled);
#endif
	_settings.endArray();
} // setVocabularyInfo
#endif

#ifndef EDITION_FREE
const void Settings::setWaitForAnswer(const int &pTime)
{
	_settings.setValue(KEY_WAITFORANSWER, pTime);
} // setWaitForAnswer

const void Settings::setWindowHeight(const int &pHeight)
{
	_settings.setValue(KEY_WINDOWHEIGHT, pHeight);
} // setWindowHeight

const void Settings::setWindowWidth(const int &pWidth)
{
	_settings.setValue(KEY_WINDOWWIDTH, pWidth);
} // setWindowWidth

const void Settings::setWindowX(const int &pX)
{
	_settings.setValue(KEY_WINDOWX, pX);
} // setWindowX

const void Settings::setWindowY(const int &pY)
{
	_settings.setValue(KEY_WINDOWY, pY);
} // setWindowY
#endif

const void Settings::setWordsFrequency(const int &pFrequency)
{
    _settings.setValue(KEY_WORDSFREQUENCY, pFrequency);
} // setWordsFrequency

#ifndef EDITION_TRY
const void Settings::updateSettings()
{
	Version evCurrent = static_cast<Version>(_settings.value(KEY_VERSION, Version1).toInt());

	if (evCurrent < Version2) {
		// move vocabulary to vocabularies group
		QString qsVocabulary = _settings.value(KEY_VOCABULARYFILE).toString();
		if (!qsVocabulary.isEmpty()) {
			_settings.beginWriteArray(ARRAY_VOCABULARIES);
			_settings.setArrayIndex(VocabularyPosition1);
			_settings.setValue(KEY_VOCABULARYFILE, qsVocabulary);
			_settings.endArray();
		} // if
		_settings.remove(KEY_VOCABULARYFILE);

		_settings.setValue(KEY_VERSION, Version2);
	} // if
} // updateSettings
#endif