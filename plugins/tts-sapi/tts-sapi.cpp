#include "tts-sapi.h"

#include <qt_windows.h>
#include <atlcomcli.h>
#include <sapi.h>
#include <sphelper.h>

TTSSAPI::~TTSSAPI()
{
} // ~TTSSAPI

const void TTSSAPI::initialize()
{
	CoInitializeEx(NULL, COINIT_MULTITHREADED);
} // initialize

const LicenseCommon::LicenseContentList TTSSAPI::licenseText() const
{
    return LicenseCommon::LicenseContentList();
} // licenseText

const TTSInterface::TTSPlugin TTSSAPI::pluginId() const
{
    return TTSInterface::TTSPluginSAPI;
} // pluginId

const QString TTSSAPI::pluginName() const
{
    return "SAPI";
} // pluginName

const void TTSSAPI::say(const QString &voice, const QString &text)
{
	CComPtr<IEnumSpObjectTokens> voiceTokens;
	ULONG count;

	SpEnumTokens(SPCAT_VOICES, NULL, NULL, &voiceTokens);
	voiceTokens->GetCount(&count);

	for(ULONG voiceIndex = 0; voiceIndex < count; voiceIndex++)
	{
		CComPtr<ISpObjectToken> voiceInfo;
		WCHAR *id;

		voiceTokens->Next(1, &voiceInfo, NULL);
		voiceInfo->GetId(&id);

		if (voice == QString::fromWCharArray(id))
		{
			CComPtr<ISpVoice> voice;
			voice.CoCreateInstance(CLSID_SpVoice);
			voice->SetVoice(voiceInfo);
			voice->Speak(reinterpret_cast<LPCWSTR>(text.unicode()), SPF_IS_NOT_XML, NULL);
			//voiceInfo->Speak();
		} // if
	} // for
} // say

const void TTSSAPI::uninitialize()
{
	CoUninitialize();
} // uninitialize

const TTSInterface::VoiceInfoList TTSSAPI::voicesInfo() const
{
	CComPtr<IEnumSpObjectTokens> voiceTokens;
    TTSInterface::VoiceInfoList voices;
	ULONG count;

	SpEnumTokens(SPCAT_VOICES, NULL, NULL, &voiceTokens);
	voiceTokens->GetCount(&count);

	for(ULONG voiceIndex = 0; voiceIndex < count; voiceIndex++)
	{
		CComPtr<ISpObjectToken> voiceInfo;
		WCHAR *description, *id;

		voiceTokens->Next(1, &voiceInfo, NULL);
		voiceInfo->GetId(&id);
		SpGetDescription(voiceInfo, &description);

        TTSInterface::VoiceInfo voice;
        voice.id          = QString::fromWCharArray(id);
        voice.description = QString::fromWCharArray(description);
		voices.append(voice);
	} // for

	return voices;
} // voicesInfo