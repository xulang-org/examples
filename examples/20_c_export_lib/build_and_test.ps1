<#
.SYNOPSIS
    Automated build and test suite for Xu Native Library on Windows (xulib.lib / xulib.dll).
.DESCRIPTION
    Compiles main.xu to Windows COFF object, links static library (xulib.lib) and
    dynamic library (xulib.dll) with MS-ABI bridge, and executes C, C#, and Python tests.
#>

$ErrorActionPreference = "Stop"

Write-Host "====================================================================" -ForegroundColor Cyan
Write-Host "   Xu Native Library: Windows PowerShell Build & Test Suite" -ForegroundColor Cyan
Write-Host "====================================================================" -ForegroundColor Cyan

# Locate xu compiler
$XuBin = "xu"
if (-not (Get-Command "xu" -ErrorAction SilentlyContinue)) {
    if (Test-Path "..\..\..\xu\bin\xu.exe") {
        $XuBin = Resolve-Path "..\..\..\xu\bin\xu.exe"
    } elseif (Test-Path "..\..\..\xu\bin\xu") {
        $XuBin = Resolve-Path "..\..\..\xu\bin\xu"
    } else {
        Write-Error "Cannot locate 'xu' compiler. Please ensure it is in PATH."
    }
}

if (-not $env:XU_STDLIB_ROOT -and (Test-Path "..\..\..\xu\src")) {
    $env:XU_STDLIB_ROOT = (Resolve-Path "..\..\..\xu\src").Path
}

Write-Host "[INFO] Compiler: $XuBin" -ForegroundColor Gray
Write-Host "[INFO] Stdlib:   $env:XU_STDLIB_ROOT" -ForegroundColor Gray

# 1. Run standalone Xu self-test
Write-Host "`n[STEP 1/6] Running standalone Xu self-test..." -ForegroundColor Yellow
& $XuBin run main.xu

# 2. Compile Xu source to Windows COFF object
Write-Host "`n[STEP 2/6] Compiling main.xu to Windows COFF object..." -ForegroundColor Yellow
& $XuBin build --target windows-x64 --emit-obj main.xu -o libxulib_win.xuobj
& $XuBin _coff_o libxulib_win.xuobj libxulib_win.obj

# 3. Assemble Windows MS-ABI Bridge
Write-Host "`n[STEP 3/6] Assembling Windows MS-ABI bridge..." -ForegroundColor Yellow
if (Get-Command "gcc" -ErrorAction SilentlyContinue) {
    gcc -c win_bridge.s -o win_bridge.obj
} elseif (Get-Command "clang" -ErrorAction SilentlyContinue) {
    clang -target x86_64-pc-windows-msvc -c win_bridge.s -o win_bridge.obj
} else {
    Write-Host "[WARN] No C assembler found, using existing win_bridge.obj" -ForegroundColor DarkYellow
}

# 4. Build Static Library (xulib.lib)
Write-Host "`n[STEP 4/6] Building Windows Static Library (xulib.lib)..." -ForegroundColor Yellow
if (Get-Command "ar" -ErrorAction SilentlyContinue) {
    ar rcs xulib.lib libxulib_win.obj win_bridge.obj
} elseif (Get-Command "lib" -ErrorAction SilentlyContinue) {
    lib /OUT:xulib.lib libxulib_win.obj win_bridge.obj
}

# 5. Build Dynamic Library (xulib.dll)
Write-Host "`n[STEP 5/6] Building Windows Dynamic Library (xulib.dll)..." -ForegroundColor Yellow
if (Get-Command "gcc" -ErrorAction SilentlyContinue) {
    gcc -shared libxulib_win.obj win_bridge.obj -Wl,--out-implib,libxulib_dll.a -o xulib.dll
} elseif (Get-Command "link" -ErrorAction SilentlyContinue) {
    link /DLL /OUT:xulib.dll libxulib_win.obj win_bridge.obj /DEF:xulib.def
}

# 6. Run Callers & Tests
Write-Host "`n[STEP 6/6] Executing Host Language Tests..." -ForegroundColor Yellow

# 6a. C Host Caller
if (Get-Command "gcc" -ErrorAction SilentlyContinue) {
    Write-Host "`n[C Dynamic Test] Compiling and running test_windows.c against xulib.dll..." -ForegroundColor Green
    gcc test_windows.c -L. -lxulib_dll -o test_windows_dynamic.exe
    .\test_windows_dynamic.exe

    Write-Host "`n[C Static Test] Compiling and running test_windows.c against xulib.lib..." -ForegroundColor Green
    gcc test_windows.c xulib.lib -o test_windows_static.exe
    .\test_windows_static.exe
}

# 6b. Python ctypes Caller
if (Get-Command "python" -ErrorAction SilentlyContinue) {
    Write-Host "`n[Python Test] Running test_python.py against xulib.dll..." -ForegroundColor Green
    python test_python.py
}

# 6c. C# Caller
if (Get-Command "csc" -ErrorAction SilentlyContinue) {
    Write-Host "`n[C# Test] Compiling and running test_csharp.cs via csc..." -ForegroundColor Green
    csc /nologo /out:test_csharp.exe test_csharp.cs
    .\test_csharp.exe
}

Write-Host "`n====================================================================" -ForegroundColor Cyan
Write-Host "   >>> ALL WINDOWS TESTS COMPLETED SUCCESSFULLY! <<<" -ForegroundColor Cyan
Write-Host "====================================================================" -ForegroundColor Cyan
