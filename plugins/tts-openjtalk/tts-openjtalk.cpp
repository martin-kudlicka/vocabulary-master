#include "tts-openjtalk.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QDir>

#define PLUGIN_CLASS TTSOpenJTalk
#define PLUGIN_NAME	 tts-openjtalk

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
const QString FN_WS_LF0 = "lf0.win1";
const QString FN_WS_LPF = "lpf.win1";
const QString FN_WS_MGC = "mgc.win2";
const QString VOICE_DIR = "hts_voice_nitech_jp_atr503_m001";

const QString TTSOpenJTalk::GetFileInSubdir(const QString &pFile, const QString &pDir) const
{
	return GetSubdir(pDir) + QDir::separator() + pFile;
} // GetFileInSubdir

const TTSInterface::eTTSPlugin TTSOpenJTalk::GetPluginId() const
{
	return TTSPluginOpenJTalk;
} // GetPluginId

const QString TTSOpenJTalk::GetSubdir(const QString &pDir) const
{
	return QCoreApplication::applicationDirPath() + QDir::separator() + pDir;
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
	char *cPath1 = static_cast<char *>(malloc(MAX_PATH + 1));
	char *cPath2 = static_cast<char *>(malloc(MAX_PATH + 1));
	char *cPath3 = static_cast<char *>(malloc(MAX_PATH + 1));

	Mecab_load(&_mMecab, GetSubdir(DIC_DIR).toLocal8Bit().data());
	strcpy(cPath1, GetFileInSubdir(FN_MS_DUR, VOICE_DIR).toLocal8Bit().data());
	strcpy(cPath2, GetFileInSubdir(FN_TS_DUR, VOICE_DIR).toLocal8Bit().data());
	HTS_Engine_load_duration_from_fn(&_heEngine, &cPath1, &cPath2, 1);
	strcpy(cPath1, GetFileInSubdir(FN_MS_MGC, VOICE_DIR).toLocal8Bit().data());
	strcpy(cPath2, GetFileInSubdir(FN_TS_MGC, VOICE_DIR).toLocal8Bit().data());
	strcpy(cPath3, GetFileInSubdir(FN_WS_MGC, VOICE_DIR).toLocal8Bit().data());
	HTS_Engine_load_parameter_from_fn(&_heEngine, &cPath1, &cPath2, &cPath3, 0, FALSE, NUM_WS_MGC, 1);
	strcpy(cPath1, GetFileInSubdir(FN_MS_LF0, VOICE_DIR).toLocal8Bit().data());
	strcpy(cPath2, GetFileInSubdir(FN_TS_LF0, VOICE_DIR).toLocal8Bit().data());
	strcpy(cPath3, GetFileInSubdir(FN_WS_LF0, VOICE_DIR).toLocal8Bit().data());
	HTS_Engine_load_parameter_from_fn(&_heEngine, &cPath1, &cPath2, &cPath3, 1, TRUE, NUM_WS_LF0, 1);
	if (HTS_Engine_get_nstream(&_heEngine) == 3) {
		strcpy(cPath1, GetFileInSubdir(FN_MS_LPF, VOICE_DIR).toLocal8Bit().data());
		strcpy(cPath2, GetFileInSubdir(FN_TS_LPF, VOICE_DIR).toLocal8Bit().data());
		strcpy(cPath3, GetFileInSubdir(FN_WS_LPF, VOICE_DIR).toLocal8Bit().data());
		HTS_Engine_load_parameter_from_fn(&_heEngine, &cPath1, &cPath2, &cPath3, 2, FALSE, NUM_WS_LPF, 1);
	} // if
	strcpy(cPath1, GetFileInSubdir(FN_MS_GVM, VOICE_DIR).toLocal8Bit().data());
	strcpy(cPath2, GetFileInSubdir(FN_TS_GVM, VOICE_DIR).toLocal8Bit().data());
	HTS_Engine_load_gv_from_fn(&_heEngine, &cPath1, &cPath2, 0, 1);
	strcpy(cPath1, GetFileInSubdir(FN_MS_GV1, VOICE_DIR).toLocal8Bit().data());
	strcpy(cPath2, GetFileInSubdir(FN_TS_GV1, VOICE_DIR).toLocal8Bit().data());
	HTS_Engine_load_gv_from_fn(&_heEngine, &cPath1, &cPath2, 1, 1);
	HTS_Engine_load_gv_switch_from_fn(&_heEngine, GetFileInSubdir(FN_GV_SWITCH, VOICE_DIR).toLocal8Bit().data());

	free(cPath1);
	free(cPath2);
	free(cPath3);
} // Initialize

const void TTSOpenJTalk::Uninitialize()
{
	Mecab_clear(&_mMecab);
	NJD_clear(&_nNjd);
	JPCommon_clear(&_jJpcommon);
	HTS_Engine_clear(&_heEngine);
} // Uninitialize

Q_EXPORT_PLUGIN2(PLUGIN_NAME, PLUGIN_CLASS)