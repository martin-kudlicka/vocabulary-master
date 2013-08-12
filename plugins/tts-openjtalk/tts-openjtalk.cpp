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

char *FN_GV_SWITCH = "gv-switch.inf";
const double ALPHA = 0.55;
const double BETA = 0.0;
const double GV_WEIGHT_LF0 = 1.0;
const double GV_WEIGHT_LPF = 1.0;
const double GV_WEIGHT_MGC = 1.0;
const double UV_THRESHOLD = 0.5;
const QString DIC_DIR = "open_jtalk_dic_utf_8";
const QString FN_MS_DUR = "dur.pdf";
const QString FN_MS_GV1 = "gv-lf0.pdf";
const QString FN_MS_GVM = "gv-mgc.pdf";
const QString FN_MS_LF0 = "lf0.pdf";
const QString FN_MS_LPF = "lpf.pdf";
const QString FN_MS_MGC = "mgc.pdf";
const QString FN_TS_DUR = "tree-dur.inf";
const QString FN_TS_GV1 = "tree-gv-lf0.inf";
const QString FN_TS_GVM = "tree-gv-mgc.inf";
const QString FN_TS_LF0 = "tree-lf0.inf";
const QString FN_TS_LPF = "tree-lpf.inf";
const QString FN_TS_MGC = "tree-mgc.inf";
const QString FN_WS_LF0_1 = "lf0.win1";
const QString FN_WS_LF0_2 = "lf0.win2";
const QString FN_WS_LF0_3 = "lf0.win3";
const QString FN_WS_LPF = "lpf.win1";
const QString FN_WS_MGC_1 = "mgc.win1";
const QString FN_WS_MGC_2 = "mgc.win2";
const QString FN_WS_MGC_3 = "mgc.win3";
const QString VOICE_DIR = "hts_voice_nitech_jp_atr503_m001";

const QString TTSOpenJTalk::GetFileInSubdir(const QString &pFile, const QString &pDir) const
{
	return GetSubdir(pDir) + QDir::separator() + pFile;
} // GetFileInSubdir

const LicenseCommon::tLicenseContentList TTSOpenJTalk::GetLicenseText() const
{
    LicenseCommon::tLicenseContentList tlclLicenses;
    LicenseCommon::sLicenseContent slcContent;

    slcContent.qsTitle = "hts_engine API";
    QFile qfLicense(":/res/hts_engine API/COPYING");
    qfLicense.open(QIODevice::ReadOnly);
    slcContent.qsText = qfLicense.readAll();
    tlclLicenses.append(slcContent);
    qfLicense.close();

    slcContent.qsTitle = "Open JTalk";
    qfLicense.setFileName(":/res/Open JTalk/COPYING");
    qfLicense.open(QIODevice::ReadOnly);
    slcContent.qsText = qfLicense.readAll();
    tlclLicenses.append(slcContent);

    return tlclLicenses;
} // GetLicenseText

const TTSInterface::eTTSPlugin TTSOpenJTalk::GetPluginId() const
{
	return TTSPluginOpenJTalk;
} // GetPluginId

const QString TTSOpenJTalk::GetPluginName() const
{
	return "Open JTalk";
} // GetPluginName

const TTSInterface::tVoiceInfoList TTSOpenJTalk::GetVoicesInfo() const
{
    TTSInterface::sVoiceInfo sviVoice;
    sviVoice.qsDescription = "NIT ATR503 M001";
    return TTSInterface::tVoiceInfoList() << sviVoice;
} // GetVoicesInfo

const QString TTSOpenJTalk::GetSubdir(const QString &pDir) const
{
	return QCoreApplication::applicationDirPath() + QDir::separator() +
#ifndef _DEBUG
		DIR_PLUGINS + QDir::separator() + "tts-openjtalk" + QDir::separator() +
#endif
		pDir;
} // GetSubdir

const void TTSOpenJTalk::Initialize()
{
	// initialize
	Mecab_initialize(&_mMecab);
	NJD_initialize(&_nNjd);
	JPCommon_initialize(&_jJpcommon);
	HTS_Engine_initialize(&_heEngine, 3);
	HTS_Engine_set_sampling_rate(&_heEngine, SAMPLING_RATE);
	HTS_Engine_set_fperiod(&_heEngine, FRAME_SHIFT);
	HTS_Engine_set_alpha(&_heEngine, ALPHA);
	HTS_Engine_set_gamma(&_heEngine, STAGE);
	HTS_Engine_set_log_gain(&_heEngine, USE_LOG_GAIN);
	HTS_Engine_set_beta(&_heEngine, BETA);
	HTS_Engine_set_audio_buff_size(&_heEngine, AUDIO_BUFF_SIZE);
	HTS_Engine_set_msd_threshold(&_heEngine, 1, UV_THRESHOLD);
	HTS_Engine_set_gv_weight(&_heEngine, 0, GV_WEIGHT_MGC);
	HTS_Engine_set_gv_weight(&_heEngine, 1, GV_WEIGHT_LF0);
	HTS_Engine_set_gv_weight(&_heEngine, 2, GV_WEIGHT_LPF);

	// load
	char **cPaths = static_cast<char **>(malloc(NUM_WS_MGC * sizeof(char *)));
	char *cPath1 = static_cast<char *>(malloc(MAX_PATH + 1));
	char *cPath2 = static_cast<char *>(malloc(MAX_PATH + 1));
	char *cPath3 = static_cast<char *>(malloc(MAX_PATH + 1));
	char *cPath4 = static_cast<char *>(malloc(MAX_PATH + 1));
	char *cPath5 = static_cast<char *>(malloc(MAX_PATH + 1));

	Mecab_load(&_mMecab, GetSubdir(DIC_DIR).toLocal8Bit().data());
	strcpy_s(cPath1, MAX_PATH + 1, GetFileInSubdir(FN_MS_DUR, VOICE_DIR).toLocal8Bit().data());
	strcpy_s(cPath2, MAX_PATH + 1, GetFileInSubdir(FN_TS_DUR, VOICE_DIR).toLocal8Bit().data());
	HTS_Engine_load_duration_from_fn(&_heEngine, &cPath1, &cPath2, 1);
	strcpy_s(cPath1, MAX_PATH + 1, GetFileInSubdir(FN_MS_MGC, VOICE_DIR).toLocal8Bit().data());
	strcpy_s(cPath2, MAX_PATH + 1, GetFileInSubdir(FN_TS_MGC, VOICE_DIR).toLocal8Bit().data());
	strcpy_s(cPath3, MAX_PATH + 1, GetFileInSubdir(FN_WS_MGC_1, VOICE_DIR).toLocal8Bit().data());
	strcpy_s(cPath4, MAX_PATH + 1, GetFileInSubdir(FN_WS_MGC_2, VOICE_DIR).toLocal8Bit().data());
	strcpy_s(cPath5, MAX_PATH + 1, GetFileInSubdir(FN_WS_MGC_3, VOICE_DIR).toLocal8Bit().data());
	cPaths[0] = cPath3;
	cPaths[1] = cPath4;
	cPaths[2] = cPath5;
	HTS_Engine_load_parameter_from_fn(&_heEngine, &cPath1, &cPath2, cPaths, 0, FALSE, NUM_WS_MGC, 1);
	strcpy_s(cPath1, MAX_PATH + 1, GetFileInSubdir(FN_MS_LF0, VOICE_DIR).toLocal8Bit().data());
	strcpy_s(cPath2, MAX_PATH + 1, GetFileInSubdir(FN_TS_LF0, VOICE_DIR).toLocal8Bit().data());
	strcpy_s(cPath3, MAX_PATH + 1, GetFileInSubdir(FN_WS_LF0_1, VOICE_DIR).toLocal8Bit().data());
	strcpy_s(cPath4, MAX_PATH + 1, GetFileInSubdir(FN_WS_LF0_2, VOICE_DIR).toLocal8Bit().data());
	strcpy_s(cPath5, MAX_PATH + 1, GetFileInSubdir(FN_WS_LF0_3, VOICE_DIR).toLocal8Bit().data());
	cPaths[0] = cPath3;
	cPaths[1] = cPath4;
	cPaths[2] = cPath5;
	HTS_Engine_load_parameter_from_fn(&_heEngine, &cPath1, &cPath2, cPaths, 1, TRUE, NUM_WS_LF0, 1);
	if (HTS_Engine_get_nstream(&_heEngine) == 3) {
		strcpy_s(cPath1, MAX_PATH + 1, GetFileInSubdir(FN_MS_LPF, VOICE_DIR).toLocal8Bit().data());
		strcpy_s(cPath2, MAX_PATH + 1, GetFileInSubdir(FN_TS_LPF, VOICE_DIR).toLocal8Bit().data());
		strcpy_s(cPath3, MAX_PATH + 1, GetFileInSubdir(FN_WS_LPF, VOICE_DIR).toLocal8Bit().data());
		HTS_Engine_load_parameter_from_fn(&_heEngine, &cPath1, &cPath2, &cPath3, 2, FALSE, NUM_WS_LPF, 1);
	} // if
	strcpy_s(cPath1, MAX_PATH + 1, GetFileInSubdir(FN_MS_GVM, VOICE_DIR).toLocal8Bit().data());
	strcpy_s(cPath2, MAX_PATH + 1, GetFileInSubdir(FN_TS_GVM, VOICE_DIR).toLocal8Bit().data());
	HTS_Engine_load_gv_from_fn(&_heEngine, &cPath1, &cPath2, 0, 1);
	strcpy_s(cPath1, MAX_PATH + 1, GetFileInSubdir(FN_MS_GV1, VOICE_DIR).toLocal8Bit().data());
	strcpy_s(cPath2, MAX_PATH + 1, GetFileInSubdir(FN_TS_GV1, VOICE_DIR).toLocal8Bit().data());
	HTS_Engine_load_gv_from_fn(&_heEngine, &cPath1, &cPath2, 1, 1);
	HTS_Engine_load_gv_switch_from_fn(&_heEngine, GetFileInSubdir(FN_GV_SWITCH, VOICE_DIR).toLocal8Bit().data());

	free(cPath1);
	free(cPath2);
	free(cPath3);
	free(cPath4);
	free(cPath5);
	free(cPaths);
} // Initialize

const void TTSOpenJTalk::Say(const QString &pVoice, const QString &pText)
{
	char *cBuffer = static_cast<char *>(calloc(2 * pText.toUtf8().size() + 1, sizeof(char)));
	text2mecab(cBuffer, pText.toUtf8().data());
	Mecab_analysis(&_mMecab, cBuffer);
	free(cBuffer);
	mecab2njd(&_nNjd, Mecab_get_feature(&_mMecab), Mecab_get_size(&_mMecab));
	njd_set_pronunciation(&_nNjd);
	njd_set_digit(&_nNjd);
	njd_set_accent_phrase(&_nNjd);
	njd_set_accent_type(&_nNjd);
	njd_set_unvoiced_vowel(&_nNjd);
	njd_set_long_vowel(&_nNjd);
	njd2jpcommon(&_jJpcommon, &_nNjd);
	JPCommon_make_label(&_jJpcommon);
	if (JPCommon_get_label_size(&_jJpcommon) > 2) {
		HTS_Engine_load_label_from_string_list(&_heEngine, JPCommon_get_label_feature(&_jJpcommon), JPCommon_get_label_size(&_jJpcommon));
		HTS_Engine_create_sstream(&_heEngine);
		HTS_Engine_create_pstream(&_heEngine);
		HTS_Engine_create_gstream(&_heEngine);
		HTS_Engine_refresh(&_heEngine);
	} // if
	JPCommon_refresh(&_jJpcommon);
	NJD_refresh(&_nNjd);
	Mecab_refresh(&_mMecab);
} // Say

const void TTSOpenJTalk::Uninitialize()
{
	Mecab_clear(&_mMecab);
	NJD_clear(&_nNjd);
	JPCommon_clear(&_jJpcommon);
	HTS_Engine_clear(&_heEngine);
} // Uninitialize