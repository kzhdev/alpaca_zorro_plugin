# Build

1. Download alpaca_zorro_plugin:

   ```git clone https://github.com/kzhdev/alpaca_zorro_plugin.git```
1. Download dependencies:

   ```cd alpaca_zorro_plugin```

   ```git submodule update --init --recursive```
1. Open the project in Visual Studio
1. Edit alpaca_zorro_plugin.vcxproj.user (**Optional**)
   After opening the soluion in Visual Studio, an alpaca_zorro_plugin.vcxproj.user file should be created in alpaca_zorro_plugin folder. Open the alpaca_zorro_plugin.vcxproj.user file in any editor and add the **\<LocalDebuggerCommand>** and **\<OutDir>** in the file. It should point to the Zorro.exe and plugin folder in your machine. This will put the build AlpacaZorroPlugin.dll directly into Zorro's plugin folder. And launch Zorro automatically when debugging the project in Visual Studio.

   ``` XML
    <?xml version="1.0" encoding="utf-8"?>
    <Project ToolsVersion="Current" xmlns="http://schemas.microsoft.com/developer/msbuild/2003">
      <PropertyGroup Condition="'$(Configuration)|$(Platform)'=='Debug|Win32'">
        <LocalDebuggerCommand>C:\Zorro\Zorro.exe</LocalDebuggerCommand>
        <DebuggerFlavor>WindowsLocalDebugger</DebuggerFlavor>
        <OutDir>C:\Zorro\Plugin\</OutDir>
        <LocalDebuggerCommandArguments>
        </LocalDebuggerCommandArguments>
      </PropertyGroup>
      <PropertyGroup Condition="'$(Configuration)|$(Platform)'=='Debug|x64'">
        <LocalDebuggerCommandArguments />
        <LocalDebuggerCommand>C:\Zorro\Zorro.exe</LocalDebuggerCommand>
        <DebuggerFlavor>WindowsLocalDebugger</DebuggerFlavor>
	    <OutDir>C:\Zorro\Plugin64\</OutDir>
      </PropertyGroup>
      <PropertyGroup Condition="'$(Configuration)|$(Platform)'=='Release|Win32'">
        <LocalDebuggerCommand>C:\Zorro\Zorro.exe</LocalDebuggerCommand>
        <DebuggerFlavor>WindowsLocalDebugger</DebuggerFlavor>
        <OutDir>C:\Zorro\Plugin\</OutDir>
        <LocalDebuggerCommandArguments>
        </LocalDebuggerCommandArguments>
      </PropertyGroup>
      <PropertyGroup Condition="'$(Configuration)|$(Platform)'=='Release|x64'">
        <LocalDebuggerCommandArguments />
        <LocalDebuggerCommand>C:\Zorro\Zorro.exe</LocalDebuggerCommand>
        <DebuggerFlavor>WindowsLocalDebugger</DebuggerFlavor>
	    <OutDir>C:\Zorro\Plugin64\</OutDir>
      </PropertyGroup>
    </Project>
   ```

   If the .user file not created, you can manually create the file and the contents.
