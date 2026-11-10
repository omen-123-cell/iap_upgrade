@echo off
rem Flash bootloader (0x08000000) + storage-area app image (0x08060000)
rem from the combined Intel HEX produced by make_flash_image.py.
rem
rem NOTE: this erases/programs the sectors covered by the HEX file.
rem If "-c SWD UR" (connect under reset) fails, replace it with "-c SWD".

setlocal
set "CLI=D:\ST-LINK Utility\ST-LINK_CLI.exe"
set "HEX=%~dp0flash_image.hex"

if not exist "%HEX%" (
    echo [ERROR] %HEX% not found.
    echo Run first: python "%~dp0make_flash_image.py"
    pause
    exit /b 1
)

rem For factory initialisation the run area must be blank, otherwise the
rem bootloader sees an old app and never restores from the storage area.
rem Sectors 4..6 = 0x08010000..0x0805FFFF (the run area). Remove "-SE 4 6"
rem if you want to keep whatever is currently in the run area.
"%CLI%" -c SWD UR -SE 4 6 -P "%HEX%" -V after_programming -NoPrompt -Rst

echo.
echo Flash command finished with exit code %ERRORLEVEL%.
pause
