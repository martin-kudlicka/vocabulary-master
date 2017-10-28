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

const auto ALPHA           = 0.55;
const auto AUDIO_BUFF_SIZE = 1600;
const auto BETA            = 0.0;
const auto DIC_DIR         = "open_jtalk_dic_utf_8";
const auto FN_GV_SWITCH    = "gv-switch.inf";
const auto FN_MS_DUR       = "dur.pdf";
const auto FN_MS_GV1       = "gv-lf0.pdf";
const auto FN_MS_GVM       = "gv-mgc.pdf";
const auto FN_MS_LF0       = "lf0.pdf";
const auto FN_MS_LPF       = "lpf.pdf";
const auto FN_MS_MGC       = "mgc.pdf";
const auto FN_TS_DUR       = "tree-dur.inf";
const auto FN_TS_GV1       = "tree-gv-lf0.inf";
const auto FN_TS_GVM       = "tree-gv-mgc.inf";
const auto FN_TS_LF0       = "tree-lf0.inf";
const auto FN_TS_LPF       = "tree-lpf.inf";
const auto FN_TS_MGC       = "tree-mgc.inf";
const auto FN_WS_LF0_1     = "lf0.win1";
const auto FN_WS_LF0_2     = "lf0.win2";
const auto FN_WS_LF0_3     = "lf0.win3";
const auto FN_WS_LPF       = "lpf.win1";
const auto FN_WS_MGC_1     = "mgc.win1";
const auto FN_WS_MGC_2     = "mgc.win2";
const auto FN_WS_MGC_3     = "mgc.win3";
const auto FRAME_SHIFT     = 240;
const auto GV_WEIGHT_LF0   = 1.0;
const auto GV_WEIGHT_LPF   = 1.0;
const auto GV_WEIGHT_MGC   = 1.0;
const auto NUM_WS_LF0      = 3;
const auto NUM_WS_LPF      = 1;
const auto NUM_WS_MGC      = 3;
const auto SAMPLING_RATE   = 48000;
const auto STAGE           = 0;
const auto USE_LOG_GAIN    = FALSE;
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
  HTS_Engine_initialize(&_htsEngine, 3);
  HTS_Engine_set_sampling_rate(&_htsEngine, SAMPLING_RATE);
  HTS_Engine_set_fperiod(&_htsEngine, FRAME_SHIFT);
  HTS_Engine_set_alpha(&_htsEngine, ALPHA);
  HTS_Engine_set_gamma(&_htsEngine, STAGE);
  HTS_Engine_set_log_gain(&_htsEngine, USE_LOG_GAIN);
  HTS_Engine_set_beta(&_htsEngine, BETA);
  HTS_Engine_set_audio_buff_size(&_htsEngine, AUDIO_BUFF_SIZE);
  HTS_Engine_set_msd_threshold(&_htsEngine, 1, UV_THRESHOLD);
  HTS_Engine_set_gv_weight(&_htsEngine, 0, GV_WEIGHT_MGC);
  HTS_Engine_set_gv_weight(&_htsEngine, 1, GV_WEIGHT_LF0);
  HTS_Engine_set_gv_weight(&_htsEngine, 2, GV_WEIGHT_LPF);

  // load
  char **cPaths = static_cast<char **>(malloc(NUM_WS_MGC * sizeof(char *)));
  auto cPath1 = static_cast<char *>(malloc(MAX_PATH + 1));
  auto cPath2 = static_cast<char *>(malloc(MAX_PATH + 1));
  auto cPath3 = static_cast<char *>(malloc(MAX_PATH + 1));
  auto cPath4 = static_cast<char *>(malloc(MAX_PATH + 1));
  auto cPath5 = static_cast<char *>(malloc(MAX_PATH + 1));

  Mecab_load(&_mecab, subdir(DIC_DIR).toLocal8Bit().data());
  strcpy_s(cPath1, MAX_PATH + 1, fileInSubdir(FN_MS_DUR, VOICE_DIR).toLocal8Bit().data());
  strcpy_s(cPath2, MAX_PATH + 1, fileInSubdir(FN_TS_DUR, VOICE_DIR).toLocal8Bit().data());
  HTS_Engine_load_duration_from_fn(&_htsEngine, &cPath1, &cPath2, 1);
  strcpy_s(cPath1, MAX_PATH + 1, fileInSubdir(FN_MS_MGC, VOICE_DIR).toLocal8Bit().data());
  strcpy_s(cPath2, MAX_PATH + 1, fileInSubdir(FN_TS_MGC, VOICE_DIR).toLocal8Bit().data());
  strcpy_s(cPath3, MAX_PATH + 1, fileInSubdir(FN_WS_MGC_1, VOICE_DIR).toLocal8Bit().data());
  strcpy_s(cPath4, MAX_PATH + 1, fileInSubdir(FN_WS_MGC_2, VOICE_DIR).toLocal8Bit().data());
  strcpy_s(cPath5, MAX_PATH + 1, fileInSubdir(FN_WS_MGC_3, VOICE_DIR).toLocal8Bit().data());
  cPaths[0] = cPath3;
  cPaths[1] = cPath4;
  cPaths[2] = cPath5;
  HTS_Engine_load_parameter_from_fn(&_htsEngine, &cPath1, &cPath2, cPaths, 0, FALSE, NUM_WS_MGC, 1);
  strcpy_s(cPath1, MAX_PATH + 1, fileInSubdir(FN_MS_LF0, VOICE_DIR).toLocal8Bit().data());
  strcpy_s(cPath2, MAX_PATH + 1, fileInSubdir(FN_TS_LF0, VOICE_DIR).toLocal8Bit().data());
  strcpy_s(cPath3, MAX_PATH + 1, fileInSubdir(FN_WS_LF0_1, VOICE_DIR).toLocal8Bit().data());
  strcpy_s(cPath4, MAX_PATH + 1, fileInSubdir(FN_WS_LF0_2, VOICE_DIR).toLocal8Bit().data());
  strcpy_s(cPath5, MAX_PATH + 1, fileInSubdir(FN_WS_LF0_3, VOICE_DIR).toLocal8Bit().data());
  cPaths[0] = cPath3;
  cPaths[1] = cPath4;
  cPaths[2] = cPath5;
  HTS_Engine_load_parameter_from_fn(&_htsEngine, &cPath1, &cPath2, cPaths, 1, TRUE, NUM_WS_LF0, 1);
  if (HTS_Engine_get_nstream(&_htsEngine) == 3)
  {
    strcpy_s(cPath1, MAX_PATH + 1, fileInSubdir(FN_MS_LPF, VOICE_DIR).toLocal8Bit().data());
    strcpy_s(cPath2, MAX_PATH + 1, fileInSubdir(FN_TS_LPF, VOICE_DIR).toLocal8Bit().data());
    strcpy_s(cPath3, MAX_PATH + 1, fileInSubdir(FN_WS_LPF, VOICE_DIR).toLocal8Bit().data());
    HTS_Engine_load_parameter_from_fn(&_htsEngine, &cPath1, &cPath2, &cPath3, 2, FALSE, NUM_WS_LPF, 1);
  }
  strcpy_s(cPath1, MAX_PATH + 1, fileInSubdir(FN_MS_GVM, VOICE_DIR).toLocal8Bit().data());
  strcpy_s(cPath2, MAX_PATH + 1, fileInSubdir(FN_TS_GVM, VOICE_DIR).toLocal8Bit().data());
  HTS_Engine_load_gv_from_fn(&_htsEngine, &cPath1, &cPath2, 0, 1);
  strcpy_s(cPath1, MAX_PATH + 1, fileInSubdir(FN_MS_GV1, VOICE_DIR).toLocal8Bit().data());
  strcpy_s(cPath2, MAX_PATH + 1, fileInSubdir(FN_TS_GV1, VOICE_DIR).toLocal8Bit().data());
  HTS_Engine_load_gv_from_fn(&_htsEngine, &cPath1, &cPath2, 1, 1);
  HTS_Engine_load_gv_switch_from_fn(&_htsEngine, fileInSubdir(FN_GV_SWITCH, VOICE_DIR).toLocal8Bit().data());

  free(cPath1);
  free(cPath2);
  free(cPath3);
  free(cPath4);
  free(cPath5);
  free(cPaths);
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
    HTS_Engine_load_label_from_string_list(&_htsEngine, JPCommon_get_label_feature(&_jpCommon), JPCommon_get_label_size(&_jpCommon));
    HTS_Engine_create_sstream(&_htsEngine);
    HTS_Engine_create_pstream(&_htsEngine);
    HTS_Engine_create_gstream(&_htsEngine);
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