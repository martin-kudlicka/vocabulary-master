#include "tts-sapi.h"

#include <Windows.h>
#include <atlcomcli.h>
#include <sapi.h>
#include <sphelper.h>

const TTSInterface::eTTSPlugin TTSSAPI::GetPluginId() const
{
    return TTSInterface::TTSPluginSAPI;
} // GetPluginId

const QString TTSSAPI::GetPluginName() const
{
    return "SAPI";
} // Initialize

const TTSInterface::tVoiceInfoList TTSSAPI::GetVoicesInfo() const
{
	CComPtr<IEnumSpObjectTokens> ccpVoices;
    TTSInterface::tVoiceInfoList tvilVoices;
	ULONG ulCount;

	SpEnumTokens(SPCAT_VOICES, NULL, NULL, &ccpVoices);
	ccpVoices->GetCount(&ulCount);

	for(ULONG ulI = 0; ulI < ulCount; ulI++) {
		CComPtr<ISpObjectToken> ccpVoiceInfo;
		WCHAR *wcDescription, *wcId;

		ccpVoices->Next(1, &ccpVoiceInfo, NULL);
		ccpVoiceInfo->GetId(&wcId);
		SpGetDescription(ccpVoiceInfo, &wcDescription);

        TTSInterface::sVoiceInfo sviVoice;
        sviVoice.qsId = QString::fromWCharArray(wcId);
        sviVoice.qsDescription = QString::fromWCharArray(wcDescription);
		tvilVoices.append(sviVoice);
	} // for

	return tvilVoices;
} // GetVoicesInfo

const void TTSSAPI::Initialize()
{
	CoInitializeEx(NULL, COINIT_MULTITHREADED);
} // Initialize

const void TTSSAPI::Say(const QString &pVoice, const QString &pText)
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
} // Say

const void TTSSAPI::Uninitialize()
{
	CoUninitialize();
} // Uninitialize

Q_EXPORT_PLUGIN2(tts-sapi, TTSSAPI)