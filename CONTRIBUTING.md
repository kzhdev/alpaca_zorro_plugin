# Contributing to Alpaca Zorro Plugin

Thank you for your interest in contributing! This document provides guidelines for contributing to the project.

## Table of Contents

- [Getting Started](#getting-started)
- [Development Setup](#development-setup)
- [Building the Project](#building-the-project)
- [Code Style Guidelines](#code-style-guidelines)
- [Submitting Changes](#submitting-changes)
- [Reporting Issues](#reporting-issues)

## Getting Started

### Prerequisites

- **Visual Studio 2022** (Professional or Community)
- **CMake 3.16+**
- **C++20 compiler**
- **Zorro Trading Platform** (for testing)
- **Git**

### Required Knowledge

- C++20 features and STL
- Windows API and DLL development
- Threading and synchronization
- WebSocket protocols
- Financial trading concepts (optional but helpful)

## Development Setup

### 1. Clone the Repository

```bash
git clone https://github.com/kzhdev/alpaca_zorro_plugin.git
cd alpaca_zorro_plugin
```

### 2. Initialize Submodules

```bash
git submodule update --init --recursive
```

### 3. Configure CMake Build

#### Option A: Using CMake GUI
1. Open CMake GUI
2. Set source directory to `alpaca_zorro_plugin`
3. Set build directory to `alpaca_zorro_plugin/build`
4. Click "Configure" and select your Visual Studio version
5. Click "Generate"

#### Option B: Using Command Line
```bash
# For x86 build
cmake -B build -G "Visual Studio 17 2022" -A Win32

# For x64 build
cmake -B build -G "Visual Studio 17 2022" -A x64
```

### 4. Optional: Set ZORRO_ROOT Environment Variable

Set the `ZORRO_ROOT` environment variable to your Zorro installation directory. This allows the build to automatically copy the DLL to the correct plugin folder:

```bash
# Windows Command Prompt
set ZORRO_ROOT=C:\Zorro

# PowerShell
$env:ZORRO_ROOT="C:\Zorro"

# Or set permanently via System Properties > Environment Variables
```

### 5. Configure Visual Studio Debugger (Optional)

For easier debugging, configure the Visual Studio user settings:

1. Open the solution in Visual Studio
2. Create or edit `alpaca_zorro_plugin.vcxproj.user` in the project root
3. Add the following configuration:

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

This configuration allows you to:
- Launch Zorro directly from Visual Studio when debugging
- The DLL will be automatically placed in the correct plugin folder when `ZORRO_ROOT` is set

## Building the Project

### Build with Visual Studio

1. Open `build/alpaca_zorro_plugin.sln` in Visual Studio
2. Select your configuration (Debug/Release) and platform (Win32/x64)
3. Build → Build Solution (or press F7)

### Build with CMake Command Line

```bash
# Debug build
cmake --build build --config Debug

# Release build
cmake --build build --config Release
```

### Build Output

- **DLL Output**: `build/bin/Release/Alpaca.dll` or `build/bin/Debug/Alpaca.dll`
- **Distribution Package**: `build/dist/AlpacaZorroPlugin_<version>.zip` (Release only)

## Code Style Guidelines

### General Principles

1. **Follow existing code style** - maintain consistency with the current codebase
2. **Use C++20 features** where appropriate
3. **Write clear, self-documenting code** with meaningful variable names
4. **Add comments** for complex logic or non-obvious implementations
5. **Keep functions focused** - each function should have a single responsibility

### Specific Guidelines

- **Indentation**: Use tabs (as per existing codebase)
- **Naming Conventions**:
  - Classes: `PascalCase`
  - Functions: `camelCase` or `snake_case` (match existing usage)
  - Variables: `snake_case_` for member variables, `snake_case` for locals
  - Constants: `UPPER_CASE`
- **Header Guards**: Use `#pragma once`
- **Threading**: Always use proper synchronization for shared state
- **Error Handling**: Use exceptions for exceptional cases, return codes for expected failures
- **Logging**: Use the `slick_logger` for all logging needs

### Example

```cpp
class MarketDataHandler {
public:
    void processQuote(const Quote& quote);

private:
    std::atomic<bool> is_running_{false};
    std::shared_ptr<Logger> logger_;
};
```

## Submitting Changes

### Before Submitting

1. **Test thoroughly**:
   - Build both x86 and x64 configurations
   - Test with Zorro in both paper and live modes
   - Verify no memory leaks or crashes

2. **Update documentation**:
   - Update `CHANGELOG` with your changes
   - Update `README.md` if adding new features
   - Add code comments for complex logic

3. **Ensure clean commits**:
   - Write clear, descriptive commit messages
   - Keep commits focused and atomic
   - Squash work-in-progress commits

### Pull Request Process

1. **Fork the repository** and create a feature branch:
   ```bash
   git checkout -b feature/your-feature-name
   ```

2. **Make your changes** following the code style guidelines

3. **Commit your changes**:
   ```bash
   git commit -m "Add feature: description of your changes"
   ```

4. **Push to your fork**:
   ```bash
   git push origin feature/your-feature-name
   ```

5. **Create a Pull Request**:
   - Provide a clear description of the changes
   - Reference any related issues
   - Include testing details

6. **Address review feedback** promptly

### Commit Message Format

```
<type>: <short summary>

<optional detailed description>

<optional footer>
```

**Types**:
- `feat`: New feature
- `fix`: Bug fix
- `docs`: Documentation changes
- `refactor`: Code refactoring
- `perf`: Performance improvements
- `test`: Adding or updating tests
- `chore`: Maintenance tasks

**Example**:
```
feat: Add support for crypto trading

- Implement crypto asset type handling
- Add crypto-specific order validation
- Update WebSocket subscription logic for crypto feeds

Fixes #123
```

## Reporting Issues

### Bug Reports

When reporting bugs, please include:

1. **Description**: Clear description of the issue
2. **Steps to Reproduce**: Detailed steps to reproduce the problem
3. **Expected Behavior**: What you expected to happen
4. **Actual Behavior**: What actually happened
5. **Environment**:
   - Zorro version
   - Plugin version
   - Windows version
   - Build configuration (x86/x64)
6. **Logs**: Relevant log files from `Logs/Alpaca_xxx.log`
7. **Screenshots**: If applicable

### Feature Requests

For feature requests, please describe:

1. **Use Case**: What problem does this solve?
2. **Proposed Solution**: How should it work?
3. **Alternatives**: What alternatives have you considered?
4. **Additional Context**: Any other relevant information

### Where to Report

Report issues on GitHub: [Issues](https://github.com/kzhdev/alpaca_zorro_plugin/issues)

## Development Tips

### Debugging

1. **Enable Logging**: Set `AlpacaLogLevel = 5` (TRACE) in Zorro.ini for verbose logging
2. **Use the Settings Dialog**: Access runtime log level changes via the plugin settings
3. **Check WebSocket Proxy**: Monitor `websocket_proxy.exe` for connection issues
4. **Attach Debugger**: Use Visual Studio's "Attach to Process" for Zorro.exe

### Testing Checklist

- [ ] Builds successfully on x86 and x64
- [ ] No compiler warnings
- [ ] No memory leaks (use Visual Studio's Memory Profiler)
- [ ] Works with paper trading
- [ ] Works with live trading (if applicable)
- [ ] WebSocket connections stable
- [ ] Historical data downloads correctly
- [ ] Order placement and management works
- [ ] Settings dialog functions properly

## License

By contributing to this project, you agree that your contributions will be licensed under the same license as the project (see [LICENSE](LICENSE) file).

## Questions?

If you have questions about contributing, feel free to:
- Open a [discussion](https://github.com/kzhdev/alpaca_zorro_plugin/discussions)
- Ask in an issue
- Contact the maintainers

Thank you for contributing!
