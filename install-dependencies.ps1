if ($env:PROCESSOR_ARCHITECTURE.Contains("64"))
{
  $regpath = "HKLM:\Software\Wow6432Node\Microsoft\Windows\CurrentVersion\Uninstall\*"
}
else
{
  $regpath = "HKLM:\Software\Microsoft\Windows\CurrentVersion\Uninstall\*"
}
$pkgs = Get-ItemProperty $regpath
$found = $false
foreach ($pkg in $pkgs)
{
  if ($pkg.BundleTag -eq "vc_mbcsmfc")
  {
    $found = $true
    break
  }
}
if (!$found)
{
  echo "vc_mbcsmfc not found, downloading and installing..."
  $exePath = "$($env:USERPROFILE)\vc_mbcsmfc.exe"
  $dlPath = "http://download.microsoft.com/download/0/2/3/02389126-40A7-46FD-9D83-802454852703/vc_mbcsmfc.exe"
  (New-Object Net.WebClient).DownloadFile($dlPath, $exePath)
  cmd /c start /wait $exePath /quiet
  cmd /c del $exePath
}
