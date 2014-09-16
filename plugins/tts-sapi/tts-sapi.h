#ifndef TTSSAPI_H
#define TTSSAPI_H

#include "../common/tts-interface.h"
#include <QtCore/QHash>

class TTSSAPI : public QObject, private TTSInterface
{
    Q_OBJECT
	Q_PLUGIN_METADATA(IID IID_TTSINTERFACE FILE "tts-sapi.json")
    Q_INTERFACES(TTSInterface)

    private:
		virtual ~TTSSAPI();

		virtual void                              initialize  ();
        virtual LicenseCommon::LicenseContentList licenseText () const;
        virtual TTSPlugin                         pluginId    () const;
        virtual QString                           pluginName  () const;
		virtual void                              say         (const QString &voice, const QString &text);
		virtual void                              uninitialize();
		virtual VoiceInfoList                     voicesInfo  () const;
}; // TTSSAPI

#endif // TTSSAPI_H