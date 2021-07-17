$NDKPath = Get-Content $PSScriptRoot/ndkpath.txt

$buildScript = "$NDKPath/build/ndk-build"

$OS = $PSVersionTable.OS
$windows = $OS.Contains("Windows")

# when core, or when on windows we want to add .cmd to the end
if ((-not ($PSVersionTable.PSEdition -eq "Core")) -or $windows) {
    $buildScript += ".cmd"
}

#get amount of processors
# if on a windows system
if ($windows)
{
    $proc = Get-ComputerInfo -Property CsProcessors
    $coreCount = $proc.CsProcessors.NumberOfCores
}
# if on a linux system
else
{
    $coreCount = & nproc
}

# if all fails, just assume 4
if (-not $coreCount)
{
    $coreCount = 4
}

$msg = "Building with " + $coreCount + " Cores!"
echo $msg
& $buildScript NDK_PROJECT_PATH=$PSScriptRoot APP_BUILD_SCRIPT=$PSScriptRoot/Android.mk NDK_APPLICATION_MK=$PSScriptRoot/Application.mk -j $coreCount