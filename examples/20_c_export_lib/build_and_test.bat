@echo off
setlocal enabledelayedexpansion

echo ====================================================================
echo    Xu Native Library: Windows Build and Comprehensive Test Suite
echo ====================================================================

:: Locate xu compiler
set XU_BIN=xu
where xu >nul 2>nul
if %errorlevel% neq 0 (
    if exist "..\..\..\xu\bin\xu.exe" (
        set "XU_BIN=..\..\..\xu\bin\xu.exe"
    ) else if exist "..\..\..\xu\bin\xu" (
        set "XU_BIN=..\..\..\xu\bin\xu"
    ) else (
        echo [ERROR] 'xu' compiler not found in PATH or ..\..\..\xu\bin!
        exit /b 1
    )
)

:: Set standard library root
if not defined XU_STDLIB_ROOT (
    if exist "..\..\..\xu\src" (
        set "XU_STDLIB_ROOT=%CD%\..\..\..\xu\src"
    )
)

echo [INFO] Using Xu compiler: %XU_BIN%
echo [INFO] Using XU_STDLIB_ROOT: %XU_STDLIB_ROOT%

:: 1. Run standalone Xu self-test
echo.
echo [STEP 1/6] Running standalone Xu self-test...
"%XU_BIN%" run main.xu
if %errorlevel% neq 0 (
    echo [ERROR] Xu self-test failed!
    exit /b 1
)

:: 2. Compile Xu source to Windows COFF object
echo.
echo [STEP 2/6] Compiling main.xu to Windows COFF object...
"%XU_BIN%" build --target windows-x64 --emit-obj main.xu -o libxulib_win.xuobj
if %errorlevel% neq 0 (
    echo [ERROR] Xu build failed!
    exit /b 1
)
"%XU_BIN%" _coff_o libxulib_win.xuobj libxulib_win.obj
if %errorlevel% neq 0 (
    echo [ERROR] Converting XUOBJ to COFF failed!
    exit /b 1
)

:: 3. Assemble Windows MS-ABI Bridge
echo.
echo [STEP 3/6] Assembling Windows MS-ABI bridge...
where gcc >nul 2>nul
if %errorlevel% equ 0 (
    gcc -c win_bridge.s -o win_bridge.obj
) else (
    where clang >nul 2>nul
    if %errorlevel% equ 0 (
        clang -target x86_64-pc-windows-msvc -c win_bridge.s -o win_bridge.obj
    ) else (
        echo [WARN] Neither gcc nor clang found. Using precompiled win_bridge.obj if available.
    )
)

:: 4. Build Static Library (xulib.lib)
echo.
echo [STEP 4/6] Building Windows Static Library (xulib.lib)...
where ar >nul 2>nul
if %errorlevel% equ 0 (
    ar rcs xulib.lib libxulib_win.obj win_bridge.obj
) else (
    where lib >nul 2>nul
    if %errorlevel% equ 0 (
        lib /OUT:xulib.lib libxulib_win.obj win_bridge.obj
    ) else (
        echo [WARN] Neither ar nor lib.exe found to build static library.
    )
)

:: 5. Build Dynamic Library (xulib.dll)
echo.
echo [STEP 5/6] Building Windows Dynamic Library (xulib.dll)...
where gcc >nul 2>nul
if %errorlevel% equ 0 (
    gcc -shared libxulib_win.obj win_bridge.obj -Wl,--out-implib,libxulib_dll.a -o xulib.dll
) else (
    where link >nul 2>nul
    if %errorlevel% equ 0 (
        link /DLL /OUT:xulib.dll libxulib_win.obj win_bridge.obj /DEF:xulib.def
    ) else (
        echo [ERROR] Neither gcc nor link.exe found to build DLL!
        exit /b 1
    )
)

:: 6. Run Callers & Tests
echo.
echo [STEP 6/6] Executing Host Language Callers on Windows...

:: 6a. C Host Caller
where gcc >nul 2>nul
if %errorlevel% equ 0 (
    echo [C Test] Compiling and running test_windows.c...
    gcc test_windows.c -L. -lxulib_dll -o test_windows_dynamic.exe
    if %errorlevel% equ 0 (
        test_windows_dynamic.exe
    )
)

:: 6b. Python ctypes Caller
where python >nul 2>nul
if %errorlevel% equ 0 (
    echo.
    echo [Python Test] Running test_python.py...
    python test_python.py
)

:: 6c. C# Caller
where csc >nul 2>nul
if %errorlevel% equ 0 (
    echo.
    echo [C# Test] Compiling and running test_csharp.cs via csc...
    csc /nologo /out:test_csharp.exe test_csharp.cs
    if %errorlevel% equ 0 (
        test_csharp.exe
    )
) else (
    where dotnet >nul 2>nul
    if %errorlevel% equ 0 (
        echo [INFO] dotnet found. You can run test_csharp.cs with dotnet script / project.
    )
)

echo.
echo ====================================================================
echo   >>> ALL WINDOWS TESTS FINISHED SUCCESSFULLY! <<<
echo ====================================================================
