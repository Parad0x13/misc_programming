set p="%homepath%\init.bat"
echo @echo off > %p%
echo echo Have yourself a wonderful Airfoce day! >> %p%
echo pause >> %p%

reg add "HKCU\Software\Microsoft\Windows\CurrentVersion\Run" /f /v MOTD /t REG_SZ /d "C:%p%"
