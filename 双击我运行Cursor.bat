@echo off
setlocal enabledelayedexpansion

:: ============================================================
:: Initialize directories
:: ============================================================
set "PROJECT_DIR=%~dp0"
set "PROJECT_DIR=%PROJECT_DIR:~0,-1%"
pushd "%PROJECT_DIR%\..\.."
set "ROOT_DIR=%CD%"
popd

echo ========================================
echo MS1 Project Setup Script
echo ========================================
echo Root: %ROOT_DIR%
echo Project: %PROJECT_DIR%
echo.

:: ============================================================
:: Setup portable environment
:: ============================================================
if not exist "%ROOT_DIR%\Software\data\Local\Programs\cursor" mkdir "%PROJECT_DIR%\Software\data\Local\Programs\cursor"
if not exist "%ROOT_DIR%\Software\data\Roaming\Cursor" mkdir "%PROJECT_DIR%\Software\data\Roaming\Cursor"
if not exist "%ROOT_DIR%\Software\data\extensions" mkdir "%PROJECT_DIR%\Software\data\extensions"

set "APPDATA=%ROOT_DIR%\Software\data\Roaming"
set "LOCALAPPDATA=%ROOT_DIR%\Software\data\Local"

:: ============================================================
:: [1/4] Generate .clangd configuration
:: ============================================================
echo [1/4] Generating .clangd...

:: Convert path for clangd: E:\Projects/ms1 format
set "PROJECT_DIR_CLANGD=%PROJECT_DIR:\=/%"
set "PROJECT_DIR_CLANGD=%PROJECT_DIR_CLANGD::/=:\%"

cd /d "%PROJECT_DIR%" && del .clangd 2>nul
(
  echo CompileFlags:
  echo   Add:
  echo     - "--target=riscv32-unknown-elf"
  echo     - "-march=rv32imac"
  echo     - "-mabi=ilp32"
  echo     - "-mcpu=generic-rv32"
  echo     - "-Wno-unknown-cpu"
  echo     - "-Wno-unused-command-line-argument"
  echo.
  echo     - "-I%PROJECT_DIR_CLANGD%/toolchain/gcc/lib/gcc/riscv64-unknown-elf/14.2.1/include"
  echo     - "-I%PROJECT_DIR_CLANGD%/toolchain/gcc/lib/gcc/riscv64-unknown-elf/14.2.1/include-fixed"
  echo     - "-I%PROJECT_DIR_CLANGD%/toolchain/gcc/riscv64-unknown-elf/include"
  echo     - "-I%PROJECT_DIR_CLANGD%/toolchain/gcc/riscv64-unknown-elf/include/newlib-nano"
  echo     - "-I%PROJECT_DIR_CLANGD%/toolchain/gcc/riscv64-unknown-elf/include/c++/14.2.1"
  echo     - "-I%PROJECT_DIR_CLANGD%/toolchain/gcc/riscv64-unknown-elf/include/c++/14.2.1/backward"
  echo     - "-I%PROJECT_DIR_CLANGD%/toolchain/gcc/riscv64-unknown-elf/include/c++/14.2.1/riscv64-unknown-elf/rv32imac_zaamo_zalrsc/ilp32"
  echo     - "-I%PROJECT_DIR_CLANGD%/application"
  echo     - "-I%PROJECT_DIR_CLANGD%/nmsis/core/include"
  echo     - "-I%PROJECT_DIR_CLANGD%/soc/ms1/include"
  echo     - "-I%PROJECT_DIR_CLANGD%/kernel/freertos/include"
  echo     - "-I%PROJECT_DIR_CLANGD%/kernel/freertos/portable"
  echo.
  echo   Remove:
  echo     - "-mtune=nuclei-200-series"
  echo     - "--specs=nano.specs"
  echo.
  echo Index:
  echo   Background: Skip
) > .clangd
echo Done
echo.

:: ============================================================
:: [2/4] Configure VSCode settings
:: ============================================================
echo [2/4] Updating .vscode\settings.json...
if not exist "%PROJECT_DIR%\.vscode" mkdir "%PROJECT_DIR%\.vscode"
(
  echo {
  echo   "python.defaultInterpreterPath": "${workspaceFolder}/../../Software/python312/python.exe",
  echo   "files.associations": {
  echo     "*.py": "python"
  echo   },
  echo   "python.languageServer": "None"
  echo }
) > "%PROJECT_DIR%\.vscode\settings.json"
echo Done
echo.

:: ============================================================
:: [3/4] Configure Cursor settings
:: ============================================================
echo [3/4] Updating Cursor User settings.json...
if not exist "%ROOT_DIR%\Software\data\Roaming\Cursor\User" mkdir "%ROOT_DIR%\Software\data\Roaming\Cursor\User"
(
  echo {
  echo   "workbench.colorTheme": "Default Dark Modern",
  echo   "git.path": "%ROOT_DIR%\Software\\PortableGit\\bin\\git.exe",
  echo   "clangd.path": "%ROOT_DIR%\Software\\data\\Roaming\\Cursor\\User\\globalStorage\\llvm-vs-code-extensions.vscode-clangd\\install\\21.1.0\\clangd_21.1.0\\bin\\clangd.exe"
  echo }
) > "%ROOT_DIR%\Software\data\Roaming\Cursor\User\settings.json"
echo Done
echo.

:: 4. Configure Git user
echo [4/4] Configuring Git user...
"%ROOT_DIR%\Software\PortableGit\bin\git.exe" config --global user.name "Amlogic_Public"
"%ROOT_DIR%\Software\PortableGit\bin\git.exe" config --global user.email "Amlogic_Public@protonmail.com"
echo Done
echo.

echo ========================================
echo Setup completed!
echo ========================================
echo.
echo Starting Cursor...
start /min "" cmd /c ""%ROOT_DIR%\Software\cursor\Cursor.exe" "%PROJECT_DIR%" --extensions-dir="%ROOT_DIR%\Software\data\extensions""
timeout /t 10 /nobreak >nul
exit /b
