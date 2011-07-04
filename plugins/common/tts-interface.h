#ifndef TTSINTERFACE_H
#define TTSINTERFACE_H

#include <QtCore/QString>
#include <QtPlugin>

const char *TTS_OPENJTALK_INTERFACE = "TTS Open JTalk Plugin 1.0";

class TTSInterface
{
	private:
		virtual const void Initialize() = 0;
		virtual const void Uninitialize() = 0;
}; // TTSInterface

Q_DECLARE_INTERFACE(TTSInterface, TTS_OPENJTALK_INTERFACE);

#endif // TTSINTERFACE_H