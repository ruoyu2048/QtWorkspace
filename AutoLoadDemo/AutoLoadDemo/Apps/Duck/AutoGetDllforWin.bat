for /r "%cd%" %%i in (*.exe) do (windeployqt.exe "%%~nxi")
pause