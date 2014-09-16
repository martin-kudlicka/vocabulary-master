#ifndef TTSINTERFACE_H
#define TTSINTERFACE_H

#include <QtCore/QString>
#include <QtPlugin>
#include "../../common/licensecommon.h"

#define IID_TTSINTERFACE "cz.martinkudlicka.VocabularMmaster.TTSInterface"
#ifndef _DEBUG
const QString DIR_PLUGINS = "plugins";
#endif

class TTSInterface
{
	public:
		enum TTSPlugin
		{
			TTPluginNone,
			TTSPluginOpenJTalk,
            TTSPluginSAPI
		}; // TTSPlugin

        struct VoiceInfo
		{
            QString id;
            QString description;
        }; // VoiceInfo

        typedef QList<VoiceInfo> VoiceInfoList;

		virtual void                              initialize  ()                                          = 0;
        virtual LicenseCommon::LicenseContentList licenseText () const                                    = 0;
		virtual TTSPlugin                         pluginId    () const                                    = 0;
		virtual QString                           pluginName  () const                                    = 0;
		virtual void                              say         (const QString &voice, const QString &text) = 0;
		virtual void                              uninitialize()                                          = 0;
		virtual VoiceInfoList                     voicesInfo  () const                                    = 0;
}; // TTSInterface

Q_DECLARE_INTERFACE(TTSInterface, IID_TTSINTERFACE);

#endif // TTSINTERFACE_H