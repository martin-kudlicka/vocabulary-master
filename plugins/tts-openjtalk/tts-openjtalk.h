#ifndef TTSOPENJTALK_H
#define TTSOPENJTALK_H

#include "../common/tts-interface.h"
#include "../../3rdparty/Open JTalk/source/mecab/src/mecab.h"
#include "../../3rdparty/Open JTalk/source/njd/njd.h"
#include "../../3rdparty/Open JTalk/source/jpcommon/jpcommon.h"
#include "../../3rdparty/hts_engine API/source/include/HTS_engine.h"

class TTSOpenJTalk : public QObject, private TTSInterface
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID IID_TTSINTERFACE FILE "tts-openjtalk.json")
  Q_INTERFACES(TTSInterface)

  private:
    HTS_Engine _htsEngine;
    Mecab      _mecab;
    NJD        _njd;
    JPCommon   _jpCommon;

    virtual ~TTSOpenJTalk() Q_DECL_OVERRIDE;

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