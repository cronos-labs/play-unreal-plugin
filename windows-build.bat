set PWD="%cd%"
"C:\Program Files\Epic Games\UE_5.1\Engine\Build\BatchFiles\RunUAT.bat" ^
    BuildPlugin ^
    -Rocket ^
    -TargetPlatforms=Win64 ^
    -Plugin=%PWD%/CronosPlayUnreal.uplugin ^
    -Package=%PWD%/Output
