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
        QHash<QString, QString> _qhVoices;

        virtual const LicenseCommon::LicenseContentList licenseText() const;
        virtual const TTSPlugin pluginId() const;
        virtual const QString pluginName() const;
		virtual const VoiceInfoList voicesInfo() const;
		virtual const void initialize();
		virtual const void say(const QString &pVoice, const QString &pText);
		virtual const void uninitialize();
}; // TTSSAPI

#endif // TTSSAPI_H
