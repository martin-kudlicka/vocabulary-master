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
		virtual ~TTSSAPI() override;

		virtual void                              initialize  ()                                          override;
        virtual LicenseCommon::LicenseContentList licenseText () const                                    override;
        virtual TTSPlugin                         pluginId    () const                                    override;
        virtual QString                           pluginName  () const                                    override;
		virtual void                              say         (const QString &voice, const QString &text) override;
		virtual void                              uninitialize()                                          override;
		virtual VoiceInfoList                     voicesInfo  () const                                    override;
}; // TTSSAPI

#endif // TTSSAPI_H