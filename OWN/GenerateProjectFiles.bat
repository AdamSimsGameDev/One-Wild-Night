echo off

del OWN.sln
rmdir /s /q .vs
rmdir /s /q Binaries
rmdir /s /q Intermediate
rem rmdir /s /q Saved
rmdir /s /q DerivedDataCache

rem SET THES FILE PATHS TO THE ONES ON YOUR PC!
set MyUVS="C:\Program Files (x86)\Epic Games\Launcher\Engine\Binaries\Win64\UnrealVersionSelector.exe"
set MyUBT="C:\Program Files\Epic Games\UE_5.3\Engine\Binaries\DotNET\UnrealBuildTool.exe"

rem change Transformation to your own project name
set MyFullPath="%cd%\OWN"


%MyUVS% /projectfiles %MyFullPath%.uproject

%MyUBT% Development Win64 -Project=%MyFullPath%.uproject -TargetType=Editor -Progress -NoEngineChanges -NoHotReloadFromIDE

%MyFullPath%.uproject

%MyFullPath%.sln
