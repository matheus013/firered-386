param(
    [ValidateSet('baseline', 'hack')]
    [string]$Mode = 'hack'
)
$ErrorActionPreference = 'Stop'
$buildScriptWindows = Join-Path $PSScriptRoot 'build.sh'
$buildScriptLinux = & wsl -d Ubuntu -u root -- wslpath -a -u $buildScriptWindows
if ($LASTEXITCODE -ne 0) { throw 'Nao foi possivel converter o caminho para WSL.' }
& wsl -d Ubuntu -u root -- bash $buildScriptLinux.Trim() $Mode
exit $LASTEXITCODE
