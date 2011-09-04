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