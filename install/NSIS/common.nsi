!include "MUI2.nsh"

; constants
!define PRODUCT_NAME "Vocabulary Master"
!define REG_SOFTWARE_PRODUCT "Software\Isshou\Vocabulary Master"

; general settings
InstallDir "$LOCALAPPDATA\Vocabulary Master"
;InstallDirRegKey HKCU ${REG_SOFTWARE_PRODUCT} ""
Name "${PRODUCT_NAME}"
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