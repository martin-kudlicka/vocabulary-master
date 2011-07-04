#ifndef TTSOPENJTALK_H
#define TTSOPENJTALK_H

#include "../common/tts-interface.h"
#include "../../3rdparty/Open JTalk/source/mecab/src/mecab.h"
#include "../../3rdparty/Open JTalk/source/njd/njd.h"
#include "../../3rdparty/Open JTalk/source/jpcommon/jpcommon.h"
#include "../../3rdparty/hts_engine API/source/include/HTS_engine.h"

class TTSOpenJTalk : private TTSInterface, public QObject
{
	Q_INTERFACES(TTSInterface)

	private:
		static const HTS_Boolean USE_LOG_GAIN = FALSE;
		static const int AUDIO_BUFF_SIZE = 1600;
		static const int FRAME_SHIFT = 240;
		static const int NUM_WS_LF0 = 1;
		static const int NUM_WS_LPF = 1;
		static const int NUM_WS_MGC = 1;
		static const int SAMPLING_RATE = 48000;
		static const int STAGE = 0;

		HTS_Engine _heEngine;
		Mecab _mMecab;
		NJD _nNjd;
		JPCommon _jJpcommon;

		const void Initialize();
		const void Uninitialize();
}; // TTSOpenJTalk

#endif // TTSOPENJTALK_H