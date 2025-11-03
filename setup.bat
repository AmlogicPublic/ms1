@echo off
setlocal enabledelayedexpansion

echo ========================================
echo MS1 Project Setup Script
echo ========================================
echo.

:: Get current drive and root directory
set "SCRIPT_DIR=%~dp0"
set "ROOT_DIR=%SCRIPT_DIR%..\..\"
pushd "%ROOT_DIR%"
set "ROOT_DIR=%CD%"
popd

echo Current root directory: %ROOT_DIR%
echo.

:: 1. Generate .clangd
echo [1/4] Generating .clangd with absolute paths...
cd /d "%SCRIPT_DIR%"
del .clangd 2>nul
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
echo     - "-I%ROOT_DIR%/Projects/ms1/toolchain/gcc/lib/gcc/riscv64-unknown-elf/14.2.1/include"
echo     - "-I%ROOT_DIR%/Projects/ms1/toolchain/gcc/lib/gcc/riscv64-unknown-elf/14.2.1/include-fixed"
echo     - "-I%ROOT_DIR%/Projects/ms1/toolchain/gcc/riscv64-unknown-elf/include"
echo     - "-I%ROOT_DIR%/Projects/ms1/toolchain/gcc/riscv64-unknown-elf/include/newlib-nano"
echo     - "-I%ROOT_DIR%/Projects/ms1/toolchain/gcc/riscv64-unknown-elf/include/c++/14.2.1"
echo     - "-I%ROOT_DIR%/Projects/ms1/toolchain/gcc/riscv64-unknown-elf/include/c++/14.2.1/backward"
echo     - "-I%ROOT_DIR%/Projects/ms1/toolchain/gcc/riscv64-unknown-elf/include/c++/14.2.1/riscv64-unknown-elf/rv32imac_zaamo_zalrsc/ilp32"
echo     - "-I%ROOT_DIR%/Projects/ms1/application"
echo     - "-I%ROOT_DIR%/Projects/ms1/nmsis/core/include"
echo     - "-I%ROOT_DIR%/Projects/ms1/soc/ms1/include"
echo     - "-I%ROOT_DIR%/Projects/ms1/kernel/freertos/include"
echo     - "-I%ROOT_DIR%/Projects/ms1/kernel/freertos/portable"
echo.
echo   Remove:
echo     - "-mtune=nuclei-200-series"
echo     - "--specs=nano.specs"
echo.
echo Index:
echo   Background: Skip
) > .clangd
echo Done: .clangd generated
echo.

:: 2. Update .vscode\settings.json - clangd.path
echo [2/4] Updating .vscode\settings.json with clangd.path...
set "VSCODE_DIR=%SCRIPT_DIR%.vscode"
if not exist "%VSCODE_DIR%" mkdir "%VSCODE_DIR%"
set "VSCODE_SETTINGS=%VSCODE_DIR%\settings.json"
set "CLANGD_PATH=%ROOT_DIR%\Software\data\Roaming\Cursor\User\globalStorage\llvm-vs-code-extensions.vscode-clangd\install\21.1.0\clangd_21.1.0\bin\clangd.exe"

powershell -Command "$ErrorActionPreference='Stop'; $path='%CLANGD_PATH%'; if (Test-Path '%VSCODE_SETTINGS%') { $content = Get-Content '%VSCODE_SETTINGS%' -Raw; if ($content.Trim()) { $json = $content | ConvertFrom-Json } else { $json = @{} } } else { $json = @{} }; $json | Add-Member -Force -MemberType NoteProperty -Name 'clangd.path' -Value $path; ($json | ConvertTo-Json -Depth 10).Replace('\\\\','\\') | Set-Content '%VSCODE_SETTINGS%' -Encoding UTF8"
echo Done: clangd.path updated
echo.

:: 3. Update Cursor User settings.json - git.path
echo [3/4] Updating Cursor User settings.json with git.path...
set "CURSOR_SETTINGS=%ROOT_DIR%\Software\data\Roaming\Cursor\User\settings.json"
set "GIT_PATH=%ROOT_DIR%\Software\PortableGit\bin\git.exe"

powershell -Command "$ErrorActionPreference='Stop'; $path='%GIT_PATH%'; if (Test-Path '%CURSOR_SETTINGS%') { $content = Get-Content '%CURSOR_SETTINGS%' -Raw; if ($content.Trim()) { $json = $content | ConvertFrom-Json } else { $json = @{} } } else { $json = @{} }; $json | Add-Member -Force -MemberType NoteProperty -Name 'git.path' -Value $path; ($json | ConvertTo-Json -Depth 10).Replace('\\\\','\\') | Set-Content '%CURSOR_SETTINGS%' -Encoding UTF8"
echo Done: git.path updated
echo.

:: 4. Configure Git user settings
echo [4/4] Configuring Git user settings...
set "GIT_EXE=%ROOT_DIR%\Software\PortableGit\bin\git.exe"
"%GIT_EXE%" config --global user.name "Amlogic_Public"
"%GIT_EXE%" config --global user.email "Amlogic_Public@protonmail.com"
echo Done: Git user.name = Amlogic_Public
echo Done: Git user.email = Amlogic_Public@protonmail.com
echo.

echo ========================================
echo Setup completed successfully!
echo ========================================
endlocal
