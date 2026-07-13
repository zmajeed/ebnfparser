# tools/patch_getopt.ps1

# exclude any nonportable code from getopt sources from musl libc
# changes include placing #ifdef NONPORTABLE_ORIGINAL_CODE around nonportable code

param (
  [string]$getoptDir
)

$getoptFile = join-path $getoptDir "getopt.c"
$getoptlongFile = join-path $getoptDir "getopt_long.c"

if(-not (test-path $getoptFile) -or -not (test-path $getoptlongFile)) {
  write-error "error: no getopt files in $getoptDir"
  exit 1
}

write-host "Munge getopt files in $getoptDir"

function wrapLine ($content, $targetLine) {
  if($content -match [regex]::Escape($targetLine)) {
    $Wrapper = "`n#ifdef NONPORTABLE_ORIGINAL_CODE`n$targetLine`n#endif"
    return $content -replace [regex]::escape($targetLine), $Wrapper
  }
  return $content
}

$lines = [system.io.file]::readalltext($getoptFile)

$lines = wrapLine $lines "#define _BSD_SOURCE"
$lines = wrapLine $lines '#include "locale_impl.h"'
$lines = wrapLine $lines '#include "stdio_impl.h"'
$lines = wrapLine $lines '#include "libc.h"'
$lines = wrapLine $lines "#include <unistd.h>"
$lines = wrapLine $lines "weak_alias(__optreset, optreset);"
$lines = wrapLine $lines "weak_alias(getopt, __posix_getopt);"

$msgRegex = "(?m)^void __getopt_msg\([^)]*\)\s*\{([\s\S]*?)^}"
$msgMatch = [regex]::match($lines, $msgRegex)
if($msgMatch.success) {
  $originalBody = $msgMatch.Groups[1].Value
  $stubBody = "`n#ifdef NONPORTABLE_ORIGINAL_CODE`n$originalBody#else`n`t(void)a; (void)b; (void)c; (void)l;`n#endif`n"
  $fullReplacement = "void __getopt_msg(const char *a, const char *b, const char *c, size_t l)`n{" + $stubBody + "}"
  $lines = $lines -replace $msgRegex, $fullReplacement
}

$wcharRegex = '(?m)^\s*wchar_t\s+c,\s*d;'
$wcharReplacement = "#ifdef NONPORTABLE_ORIGINAL_CODE`n`twchar_t c, d;`n#else`n`tint c, d;`n#endif"
$lines = [regex]::Replace($lines, $wcharRegex, $wcharReplacement)

$mbtowcRegex = "(?m)if\s*\(\(k\s*=\s*mbtowc\(&c,\s*([^,]+),\s*[^)]+\)\)\s*<\s*0\)\s*\{[^}]*\}"
$byteFallbackC = "c = (unsigned char)*(`$1); k = (c ? 1 : 0);"
$lines = [regex]::replace($lines, $mbtowcRegex, "#ifdef NONPORTABLE_ORIGINAL_CODE`n`$&`n#else`n`t`t$byteFallbackC`n#endif")

$mbtowcRegex2 = '(?m)l\s*=\s*mbtowc\(&d,\s*([^,]+),\s*[^)]+\);'
$byteFallbackC2 = "d = (unsigned char)*(`$1); l = (d ? 1 : 0);"
$lines = [regex]::Replace($lines, $mbtowcRegex2, "#ifdef NONPORTABLE_ORIGINAL_CODE`n`$&`n#else`n`t`t$byteFallbackC2`n#endif")

[system.io.file]::writealltext($getoptFile, $lines)
write-host "Patched getopt.c"

$lines = [system.io.file]::readalltext($getoptlongFile)

$lines = wrapLine $lines '#include "stdio_impl.h"'

$mblenRegex = "(?m)if\s*\(\(k\s*=\s*mblen\(([^,]+),\s*[^)]+\)\)\s*<\s*0\)\s*k\s*=\s*1;"
$byteFallbackLong = "k = (*(`$1) ? 1 : 0);"
$lines = [regex]::replace($lines, $mblenRegex, "#ifdef NONPORTABLE_ORIGINAL_CODE`n`$&`n#else`n`t`t$byteFallbackLong`n#endif")

[system.io.file]::writealltext($getoptlongFile, $lines)
write-host "Patched getopt_long.c"

write-host "Done munging getopt files"

