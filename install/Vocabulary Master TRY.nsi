!include "NSIS\common.nsi"

; general settings
OutFile "..\output\Vocabulary Master TRY Setup.exe"

; installer sections
Section "Install"
	; main files
	SetOutPath "$INSTDIR"
	file "..\output\ReleaseTRY\Vocabulary Master.exe"

	!include "NSIS\filescommon.nsi"
	!include "NSIS\filesfull.nsi"

	!include "NSIS\installpostcommon.nsi"
SectionEnd

!include "NSIS\commonfunctions.nsi"