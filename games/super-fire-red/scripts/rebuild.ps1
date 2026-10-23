$ErrorActionPreference = 'Stop'
$projectRoot = Split-Path (Split-Path (Split-Path $PSScriptRoot -Parent) -Parent) -Parent
$gameRoot = Join-Path $projectRoot 'games/super-fire-red'
$changedSourceFiles = @(& python (Join-Path $PSScriptRoot 'source_base.py') --changed-files)
if ($LASTEXITCODE -ne 0) { throw 'Falha ao listar fontes alteradas.' }
[IO.File]::WriteAllLines((Join-Path $projectRoot 'games/super-fire-red/output/changed-files.txt'), $changedSourceFiles)
$buildScriptLinux = & wsl -d Ubuntu -u root -- wslpath -a -u (Join-Path $PSScriptRoot 'rebuild-current.sh')
if ($LASTEXITCODE -ne 0) { throw 'Falha ao converter caminho para WSL.' }
& wsl -d Ubuntu -u root -- bash $buildScriptLinux.Trim()
exit $LASTEXITCODE
