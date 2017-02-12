; mjutil.nsi
;

;--------------------------------

!define QtDllDir "C:\Qt\5.5\mingw492_32\bin"
!define QtReleaseDir "..\..\..\build-mjutil-Desktop_Qt_5_5_1_MinGW_32bit-Release"

; The name of the installer
Name "mjutil installer"

; The file to write
OutFile "mjutil-installer.exe"

; The default installation directory
InstallDir $PROGRAMFILES\Stormgears5422\mjutil

; Registry key to check for directory (so if you install again, it will 
; overwrite the old one automatically)
InstallDirRegKey HKLM "Software\Stormgears5422\mjutil" "Install_Dir"

; Request application privileges for Windows Vista
RequestExecutionLevel admin

;--------------------------------

; Pages

Page components
Page directory
Page instfiles

UninstPage uninstConfirm
UninstPage instfiles

;--------------------------------

; The stuff to install
Section "Mjiutil app"

  SectionIn RO
  
  ; Set output path to the installation directory.
  SetOutPath $INSTDIR
  
  ; Put file there
  ;File "mjutil.nsi"
  File /oname=mjutil.exe ${QtReleaseDir}\app\release\mjutil.exe
  File /oname=mjcommon.dll ${QtReleaseDir}\mjcommon\release\mjcommon.dll
  File /oname=Qt5Core.dll ${QtDllDir}\Qt5Core.dll
  File /oname=Qt5Gui.dll ${QtDllDir}\Qt5Gui.dll
  File /oname=Qt5Network.dll ${QtDllDir}\Qt5Network.dll
  File /oname=Qt5Widgets.dll ${QtDllDir}\Qt5Widgets.dll
  File /oname=libgcc_s_dw2-1.dll ${QtDllDir}\libgcc_s_dw2-1.dll
  File /oname=libstdc++-6.dll ${QtDllDir}\libstdc++-6.dll
  File /oname=libwinpthread-1.dll ${QtDllDir}\libwinpthread-1.dll
  
  
  ; Write the installation path into the registry
  WriteRegStr HKLM SOFTWARE\NSIS_mjiutil "Install_Dir" "$INSTDIR"
  
  ; Write the uninstall keys for Windows
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\mjutil" "DisplayName" "Motion JPEG Utility"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\mjutil" "UninstallString" '"$INSTDIR\uninstall.exe"'
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\mjutil" "NoModify" 1
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\mjutil" "NoRepair" 1
  WriteUninstaller "uninstall.exe"
  
SectionEnd

; Optional section (can be disabled by the user)
Section "Start Menu Shortcuts"

  CreateDirectory "$SMPROGRAMS\mjutil"
  CreateShortCut "$SMPROGRAMS\mjutil\Uninstall.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 0
  CreateShortCut "$SMPROGRAMS\mjutil\mjutil.lnk" "$INSTDIR\mjutil.exe" "" "$INSTDIR\mjutil.exe" 0
  
SectionEnd

;--------------------------------

; Uninstaller

Section "Uninstall"
  
  ; Remove registry keys
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\mjutil"
  DeleteRegKey HKLM SOFTWARE\NSIS_mjutil

  ; Remove files and uninstaller
  ;Delete $INSTDIR\example2.nsi
  Delete $INSTDIR\uninstall.exe
  Delete $INSTDIR\mjutil.exe
  Delete $INSTDIR\*.dll

  ; Remove shortcuts, if any
  Delete "$SMPROGRAMS\mjutil\*.*"

  ; Remove directories used
  RMDir "$SMPROGRAMS\mjutil"
  RMDir "$INSTDIR"

SectionEnd
