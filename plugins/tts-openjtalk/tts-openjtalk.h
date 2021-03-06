#ifndef TTSOPENJTALK_H
#define TTSOPENJTALK_H

#include "../common/tts-interface.h"

class TTSOpenJTalk : public QObject, private TTSInterface
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID IID_TTSINTERFACE)
  Q_INTERFACES(TTSInterface)

  private:
    HTS_Engine _htsEngine;
    Mecab      _mecab;
    NJD        _njd;
    JPCommon   _jpCommon;

    virtual ~TTSOpenJTalk() Q_DECL_OVERRIDE Q_DECL_EQ_DEFAULT;

    QString fileInSubdir(const QString &file, const QString &dir) const;
    QString subdir      (const QString &dir)                      const;

    virtual void                              initialize  ()                                          Q_DECL_OVERRIDE;
    virtual LicenseCommon::LicenseContentList licenseText () const                                    Q_DECL_OVERRIDE;
    virtual TTSPlugin                         pluginId    () const                                    Q_DECL_OVERRIDE;
    virtual QString                           pluginName  () const                                    Q_DECL_OVERRIDE;
    virtual void                              say         (const QString &voice, const QString &text) Q_DECL_OVERRIDE;
    virtual void                              uninitialize()                                          Q_DECL_OVERRIDE;
    virtual VoiceInfoList                     voicesInfo  () const                                    Q_DECL_OVERRIDE;
};

#endif