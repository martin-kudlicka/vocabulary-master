#include "pch.h"
#include "tts-sapi.h"

void TTSSAPI::initialize()
{
  CoInitializeEx(Q_NULLPTR, COINIT_MULTITHREADED);
}

LicenseCommon::LicenseContentList TTSSAPI::licenseText() const
{
  return LicenseCommon::LicenseContentList();
}

TTSInterface::TTSPlugin TTSSAPI::pluginId() const
{
  return TTSInterface::TTSPlugin::SAPI;
}

QString TTSSAPI::pluginName() const
{
  return "SAPI";
}

void TTSSAPI::say(const QString &voice, const QString &text)
{
  CComPtr<IEnumSpObjectTokens> voiceTokens;
  ULONG count;

  SpEnumTokens(SPCAT_VOICES, Q_NULLPTR, Q_NULLPTR, &voiceTokens);
  voiceTokens->GetCount(&count);

  for (decltype(count) voiceIndex = 0; voiceIndex < count; ++voiceIndex)
  {
    CComPtr<ISpObjectToken> voiceInfo;
    WCHAR *id;

    voiceTokens->Next(1, &voiceInfo, Q_NULLPTR);
    voiceInfo->GetId(&id);

    if (voice == QString::fromWCharArray(id))
    {
      CComPtr<ISpVoice> speechVoice;
      speechVoice.CoCreateInstance(CLSID_SpVoice);
      speechVoice->SetVoice(voiceInfo);
      speechVoice->Speak(reinterpret_cast<LPCWSTR>(text.unicode()), SPF_IS_NOT_XML, Q_NULLPTR);
      //voiceInfo->Speak();
    }
  }
}

void TTSSAPI::uninitialize()
{
  CoUninitialize();
}

TTSInterface::VoiceInfoList TTSSAPI::voicesInfo() const
{
  CComPtr<IEnumSpObjectTokens> voiceTokens;
  TTSInterface::VoiceInfoList voices;
  ULONG count;

  SpEnumTokens(SPCAT_VOICES, Q_NULLPTR, Q_NULLPTR, &voiceTokens);
  voiceTokens->GetCount(&count);

  for (decltype(count) voiceIndex = 0; voiceIndex < count; ++voiceIndex)
  {
    CComPtr<ISpObjectToken> voiceInfo;
    WCHAR *description, *id;

    voiceTokens->Next(1, &voiceInfo, Q_NULLPTR);
    voiceInfo->GetId(&id);
    auto result = SpGetDescription(voiceInfo, &description);
    if (SUCCEEDED(result))
    {
      TTSInterface::VoiceInfo voice;
      voice.id          = QString::fromWCharArray(id);
      voice.description = QString::fromWCharArray(description);
      voices.append(voice);
    }
  }

  return voices;
}