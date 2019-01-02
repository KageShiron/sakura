@echo off
set DSTZIP=%1
set SRCDIR=%2

if exist "%DSTZIP%" del "%DSTZIP%"

call %~dp0find-7z.bat

@echo archiving %SRCZIP%.
@rem CMD_7Z contains double quotes.
if "%CMD_7Z%" == "" (
	@echo -------------------------------------------------------
	@echo ---- you can make this faster by installing 7-zip. ----
	@echo -------------------------------------------------------
	powershell -ExecutionPolicy RemoteSigned -File %~dp0zip.ps1 %DSTZIP% %SRCDIR%
) else (
	C:\Windows\System32\tar.exe --version
	echo C:\Windows\System32\tar.exe -a -cf "%DSTZIP%" "%SRCDIR%"
	C:\Windows\System32\tar.exe -a -cf "%DSTZIP%" "%SRCDIR%"
)
@echo on
