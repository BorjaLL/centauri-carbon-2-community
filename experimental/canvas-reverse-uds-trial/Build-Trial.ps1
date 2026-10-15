[CmdletBinding()]
param([string]$Distro='Ubuntu')
$ErrorActionPreference='Stop'
$full=(Resolve-Path -LiteralPath $PSScriptRoot).Path
if ($full.Length -lt 3 -or $full.Substring(1,2) -ne ':\') {throw 'Extract to a local Windows drive.'}
$linuxRoot='/mnt/'+$full.Substring(0,1).ToLowerInvariant()+'/'+$full.Substring(3).Replace('\','/')
& wsl.exe -d $Distro -u root --exec python3 -u "$linuxRoot/build-trial.py"
if ($LASTEXITCODE -ne 0) {throw 'Trial build failed. Paste the first error.'}
