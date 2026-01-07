param(
    [string]$Root = ".",
    [switch]$WhatIf
)

$patterns = @("*.sln", "*.pro.user", "*.vcxproj", "*.vcxproj.user", "*.vcxproj.filters", "*.qmake.stash", "Makefile", "*.Makefile.Debug", "*.Makefile.Release")

$resolvedRoot = Resolve-Path $Root
Write-Host "Scanning '$($resolvedRoot.Path)' for solution and project files..." -ForegroundColor Cyan

$files = Get-ChildItem -Path $resolvedRoot -Recurse -File -Include $patterns -ErrorAction SilentlyContinue

if (-not $files) {
    Write-Host "No matching files found." -ForegroundColor Yellow
    return
}

if ($WhatIf) {
    Write-Host "Dry run (use without -WhatIf to delete):" -ForegroundColor Yellow
    $files | ForEach-Object { Write-Host $_.FullName }
    return
}

$failed = @()
foreach ($file in $files) {
    try {
        Remove-Item -LiteralPath $file.FullName -Force
    } catch {
        $failed += $file.FullName
    }
}

$deletedCount = $files.Count - $failed.Count
Write-Host "Deleted $deletedCount file(s)." -ForegroundColor Green

if ($failed.Count -gt 0) {
    Write-Host "Failed to delete the following files:" -ForegroundColor Red
    $failed | ForEach-Object { Write-Host $_ }
}
