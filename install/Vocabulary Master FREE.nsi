!include "NSIS\common.nsi"

; general settings
OutFile "..\output\Vocabulary Master FREE Setup.exe"

; installer sections
Section "Install"
	; main files
	SetOutPath "$INSTDIR"
	file "..\output\ReleaseFREE\Vocabulary Master.exe"

	!include "NSIS\filescommon.nsi"

	!include "NSIS\installpostcommon.nsi"
SectionEnd

!include "NSIS\commonfunctions.nsi"