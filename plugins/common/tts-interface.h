#ifndef TTSINTERFACE_H
#define TTSINTERFACE_H

#include <QtCore/QString>
#include <QtPlugin>

static const char *TTS_INTERFACE = "TTS Interface 1.0";

class TTSInterface
{
	public:
		enum eTTSPlugin {
			TTSPluginOpenJTalk
		}; // eTTS

		virtual const eTTSPlugin GetPluginId() const = 0;
		virtual const void Initialize() = 0;
		virtual const void Say(const QString &pText) = 0;
		virtual const void Uninitialize() = 0;
}; // TTSInterface

Q_DECLARE_INTERFACE(TTSInterface, TTS_INTERFACE);

#endif // TTSINTERFACE_H