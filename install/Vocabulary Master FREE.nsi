!include "MUI2.nsh"

; constants
!define PRODUCT_NAME "Vocabulary Master"
!define REG_SOFTWARE_PRODUCT "Software\Isshou\Vocabulary Master"

; general settings
InstallDir "$LOCALAPPDATA\Vocabulary Master"
;InstallDirRegKey HKCU ${REG_SOFTWARE_PRODUCT} ""
Name "${PRODUCT_NAME}"
OutFile "..\output\Vocabulary Master FREE Setup.exe"
RequestExecutionLevel user

; remember the installer language
!define MUI_LANGDLL_REGISTRY_ROOT "HKCU" 
!define MUI_LANGDLL_REGISTRY_KEY "${REG_SOFTWARE_PRODUCT}"
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
	CreateDirectory "$INSTDIR\sqldrivers"
	file /oname=sqldrivers\qsqlite4.dll \Programy\Qt\4.7.3\plugins\sqldrivers\qsqlite4.dll

	; store installation folder
	;WriteRegStr HKCU ${REG_SOFTWARE_PRODUCT} "" $INSTDIR

	; create uninstaller
	WriteUninstaller "$INSTDIR\Uninstall.exe"

	; shortcuts
	CreateDirectory "$SMPROGRAMS\${PRODUCT_NAME}"
	CreateShortCut "$SMPROGRAMS\${PRODUCT_NAME}\Uninstall.lnk" "$INSTDIR\Uninstall.exe" "" "$INSTDIR\Uninstall.exe" 0
	CreateShortCut "$SMPROGRAMS\${PRODUCT_NAME}\${PRODUCT_NAME}.lnk" "$INSTDIR\Vocabulary Master.exe" "" "$INSTDIR\Vocabulary Master.exe" 0
SectionEnd

; installer Functions
Function .onInit
	!insertmacro MUI_LANGDLL_DISPLAY
FunctionEnd

; uninstaller section
Section "Uninstall"
	RMDir /r "$SMPROGRAMS\${PRODUCT_NAME}"
	RMDir /r "$INSTDIR"

	DeleteRegKey HKCU "${REG_SOFTWARE_PRODUCT}"
SectionEnd

; uninstaller Functions
Function un.onInit
	!insertmacro MUI_UNGETLANGUAGE
FunctionEnd