; exp-html plugin
SetOutPath "$INSTDIR\plugins\exp-html"
file ..\output\Release\exp-html.dll
SetOutPath "$INSTDIR\plugins\exp-html\lang"
file ..\plugins\exp-html\lang\cs_CZ.qm
; exp-pdf plugin
SetOutPath "$INSTDIR\plugins\exp-pdf"
file ..\output\Release\exp-pdf.dll
SetOutPath "$INSTDIR\plugins\exp-pdf\lang"
file ..\plugins\exp-pdf\lang\cs_CZ.qm
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
file "..\3rdparty\HTS Voice NIT ATR503 M001\*.*"
SetOutPath "$INSTDIR\plugins\tts-openjtalk\open_jtalk_dic_utf_8"
file "..\3rdparty\Open JTalk dictionary UTF-8\*.*"
; tts-sapi plugin
SetOutPath "$INSTDIR\plugins\tts-sapi"
file ..\output\Release\tts-sapi.dll
; Qt files
SetOutPath "$INSTDIR"
file \Programy\Qt\4.7.4\bin\QtTest4.dll
SetOutPath "$INSTDIR\codecs"
file \Programy\Qt\4.7.4\plugins\codecs\qcncodecs4.dll
file \Programy\Qt\4.7.4\plugins\codecs\qjpcodecs4.dll
file \Programy\Qt\4.7.4\plugins\codecs\qkrcodecs4.dll
file \Programy\Qt\4.7.4\plugins\codecs\qtwcodecs4.dll