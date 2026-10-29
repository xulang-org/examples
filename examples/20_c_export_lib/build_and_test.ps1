<# Build and execute the Windows x64 C, Python and C# interop examples. #>
param(
    [string]$Compiler = "xu",
    [string]$Cc = "x86_64-w64-mingw32-gcc",
    [string]$Ar = "x86_64-w64-mingw32-ar",
    [string]$Python = "python",
    [string]$Csc = "",
    [string]$BuildDirectory = (Join-Path $PSScriptRoot "..\..\build\c_export_lib\windows-x64")
)
$ErrorActionPreference = "Stop"
function Invoke-Checked {
    param([string]$Program, [string[]]$Arguments)
    & $Program @Arguments
    if ($LASTEXITCODE -ne 0) { throw "$Program failed with exit $LASTEXITCODE" }
}
if (-not (Get-Command $Compiler -ErrorAction SilentlyContinue)) {
    $candidate = Join-Path $PSScriptRoot "..\..\..\xu\bin\xu.exe"
    if (-not (Test-Path -LiteralPath $candidate)) { throw "Set -Compiler to the product xu.exe" }
    $Compiler = (Resolve-Path -LiteralPath $candidate).Path
}
$Compiler = (Get-Command $Compiler -ErrorAction Stop).Source
$Cc = (Get-Command $Cc -ErrorAction Stop).Source
$Ar = (Get-Command $Ar -ErrorAction Stop).Source
if (-not $env:XU_STDLIB_ROOT) {
    $stdlib = Join-Path $PSScriptRoot "..\..\..\xu\src"
    if (Test-Path -LiteralPath $stdlib) { $env:XU_STDLIB_ROOT = (Resolve-Path -LiteralPath $stdlib).Path }
}
New-Item -ItemType Directory -Force -Path $BuildDirectory | Out-Null
$BuildDirectory = (Resolve-Path -LiteralPath $BuildDirectory).Path
Push-Location -LiteralPath $BuildDirectory
try {
    Invoke-Checked $Compiler -Arguments @("run", (Join-Path $PSScriptRoot "main.xu"))
    # The example exports regular Xu function labels. O0 keeps those labels
    # available instead of inlining the scalar functions into the self-test.
    Invoke-Checked $Compiler -Arguments @("build", "-O0", "--target", "windows-x64", "--emit-obj", (Join-Path $PSScriptRoot "main.xu"), "-o", "libxulib_win.xuobj")
    Invoke-Checked $Compiler -Arguments @("_coff_o", "libxulib_win.xuobj", "libxulib_win.obj")
    Invoke-Checked $Cc -Arguments @("-c", (Join-Path $PSScriptRoot "win_bridge.s"), "-o", "win_bridge.obj")
    Invoke-Checked $Cc -Arguments @("-c", (Join-Path $PSScriptRoot "windows_headless.c"), "-o", "windows_headless.obj")
    Invoke-Checked $Ar -Arguments @("rcs", "xulib.lib", "libxulib_win.obj", "win_bridge.obj", "windows_headless.obj")
    Invoke-Checked $Cc -Arguments @("-shared", "libxulib_win.obj", "win_bridge.obj", "windows_headless.obj", (Join-Path $PSScriptRoot "xulib.def"), "-Wl,--out-implib,libxulib_dll.a", "-o", "xulib.dll")
    Invoke-Checked $Cc -Arguments @((Join-Path $PSScriptRoot "test_windows.c"), "libxulib_dll.a", "-o", "test_windows_dynamic.exe")
    Invoke-Checked (Join-Path $BuildDirectory "test_windows_dynamic.exe") -Arguments @()
    Invoke-Checked $Cc -Arguments @((Join-Path $PSScriptRoot "test_windows.c"), "xulib.lib", "-o", "test_windows_static.exe")
    Invoke-Checked (Join-Path $BuildDirectory "test_windows_static.exe") -Arguments @()
    $pythonCommand = Get-Command $Python -ErrorAction SilentlyContinue
    if ($pythonCommand -and $pythonCommand.Source -notlike "*WindowsApps*") {
        Copy-Item -LiteralPath (Join-Path $PSScriptRoot "test_python.py") -Destination . -Force
        Invoke-Checked $pythonCommand.Source -Arguments @((Join-Path $BuildDirectory "test_python.py"))
    } else { Write-Host "SKIP Python: pass -Python with an x64 interpreter" }
    if (-not $Csc) {
        $cscCommand = Get-Command "csc" -ErrorAction SilentlyContinue
        if ($cscCommand) { $Csc = $cscCommand.Source }
        else { $Csc = Join-Path $env:WINDIR "Microsoft.NET\Framework64\v4.0.30319\csc.exe" }
    }
    if (Test-Path -LiteralPath $Csc) {
        Invoke-Checked $Csc -Arguments @("/nologo", "/platform:x64", "/out:$BuildDirectory\test_csharp.exe", (Join-Path $PSScriptRoot "test_csharp.cs"))
        Invoke-Checked (Join-Path $BuildDirectory "test_csharp.exe") -Arguments @()
    } else { Write-Host "SKIP C#: pass -Csc with an x64-compatible C# compiler" }
    Write-Host "Windows x64 interop checks completed; any optional skips are listed above."
} finally { Pop-Location }
