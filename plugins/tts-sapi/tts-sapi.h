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

        virtual const LicenseCommon::tLicenseContentList GetLicenseText() const;
        virtual const eTTSPlugin GetPluginId() const;
        virtual const QString GetPluginName() const;
		virtual const tVoiceInfoList GetVoicesInfo() const;
		virtual const void Initialize();
		virtual const void Say(const QString &pVoice, const QString &pText);
		virtual const void Uninitialize();
}; // TTSSAPI

#endif // TTSSAPI_H
