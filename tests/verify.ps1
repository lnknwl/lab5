#requires -Version 7.0
$ErrorActionPreference = 'Stop'
$root = Split-Path $PSScriptRoot -Parent
$encoding = [Text.Encoding]::GetEncoding(1251)

# Клиенты запускаются без каталога компилятора в PATH
function Run-Client([string]$File, [string]$Arguments, [string]$InputText) {
    $start = [Diagnostics.ProcessStartInfo]::new((Join-Path $root $File))
    $start.WorkingDirectory = $root
    $start.Arguments = $Arguments
    $start.UseShellExecute = $false
    $start.RedirectStandardInput = $true
    $start.RedirectStandardOutput = $true
    $start.RedirectStandardError = $true
    $start.StandardInputEncoding = $encoding
    $start.StandardOutputEncoding = $encoding
    $start.StandardErrorEncoding = $encoding
    $start.Environment['PATH'] = "$env:SystemRoot\System32;$env:SystemRoot"
    $process = [Diagnostics.Process]::Start($start)
    if ($InputText) { $process.StandardInput.Write($InputText) }
    $process.StandardInput.Close()
    $output = $process.StandardOutput.ReadToEnd()
    $errorText = $process.StandardError.ReadToEnd()
    $process.WaitForExit()
    return [PSCustomObject]@{Code=$process.ExitCode; Output=$output; Error=$errorText}
}

$static = 'build\static\lab5-static.exe'
$dynamic = 'build\shared\lab5-dynamic.exe'
foreach ($client in @($static, $dynamic)) {
    if (!(Test-Path -LiteralPath (Join-Path $root $client))) {
        throw 'Сначала выполните build.bat'
    }
}

$staticTests = Run-Client $static '--test' ''
$dynamicTests = Run-Client $dynamic '--test' ''
if ($staticTests.Code -ne 0 -or $dynamicTests.Code -ne 0 -or
    $staticTests.Error -or $dynamicTests.Error -or
    $staticTests.Output -ne $dynamicTests.Output) {
    throw "Ошибка автоматических проверок библиотек`n$($staticTests.Output)`n$($dynamicTests.Output)`n$($staticTests.Error)`n$($dynamicTests.Error)"
}

$inputLines = @('1', '3 4', '2', '+', '1 -2', '2', 'a++', '2', '/', '0 0', '3', '0')
$inputText = ($inputLines -join "`n") + "`n"
$staticSession = Run-Client $static '' $inputText
$dynamicSession = Run-Client $dynamic '' $inputText
if ($staticSession.Code -ne 0 -or $dynamicSession.Code -ne 0 -or
    $staticSession.Error -or $dynamicSession.Error -or
    $staticSession.Output -ne $dynamicSession.Output -or
    !$staticSession.Output.Contains('Текущее число: 5 + 2i') -or
    !$staticSession.Output.Contains('Ошибка: Деление на нулевое комплексное число')) {
    throw 'Консольные сеансы двух клиентов различаются или содержат неправильный результат'
}

foreach ($client in @($static, $dynamic)) {
    $wrongArgument = Run-Client $client '--unknown' ''
    if ($wrongArgument.Code -ne 1) { throw 'Неправильный аргумент не отклонен' }
}

Push-Location $root
try {
    $staticImports = (& objdump -p $static | Out-String)
    if ($LASTEXITCODE -ne 0) { throw 'Не удалось прочитать импорты статического клиента' }
    $dynamicImports = (& objdump -p $dynamic | Out-String)
    if ($LASTEXITCODE -ne 0) { throw 'Не удалось прочитать импорты динамического клиента' }
}
finally {
    Pop-Location
}
if ($staticImports -match 'DLL Name:\s+lab3\.dll' -or
    $dynamicImports -notmatch 'DLL Name:\s+lab3\.dll') {
    throw 'Способ линковки клиентов не соответствует заданию'
}

[IO.File]::WriteAllText((Join-Path $root 'build\session-output.txt'), $staticSession.Output, [Text.UTF8Encoding]::new($false))
[IO.File]::WriteAllText((Join-Path $root 'build\session-input.txt'), $inputText, [Text.UTF8Encoding]::new($false))
Write-Output 'Обе программы работают одинаково, способы линковки подтверждены'
