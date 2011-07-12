#include "tts-sapi.h"

#include <QtCore/QStringList>
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

const QStringList TTSSAPI::GetVoicesIds() const
{
	CComPtr<IEnumSpObjectTokens> ccpVoices;
	QStringList qslVoiceIds;
	ULONG ulCount;

	SpEnumTokens(SPCAT_VOICES, NULL, NULL, &ccpVoices);
	ccpVoices->GetCount(&ulCount);

	for(ULONG ulI = 0; ulI < ulCount; ulI++) {
		CComPtr<ISpObjectToken> ccpVoice;
		WCHAR *wcId;

		ccpVoices->Next(1, &ccpVoice, NULL);
		ccpVoice->GetId(&wcId);
		//SpGetDescription(ccpVoice, &wcDescription);

		qslVoiceIds.append(QString::fromWCharArray(wcId));
	} // for

	return qslVoiceIds;
} // GetVoicesIds

const void TTSSAPI::Initialize()
{
	CoInitializeEx(NULL, COINIT_MULTITHREADED);
} // Initialize

const void TTSSAPI::Say(const QString &pVoice, const QString &pText)
{

} // Say

const void TTSSAPI::Uninitialize()
{
	CoUninitialize();
} // Uninitialize

Q_EXPORT_PLUGIN2(tts-sapi, TTSSAPI)