!include "MUI2.nsh"

; general settings
InstallDir "$LOCALAPPDATA\Vocabulary Master"
;InstallDirRegKey HKCU "Software\Isshou\Vocabulary Master" ""
Name "Vocabulary Master"
OutFile "..\output\Vocabulary Master FREE Setup.exe"
RequestExecutionLevel user

; remember the installer language
!define MUI_LANGDLL_REGISTRY_ROOT "HKCU" 
!define MUI_LANGDLL_REGISTRY_KEY "Software\Isshou\Vocabulary Master" 
!define MUI_LANGDLL_REGISTRY_VALUENAME "Installer Language"

; variables
Var StartMenuFolder

; installer pages
!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_STARTMENU "Vocabulary Master" $StartMenuFolder
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH

; uninstaller pages
!insertmacro MUI_UNPAGE_WELCOME
!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES
!insertmacro MUI_UNPAGE_FINISH

; languages
!insertmacro MUI_LANGUAGE "English"
!insertmacro MUI_LANGUAGE "Czech"

; reserve files
!insertmacro MUI_RESERVEFILE_LANGDLL

; installer sections
Section "Install"
	SetOutPath "$INSTDIR"

	; main files
	file "..\output\Release FREE\Vocabulary Master.exe"
	; language files
	CreateDirectory "$INSTDIR\lang"
	file /oname=lang\cs_CZ.qm ..\source\lang\cs_CZ.qm 
	; Qt files
	file \Programy\Qt\4.7.3\bin\QtCore4.dll
	file \Programy\Qt\4.7.3\bin\QtGui4.dll
	file \Programy\Qt\4.7.3\bin\QtSql4.dll
	file \Programy\Qt\4.7.3\bin\QtTest4.dll
	CreateDirectory "$INSTDIR\sqldrivers"
	file /oname=sqldrivers\qsqlite4.dll \Programy\Qt\4.7.3\plugins\sqldrivers\qsqlite4.dll

	; store installation folder
	;WriteRegStr HKCU "Software\Isshou\Vocabulary Master" "" $INSTDIR

	; create uninstaller
	WriteUninstaller "$INSTDIR\Uninstall.exe"
SectionEnd

; installer Functions
Function .onInit
	!insertmacro MUI_LANGDLL_DISPLAY
FunctionEnd

; uninstaller section
Section "Uninstall"
	RMDir /r "$INSTDIR"

	DeleteRegKey HKCU "Software\Isshou\Vocabulary Master"
SectionEnd

; uninstaller Functions
Function un.onInit
	!insertmacro MUI_UNGETLANGUAGE
FunctionEnd