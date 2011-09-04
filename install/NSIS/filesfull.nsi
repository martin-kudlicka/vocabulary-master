; exp-html plugin
SetOutPath "$INSTDIR\plugins\tts-exp-html"
file ..\output\Release\exp-html.dll
SetOutPath "$INSTDIR\plugins\tts-exp-html\lang"
file ..\plugins\exp-html\lang\cs_CZ.qm
; exp-plaintext plugin
SetOutPath "$INSTDIR\plugins\exp-plaintext"
file ..\output\Release\exp-plaintext.dll
SetOutPath "$INSTDIR\plugins\exp-plaintext\lang"
file ..\plugins\exp-plaintext\lang\cs_CZ.qm
; imp-anki plugin
SetOutPath "$INSTDIR\plugins\imp-anki"
file ..\output\Release\imp-anki.dll
SetOutPath "$INSTDIR\plugins\imp-anki\lang"
file ..\plugins\imp-anki\lang\cs_CZ.qm
; imp-plaintext plugin
SetOutPath "$INSTDIR\plugins\imp-plaintext"
file ..\output\Release\imp-plaintext.dll
SetOutPath "$INSTDIR\plugins\imp-plaintext\lang"
file ..\plugins\imp-plaintext\lang\cs_CZ.qm
; tts-openjtalk plugin
SetOutPath "$INSTDIR\plugins\tts-openjtalk"
file ..\output\Release\tts-openjtalk.dll
SetOutPath "$INSTDIR\plugins\tts-openjtalk\hts_voice_nitech_jp_atr503_m001"
file /r "..\3rdparty\HTS Voice NIT ATR503 M001\*.*"
SetOutPath "$INSTDIR\plugins\tts-openjtalk\open_jtalk_dic_utf_8"
file /r "..\3rdparty\Open JTalk dictionary UTF-8\*.*"
; tts-sapi plugin
SetOutPath "$INSTDIR\plugins\tts-sapi"
file ..\output\Release\tts-sapi.dll
; Qt files
SetOutPath "$INSTDIR"
file \Programy\Qt\4.7.3\bin\QtTest4.dll