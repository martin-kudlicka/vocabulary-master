#include "tts-sapi.h"

#include <qt_windows.h>
#include <atlcomcli.h>
#include <sapi.h>
#include <sphelper.h>

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
} // Initialize

const TTSInterface::VoiceInfoList TTSSAPI::voicesInfo() const
{
	CComPtr<IEnumSpObjectTokens> ccpVoices;
    TTSInterface::VoiceInfoList tvilVoices;
	ULONG ulCount;

	SpEnumTokens(SPCAT_VOICES, NULL, NULL, &ccpVoices);
	ccpVoices->GetCount(&ulCount);

	for(ULONG ulI = 0; ulI < ulCount; ulI++) {
		CComPtr<ISpObjectToken> ccpVoiceInfo;
		WCHAR *wcDescription, *wcId;

		ccpVoices->Next(1, &ccpVoiceInfo, NULL);
		ccpVoiceInfo->GetId(&wcId);
		SpGetDescription(ccpVoiceInfo, &wcDescription);

        TTSInterface::VoiceInfo sviVoice;
        sviVoice.id = QString::fromWCharArray(wcId);
        sviVoice.description = QString::fromWCharArray(wcDescription);
		tvilVoices.append(sviVoice);
	} // for

	return tvilVoices;
} // voicesInfo

const void TTSSAPI::initialize()
{
	CoInitializeEx(NULL, COINIT_MULTITHREADED);
} // initialize

const void TTSSAPI::say(const QString &pVoice, const QString &pText)
{
    CComPtr<IEnumSpObjectTokens> ccpVoices;
    ULONG ulCount;

    SpEnumTokens(SPCAT_VOICES, NULL, NULL, &ccpVoices);
    ccpVoices->GetCount(&ulCount);

    for(ULONG ulI = 0; ulI < ulCount; ulI++) {
        CComPtr<ISpObjectToken> ccpVoiceInfo;
        WCHAR *wcId;

        ccpVoices->Next(1, &ccpVoiceInfo, NULL);
        ccpVoiceInfo->GetId(&wcId);

        if (pVoice == QString::fromWCharArray(wcId)) {
            CComPtr<ISpVoice> ccpVoice;
            ccpVoice.CoCreateInstance(CLSID_SpVoice);
            ccpVoice->SetVoice(ccpVoiceInfo);
            ccpVoice->Speak(reinterpret_cast<LPCWSTR>(pText.unicode()), SPF_IS_NOT_XML, NULL);
            //ccpVoiceInfo->Speak();
        } // if
    } // for
} // say

const void TTSSAPI::uninitialize()
{
	CoUninitialize();
} // uninitialize