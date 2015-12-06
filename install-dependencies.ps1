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

$boostDir = "C:\Libraries\boost\"
$bootstrap = $boostDir+"bootstrap.bat"
if (!(Test-Path $bootstrap))
{
  return
}
echo "Building Boost.Build engine..."
pushd $boostDir
cmd /c $bootstrap
echo "Done."
echo "Building Boost.Regex..."
$bjamExe = $boostDir+"bjam.exe"
$bjamArgsDebug = "--with-regex toolset=msvc-12.0 link=static threading=multi runtime-link=static debug stage"
$bjamArgsRelease = "--with-regex toolset=msvc-12.0 link=static threading=multi runtime-link=static release stage"
start-process -FilePath $bjamExe -WorkingDirectory $boostDir -ArgumentList $bjamArgsDebug -Wait
start-process -FilePath $bjamExe -WorkingDirectory $boostDir -ArgumentList $bjamArgsRelease -Wait
echo "Done."
popd
echo "Available boost libraries:"
Get-ChildItem -Path ($boostDir+"stage\lib\") -File -Name
