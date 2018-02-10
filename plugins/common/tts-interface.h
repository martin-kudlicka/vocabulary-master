#ifndef TTSINTERFACE_H
#define TTSINTERFACE_H

#include <QtCore/QString>
#include <QtPlugin>
#include "../../common/licensecommon.h"

#define IID_TTSINTERFACE "cz.martink.VocabularyMaster.TTSInterface"
#ifndef _DEBUG
const auto DIR_PLUGINS = "plugins";
#endif

class TTSInterface
{
  public:
    enum class TTSPlugin
    {
      None,
      OpenJTalk,
      SAPI
    };

    struct VoiceInfo
    {
      QString id;
      QString description;
    };

    using VoiceInfoList = QList<VoiceInfo>;

    virtual void                              initialize  ()                                          = 0;
    virtual LicenseCommon::LicenseContentList licenseText () const                                    = 0;
    virtual TTSPlugin                         pluginId    () const                                    = 0;
    virtual QString                           pluginName  () const                                    = 0;
    virtual void                              say         (const QString &voice, const QString &text) = 0;
    virtual void                              uninitialize()                                          = 0;
    virtual VoiceInfoList                     voicesInfo  () const                                    = 0;
};

Q_DECLARE_INTERFACE(TTSInterface, IID_TTSINTERFACE);

#endif