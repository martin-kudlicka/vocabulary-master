#include "settings.h"

const QString APPLICATION                   = "Vocabulary Master";
#ifndef EDITION_TRY
const QString ARRAY_VOCABULARIES            = "Vocabularies";
#endif
#ifndef EDITION_FREE
const QString DEFAULT_COLORFLASH            = "chartreuse";
#endif
const QString KEY_ALWAYSONTOP               = "AlwaysOnTop";
#ifndef EDITION_FREE
const QString KEY_CANCHANGECATEGORYPRIORITY = "CanChangeCategoryPriority";
const QString KEY_CANENABLECATEGORIES       = "CanEnableCategories";
const QString KEY_COLORFLASH                = "ColorFlash";
const QString KEY_ENABLED                   = "Enabled";
const QString KEY_HORIZONTALLAYOUT          = "HorizontalLayout";
# ifdef Q_OS_WIN
const QString KEY_HOTKEY                    = "Hotkey";
# endif
const QString KEY_LEARNDISABLEDWORDS        = "LearnDisabledWords";
const QString KEY_LICENSE                   = "License";
const QString KEY_MINIMIZETOTRAY            = "MinimizeToTray";
const QString KEY_MUTE                      = "Mute";
const QString KEY_NEWWORDFLASH              = "NewWordFlash";
const QString KEY_NEWWORDSOUND              = "NewWordSound";
const QString KEY_NEWWORDSOUNDFILE          = "NewWordSoundFile";
const QString KEY_NEWWORDSOUNDTYPE          = "NewWordSoundType";
#endif
const QString KEY_PROXYHOSTNAME             = "ProxyHostname";
const QString KEY_PROXYPASSWORD             = "ProxyPassword";
const QString KEY_PROXYPORT                 = "ProxyPort";
const QString KEY_PROXYTYPE                 = "ProxyType";
const QString KEY_PROXYUSERNAME             = "ProxyUsername";
#ifndef EDITION_FREE
const QString KEY_REMEMBERWINDOWPOSITION    = "RememberWindowPosition";
const QString KEY_SHOWCATEGORYNAME          = "ShowCategoryName";
const QString KEY_SHOWLANGUAGENAMES         = "ShowLanguageNames";
const QString KEY_SHOWRECORDCONTROLS        = "ShowRecordControls";
const QString KEY_SHOWSTATUSBAR             = "ShowStatusBar";
const QString KEY_SHOWTOOLBAR               = "ShowToolBar";
const QString KEY_SHOWWORDSINTRAYBALLOON    = "ShowWordsInTrayBalloon";
const QString KEY_STARTLEARNINGONSTARTUP    = "StartLearningOnStartup";
#endif
const QString KEY_SWITCHLEARNINGDIRECTION   = "SwitchLearningDirection";
#ifndef EDITION_FREE
const QString KEY_SYSTEMTRAYICON            = "SystemTrayIcon";
#endif
const QString KEY_TRANSLATION               = "Translation";
const QString KEY_UPDATECHECK               = "UpdateCheck";
const QString KEY_USEPROXY                  = "UseProxy";
#ifndef EDITION_TRY
const QString KEY_VERSION                   = "Version";
#endif
const QString KEY_VOCABULARYFILE            = "VocabularyFile";
#ifndef EDITION_FREE
const QString KEY_WAITFORANSWER             = "WaitForAnswer";
const QString KEY_WINDOWHEIGHT              = "WindowHeight";
const QString KEY_WINDOWWIDTH               = "WindowWidth";
const QString KEY_WINDOWX                   = "WindowX";
const QString KEY_WINDOWY                   = "WindowY";
#endif
const QString KEY_WORDSFREQUENCY            = "WordsFrequency";
const QString ORGANIZATION                  = "Isshou";
#if !defined(EDITION_FREE) && defined(Q_OS_WIN)
const QString SHORTCUT_ANSWER               = "Answer";
const QString SHORTCUT_MINIMIZE             = "Minimize";
const QString SHORTCUT_NEXT                 = "Next";
const QString SHORTCUT_RESTORE              = "Restore";
#endif

Settings::Settings() : _settings(ORGANIZATION, APPLICATION)
{
#ifndef EDITION_TRY
	updateSettings();
#endif
} // Settings

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
const Settings::HotkeyInfo Settings::hotkey(const Hotkey &type) const
{
	HotkeyInfo hotkeyInfo;

	hotkeyInfo.text       = _settings.value(hotkeyKeyText(type)).toString();
	hotkeyInfo.virtualKey = _settings.value(hotkeyKeyVirtualKey(type)).toUInt();

    return hotkeyInfo;
} // hotkey
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
    return static_cast<NewWordSoundType>(_settings.value(KEY_NEWWORDSOUNDTYPE, NewWordSoundTypeSystem).toUInt());
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
	return _settings.value(KEY_PROXYPORT, 8080).toUInt();
} // proxyPort

const QNetworkProxy::ProxyType Settings::proxyType() const
{
	return static_cast<QNetworkProxy::ProxyType>(_settings.value(KEY_PROXYTYPE, QNetworkProxy::HttpProxy).toUInt());
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
#endif

const void Settings::setAlwaysOnTop(const bool &enable)
{
	_settings.setValue(KEY_ALWAYSONTOP, enable);
} // setAlwaysOnTop

#ifndef EDITION_FREE
const void Settings::setCanChangeCategoryPriority(const bool &enable)
{
    _settings.setValue(KEY_CANCHANGECATEGORYPRIORITY, enable);
} // setCanChangeCategoryPriority

const void Settings::setCanEnableCategories(const bool &enable)
{
    _settings.setValue(KEY_CANENABLECATEGORIES, enable);
} // setCanEnableCategories

const void Settings::setColorFlash(const QString &color)
{
    _settings.setValue(KEY_COLORFLASH, color);
} // setColorFlash

const void Settings::setHorizontalLayout(const bool &enable)
{
    _settings.setValue(KEY_HORIZONTALLAYOUT, enable);
} // setHorizontalLayout

# ifdef Q_OS_WIN
const void Settings::setHotkey(const Hotkey &type, const HotkeyInfo &hotkeyInfo)
{
    _settings.setValue(hotkeyKeyText(type), hotkeyInfo.text);
	_settings.setValue(hotkeyKeyVirtualKey(type), hotkeyInfo.virtualKey);
} // setHotkey
# endif

const void Settings::setLearnDisabledWords(const bool &enable)
{
	_settings.setValue(KEY_LEARNDISABLEDWORDS, enable);
} // setLearnDisabledWords

const void Settings::setLicense(const QByteArray &licenseData)
{
	_settings.setValue(KEY_LICENSE, licenseData);
} // setLicense

const void Settings::setMinimizeToTray(const bool &enable)
{
	_settings.setValue(KEY_MINIMIZETOTRAY, enable);
} // setMinimizeToTray

const void Settings::setMute(const bool &enable)
{
    _settings.setValue(KEY_MUTE, enable);
} // setMute

const void Settings::setNewWordFlash(const bool &enable)
{
	_settings.setValue(KEY_NEWWORDFLASH, enable);
} // setNewWordFlash

const void Settings::setNewWordSound(const bool &enable)
{
	_settings.setValue(KEY_NEWWORDSOUND, enable);
} // setNewWordSound

const void Settings::setNewWordSoundFile(const QString &file)
{
    _settings.setValue(KEY_NEWWORDSOUNDFILE, file);
} // setNewWordSoundFile

const void Settings::setNewWordSoundType(const NewWordSoundType &type)
{
    _settings.setValue(KEY_NEWWORDSOUNDTYPE, type);
} // setNewWordSoundType
#endif

const void Settings::setProxyHostname(const QString &hostname)
{
	_settings.setValue(KEY_PROXYHOSTNAME, hostname);
} // setProxyHostname

const void Settings::setProxyPassword(const QString &password)
{
	_settings.setValue(KEY_PROXYPASSWORD, password);
} // setProxyPassword

const void Settings::setProxyPort(const quint16 &port)
{
	_settings.setValue(KEY_PROXYPORT, port);
} // setProxyPort

const void Settings::setProxyType(const QNetworkProxy::ProxyType &type)
{
	_settings.setValue(KEY_PROXYTYPE, type);
} // setProxyType

const void Settings::setProxyUsername(const QString &username)
{
	_settings.setValue(KEY_PROXYUSERNAME, username);
} // setProxyUsername

#ifndef EDITION_FREE
const void Settings::setRememberWindowPosition(const bool &enable)
{
    _settings.setValue(KEY_REMEMBERWINDOWPOSITION, enable);
} // setRememberWindowPosition

const void Settings::setShowCategoryName(const bool &show)
{
	_settings.setValue(KEY_SHOWCATEGORYNAME, show);
} // setShowCategoryName

const void Settings::setShowLanguageNames(const bool &show)
{
	_settings.setValue(KEY_SHOWLANGUAGENAMES, show);
} // setShowLanguageNames

const void Settings::setShowRecordControls(const bool &show)
{
	_settings.setValue(KEY_SHOWRECORDCONTROLS, show);
} // setShowRecordControls

const void Settings::setShowStatusBar(const bool &show)
{
	_settings.setValue(KEY_SHOWSTATUSBAR, show);
} // setShowStatusBar

const void Settings::setShowToolBar(const bool &show)
{
	_settings.setValue(KEY_SHOWTOOLBAR, show);
} // setShowToolBar

const void Settings::setShowWordsInTrayBalloon(const bool &enable)
{
	_settings.setValue(KEY_SHOWWORDSINTRAYBALLOON, enable);
} // setShowWordsInTrayBalloon

const void Settings::setStartLearningOnStartup(const bool &enable)
{
	_settings.setValue(KEY_STARTLEARNINGONSTARTUP, enable);
} // setStartLearningOnStartup
#endif

const void Settings::setSwitchLearningDirection(const Qt::CheckState &switchDirection)
{
	_settings.setValue(KEY_SWITCHLEARNINGDIRECTION, switchDirection);
} // setSwitchLearningDirection

#ifndef EDITION_FREE
const void Settings::setSystemTrayIcon(const bool &enable)
{
    _settings.setValue(KEY_SYSTEMTRAYICON, enable);
} // setSystemTrayIcon
#endif

const void Settings::setTranslation(const QString &translationName)
{
    _settings.setValue(KEY_TRANSLATION, translationName);
} // setTranslation

const void Settings::setUpdateCheck(const bool &check)
{
	_settings.setValue(KEY_UPDATECHECK, check);
} // setUpdateCheck

const void Settings::setUseProxy(const bool &use)
{
	_settings.setValue(KEY_USEPROXY, use);
} // setUseProxy

#ifndef EDITION_TRY
const void Settings::setVocabularyCount(const quint8 &count)
{
	_settings.beginWriteArray(ARRAY_VOCABULARIES, count);
	_settings.endArray();
} // setVocabularyCount

const void Settings::setVocabularyInfo(const quint8 &index, const VocabularyInfo &info)
{
	_settings.beginWriteArray(ARRAY_VOCABULARIES);
	_settings.setArrayIndex(index);
	_settings.setValue(KEY_VOCABULARYFILE, info.filePath);
#if !defined(EDITION_FREE) && !defined(EDITION_TRY)
	_settings.setValue(KEY_ENABLED, info.enabled);
#endif
	_settings.endArray();
} // setVocabularyInfo
#endif

#ifndef EDITION_FREE
const void Settings::setWaitForAnswer(const quint8 &time)
{
	_settings.setValue(KEY_WAITFORANSWER, time);
} // setWaitForAnswer

const void Settings::setWindowHeight(const quint16 &height)
{
	_settings.setValue(KEY_WINDOWHEIGHT, height);
} // setWindowHeight

const void Settings::setWindowWidth(const quint16 &width)
{
	_settings.setValue(KEY_WINDOWWIDTH, width);
} // setWindowWidth

const void Settings::setWindowX(const quint16 &x)
{
	_settings.setValue(KEY_WINDOWX, x);
} // setWindowX

const void Settings::setWindowY(const quint16 &y)
{
	_settings.setValue(KEY_WINDOWY, y);
} // setWindowY
#endif

const void Settings::setWordsFrequency(const quint8 &frequency)
{
    _settings.setValue(KEY_WORDSFREQUENCY, frequency);
} // setWordsFrequency

#ifndef EDITION_FREE
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
	return static_cast<Qt::CheckState>(_settings.value(KEY_SWITCHLEARNINGDIRECTION, false).toUInt());
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
const quint8 Settings::vocabularyCount()
{
	const quint8 count = _settings.beginReadArray(ARRAY_VOCABULARIES);
	_settings.endArray();

#ifdef EDITION_FREE
	return qMin(count, EDITION_FREE_VOCABULARIES_MAX);
#else
	return count;
#endif
} // vocabularyCount

const Settings::VocabularyInfo Settings::vocabularyInfo(const quint8 &index)
{
	VocabularyInfo vocabularyInfo;

	_settings.beginReadArray(ARRAY_VOCABULARIES);
	_settings.setArrayIndex(index);
	vocabularyInfo.filePath = _settings.value(KEY_VOCABULARYFILE).toString();
#if !defined(EDITION_FREE) && !defined(EDITION_TRY)
	vocabularyInfo.enabled  = _settings.value(KEY_ENABLED, true).toBool();
#endif
	_settings.endArray();

	return vocabularyInfo;
} // vocabularyInfo
#endif

const quint8 Settings::waitForAnswer() const
{
#ifndef EDITION_FREE
	return _settings.value(KEY_WAITFORANSWER, DEFAULT_WAIT).toUInt();
#else
    return DEFAULT_WAIT;
#endif
} // waitForAnswer

#ifndef EDITION_FREE
const quint16 Settings::windowHeight() const
{
	return _settings.value(KEY_WINDOWHEIGHT, DEFAULT_DIMENSION).toUInt();
} // windowHeight

const quint16 Settings::windowWidth() const
{
	return _settings.value(KEY_WINDOWWIDTH, DEFAULT_DIMENSION).toUInt();
} // windowWidth

const quint16 Settings::windowX() const
{
	return _settings.value(KEY_WINDOWX, DEFAULT_DIMENSION).toUInt();
} // windowX

const quint16 Settings::windowY() const
{
	return _settings.value(KEY_WINDOWY, DEFAULT_DIMENSION).toUInt();
} // windowY
#endif

const quint8 Settings::wordsFrequency() const
{
    return _settings.value(KEY_WORDSFREQUENCY, DEFAULT_FREQUENCY).toUInt();
} // wordsFrequency

#if !defined(EDITION_FREE) && defined(Q_OS_WIN)
const QString Settings::hotkeyKey(const Hotkey &type) const
{
	QString key = KEY_HOTKEY;

	switch (type)
	{
	case HotkeyAnswer:
		key += SHORTCUT_ANSWER;
		break;
	case HotkeyMinimize:
		key += SHORTCUT_MINIMIZE;
		break;
	case HotkeyNext:
		key += SHORTCUT_NEXT;
		break;
	case HotkeyRestore:
		key += SHORTCUT_RESTORE;
		break;
	} // switch

	return key;
} // hotkeyKey

const QString Settings::hotkeyKeyText(const Hotkey &type) const
{
	return hotkeyKey(type) + "Text";
} // hotkeyKeyText

const QString Settings::hotkeyKeyVirtualKey(const Hotkey &type) const
{
	return hotkeyKey(type) + "VirtualKey";
} // hotkeyKeyVirtualKey
#endif

#ifndef EDITION_TRY
const void Settings::updateSettings()
{
	const Version current = static_cast<Version>(_settings.value(KEY_VERSION, Version1).toUInt());

	if (current < Version2) {
		// move vocabulary to vocabularies group
		const QString vocabulary = _settings.value(KEY_VOCABULARYFILE).toString();
		if (!vocabulary.isEmpty()) {
			_settings.beginWriteArray(ARRAY_VOCABULARIES);
			_settings.setArrayIndex(VocabularyPosition1);
			_settings.setValue(KEY_VOCABULARYFILE, vocabulary);
			_settings.endArray();
		} // if
		_settings.remove(KEY_VOCABULARYFILE);

		_settings.setValue(KEY_VERSION, Version2);
	} // if
} // updateSettings
#endif