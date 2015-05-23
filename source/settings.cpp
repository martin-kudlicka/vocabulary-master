#include "settings.h"

const QString APPLICATION                   = "Vocabulary Master";
#ifndef EDITION_TRY
const QString ARRAY_VOCABULARIES            = "Vocabularies";
#endif
#ifndef EDITION_FREE
const QString DEFAULT_COLORFLASH            = "chartreuse";
#endif
const QString KEY_ALWAYSONTOP               = "AlwaysOnTop";
#ifndef EDITION_TRY
const QString KEY_CACHEVOCABULARY           = "CacheVocabulary";
#endif
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
#ifndef EDITION_TRY
const QString KEY_RECORDSTOCACHE            = "RecordsToCache";
#endif
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

bool Settings::alwaysOnTop() const
{
    return _settings.value(KEY_ALWAYSONTOP, false).toBool();
} // alwaysOnTop

#ifndef EDITION_TRY
bool Settings::cacheVocabulary() const
{
	return _settings.value(KEY_CACHEVOCABULARY, true).toBool();
} // cacheVocabulary
#endif

#ifndef EDITION_FREE
bool Settings::canChangeCategoryPriority() const
{
    return _settings.value(KEY_CANCHANGECATEGORYPRIORITY, true).toBool();
} // canChangeCategoryPriority

bool Settings::canEnableCategories() const
{
    return _settings.value(KEY_CANENABLECATEGORIES, true).toBool();
} // canEnableCategories

QString Settings::colorFlash() const
{
    return _settings.value(KEY_COLORFLASH, DEFAULT_COLORFLASH).toString();
} // colorFlash

bool Settings::horizontalLayout() const
{
    return _settings.value(KEY_HORIZONTALLAYOUT, false).toBool();
} // horizontalLayout

# ifdef Q_OS_WIN
Settings::HotkeyInfo Settings::hotkey(Hotkey type) const
{
	HotkeyInfo hotkeyInfo;

	hotkeyInfo.text       = _settings.value(hotkeyKeyText(type)).toString();
	hotkeyInfo.virtualKey = _settings.value(hotkeyKeyVirtualKey(type)).toUInt();

    return hotkeyInfo;
} // hotkey
# endif

bool Settings::learnDisabledWords() const
{
	return _settings.value(KEY_LEARNDISABLEDWORDS, false).toBool();
} // learnDisabledWords

QByteArray Settings::license() const
{
    return _settings.value(KEY_LICENSE).toByteArray();
} // license

bool Settings::minimizeToTray() const
{
	return _settings.value(KEY_MINIMIZETOTRAY, false).toBool();
} // minimizeToTray

bool Settings::mute() const
{
    return _settings.value(KEY_MUTE, false).toBool();
} // mute

bool Settings::newWordFlash() const
{
	return _settings.value(KEY_NEWWORDFLASH, false).toBool();
} // newWordFlash

bool Settings::newWordSound() const
{
	return _settings.value(KEY_NEWWORDSOUND, true).toBool();
} // newWordSound

QString Settings::newWordSoundFile() const
{
    return _settings.value(KEY_NEWWORDSOUNDFILE).toString();
} // newWordSoundFile

Settings::NewWordSoundType Settings::newWordSoundType() const
{
    return static_cast<NewWordSoundType>(_settings.value(KEY_NEWWORDSOUNDTYPE, NewWordSoundTypeSystem).toUInt());
} // newWordSoundType
#endif

QString Settings::proxyHostname() const
{
	return _settings.value(KEY_PROXYHOSTNAME).toString();
} // proxyHostname

QString Settings::proxyPassword() const
{
	return _settings.value(KEY_PROXYPASSWORD).toString();
} // proxyPassword

quint16 Settings::proxyPort() const
{
	return _settings.value(KEY_PROXYPORT, 8080).toUInt();
} // proxyPort

QNetworkProxy::ProxyType Settings::proxyType() const
{
	return static_cast<QNetworkProxy::ProxyType>(_settings.value(KEY_PROXYTYPE, QNetworkProxy::HttpProxy).toUInt());
} // proxyType

QString Settings::proxyUsername() const
{
	return _settings.value(KEY_PROXYUSERNAME).toString();
} // proxyUsername

#ifndef EDITION_TRY
quint32 Settings::recordsToCache() const
{
	return _settings.value(KEY_RECORDSTOCACHE, 200000).toUInt();
} // recordsToCache
#endif

#ifndef EDITION_FREE
bool Settings::rememberWindowPosition() const
{
    return _settings.value(KEY_REMEMBERWINDOWPOSITION, true).toBool();
} // rememberWindowPosition
#endif

void Settings::setAlwaysOnTop(bool enable)
{
	_settings.setValue(KEY_ALWAYSONTOP, enable);
} // setAlwaysOnTop

#ifndef EDITION_TRY
void Settings::setCacheVocabulary(bool enable)
{
	_settings.setValue(KEY_CACHEVOCABULARY, enable);
} // setCacheVocabulary
#endif

#ifndef EDITION_FREE
void Settings::setCanChangeCategoryPriority(bool enable)
{
    _settings.setValue(KEY_CANCHANGECATEGORYPRIORITY, enable);
} // setCanChangeCategoryPriority

void Settings::setCanEnableCategories(bool enable)
{
    _settings.setValue(KEY_CANENABLECATEGORIES, enable);
} // setCanEnableCategories

void Settings::setColorFlash(const QString &color)
{
    _settings.setValue(KEY_COLORFLASH, color);
} // setColorFlash

void Settings::setHorizontalLayout(bool enable)
{
    _settings.setValue(KEY_HORIZONTALLAYOUT, enable);
} // setHorizontalLayout

# ifdef Q_OS_WIN
void Settings::setHotkey(Hotkey type, const HotkeyInfo &hotkeyInfo)
{
    _settings.setValue(hotkeyKeyText(type), hotkeyInfo.text);
	_settings.setValue(hotkeyKeyVirtualKey(type), hotkeyInfo.virtualKey);
} // setHotkey
# endif

void Settings::setLearnDisabledWords(bool enable)
{
	_settings.setValue(KEY_LEARNDISABLEDWORDS, enable);
} // setLearnDisabledWords

void Settings::setLicense(const QByteArray &licenseData)
{
	_settings.setValue(KEY_LICENSE, licenseData);
} // setLicense

void Settings::setMinimizeToTray(bool enable)
{
	_settings.setValue(KEY_MINIMIZETOTRAY, enable);
} // setMinimizeToTray

void Settings::setMute(bool enable)
{
    _settings.setValue(KEY_MUTE, enable);
} // setMute

void Settings::setNewWordFlash(bool enable)
{
	_settings.setValue(KEY_NEWWORDFLASH, enable);
} // setNewWordFlash

void Settings::setNewWordSound(bool enable)
{
	_settings.setValue(KEY_NEWWORDSOUND, enable);
} // setNewWordSound

void Settings::setNewWordSoundFile(const QString &file)
{
    _settings.setValue(KEY_NEWWORDSOUNDFILE, file);
} // setNewWordSoundFile

void Settings::setNewWordSoundType(NewWordSoundType type)
{
    _settings.setValue(KEY_NEWWORDSOUNDTYPE, type);
} // setNewWordSoundType
#endif

void Settings::setProxyHostname(const QString &hostname)
{
	_settings.setValue(KEY_PROXYHOSTNAME, hostname);
} // setProxyHostname

void Settings::setProxyPassword(const QString &password)
{
	_settings.setValue(KEY_PROXYPASSWORD, password);
} // setProxyPassword

void Settings::setProxyPort(quint16 port)
{
	_settings.setValue(KEY_PROXYPORT, port);
} // setProxyPort

void Settings::setProxyType(QNetworkProxy::ProxyType type)
{
	_settings.setValue(KEY_PROXYTYPE, type);
} // setProxyType

void Settings::setProxyUsername(const QString &username)
{
	_settings.setValue(KEY_PROXYUSERNAME, username);
} // setProxyUsername

#ifndef EDITION_TRY
void Settings::setRecordsToCache(quint32 count)
{
	_settings.setValue(KEY_RECORDSTOCACHE, count);
} // setRecordsToCache
#endif

#ifndef EDITION_FREE
void Settings::setRememberWindowPosition(bool enable)
{
    _settings.setValue(KEY_REMEMBERWINDOWPOSITION, enable);
} // setRememberWindowPosition

void Settings::setShowCategoryName(bool show)
{
	_settings.setValue(KEY_SHOWCATEGORYNAME, show);
} // setShowCategoryName

void Settings::setShowLanguageNames(bool show)
{
	_settings.setValue(KEY_SHOWLANGUAGENAMES, show);
} // setShowLanguageNames

void Settings::setShowRecordControls(bool show)
{
	_settings.setValue(KEY_SHOWRECORDCONTROLS, show);
} // setShowRecordControls

void Settings::setShowStatusBar(bool show)
{
	_settings.setValue(KEY_SHOWSTATUSBAR, show);
} // setShowStatusBar

void Settings::setShowToolBar(bool show)
{
	_settings.setValue(KEY_SHOWTOOLBAR, show);
} // setShowToolBar

void Settings::setShowWordsInTrayBalloon(bool enable)
{
	_settings.setValue(KEY_SHOWWORDSINTRAYBALLOON, enable);
} // setShowWordsInTrayBalloon

void Settings::setStartLearningOnStartup(bool enable)
{
	_settings.setValue(KEY_STARTLEARNINGONSTARTUP, enable);
} // setStartLearningOnStartup
#endif

void Settings::setSwitchLearningDirection(Qt::CheckState switchDirection)
{
	_settings.setValue(KEY_SWITCHLEARNINGDIRECTION, switchDirection);
} // setSwitchLearningDirection

#ifndef EDITION_FREE
void Settings::setSystemTrayIcon(bool enable)
{
    _settings.setValue(KEY_SYSTEMTRAYICON, enable);
} // setSystemTrayIcon
#endif

void Settings::setTranslation(const QString &translationName)
{
    _settings.setValue(KEY_TRANSLATION, translationName);
} // setTranslation

void Settings::setUpdateCheck(bool check)
{
	_settings.setValue(KEY_UPDATECHECK, check);
} // setUpdateCheck

void Settings::setUseProxy(bool use)
{
	_settings.setValue(KEY_USEPROXY, use);
} // setUseProxy

#ifndef EDITION_TRY
void Settings::setVocabularyCount(quint8 count)
{
	_settings.beginWriteArray(ARRAY_VOCABULARIES, count);
	_settings.endArray();
} // setVocabularyCount

void Settings::setVocabularyInfo(quint8 index, const VocabularyInfo &info)
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
void Settings::setWaitForAnswer(quint8 time)
{
	_settings.setValue(KEY_WAITFORANSWER, time);
} // setWaitForAnswer

void Settings::setWindowHeight(quint16 height)
{
	_settings.setValue(KEY_WINDOWHEIGHT, height);
} // setWindowHeight

void Settings::setWindowWidth(quint16 width)
{
	_settings.setValue(KEY_WINDOWWIDTH, width);
} // setWindowWidth

void Settings::setWindowX(quint16 x)
{
	_settings.setValue(KEY_WINDOWX, x);
} // setWindowX

void Settings::setWindowY(quint16 y)
{
	_settings.setValue(KEY_WINDOWY, y);
} // setWindowY
#endif

void Settings::setWordsFrequency(quint8 frequency)
{
    _settings.setValue(KEY_WORDSFREQUENCY, frequency);
} // setWordsFrequency

#ifndef EDITION_FREE
bool Settings::showCategoryName() const
{
	return _settings.value(KEY_SHOWCATEGORYNAME, true).toBool();
} // showCategoryName

bool Settings::showLanguageNames() const
{
	return _settings.value(KEY_SHOWLANGUAGENAMES, true).toBool();
} // showLanguageNames

bool Settings::showRecordControls() const
{
	return _settings.value(KEY_SHOWRECORDCONTROLS, true).toBool();
} // showRecordControls

bool Settings::showStatusBar() const
{
	return _settings.value(KEY_SHOWSTATUSBAR, true).toBool();
} // showStatusBar

bool Settings::showToolBar() const
{
	return _settings.value(KEY_SHOWTOOLBAR, true).toBool();
} // showToolBar

bool Settings::showWordsInTrayBalloon() const
{
	return _settings.value(KEY_SHOWWORDSINTRAYBALLOON, false).toBool();
} // showWordsInTrayBalloon

bool Settings::startLearningOnStartup() const
{
	return _settings.value(KEY_STARTLEARNINGONSTARTUP, false).toBool();
} // startLearningOnStartup
#endif

Qt::CheckState Settings::switchLearningDirection() const
{
	return static_cast<Qt::CheckState>(_settings.value(KEY_SWITCHLEARNINGDIRECTION, false).toUInt());
} // switchLearningDirection

#ifndef EDITION_FREE
bool Settings::systemTrayIcon() const
{
    return _settings.value(KEY_SYSTEMTRAYICON, false).toBool();
} // systemTrayIcon
#endif

QString Settings::translation() const
{
    return _settings.value(KEY_TRANSLATION).toString();
} // translation

bool Settings::updateCheck() const
{
	return _settings.value(KEY_UPDATECHECK, true).toBool();
} // updateCheck

bool Settings::useProxy() const
{
	return _settings.value(KEY_USEPROXY, false).toBool();
} // useProxy

#ifndef EDITION_TRY
quint8 Settings::vocabularyCount()
{
	const quint8 count = _settings.beginReadArray(ARRAY_VOCABULARIES);
	_settings.endArray();

#ifdef EDITION_FREE
	return qMin(count, EDITION_FREE_VOCABULARIES_MAX);
#else
	return count;
#endif
} // vocabularyCount

Settings::VocabularyInfo Settings::vocabularyInfo(quint8 index)
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

quint8 Settings::waitForAnswer() const
{
#ifndef EDITION_FREE
	return _settings.value(KEY_WAITFORANSWER, DEFAULT_WAIT).toUInt();
#else
    return DEFAULT_WAIT;
#endif
} // waitForAnswer

#ifndef EDITION_FREE
quint16 Settings::windowHeight() const
{
	return _settings.value(KEY_WINDOWHEIGHT, DEFAULT_DIMENSION).toUInt();
} // windowHeight

quint16 Settings::windowWidth() const
{
	return _settings.value(KEY_WINDOWWIDTH, DEFAULT_DIMENSION).toUInt();
} // windowWidth

quint16 Settings::windowX() const
{
	return _settings.value(KEY_WINDOWX, DEFAULT_DIMENSION).toUInt();
} // windowX

quint16 Settings::windowY() const
{
	return _settings.value(KEY_WINDOWY, DEFAULT_DIMENSION).toUInt();
} // windowY
#endif

quint8 Settings::wordsFrequency() const
{
    return _settings.value(KEY_WORDSFREQUENCY, DEFAULT_FREQUENCY).toUInt();
} // wordsFrequency

#if !defined(EDITION_FREE) && defined(Q_OS_WIN)
QString Settings::hotkeyKey(Hotkey type) const
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

QString Settings::hotkeyKeyText(Hotkey type) const
{
	return hotkeyKey(type) + "Text";
} // hotkeyKeyText

QString Settings::hotkeyKeyVirtualKey(Hotkey type) const
{
	return hotkeyKey(type) + "VirtualKey";
} // hotkeyKeyVirtualKey
#endif

#ifndef EDITION_TRY
void Settings::updateSettings()
{
	const Version current = static_cast<Version>(_settings.value(KEY_VERSION, Version1).toUInt());

	if (current < Version2)
	{
		// move vocabulary to vocabularies group
		const QString vocabulary = _settings.value(KEY_VOCABULARYFILE).toString();
		if (!vocabulary.isEmpty())
		{
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