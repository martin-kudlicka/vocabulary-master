!include "NSIS\common.nsi"

; general settings
OutFile "..\output\Vocabulary Master FULL Setup.exe"
Name "${PRODUCT_NAME} FULL"

; installer sections
Section "Install"
	; main files
	SetOutPath "$INSTDIR"
	file "..\output\Release\Vocabulary Master.exe"
	file "..\output\Release\rsa.dll"

	!include "NSIS\filescommon.nsi"
	!include "NSIS\filesfull.nsi"

	!include "NSIS\installpostcommon.nsi"
SectionEnd

!include "NSIS\commonfunctions.nsi"