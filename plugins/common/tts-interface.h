#ifndef TTSINTERFACE_H
#define TTSINTERFACE_H

#include <QtCore/QString>
#include <QtPlugin>

static const char *TTS_INTERFACE = "TTS Interface 1.0";
#ifndef _DEBUG
const QString DIR_PLUGINS = "plugins";
#endif

class TTSInterface
{
	public:
		enum eTTSPlugin {
			TTPluginNone,
			TTSPluginOpenJTalk,
            TTSPluginSAPI
		}; // eTTS

        struct sVoiceInfo {
            QString qsId;
            QString qsDescription;
        }; // sVoiceInfo

        typedef QList<sVoiceInfo> tVoiceInfoList;

		virtual const eTTSPlugin GetPluginId() const = 0;
		virtual const QString GetPluginName() const = 0;
        virtual const tVoiceInfoList GetVoicesInfo() const = 0;
		virtual const void Initialize() = 0;
		virtual const void Say(const QString &pVoice, const QString &pText) = 0;
		virtual const void Uninitialize() = 0;
}; // TTSInterface

Q_DECLARE_INTERFACE(TTSInterface, TTS_INTERFACE);

#endif // TTSINTERFACE_H