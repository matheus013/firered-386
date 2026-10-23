$ErrorActionPreference = 'Stop'
$projectRoot = Split-Path $PSScriptRoot -Parent
$gameRoot = Join-Path $projectRoot 'game'
$changedSourceFiles = @(& python (Join-Path $PSScriptRoot 'source_base.py') --changed-files)
if ($LASTEXITCODE -ne 0) { throw 'Falha ao listar fontes alteradas.' }
[IO.File]::WriteAllLines((Join-Path $projectRoot 'output/changed-files.txt'), $changedSourceFiles)
$buildScriptLinux = & wsl -d Ubuntu -u root -- wslpath -a -u (Join-Path $PSScriptRoot 'rebuild-current.sh')
if ($LASTEXITCODE -ne 0) { throw 'Falha ao converter caminho para WSL.' }
& wsl -d Ubuntu -u root -- bash $buildScriptLinux.Trim()
exit $LASTEXITCODE
