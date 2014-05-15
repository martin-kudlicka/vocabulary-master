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

		virtual const void                              initialize  ()                                          = 0;
        virtual const LicenseCommon::LicenseContentList licenseText () const                                    = 0;
		virtual const TTSPlugin                         pluginId    () const                                    = 0;
		virtual const QString                           pluginName  () const                                    = 0;
		virtual const void                              say         (const QString &voice, const QString &text) = 0;
		virtual const void                              uninitialize()                                          = 0;
		virtual const VoiceInfoList                     voicesInfo  () const                                    = 0;
}; // TTSInterface

Q_DECLARE_INTERFACE(TTSInterface, IID_TTSINTERFACE);

#endif // TTSINTERFACE_H