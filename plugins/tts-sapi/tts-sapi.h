#ifndef TTSSAPI_H
#define TTSSAPI_H

#include "../common/tts-interface.h"
#include <QtCore/QHash>

class TTSSAPI : public QObject, private TTSInterface
{
    Q_OBJECT
    Q_INTERFACES(TTSInterface)

    private:
        QHash<QString, QString> _qhVoices;

        const eTTSPlugin GetPluginId() const;
        const QString GetPluginName() const;
		const tVoiceInfoList GetVoicesInfo() const;
		const void Initialize();
		const void Say(const QString &pVoice, const QString &pText);
		const void Uninitialize();
}; // TTSSAPI

#endif // TTSSAPI_H
