#include "tts-openjtalk.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include "../../3rdparty/Open JTalk/source/text2mecab/text2mecab.h"
#include "../../3rdparty/Open JTalk/source/mecab2njd/mecab2njd.h"
#include "../../3rdparty/Open JTalk/source/njd_set_pronunciation/njd_set_pronunciation.h"
#include "../../3rdparty/Open JTalk/source/njd_set_digit/njd_set_digit.h"
#include "../../3rdparty/Open JTalk/source/njd_set_accent_phrase/njd_set_accent_phrase.h"
#include "../../3rdparty/Open JTalk/source/njd_set_accent_type/njd_set_accent_type.h"
#include "../../3rdparty/Open JTalk/source/njd_set_unvoiced_vowel/njd_set_unvoiced_vowel.h"
#include "../../3rdparty/Open JTalk/source/njd_set_long_vowel/njd_set_long_vowel.h"
#include "../../3rdparty/Open JTalk/source/njd2jpcommon/njd2jpcommon.h"

const auto DIC_DIR   = "open_jtalk_dic_utf_8";
const auto VOICE_DIR = "hts_voice_nitech_jp_atr503_m001";

TTSOpenJTalk::~TTSOpenJTalk()
{
}

QString TTSOpenJTalk::fileInSubdir(const QString &file, const QString &dir) const
{
  return subdir(dir) + QDir::separator() + file;
}

QString TTSOpenJTalk::subdir(const QString &dir) const
{
  return QCoreApplication::applicationDirPath() + QDir::separator() +
#ifndef _DEBUG
    DIR_PLUGINS + QDir::separator() + "tts-openjtalk" + QDir::separator() +
#endif
    dir;
}

void TTSOpenJTalk::initialize()
{
  // initialize
  Mecab_initialize(&_mecab);
  NJD_initialize(&_njd);
  JPCommon_initialize(&_jpCommon);
  HTS_Engine_initialize(&_htsEngine);

  // load
  Mecab_load(&_mecab, subdir(DIC_DIR).toLocal8Bit().data());

  auto voiceFilePath = fileInSubdir("nitech_jp_atr503_m001.htsvoice", VOICE_DIR);
  char voiceFilePathBuffer[MAX_PATH];
  strcpy(voiceFilePathBuffer, voiceFilePath.toLocal8Bit().data());
  auto voiceFilePath2 = voiceFilePathBuffer;
  HTS_Engine_load(&_htsEngine, &voiceFilePath2, 1);
  HTS_Engine_get_fullcontext_label_format(&_htsEngine);
}

LicenseCommon::LicenseContentList TTSOpenJTalk::licenseText() const
{
  LicenseCommon::LicenseContentList licenses;
  LicenseCommon::LicenseContent content;

  content.title = "hts_engine API";
  QFile licenseFile(":/res/hts_engine API/COPYING");
  licenseFile.open(QIODevice::ReadOnly);
  content.text = licenseFile.readAll();
  licenses.append(content);
  licenseFile.close();

  content.title = "Open JTalk";
  licenseFile.setFileName(":/res/Open JTalk/COPYING");
  licenseFile.open(QIODevice::ReadOnly);
  content.text = licenseFile.readAll();
  licenses.append(content);

  return licenses;
}

TTSInterface::TTSPlugin TTSOpenJTalk::pluginId() const
{
  return TTSPlugin::OpenJTalk;
}

QString TTSOpenJTalk::pluginName() const
{
  return "Open JTalk";
}

void TTSOpenJTalk::say(const QString &voice, const QString &text)
{
  auto cBuffer = static_cast<char *>(calloc(2 * text.toUtf8().size() + 1, sizeof(char)));
  text2mecab(cBuffer, text.toUtf8().data());
  Mecab_analysis(&_mecab, cBuffer);
  free(cBuffer);
  mecab2njd(&_njd, Mecab_get_feature(&_mecab), Mecab_get_size(&_mecab));
  njd_set_pronunciation(&_njd);
  njd_set_digit(&_njd);
  njd_set_accent_phrase(&_njd);
  njd_set_accent_type(&_njd);
  njd_set_unvoiced_vowel(&_njd);
  njd_set_long_vowel(&_njd);
  njd2jpcommon(&_jpCommon, &_njd);
  JPCommon_make_label(&_jpCommon);
  if (JPCommon_get_label_size(&_jpCommon) > 2)
  {
    HTS_Engine_synthesize_from_strings(&_htsEngine, JPCommon_get_label_feature(&_jpCommon), JPCommon_get_label_size(&_jpCommon));
    HTS_Engine_refresh(&_htsEngine);
  }
  JPCommon_refresh(&_jpCommon);
  NJD_refresh(&_njd);
  Mecab_refresh(&_mecab);
}

void TTSOpenJTalk::uninitialize()
{
  Mecab_clear(&_mecab);
  NJD_clear(&_njd);
  JPCommon_clear(&_jpCommon);
  HTS_Engine_clear(&_htsEngine);
}

TTSInterface::VoiceInfoList TTSOpenJTalk::voicesInfo() const
{
  TTSInterface::VoiceInfo sviVoice;
  sviVoice.description = "NIT ATR503 M001";
  return TTSInterface::VoiceInfoList() << sviVoice;
}