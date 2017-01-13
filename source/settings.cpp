#include "settings.h"

const auto DEFAULT_FREQUENCY = 60;
const auto DEFAULT_WAIT      = 0;

const auto APPLICATION                   = "Vocabulary Master";
#ifndef EDITION_TRY
const auto ARRAY_VOCABULARIES            = "Vocabularies";
#endif
#ifndef EDITION_FREE
const auto DEFAULT_COLORFLASH            = "chartreuse";
#endif
const auto KEY_ALWAYSONTOP               = "AlwaysOnTop";
#ifndef EDITION_TRY
const auto KEY_CACHEVOCABULARY           = "CacheVocabulary";
#endif
#ifndef EDITION_FREE
const auto KEY_CANCHANGECATEGORYPRIORITY = "CanChangeCategoryPriority";
const auto KEY_CANENABLECATEGORIES       = "CanEnableCategories";
const auto KEY_COLORFLASH                = "ColorFlash";
const auto KEY_ENABLED                   = "Enabled";
const auto KEY_HORIZONTALLAYOUT          = "HorizontalLayout";
# ifdef Q_OS_WIN
const auto KEY_HOTKEY                    = "Hotkey";
# endif
const auto KEY_LEARNDISABLEDWORDS        = "LearnDisabledWords";
const auto KEY_LICENSE                   = "License";
const auto KEY_MINIMIZETOTRAY            = "MinimizeToTray";
const auto KEY_MUTE                      = "Mute";
const auto KEY_NEWWORDFLASH              = "NewWordFlash";
const auto KEY_NEWWORDSOUND              = "NewWordSound";
const auto KEY_NEWWORDSOUNDFILE          = "NewWordSoundFile";
const auto KEY_NEWWORDSOUNDTYPE          = "NewWordSoundType";
#endif
const auto KEY_PROXYHOSTNAME             = "ProxyHostname";
const auto KEY_PROXYPASSWORD             = "ProxyPassword";
const auto KEY_PROXYPORT                 = "ProxyPort";
const auto KEY_PROXYTYPE                 = "ProxyType";
const auto KEY_PROXYUSERNAME             = "ProxyUsername";
#ifndef EDITION_TRY
const auto KEY_RECORDSTOCACHE            = "RecordsToCache";
#endif
#ifndef EDITION_FREE
const auto KEY_REMEMBERWINDOWPOSITION    = "RememberWindowPosition";
const auto KEY_SHOWCATEGORYNAME          = "ShowCategoryName";
const auto KEY_SHOWLANGUAGENAMES         = "ShowLanguageNames";
const auto KEY_SHOWRECORDCONTROLS        = "ShowRecordControls";
const auto KEY_SHOWSTATUSBAR             = "ShowStatusBar";
const auto KEY_SHOWTOOLBAR               = "ShowToolBar";
const auto KEY_SHOWWORDSINTRAYBALLOON    = "ShowWordsInTrayBalloon";
const auto KEY_STARTLEARNINGONSTARTUP    = "StartLearningOnStartup";
#endif
const auto KEY_SWITCHLEARNINGDIRECTION   = "SwitchLearningDirection";
#ifndef EDITION_FREE
const auto KEY_SYSTEMTRAYICON            = "SystemTrayIcon";
#endif
const auto KEY_TRANSLATION               = "Translation";
const auto KEY_UPDATECHECK               = "UpdateCheck";
const auto KEY_USEPROXY                  = "UseProxy";
#ifndef EDITION_TRY
const auto KEY_VERSION                   = "Version";
#endif
const auto KEY_VOCABULARYFILE            = "VocabularyFile";
#ifndef EDITION_FREE
const auto KEY_WAITFORANSWER             = "WaitForAnswer";
const auto KEY_WINDOWHEIGHT              = "WindowHeight";
const auto KEY_WINDOWWIDTH               = "WindowWidth";
const auto KEY_WINDOWX                   = "WindowX";
const auto KEY_WINDOWY                   = "WindowY";
#endif
const auto KEY_WORDSFREQUENCY            = "WordsFrequency";
const auto ORGANIZATION                  = "Isshou";
#if !defined(EDITION_FREE) && defined(Q_OS_WIN)
const auto SHORTCUT_ANSWER               = "Answer";
const auto SHORTCUT_MINIMIZE             = "Minimize";
const auto SHORTCUT_NEXT                 = "Next";
const auto SHORTCUT_RESTORE              = "Restore";
#endif

Settings::Settings() : _settings(ORGANIZATION, APPLICATION)
{
#ifndef EDITION_TRY
  updateSettings();
#endif
}

bool Settings::alwaysOnTop() const
{
  return _settings.value(KEY_ALWAYSONTOP, false).toBool();
}

#ifndef EDITION_TRY
bool Settings::cacheVocabulary() const
{
  return _settings.value(KEY_CACHEVOCABULARY, true).toBool();
}
#endif

#ifndef EDITION_FREE
bool Settings::canChangeCategoryPriority() const
{
  return _settings.value(KEY_CANCHANGECATEGORYPRIORITY, true).toBool();
}

bool Settings::canEnableCategories() const
{
  return _settings.value(KEY_CANENABLECATEGORIES, true).toBool();
}

QString Settings::colorFlash() const
{
  return _settings.value(KEY_COLORFLASH, DEFAULT_COLORFLASH).toString();
}

bool Settings::horizontalLayout() const
{
  return _settings.value(KEY_HORIZONTALLAYOUT, false).toBool();
}

# ifdef Q_OS_WIN
Settings::HotkeyInfo Settings::hotkey(Hotkey type) const
{
  HotkeyInfo hotkeyInfo;

  hotkeyInfo.text       = _settings.value(hotkeyKeyText(type)).toString();
  hotkeyInfo.virtualKey = _settings.value(hotkeyKeyVirtualKey(type)).toUInt();

  return hotkeyInfo;
}
# endif

bool Settings::learnDisabledWords() const
{
  return _settings.value(KEY_LEARNDISABLEDWORDS, false).toBool();
}

QByteArray Settings::license() const
{
  return _settings.value(KEY_LICENSE).toByteArray();
}

bool Settings::minimizeToTray() const
{
  return _settings.value(KEY_MINIMIZETOTRAY, false).toBool();
}

bool Settings::mute() const
{
  return _settings.value(KEY_MUTE, false).toBool();
}

bool Settings::newWordFlash() const
{
  return _settings.value(KEY_NEWWORDFLASH, false).toBool();
}

bool Settings::newWordSound() const
{
  return _settings.value(KEY_NEWWORDSOUND, true).toBool();
}

QString Settings::newWordSoundFile() const
{
  return _settings.value(KEY_NEWWORDSOUNDFILE).toString();
}

Settings::NewWordSoundType Settings::newWordSoundType() const
{
  return static_cast<NewWordSoundType>(_settings.value(KEY_NEWWORDSOUNDTYPE, static_cast<quintptr>(NewWordSoundType::System)).toUInt());
}
#endif

QString Settings::proxyHostname() const
{
  return _settings.value(KEY_PROXYHOSTNAME).toString();
}

QString Settings::proxyPassword() const
{
  return _settings.value(KEY_PROXYPASSWORD).toString();
}

quintptr Settings::proxyPort() const
{
  return _settings.value(KEY_PROXYPORT, 8080).toUInt();
}

QNetworkProxy::ProxyType Settings::proxyType() const
{
  return static_cast<QNetworkProxy::ProxyType>(_settings.value(KEY_PROXYTYPE, QNetworkProxy::HttpProxy).toUInt());
}

QString Settings::proxyUsername() const
{
  return _settings.value(KEY_PROXYUSERNAME).toString();
}

#ifndef EDITION_TRY
quintptr Settings::recordsToCache() const
{
  return _settings.value(KEY_RECORDSTOCACHE, 200000).toUInt();
}
#endif

#ifndef EDITION_FREE
bool Settings::rememberWindowPosition() const
{
  return _settings.value(KEY_REMEMBERWINDOWPOSITION, true).toBool();
}
#endif

void Settings::setAlwaysOnTop(bool enable)
{
  _settings.setValue(KEY_ALWAYSONTOP, enable);
}

#ifndef EDITION_TRY
void Settings::setCacheVocabulary(bool enable)
{
  _settings.setValue(KEY_CACHEVOCABULARY, enable);
}
#endif

#ifndef EDITION_FREE
void Settings::setCanChangeCategoryPriority(bool enable)
{
  _settings.setValue(KEY_CANCHANGECATEGORYPRIORITY, enable);
}

void Settings::setCanEnableCategories(bool enable)
{
  _settings.setValue(KEY_CANENABLECATEGORIES, enable);
}

void Settings::setColorFlash(const QString &color)
{
  _settings.setValue(KEY_COLORFLASH, color);
}

void Settings::setHorizontalLayout(bool enable)
{
  _settings.setValue(KEY_HORIZONTALLAYOUT, enable);
}

# ifdef Q_OS_WIN
void Settings::setHotkey(Hotkey type, const HotkeyInfo &hotkeyInfo)
{
  _settings.setValue(hotkeyKeyText(type),       hotkeyInfo.text);
  _settings.setValue(hotkeyKeyVirtualKey(type), hotkeyInfo.virtualKey);
}
# endif

void Settings::setLearnDisabledWords(bool enable)
{
  _settings.setValue(KEY_LEARNDISABLEDWORDS, enable);
}

void Settings::setLicense(const QByteArray &licenseData)
{
  _settings.setValue(KEY_LICENSE, licenseData);
}

void Settings::setMinimizeToTray(bool enable)
{
  _settings.setValue(KEY_MINIMIZETOTRAY, enable);
}

void Settings::setMute(bool enable)
{
  _settings.setValue(KEY_MUTE, enable);
}

void Settings::setNewWordFlash(bool enable)
{
  _settings.setValue(KEY_NEWWORDFLASH, enable);
}

void Settings::setNewWordSound(bool enable)
{
  _settings.setValue(KEY_NEWWORDSOUND, enable);
}

void Settings::setNewWordSoundFile(const QString &file)
{
  _settings.setValue(KEY_NEWWORDSOUNDFILE, file);
}

void Settings::setNewWordSoundType(NewWordSoundType type)
{
  _settings.setValue(KEY_NEWWORDSOUNDTYPE, static_cast<quintptr>(type));
}
#endif

void Settings::setProxyHostname(const QString &hostname)
{
  _settings.setValue(KEY_PROXYHOSTNAME, hostname);
}

void Settings::setProxyPassword(const QString &password)
{
  _settings.setValue(KEY_PROXYPASSWORD, password);
}

void Settings::setProxyPort(quintptr port)
{
  _settings.setValue(KEY_PROXYPORT, port);
}

void Settings::setProxyType(QNetworkProxy::ProxyType type)
{
  _settings.setValue(KEY_PROXYTYPE, type);
}

void Settings::setProxyUsername(const QString &username)
{
  _settings.setValue(KEY_PROXYUSERNAME, username);
}

#ifndef EDITION_TRY
void Settings::setRecordsToCache(quintptr count)
{
  _settings.setValue(KEY_RECORDSTOCACHE, count);
}
#endif

#ifndef EDITION_FREE
void Settings::setRememberWindowPosition(bool enable)
{
  _settings.setValue(KEY_REMEMBERWINDOWPOSITION, enable);
}

void Settings::setShowCategoryName(bool show)
{
  _settings.setValue(KEY_SHOWCATEGORYNAME, show);
}

void Settings::setShowLanguageNames(bool show)
{
  _settings.setValue(KEY_SHOWLANGUAGENAMES, show);
}

void Settings::setShowRecordControls(bool show)
{
  _settings.setValue(KEY_SHOWRECORDCONTROLS, show);
}

void Settings::setShowStatusBar(bool show)
{
  _settings.setValue(KEY_SHOWSTATUSBAR, show);
}

void Settings::setShowToolBar(bool show)
{
  _settings.setValue(KEY_SHOWTOOLBAR, show);
}

void Settings::setShowWordsInTrayBalloon(bool enable)
{
  _settings.setValue(KEY_SHOWWORDSINTRAYBALLOON, enable);
}

void Settings::setStartLearningOnStartup(bool enable)
{
  _settings.setValue(KEY_STARTLEARNINGONSTARTUP, enable);
}
#endif

void Settings::setSwitchLearningDirection(Qt::CheckState switchDirection)
{
  _settings.setValue(KEY_SWITCHLEARNINGDIRECTION, switchDirection);
}

#ifndef EDITION_FREE
void Settings::setSystemTrayIcon(bool enable)
{
  _settings.setValue(KEY_SYSTEMTRAYICON, enable);
}
#endif

void Settings::setTranslation(const QString &translationName)
{
  _settings.setValue(KEY_TRANSLATION, translationName);
}

void Settings::setUpdateCheck(bool check)
{
  _settings.setValue(KEY_UPDATECHECK, check);
}

void Settings::setUseProxy(bool use)
{
  _settings.setValue(KEY_USEPROXY, use);
}

#ifndef EDITION_TRY
void Settings::setVocabularyCount(quintptr count)
{
  _settings.beginWriteArray(ARRAY_VOCABULARIES, count);
  _settings.endArray();
}

void Settings::setVocabularyInfo(quintptr index, const VocabularyInfo &info)
{
  _settings.beginWriteArray(ARRAY_VOCABULARIES);
  _settings.setArrayIndex(index);
  _settings.setValue(KEY_VOCABULARYFILE, info.filePath);
#if !defined(EDITION_FREE) && !defined(EDITION_TRY)
  _settings.setValue(KEY_ENABLED,        info.enabled);
#endif
  _settings.endArray();
}
#endif

#ifndef EDITION_FREE
void Settings::setWaitForAnswer(quintptr time)
{
  _settings.setValue(KEY_WAITFORANSWER, time);
}

void Settings::setWindowHeight(quintptr height)
{
  _settings.setValue(KEY_WINDOWHEIGHT, height);
}

void Settings::setWindowWidth(quintptr width)
{
  _settings.setValue(KEY_WINDOWWIDTH, width);
}

void Settings::setWindowX(quintptr x)
{
  _settings.setValue(KEY_WINDOWX, x);
}

void Settings::setWindowY(quintptr y)
{
  _settings.setValue(KEY_WINDOWY, y);
}
#endif

void Settings::setWordsFrequency(quintptr frequency)
{
  _settings.setValue(KEY_WORDSFREQUENCY, frequency);
}

#ifndef EDITION_FREE
bool Settings::showCategoryName() const
{
  return _settings.value(KEY_SHOWCATEGORYNAME, true).toBool();
}

bool Settings::showLanguageNames() const
{
  return _settings.value(KEY_SHOWLANGUAGENAMES, true).toBool();
}

bool Settings::showRecordControls() const
{
  return _settings.value(KEY_SHOWRECORDCONTROLS, true).toBool();
}

bool Settings::showStatusBar() const
{
  return _settings.value(KEY_SHOWSTATUSBAR, true).toBool();
}

bool Settings::showToolBar() const
{
  return _settings.value(KEY_SHOWTOOLBAR, true).toBool();
}

bool Settings::showWordsInTrayBalloon() const
{
  return _settings.value(KEY_SHOWWORDSINTRAYBALLOON, false).toBool();
}

bool Settings::startLearningOnStartup() const
{
  return _settings.value(KEY_STARTLEARNINGONSTARTUP, false).toBool();
}
#endif

Qt::CheckState Settings::switchLearningDirection() const
{
  return static_cast<Qt::CheckState>(_settings.value(KEY_SWITCHLEARNINGDIRECTION, false).toUInt());
}

#ifndef EDITION_FREE
bool Settings::systemTrayIcon() const
{
  return _settings.value(KEY_SYSTEMTRAYICON, false).toBool();
}
#endif

QString Settings::translation() const
{
  return _settings.value(KEY_TRANSLATION).toString();
}

bool Settings::updateCheck() const
{
  return _settings.value(KEY_UPDATECHECK, true).toBool();
}

bool Settings::useProxy() const
{
  return _settings.value(KEY_USEPROXY, false).toBool();
}

#ifndef EDITION_TRY
quintptr Settings::vocabularyCount()
{
  const auto count = _settings.beginReadArray(ARRAY_VOCABULARIES);
  _settings.endArray();

#ifdef EDITION_FREE
  return qMin(count, EDITION_FREE_VOCABULARIES_MAX);
#else
  return count;
#endif
}

Settings::VocabularyInfo Settings::vocabularyInfo(quintptr index)
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
}
#endif

quintptr Settings::waitForAnswer() const
{
#ifndef EDITION_FREE
  return _settings.value(KEY_WAITFORANSWER, DEFAULT_WAIT).toUInt();
#else
  return DEFAULT_WAIT;
#endif
}

#ifndef EDITION_FREE
quintptr Settings::windowHeight() const
{
  return _settings.value(KEY_WINDOWHEIGHT, DEFAULT_DIMENSION).toUInt();
}

quintptr Settings::windowWidth() const
{
  return _settings.value(KEY_WINDOWWIDTH, DEFAULT_DIMENSION).toUInt();
}

quintptr Settings::windowX() const
{
  return _settings.value(KEY_WINDOWX, DEFAULT_DIMENSION).toUInt();
}

quintptr Settings::windowY() const
{
  return _settings.value(KEY_WINDOWY, DEFAULT_DIMENSION).toUInt();
}
#endif

quintptr Settings::wordsFrequency() const
{
  return _settings.value(KEY_WORDSFREQUENCY, DEFAULT_FREQUENCY).toUInt();
}

#if !defined(EDITION_FREE) && defined(Q_OS_WIN)
QString Settings::hotkeyKey(Hotkey type) const
{
  QString key = KEY_HOTKEY;

  switch (type)
  {
    case Hotkey::Answer:
      key += SHORTCUT_ANSWER;
      break;
    case Hotkey::Minimize:
      key += SHORTCUT_MINIMIZE;
      break;
    case Hotkey::Next:
      key += SHORTCUT_NEXT;
      break;
    case Hotkey::Restore:
      key += SHORTCUT_RESTORE;
      break;
  }

  return key;
}

QString Settings::hotkeyKeyText(Hotkey type) const
{
  return hotkeyKey(type) + "Text";
}

QString Settings::hotkeyKeyVirtualKey(Hotkey type) const
{
  return hotkeyKey(type) + "VirtualKey";
}
#endif

#ifndef EDITION_TRY
void Settings::updateSettings()
{
  const auto current = static_cast<Version>(_settings.value(KEY_VERSION, static_cast<quintptr>(Version::N1)).toUInt());

  if (current < Version::N2)
  {
    // move vocabulary to vocabularies group
    const auto vocabulary = _settings.value(KEY_VOCABULARYFILE).toString();
    if (!vocabulary.isEmpty())
    {
      _settings.beginWriteArray(ARRAY_VOCABULARIES);
      _settings.setArrayIndex(static_cast<int>(VocabularyPosition::N1));
      _settings.setValue(KEY_VOCABULARYFILE, vocabulary);
      _settings.endArray();
    }
    _settings.remove(KEY_VOCABULARYFILE);

    _settings.setValue(KEY_VERSION, static_cast<quintptr>(Version::N2));
  }
}
#endif