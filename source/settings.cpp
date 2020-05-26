#include "pch.h"
#include "settings.h"

const auto DEFAULT_FREQUENCY = 60;
const auto DEFAULT_WAIT      = 0;

const auto ARRAY_VOCABULARIES            = "vocabularies";
const auto DEFAULT_COLORFLASH            = "chartreuse";
const auto KEY_ALWAYSONTOP               = "alwaysOnTop";
const auto KEY_CACHEVOCABULARY           = "cacheVocabulary";
const auto KEY_CANCHANGECATEGORYPRIORITY = "canChangeCategoryPriority";
const auto KEY_CANENABLECATEGORIES       = "canEnableCategories";
const auto KEY_COLORFLASH                = "colorFlash";
const auto KEY_ENABLED                   = "enabled";
const auto KEY_HORIZONTALLAYOUT          = "horizontalLayout";
#ifdef Q_OS_WIN
const auto KEY_HOTKEY                    = "hotkey";
#endif
const auto KEY_LEARNDISABLEDWORDS        = "learnDisabledWords";
const auto KEY_LICENSE                   = "license";
const auto KEY_MINIMIZETOTRAY            = "minimizeToTray";
const auto KEY_MUTE                      = "mute";
const auto KEY_NEWWORDFLASH              = "newWordFlash";
const auto KEY_NEWWORDSOUND              = "newWordSound";
const auto KEY_NEWWORDSOUNDFILE          = "newWordSoundFile";
const auto KEY_NEWWORDSOUNDTYPE          = "newWordSoundType";
const auto KEY_PROXYHOSTNAME             = "proxyHostname";
const auto KEY_PROXYPASSWORD             = "proxyPassword";
const auto KEY_PROXYPORT                 = "proxyPort";
const auto KEY_PROXYTYPE                 = "proxyType";
const auto KEY_PROXYUSERNAME             = "proxyUsername";
const auto KEY_RECORDSTOCACHE            = "recordsToCache";
const auto KEY_REMEMBERWINDOWPOSITION    = "rememberWindowPosition";
const auto KEY_SHOWCATEGORYNAME          = "showCategoryName";
const auto KEY_SHOWLANGUAGENAMES         = "showLanguageNames";
const auto KEY_SHOWRECORDCONTROLS        = "showRecordControls";
const auto KEY_SHOWSTATUSBAR             = "showStatusBar";
const auto KEY_SHOWTOOLBAR               = "showToolBar";
const auto KEY_SHOWWORDSINTRAYBALLOON    = "showWordsInTrayBalloon";
const auto KEY_STARTLEARNINGONSTARTUP    = "startLearningOnStartup";
const auto KEY_SWITCHLEARNINGDIRECTION   = "switchLearningDirection";
const auto KEY_SYSTEMTRAYICON            = "systemTrayIcon";
const auto KEY_TRANSLATION               = "translation";
const auto KEY_UPDATECHECK               = "updateCheck";
const auto KEY_USEPROXY                  = "useProxy";
const auto KEY_VERSION                   = "version";
const auto KEY_VOCABULARYFILE            = "vocabularyFile";
const auto KEY_WAITFORANSWER             = "waitForAnswer";
const auto KEY_WINDOWHEIGHT              = "windowHeight";
const auto KEY_WINDOWWIDTH               = "windowWidth";
const auto KEY_WINDOWX                   = "windowX";
const auto KEY_WINDOWY                   = "windowY";
const auto KEY_WORDSFREQUENCY            = "wordsFrequency";
#ifdef Q_OS_WIN
const auto SHORTCUT_ANSWER               = "Answer";
const auto SHORTCUT_MINIMIZE             = "Minimize";
const auto SHORTCUT_NEXT                 = "Next";
const auto SHORTCUT_RESTORE              = "Restore";
#endif

Settings::Settings()
{
  updateSettings();
}

bool Settings::alwaysOnTop() const
{
  return _settings.value(KEY_ALWAYSONTOP, false).toBool();
}

bool Settings::cacheVocabulary() const
{
  return _settings.value(KEY_CACHEVOCABULARY, true).toBool();
}

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

#ifdef Q_OS_WIN
Settings::HotkeyInfo Settings::hotkey(Hotkey type) const
{
  HotkeyInfo hotkeyInfo;

  hotkeyInfo.text       = _settings.value(hotkeyKeyText(type)).toString();
  hotkeyInfo.virtualKey = _settings.value(hotkeyKeyVirtualKey(type)).toUInt();

  return hotkeyInfo;
}
#endif

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

quintptr Settings::recordsToCache() const
{
  return _settings.value(KEY_RECORDSTOCACHE, 200000).toUInt();
}

bool Settings::rememberWindowPosition() const
{
  return _settings.value(KEY_REMEMBERWINDOWPOSITION, true).toBool();
}

void Settings::setAlwaysOnTop(bool enable)
{
  _settings.setValue(KEY_ALWAYSONTOP, enable);
}

void Settings::setCacheVocabulary(bool enable)
{
  _settings.setValue(KEY_CACHEVOCABULARY, enable);
}

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

#ifdef Q_OS_WIN
void Settings::setHotkey(Hotkey type, const HotkeyInfo &hotkeyInfo)
{
  _settings.setValue(hotkeyKeyText(type),       hotkeyInfo.text);
  _settings.setValue(hotkeyKeyVirtualKey(type), hotkeyInfo.virtualKey);
}
#endif

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

void Settings::setRecordsToCache(quintptr count)
{
  _settings.setValue(KEY_RECORDSTOCACHE, count);
}

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

void Settings::setSwitchLearningDirection(Qt::CheckState switchDirection)
{
  _settings.setValue(KEY_SWITCHLEARNINGDIRECTION, switchDirection);
}

void Settings::setSystemTrayIcon(bool enable)
{
  _settings.setValue(KEY_SYSTEMTRAYICON, enable);
}

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
  _settings.setValue(KEY_ENABLED,        info.enabled);
  _settings.endArray();
}

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

void Settings::setWordsFrequency(quintptr frequency)
{
  _settings.setValue(KEY_WORDSFREQUENCY, frequency);
}

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

Qt::CheckState Settings::switchLearningDirection() const
{
  return static_cast<Qt::CheckState>(_settings.value(KEY_SWITCHLEARNINGDIRECTION, false).toUInt());
}

bool Settings::systemTrayIcon() const
{
  return _settings.value(KEY_SYSTEMTRAYICON, false).toBool();
}

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

quintptr Settings::vocabularyCount()
{
  const auto count = _settings.beginReadArray(ARRAY_VOCABULARIES);
  _settings.endArray();

  return count;
}

Settings::VocabularyInfo Settings::vocabularyInfo(quintptr index)
{
  VocabularyInfo vocabularyInfo;

  _settings.beginReadArray(ARRAY_VOCABULARIES);
  _settings.setArrayIndex(index);
  vocabularyInfo.filePath = _settings.value(KEY_VOCABULARYFILE).toString();
  vocabularyInfo.enabled  = _settings.value(KEY_ENABLED, true).toBool();
  _settings.endArray();

  return vocabularyInfo;
}

quintptr Settings::waitForAnswer() const
{
  return _settings.value(KEY_WAITFORANSWER, DEFAULT_WAIT).toUInt();
}

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

quintptr Settings::wordsFrequency() const
{
  return _settings.value(KEY_WORDSFREQUENCY, DEFAULT_FREQUENCY).toUInt();
}

#ifdef Q_OS_WIN
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