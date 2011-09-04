; store installation folder
;WriteRegStr HKCU ${REG_SOFTWARE_PRODUCT} "" $INSTDIR

; create uninstaller
WriteUninstaller "$INSTDIR\Uninstall.exe"

; shortcuts
CreateDirectory "$SMPROGRAMS\${PRODUCT_NAME}"
CreateShortCut "$SMPROGRAMS\${PRODUCT_NAME}\Uninstall.lnk" "$INSTDIR\Uninstall.exe" "" "$INSTDIR\Uninstall.exe" 0
CreateShortCut "$SMPROGRAMS\${PRODUCT_NAME}\${PRODUCT_NAME}.lnk" "$INSTDIR\Vocabulary Master.exe" "" "$INSTDIR\Vocabulary Master.exe" 0