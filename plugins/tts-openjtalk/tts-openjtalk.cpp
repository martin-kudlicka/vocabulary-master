#include "tts-openjtalk.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <text2mecab/text2mecab.h>
#include <mecab2njd/mecab2njd.h>
#include <njd_set_pronunciation/njd_set_pronunciation.h>
#include <njd_set_digit/njd_set_digit.h>
#include <njd_set_accent_phrase/njd_set_accent_phrase.h>
#include <njd_set_accent_type/njd_set_accent_type.h>
#include <njd_set_unvoiced_vowel/njd_set_unvoiced_vowel.h>
#include <njd_set_long_vowel/njd_set_long_vowel.h>
#include <njd2jpcommon/njd2jpcommon.h>

const auto ALPHA           = 0.55;
const auto AUDIO_BUFF_SIZE = 1600;
const auto BETA            = 0.0;
const auto DIC_DIR         = "open_jtalk_dic_utf_8";
const auto FRAME_SHIFT     = 240;
const auto GV_WEIGHT_LF0   = 1.0;
const auto GV_WEIGHT_LPF   = 1.0;
const auto GV_WEIGHT_MGC   = 1.0;
const auto UV_THRESHOLD    = 0.5;
const auto VOICE_DIR       = "hts_voice_nitech_jp_atr503_m001";

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
  HTS_Engine_set_fperiod(&_htsEngine, FRAME_SHIFT);
  HTS_Engine_set_alpha(&_htsEngine, ALPHA);
  HTS_Engine_set_beta(&_htsEngine, BETA);

  // load
  auto cPath1 = static_cast<char *>(malloc(MAX_PATH + 1));

  Mecab_load(&_mecab, subdir(DIC_DIR).toLocal8Bit().data());
  strcpy_s(cPath1, MAX_PATH, fileInSubdir("nitech_jp_atr503_m001.htsvoice", VOICE_DIR).toLocal8Bit().data());
  HTS_Engine_load(&_htsEngine, &cPath1, 1);
  HTS_Engine_set_audio_buff_size(&_htsEngine, AUDIO_BUFF_SIZE);
  HTS_Engine_set_msd_threshold(&_htsEngine, 1, UV_THRESHOLD);
  HTS_Engine_set_gv_weight(&_htsEngine, 0, GV_WEIGHT_MGC);
  HTS_Engine_set_gv_weight(&_htsEngine, 1, GV_WEIGHT_LF0);
  HTS_Engine_set_gv_weight(&_htsEngine, 2, GV_WEIGHT_LPF);

  free(cPath1);
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
  Q_UNUSED(voice);

  char cBuffer[1024];
  text2mecab(cBuffer, text.toUtf8().data());
  Mecab_analysis(&_mecab, cBuffer);
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