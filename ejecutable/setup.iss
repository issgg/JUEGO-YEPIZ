[Setup]
AppName=The Snake Game
AppVersion=1.0
DefaultDirName={pf}\The Snake Game
DefaultGroupName=The Snake Game
OutputBaseFilename=ThesnakeSetup
Compression=lzma2
SolidCompression=yes

[Files]
Source: "Thesnake.exe"; DestDir: "{app}"
Source: "raylib.h"; DestDir: "{app}"
Source: "assets\*"; DestDir: "{app}\assets"; Flags: recursesubdirs
Source: "config.ini"; DestDir: "{app}"
Source: "README.txt"; DestDir: "{app}"

[Icons]
Name: "{group}\The Snake Game"; Filename: "{app}\Thesnake.exe"

[Run]
Filename: "{app}\Thesnake.exe"; Description: "Ejecutar The Snake Game"; Flags: postinstall
