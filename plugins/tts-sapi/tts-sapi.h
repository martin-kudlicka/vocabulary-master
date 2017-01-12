#ifndef TTSSAPI_H
#define TTSSAPI_H

#include "../common/tts-interface.h"

class TTSSAPI : public QObject, private TTSInterface
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID IID_TTSINTERFACE FILE "tts-sapi.json")
  Q_INTERFACES(TTSInterface)

  private:
    virtual ~TTSSAPI() Q_DECL_OVERRIDE;

    virtual void                              initialize  ()                                          Q_DECL_OVERRIDE;
    virtual LicenseCommon::LicenseContentList licenseText () const                                    Q_DECL_OVERRIDE;
    virtual TTSPlugin                         pluginId    () const                                    Q_DECL_OVERRIDE;
    virtual QString                           pluginName  () const                                    Q_DECL_OVERRIDE;
    virtual void                              say         (const QString &voice, const QString &text) Q_DECL_OVERRIDE;
    virtual void                              uninitialize()                                          Q_DECL_OVERRIDE;
    virtual VoiceInfoList                     voicesInfo  () const                                    Q_DECL_OVERRIDE;
};

#endif