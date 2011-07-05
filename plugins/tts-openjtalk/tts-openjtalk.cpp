#include "tts-openjtalk.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QDir>

#define PLUGIN_CLASS TTSOpenJTalk
#define PLUGIN_NAME	 tts-openjtalk

char *FN_GV_SWITCH = "gv-switch.inf";
char *FN_MS_DUR = "dur.pdf";
char *FN_MS_GV1 = "gv-lf0.pdf";
char *FN_MS_GVM = "gv-mgc.pdf";
char *FN_MS_LF0 = "lf0.pdf";
char *FN_MS_LPF = "lpf.pdf";
char *FN_MS_MGC = "mgc.pdf";
char *FN_TS_DUR = "tree-dur.inf";
char *FN_TS_GV1 = "tree-gv-lf0.inf";
char *FN_TS_GVM = "tree-gv-mgc.inf";
char *FN_TS_LF0 = "tree-lf0.inf";
char *FN_TS_LPF = "tree-lpf.inf";
char *FN_TS_MGC = "tree-mgc.inf";
char *FN_WS_LF0 = "lf0.win1";
char *FN_WS_LPF = "lpf.win1";
char *FN_WS_MGC = "mgc.win2";
const double ALPHA = 0.55;
const double BETA = 0.0;
const double GV_WEIGHT_LF0 = 1.0;
const double GV_WEIGHT_LPF = 1.0;
const double GV_WEIGHT_MGC = 1.0;
const double UV_THRESHOLD = 0.5;
const QString DIC_DIR = "open_jtalk_dic_utf_8";

const TTSInterface::eTTSPlugin TTSOpenJTalk::GetPluginId() const
{
	return TTSPluginOpenJTalk;
} // GetPluginId

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
	Mecab_load(&_mMecab, QString(QCoreApplication::applicationDirPath() + QDir::separator() + DIC_DIR).toLocal8Bit().data());
	HTS_Engine_load_duration_from_fn(&_heEngine, &FN_MS_DUR, &FN_TS_DUR, 1);
	HTS_Engine_load_parameter_from_fn(&_heEngine, &FN_MS_MGC, &FN_TS_MGC, &FN_WS_MGC, 0, FALSE, NUM_WS_MGC, 1);
	HTS_Engine_load_parameter_from_fn(&_heEngine, &FN_MS_LF0, &FN_TS_LF0, &FN_WS_LF0, 1, TRUE, NUM_WS_LF0, 1);
	if (HTS_Engine_get_nstream(&_heEngine) == 3) {
		HTS_Engine_load_parameter_from_fn(&_heEngine, &FN_MS_LPF, &FN_TS_LPF, &FN_WS_LPF, 2, FALSE, NUM_WS_LPF, 1);
	} // if
	HTS_Engine_load_gv_from_fn(&_heEngine, &FN_MS_GVM, &FN_TS_GVM, 0, 1);
	HTS_Engine_load_gv_from_fn(&_heEngine, &FN_MS_GV1, &FN_TS_GV1, 1, 1);
	HTS_Engine_load_gv_switch_from_fn(&_heEngine, FN_GV_SWITCH);
} // Initialize

const void TTSOpenJTalk::Uninitialize()
{
	Mecab_clear(&_mMecab);
	NJD_clear(&_nNjd);
	JPCommon_clear(&_jJpcommon);
	HTS_Engine_clear(&_heEngine);
} // Uninitialize

Q_EXPORT_PLUGIN2(PLUGIN_NAME, PLUGIN_CLASS)