@echo off
setlocal enabledelayedexpansion
cd /d %_NTBINDIR%\private
echo xdbgsym.cmd - Debug Symbol Placer
echo.
echo This script will copy all symbol files found in "%_NTBINDIR%\private" to %_NT386TREE%\Symbols.pri
echo The structure will be recursive folder layout of the source tree.
echo.
pause
rem Create folder if needed, set the source and destination for the symbols
if not exist "%_NT386TREE%\Symbols.pri" mkdir %_NT386TREE%\Symbols.pri
set "SOURCE_DIR=%_NTBINDIR%\private"
set "DEST_DIR=%_NT386TREE%\Symbols.pri"
REM This can be whatever file type or name
set FILENAMES_TO_COPY=*.pdb
rem recursive look through the directories to find *.pdb files
for /R "%SOURCE_DIR%" %%F IN (%FILENAMES_TO_COPY%) do (
    if exist "%%F" (
        set FILE_DIR=%%~dpF
        set FILE_INTERMEDIATE_DIR=!FILE_DIR:%SOURCE_DIR%=!
        xcopy /E /I /Y "%%F" "%DEST_DIR%!FILE_INTERMEDIATE_DIR!"
    )
)
exit /b