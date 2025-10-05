# Building Alpaca Zorro Plugin

This guide explains how to build the Alpaca Zorro Plugin from source.

## Table of Contents

- [Prerequisites](#prerequisites)
- [Quick Start](#quick-start)
- [Build Methods](#build-methods)
- [Build Configurations](#build-configurations)
- [Advanced Configuration](#advanced-configuration)
- [Troubleshooting](#troubleshooting)

## Prerequisites

### Required Software

- **Visual Studio 2022** (Professional or Community Edition)
  - C++ Desktop Development workload
  - Windows 10 SDK
- **CMake 3.16 or higher**
- **Git** (for cloning the repository)

### Optional

- **Zorro Trading Platform** (for testing and automatic deployment)
- **Ninja** or **Visual Studio Code** (alternative build tools)

## Quick Start

### 1. Clone the Repository

```bash
git clone https://github.com/kzhdev/alpaca_zorro_plugin.git
cd alpaca_zorro_plugin
```

> **Note**: All dependencies are included in the `3rdparty` folder. No submodule initialization is needed.

### 2. Configure CMake

#### Using CMake GUI (Recommended for beginners)

1. Launch CMake GUI
2. **Source directory**: Browse to the cloned `alpaca_zorro_plugin` folder
3. **Build directory**: Browse to `alpaca_zorro_plugin/build`
4. Click **Configure**
   - Select **Visual Studio 17 2022**
   - Choose platform: **Win32** (x86) or **x64**
5. Click **Generate**
6. Click **Open Project** to launch Visual Studio

#### Using Command Line

For **x86 (Win32)** build:
```bash
cmake -B build -G "Visual Studio 17 2022" -A Win32
```

For **x64** build:
```bash
cmake -B build -G "Visual Studio 17 2022" -A x64
```

### 3. Build the Project

#### Option A: Visual Studio IDE

1. Open `build/alpaca_zorro_plugin.sln`
2. Select configuration: **Debug** or **Release**
3. Select platform: **Win32** or **x64**
4. Press **F7** or go to **Build → Build Solution**

#### Option B: Command Line

For **Debug** build:
```bash
cmake --build build --config Debug
```

For **Release** build:
```bash
cmake --build build --config Release
```

### 4. Locate Build Output

After successful build, the DLL will be located at:

- **Debug**: `build/bin/Debug/Alpaca.dll`
- **Release**: `build/bin/Release/Alpaca.dll`

For **Release** builds, a distribution package is also created:
- `build/dist/AlpacaZorroPlugin_<version>.zip`

## Build Methods

### Method 1: Visual Studio IDE (Recommended)

Best for: Development, debugging, and testing

1. Configure CMake as described above
2. Open the generated solution in Visual Studio
3. Build using the IDE

**Advantages:**
- Full IntelliSense support
- Integrated debugging
- Visual error navigation
- Easy breakpoint management

### Method 2: CMake Command Line

Best for: CI/CD, automated builds, scripting

```bash
# Configure
cmake -B build -G "Visual Studio 17 2022" -A Win32

# Build Debug
cmake --build build --config Debug

# Build Release
cmake --build build --config Release
```

**Advantages:**
- Automation friendly
- Faster for batch builds
- No GUI required

### Method 3: MSBuild Command Line

Best for: Advanced users, build scripts

```bash
# After CMake configuration
msbuild build\alpaca_zorro_plugin.sln /p:Configuration=Release /p:Platform=Win32
```

## Build Configurations

### Platform Options

| Platform | Description | Output Folder | Zorro Plugin Folder |
|----------|-------------|---------------|---------------------|
| **Win32** | 32-bit (x86) build | `build/bin/Release/` | `Zorro/Plugin/` |
| **x64** | 64-bit build | `build/bin/Release/` | `Zorro/Plugin64/` |

### Configuration Options

| Configuration | Description | Use Case |
|---------------|-------------|----------|
| **Debug** | With debug symbols, no optimization | Development, debugging |
| **Release** | Optimized, no debug info | Production, distribution |

### Build All Configurations

To build both x86 and x64 for both Debug and Release:

```bash
# x86 Debug & Release
cmake -B build/x86 -G "Visual Studio 17 2022" -A Win32
cmake --build build/x86 --config Debug
cmake --build build/x86 --config Release

# x64 Debug & Release
cmake -B build/x64 -G "Visual Studio 17 2022" -A x64
cmake --build build/x64 --config Debug
cmake --build build/x64 --config Release
```

## Advanced Configuration

### Automatic Deployment to Zorro

Set the `ZORRO_ROOT` environment variable to automatically copy the built DLL to Zorro's plugin folder:

#### Windows Command Prompt
```cmd
set ZORRO_ROOT=C:\Zorro
cmake -B build -G "Visual Studio 17 2022" -A Win32
cmake --build build --config Release
```

#### PowerShell
```powershell
$env:ZORRO_ROOT="C:\Zorro"
cmake -B build -G "Visual Studio 17 2022" -A Win32
cmake --build build --config Release
```

#### Permanent Environment Variable
1. Open **System Properties** → **Environment Variables**
2. Add new User or System variable:
   - Name: `ZORRO_ROOT`
   - Value: `C:\Zorro` (your Zorro installation path)
3. Restart your terminal/IDE

When `ZORRO_ROOT` is set:
- **x86 Debug**: Copies to `%ZORRO_ROOT%\Plugin\Alpaca.dll`
- **x86 Release**: Copies to `%ZORRO_ROOT%\Plugin\Alpaca.dll`
- **x64 Debug**: Copies to `%ZORRO_ROOT%\Plugin64\Alpaca.dll`
- **x64 Release**: Copies to `%ZORRO_ROOT%\Plugin64\Alpaca.dll`

### Visual Studio Debugger Configuration

To launch Zorro directly from Visual Studio when debugging:

1. After opening the solution in Visual Studio, create/edit:
   `alpaca_zorro_plugin.vcxproj.user`

2. Add the following content:

```xml
<?xml version="1.0" encoding="utf-8"?>
<Project ToolsVersion="Current" xmlns="http://schemas.microsoft.com/developer/msbuild/2003">
  <PropertyGroup Condition="'$(Configuration)|$(Platform)'=='Debug|Win32'">
    <LocalDebuggerCommand>C:\Zorro\Zorro.exe</LocalDebuggerCommand>
    <DebuggerFlavor>WindowsLocalDebugger</DebuggerFlavor>
  </PropertyGroup>
  <PropertyGroup Condition="'$(Configuration)|$(Platform)'=='Debug|x64'">
    <LocalDebuggerCommand>C:\Zorro\Zorro64\Zorro64.exe</LocalDebuggerCommand>
    <DebuggerFlavor>WindowsLocalDebugger</DebuggerFlavor>
  </PropertyGroup>
  <PropertyGroup Condition="'$(Configuration)|$(Platform)'=='Release|Win32'">
    <LocalDebuggerCommand>C:\Zorro\Zorro.exe</LocalDebuggerCommand>
    <DebuggerFlavor>WindowsLocalDebugger</DebuggerFlavor>
  </PropertyGroup>
  <PropertyGroup Condition="'$(Configuration)|$(Platform)'=='Release|x64'">
    <LocalDebuggerCommand>C:\Zorro\Zorro64\Zorro64.exe</LocalDebuggerCommand>
    <DebuggerFlavor>WindowsLocalDebugger</DebuggerFlavor>
  </PropertyGroup>
</Project>
```

3. Update the paths to match your Zorro installation

**Benefits:**
- Press **F5** to debug with Zorro automatically launching
- Breakpoints work seamlessly
- Full Visual Studio debugging experience

### CMake Build Options

You can customize the build with CMake options:

```bash
# Specify build type during configuration
cmake -B build -G "Visual Studio 17 2022" -A Win32 -DCMAKE_BUILD_TYPE=Release

# Verbose build output
cmake --build build --config Release --verbose

# Parallel build (faster)
cmake --build build --config Release --parallel 8
```

### Custom Output Directories

To change the output directory:

```bash
cmake -B build -G "Visual Studio 17 2022" -A Win32 ^
  -DCMAKE_RUNTIME_OUTPUT_DIRECTORY="C:/MyCustomPath"
```

## Troubleshooting

### Common Issues

#### Issue: CMake can't find Visual Studio

**Error:** `Could not find any instance of Visual Studio.`

**Solution:**
- Install Visual Studio 2022 with C++ Desktop Development workload
- Or specify an older Visual Studio version:
  ```bash
  cmake -B build -G "Visual Studio 16 2019" -A Win32
  ```

#### Issue: MSB8028 Warning (Shared Intermediate Directory)

**Warning:** `The intermediate directory contains files shared from another project`

**Solution:** This warning should be fixed in the latest version. If you still see it:
1. Delete the `build` folder
2. Reconfigure CMake
3. Build again

#### Issue: C4293 Warning (Shift Count in x86)

**Warning:** `'>>': shift count negative or too big`

**Solution:** This warning should be suppressed in the latest version of slick_logger included in `3rdparty/slick_logger`.

#### Issue: Missing DLL after build

**Problem:** Can't find `Alpaca.dll` after building

**Solution:** Check the correct output folder:
- Debug: `build/bin/Debug/Alpaca.dll`
- Release: `build/bin/Release/Alpaca.dll`

#### Issue: Build fails with "Access Denied"

**Problem:** Can't write to output directory

**Solution:**
- Close Zorro if it's running (it locks the DLL)
- Run Visual Studio as Administrator
- Check file permissions on the output folder

### Clean Build

To perform a clean build:

```bash
# Delete build folder
rmdir /s /q build

# Reconfigure and build
cmake -B build -G "Visual Studio 17 2022" -A Win32
cmake --build build --config Release
```

### Get Help

If you encounter other issues:

1. Check the [CHANGELOG](CHANGELOG) for known issues
2. Search [GitHub Issues](https://github.com/kzhdev/alpaca_zorro_plugin/issues)
3. Create a new issue with:
   - Your Visual Studio version
   - CMake version (`cmake --version`)
   - Full error message
   - Build configuration (x86/x64, Debug/Release)

## Build Verification

After building, verify the DLL:

```bash
# Check DLL exists
dir build\bin\Release\Alpaca.dll

# Check DLL architecture (x86 vs x64)
dumpbin /headers build\bin\Release\Alpaca.dll | findstr machine
```

Expected output:
- x86: `14C machine (x86)`
- x64: `8664 machine (x64)`

## Next Steps

After successfully building:

1. Copy `Alpaca.dll` to your Zorro plugin folder:
   - x86: `Zorro/Plugin/Alpaca.dll`
   - x64: `Zorro/Plugin64/Alpaca.dll`

2. Copy WebSocket Proxy:
   - From: `3rdparty/websocket_proxy/bin/`
   - To: Zorro plugin folder (same as Alpaca.dll)

3. Configure your Alpaca credentials in Zorro

4. Test with a simple Zorro script

For development workflow and contributing guidelines, see [CONTRIBUTING.md](CONTRIBUTING.md)
