#ifndef TTSOPENJTALK_H
#define TTSOPENJTALK_H

#include "../common/tts-interface.h"
#include "../../3rdparty/Open JTalk/source/mecab/src/mecab.h"
#include "../../3rdparty/Open JTalk/source/njd/njd.h"
#include "../../3rdparty/Open JTalk/source/jpcommon/jpcommon.h"
#include "../../3rdparty/hts_engine API/source/include/HTS_engine.h"

class TTSOpenJTalk : public QObject, private TTSInterface
{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID IID_TTSINTERFACE FILE "tts-openjtalk.json")
	Q_INTERFACES(TTSInterface)

	private:
		static const HTS_Boolean USE_LOG_GAIN    = FALSE;
		static const int         AUDIO_BUFF_SIZE = 1600;
		static const int         FRAME_SHIFT     = 240;
		static const int         NUM_WS_LF0      = 3;
		static const int         NUM_WS_LPF      = 1;
		static const int         NUM_WS_MGC      = 3;
		static const int         SAMPLING_RATE   = 48000;
		static const int         STAGE           = 0;

		HTS_Engine _htsEngine;
		Mecab      _mecab;
		NJD        _njd;
		JPCommon   _jpCommon;

		virtual ~TTSOpenJTalk();

		        const QString                           fileInSubdir(const QString &file, const QString &dir) const;
		virtual const void                              initialize  ();
        virtual const LicenseCommon::LicenseContentList licenseText () const;
		virtual const TTSPlugin                         pluginId    () const;
		virtual const QString                           pluginName  () const;
		virtual const void                              say         (const QString &voice, const QString &text);
		        const QString                           subdir      (const QString &dir) const;
		virtual const void                              uninitialize();
        virtual const VoiceInfoList                     voicesInfo  () const;
}; // TTSOpenJTalk

#endif // TTSOPENJTALK_H