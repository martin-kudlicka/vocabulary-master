; language files
SetOutPath "$INSTDIR\lang"
file ..\source\lang\cs_CZ.qm 
; Qt files
SetOutPath "$INSTDIR"
file \Programy\Qt\4.7.3\bin\QtCore4.dll
file \Programy\Qt\4.7.3\bin\QtGui4.dll
file \Programy\Qt\4.7.3\bin\QtSql4.dll
SetOutPath "$INSTDIR\sqldrivers"
file \Programy\Qt\4.7.3\plugins\sqldrivers\qsqlite4.dll