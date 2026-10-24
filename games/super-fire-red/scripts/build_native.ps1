param(
    [ValidateSet('linux', 'windows')]
    [string]$Platform = 'windows'
)
$ErrorActionPreference = 'Stop'
$script = Join-Path $PSScriptRoot 'build_native.py'
$linuxPath = & wsl -d Ubuntu -u root -- wslpath -a -u $script
if ($LASTEXITCODE -ne 0) { throw 'Nao foi possivel converter o caminho para WSL.' }
& wsl -d Ubuntu -u root -- python3 $linuxPath.Trim() --platform $Platform
exit $LASTEXITCODE
