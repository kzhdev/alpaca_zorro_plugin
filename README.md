# Alpaca Zorro Plugin

[![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)
[![GitHub release](https://img.shields.io/github/v/release/kzhdev/alpaca_zorro_plugin)](https://github.com/kzhdev/alpaca_zorro_plugin/releases)

A powerful integration between **[Alpaca](https://alpaca.markets)** and **[Zorro](https://zorro-project.com/)**, enabling algorithmic trading with real-time market data, paper trading, and live trading capabilities.

## Table of Contents

- [Features](#features)
- [Installation](#installation)
- [Quick Start](#quick-start)
- [Configuration](#configuration)
- [Usage Examples](#usage-examples)
- [API Reference](#api-reference)
- [Building from Source](#building-from-source)
- [Troubleshooting](#troubleshooting)
- [Contributing](#contributing)
- [License](#license)

## Features

### Core Capabilities

- ✅ **Real-time Market Data** via Alpaca Market Data API V2
- ✅ **WebSocket Support** with multi-instance connection sharing
- ✅ **Paper & Live Trading** - Test strategies safely before going live
- ✅ **Multiple Order Types** - Market, Limit, Stop, Stop-Limit
- ✅ **Time-in-Force Options** - FOK, IOC, GTC, DAY, OPG, CLS
- ✅ **Fractional Shares** - Trade fractional quantities (e.g., 0.001 shares)
- ✅ **Asset Types** - Stocks, Options, Crypto
- ✅ **Historical Data** - Download bars with adjustments (splits, dividends)
- ✅ **Runtime Configuration** - F2 settings dialog for log level changes
- ✅ **x86 & x64 Support** - Works with both 32-bit and 64-bit Zorro

### WebSocket Proxy

The plugin includes **ZorroWebsocketProxy**, allowing multiple Zorro instances to share a single WebSocket connection:

- **Lock-free design** for high performance
- **Automatic connection management**
- **Reduced API rate limits** by sharing connections
- **Note**: High CPU usage is normal due to lock-free spinning design

## Installation

### Prerequisites

- [Zorro](https://zorro-project.com/) (version 2.30 or higher recommended)
- [Alpaca account](https://alpaca.markets/) with API credentials
- Windows 10/11 (x86 or x64)

### Steps

1. **Download** the [latest release](https://github.com/kzhdev/alpaca_zorro_plugin/releases)

2. **Extract** the downloaded ZIP file

3. **Copy files** to Zorro directory:
   - For **x86 (32-bit)**:
     - `Alpaca.dll` → `<Zorro>/Plugin/`
     - `websocket_proxy/` → `<Zorro>/Plugin/websocket_proxy/`
   - For **x64 (64-bit)**:
     - `Alpaca.dll` → `<Zorro>/Plugin64/`
     - `websocket_proxy/` → `<Zorro>/Plugin/websocket_proxy/`

4. **Copy header file** (optional, for advanced usage):
   - `AlpacaBrokerCommands.h` → `<Zorro>/include/`

## Quick Start

### 1. Get Alpaca API Credentials

1. Sign up at [Alpaca Markets](https://alpaca.markets/)
2. Navigate to **Paper Trading** or **Live Trading** dashboard
3. Generate **API Key** and **Secret Key**

### 2. Configure Zorro

1. Launch **Zorro**
2. Select **Alpaca** from the broker dropdown
3. Enter credentials:
   - **User ID**: Your Alpaca API Key
   - **Password**: Your Alpaca Secret Key
4. Press **F2** to open settings dialog (optional)

### 3. Run Your First Strategy

```c
// SimpleAlpacaStrategy.c
function run()
{
    set(LOGFILE);
    BarPeriod = 60; // 1-hour bars

    asset("SPY");

    if(crossOver(SMA(12), SMA(26))) {
        enterLong();
    }
    else if(crossUnder(SMA(12), SMA(26))) {
        enterShort();
    }
}
```

## Configuration

### Plugin Settings

Add the following optional configurations to `ZorroFix.ini` (preferred) or `Zorro.ini`:

```ini
AlpacaPaidDataPlan = 1                  # 0 = Basic (Free), 1 = Pro (Paid). Default: 1
AlpacaLogLevel = 0                      # 0=OFF, 1=ERROR, 2=WARN, 3=INFO, 4=DEBUG, 5=TRACE. Default: 0
AlpacaUseWebsocket = 1                  # 0 = REST API only, 1 = WebSocket. Default: 1
AlpacaFractionalLotAmount = 1.0         # Lot multiplier for fractional shares. Default: 1.0
AlpacaPassLogLevelToWebsocketProxy = 0  # Passing Log Level to WebsocketProxy. 0 = No, 1 = Yes. Default: 0
```

### Configuration Details

| Parameter | Description | Values | Default |
|-----------|-------------|--------|---------|
| **AlpacaPaidDataPlan** | Market data plan tier | 0 = Basic (Free)<br>1 = Pro (Paid) | 1 |
| **AlpacaLogLevel** | Plugin logging verbosity | 0 = OFF<br>1 = ERROR<br>2 = WARNING<br>3 = INFO<br>4 = DEBUG<br>5 = TRACE | 0 |
| **AlpacaLogType** | Types of data to log | Bitmask value | ALL except WEB_SOCKET_DATA |
| **AlpacaUseWebsocket** | Real-time data method | 0 = REST polling<br>1 = WebSocket streaming | 1 |
| **AlpacaFractionalLotAmount** | Fractional trading multiplier | Any value < 1.0 enables fractional shares<br>(e.g., 0.001 = 0.1% of 1 share) | 1.0 |
| **AlpacaPassLogLevelToWebsocketProxy** | WebSocket proxy log level | Same as AlpacaLogLevel | 0 |

### Runtime Settings

Press **F2** in Zorro to open the plugin settings dialog where you can:
- Adjust log levels without restarting
- View current configuration
- Test connection status

## Usage Examples

### Order Types

#### Market Orders (Default)

```c
// Simple market orders
enterLong(5);    // Buy 5 lots at market price
enterShort(5);   // Sell 5 lots at market price
```

#### Limit Orders

```c
// Place limit orders
OrderLimit = 100.00;  // Set limit price
enterLong(5);         // Buy 5 lots at $100.00 limit

OrderLimit = 200.00;
enterShort(3);        // Sell 3 lots at $200.00 limit
```

### Time-in-Force Options

Supported TIF types: **FOK** (default), **IOC**, **GTC**, **DAY**, **OPG**, **CLS**

See [Alpaca Orders Documentation](https://alpaca.markets/docs/trading-on-alpaca/orders/#time-in-force) for details.

```c
#include <AlpacaBrokerCommands.h>

// Day order with limit price
brokerCommand(SET_ORDERTYPE, ORDERTYPE_DAY);
OrderLimit = 100.00;
enterShort(5);    // Sell 5 lots, Day order at $100.00 limit

// Market-on-Close order
OrderLimit = 0;   // Market order
brokerCommand(SET_ORDERTYPE, ORDERTYPE_CLS);
enterLong(5);     // Buy 5 lots at market close
```

**Available TIF Types:**
- `ORDERTYPE_FOK` - Fill-or-Kill (default)
- `ORDERTYPE_IOC` - Immediate-or-Cancel
- `ORDERTYPE_GTC` - Good-till-Canceled
- `ORDERTYPE_DAY` - Day order
- `ORDERTYPE_OPG` - Market-on-Open
- `ORDERTYPE_CLS` - Market-on-Close

### Price Types

```c
// Use Last Trade price (default is Last Quote)
brokerCommand(SET_PRICETYPE, 2);  // Trades
brokerCommand(SET_PRICETYPE, 1);  // Quotes (Ask/Bid)
```

### Fractional Trading

```c
// ZorroFix.ini: AlpacaFractionalLotAmount = 0.001

#include <AlpacaBrokerCommands.h>

asset("TSLA");
brokerCommand(SET_ORDERTYPE, ORDERTYPE_DAY);  // Required for fractional
LotAmount = 5;
enterLong();      // Buys 0.005 shares of TSLA (5 * 0.001)
```

**Note**: Fractional trading only works with **Market** and **Day** order types.

### Custom Order Text

```c
// Add custom identifier to orders
brokerCommand(SET_ORDERTEXT, "MyStrategy_v1");
enterLong(5);  // Order will have "MyStrategy_v1" in ClientOrderId
```

### Position Management

```c
// Get current position for specific asset
var position = brokerCommand(GET_POSITION, "AAPL");
printf("AAPL position: %.2f", position);
```

### Asset Properties

```c
#include <AlpacaBrokerCommands.h>

asset("GME");

// Check asset capabilities
int isFractionable = brokerCommand(IS_ASSET_FRACTIONABLE, Asset);
int isShortable = brokerCommand(IS_ASSET_SHORTABLE, Asset);
int isEasyToBorrow = brokerCommand(IS_ASSET_EASY_TO_BORROW, Asset);
int isMarginable = brokerCommand(IS_ASSET_MARGINABLE, Asset);

if(isFractionable) {
    printf("GME supports fractional shares");
}
```

### Historical Data Adjustments

```c
#include <AlpacaBrokerCommands.h>

// Set adjustment type for historical bars
brokerCommand(SET_ADJUSTMENT, ADJUSTMENT_SPLIT);  // Split-adjusted only

// Available adjustment types:
// ADJUSTMENT_RAW      - 0 (no adjustments)
// ADJUSTMENT_SPLIT    - 1 (split-adjusted)
// ADJUSTMENT_DIVIDEND - 2 (dividend-adjusted)
// ADJUSTMENT_ALL      - 3 (both splits and dividends)
```

### Generate Asset List

```c
#include <AlpacaBrokerCommands.h>

function main()
{
    // Generate CSV for specific symbols
    brokerCommand(CREATE_ASSETLIST, "SPY,AAPL,MSFT,TSLA");

    // Or generate for all tradeable assets
    // brokerCommand(CREATE_ASSETLIST, 0);

    // Output: AssetAlpaca.csv in Log directory
}
```

## API Reference

### Implemented Zorro Broker API Functions

| Function | Description |
|----------|-------------|
| `BrokerOpen` | Initialize broker connection |
| `BrokerHTTP` | HTTP communication with Alpaca API |
| `BrokerLogin` | Authenticate with API credentials |
| `BrokerTime` | Get current market time |
| `BrokerAsset` | Retrieve asset information |
| `BrokerHistory2` | Download historical bar data |
| `BrokerBuy2` | Place buy orders |
| `BrokerTrade` | Manage trade operations |
| `BrokerCommand` | Execute plugin-specific commands |

### Supported Broker Commands

| Command | Description |
|---------|-------------|
| `GET_COMPLIANCE` | Get NFA compiance |
| `GET_MAXTICKS` | Get max ticks per request |
| `GET_MAXREQUESTS` | Get max requests per period |
| `GET_LOCK` | Get resource lock |
| `GET_POSITION` | Get position for asset |
| `GET_PRICETYPE` | Get current price type |
| `GET_VOLTYPE` | Get volume type |
| `SET_ORDERTEXT` | Set custom order text |
| `SET_SYMBOL` | Set trading symbol |
| `SET_ORDERTYPE` | Set order TIF type |
| `SET_PRICETYPE` | Set price type (quote/trade) |
| `SET_DIAGNOSTICS` | Enable diagnostic mode |
| `SET_ADJUSTMENT` | Set historical data adjustment |
| `IS_ASSET_FRACTIONABLE` | Check if asset supports fractional shares |
| `IS_ASSET_SHORTABLE` | Check if asset is shortable |
| `IS_ASSET_EASY_TO_BORROW` | Check borrow availability |
| `IS_ASSET_MARGINABLE` | Check margin eligibility |
| `CREATE_ASSETLIST` | Generate asset list CSV |

## Building from Source

See [BUILD.md](BUILD.md) for detailed build instructions.

### Quick Build

```bash
git clone https://github.com/kzhdev/alpaca_zorro_plugin.git
cd alpaca_zorro_plugin
cmake -B build -G "Visual Studio 17 2022" -A Win32
cmake --build build --config Release
```

## Troubleshooting

### Common Issues

**Problem**: Plugin doesn't load in Zorro
- **Solution**: Ensure DLL is in correct folder (`Plugin/` for x86, `Plugin64/` for x64)
- Verify `websocket_proxy` folder is also copied

**Problem**: WebSocket not connecting
- **Solution**: Check firewall settings
- Verify API credentials are correct
- Ensure `AlpacaUseWebsocket = 1` in config

**Problem**: "Invalid API Key" error
- **Solution**: Regenerate API keys in Alpaca dashboard
- Ensure using Paper keys for paper trading, Live keys for live trading

**Problem**: High CPU usage
- **Solution**: This is normal for WebSocket Proxy (lock-free design)
- If problematic, disable WebSocket: `AlpacaUseWebsocket = 0`

**Problem**: Orders not executing
- **Solution**: Check market hours
- Verify account has sufficient buying power
- Check if asset is tradeable (use `IS_ASSET_*` commands)

### Debug Logging

Enable detailed logging to diagnose issues:

```ini
# In ZorroFix.ini
AlpacaLogLevel = 5              # TRACE level
```

Logs are saved to: `<Zorro>/Log/`

## Contributing

Contributions are welcome! Please see [CONTRIBUTING.md](CONTRIBUTING.md) for guidelines.

### Development Setup

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Build and test thoroughly
5. Submit a pull request

## Support

- **Issues**: [GitHub Issues](https://github.com/kzhdev/alpaca_zorro_plugin/issues)
- **Discussions**: [GitHub Discussions](https://github.com/kzhdev/alpaca_zorro_plugin/discussions)
- **Alpaca Docs**: [Alpaca Documentation](https://alpaca.markets/docs/)
- **Zorro Docs**: [Zorro Manual](https://zorro-project.com/manual/)

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgments

- [Alpaca Markets](https://alpaca.markets/) for their excellent trading API
- [Zorro](https://zorro-project.com/) for the algorithmic trading platform
- All contributors who have helped improve this plugin

---

**Disclaimer**: Trading involves risk. This plugin is provided as-is without warranties. Always test strategies thoroughly in paper trading before going live.


