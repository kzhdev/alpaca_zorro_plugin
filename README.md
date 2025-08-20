# AlpacaZorroPlugin

The AlpacaZorroPlugin is a powerful integration between **[Alpaca](http://alpaca.markets)**, a brokerage providing real-time market data and trading capabilities, and **[Zorro](https://zorro-project.com/)**,
an institutional-grade development tool for financial research and automated trading. With this plugin, you can seamlessly trade using Alpaca within the Zorro environment.

## Installation

To install the plugin, follow these steps:

1. Download the [latest release](https://github.com/kzhdev/alpaca_zorro_plugin/releases).
2. Unzip the downloaded file.
3. Copy the `Alpaca.dll` file and the `websocket_proxy` folder into the `plugin` folder (or `plugin64` folder for 64bit build) under Zorro's root path.
4. Copy the `AlpacaBrokderCommands.h` file into the `include` folder under Zorro's root path.

## Usage

To use the plugin, follow these steps:

1. Generate an API Key on the Alpaca website.
2. Open Zorro, select Alpaca as the broker.
3. Enter the **API Key** in the **User ID** input box.
4. Enter the **Secret Key** in the **Password** input box.
5. Press **F2** to open the plugin settings dialog and adjust the logging level at runtime.

## AlpacaPlugin Specific Config

Since version 1.0.0, the AlpacaZorroPlugin supports several optional Alpaca-specific configurations, which can be added to either the `Zorro.ini` or `ZorroFix.ini` file (preferred):

  ```text
  AlpacaPaidDataPlan = 1                // 0 = False (Use Basic Data Plan), 1 = True (Use Pro Data Plan). Default to 1.
  AlpacaLogLevel = 0                    // 0 = OFF, 1 = ERROR, 2 = WARNING, 3 = INFO, 4 = DEBUG, 5 = TRACE. Default to 0.
  AlpacaLogType = 127                   // 1 = DEFAULT, 2 = ACCOUNT, 4 = BALANCE, 8 = POSITION, 16 = ORDER, 32 = HISTORY, 64 = MD, 128 = WEB_SOCKET_DATA,
                                        // 127 = DEFAULT | ACCOUNT | BALANCE | POSITION | ORDER | HISTORY | MD, 255 = ALL
  AlpacaUseWebsocket = 0                // 0 = Not use Websocket, 1 = Use Websocket. Default to 1.
  AlpacaFractionalLotAmount = 0.001     // The LotAmount for fractionable asset. Default to 1.
  ```

The Configurations are:

* **AlpacaPaidDataPlan**: Choose between free or paid Alpaca MarketData endpoint (default is the paid data plan).
* **AlpacaLogLevel**: Set the level of AlpacaPlugin logging (default is OFF).
* **AlpacaLogType**: Set which type of datt is logged (default is ALL except WEB_SOCKET_DATA).
* **AlpacaUseWebsocket**: Speficy whether to receive price updates via WebSocket or REST API (default is WebSocket).
* **AlpacaFractionalLotAmount**: Enable fractional quantity by setting this value to less than 1 (default is 1). Note that fractional quantity is only available for Market and Day order types.

## Features

* Support Alpaca MarketData V2 Websocket Real-Time Data. Multiple ZorroS instances can share one WebSocket connection through ZorroWebsocketProxy. 
  The Websocket can be turned off by the `AlpacaUseWebsocket` configuration. if `AlpacaUseWebsocket` is set to 0, the last price will be polled from the Snapshot REST endpoint.<br/>
  **NTOE:** ZorroWebsocketProxy has a lock-free design, it spins on one CPU core. High CPU usage is normal and expected.

* Support **Limit**, **Market** order types. 

  ```C++
  // By default, enterLong/enterShort places a Market order
  enterLong(5);   // long 5 lot at Market price
  enterShort(5);  // short 5 lot at Market price

  // Place a limit
  OrderLimit = 100.00;  // set a Limit price first
  enterLong(5);   // place 5 lot at 100.00
  ```

* Support **FOK**, **IOC**, **GTC**, **DAY**, **OPG**, **CLS** TimeInForce types. See [Understand Orders](https://alpaca.markets/docs/trading-on-alpaca/orders/#time-in-force) for more info.

  ```C++
  // By default, enterLong/enterShort places an order with FOK TimeInfoForce type
  // Use brokerCommand(SET_ORDERTYPE, tif) to change TimeInForce.
  // Valid TimeInForce value is
  //  ORDERTYPE_IOC - ICO
  //  ORDERTYPE_GTC - GTC
  //  ORDERTYPE_FOK - FOK (default)
  //  ORDERTYPE_DAY - DAY
  //  ORDERTYPE_OPG - OPG
  //  ORDERTYPE_CLS - CLS
  //
  // NOTE: brokerCommand(SET_ORDERTYPE, 0) will be ignored, this is because Zorro always calls brokerCommand(SET_ORDERTYPE, 0) before setting the limit price.

  #include <AlpacaBrokerCommands.h>
  brokerCommand(SET_ORDERTYPE, ORDERTYPE_DAY);  // set TIF to Day
  OrderLimit = 100.00;
  enterShort(5);    // Sell 5 lot Day order at limit price 100.00

  OrderLimit = 0;   // set order type back to Market
  brokerCommand(SET_ORDERTYPE, ORDERTYPE_CLS);  // set TIF to CLS
  enteryLong(5);    // Buy 5 lot MarketOnClose order
  ```

* Support LastQuote/LastTrade price type.

  ```C++
  // By default, it use ask/bid price mode
  // Use brokerCommand(SET_PRICETYPE, 2) to change price type to trades
  brokerCommand(SET_PRICETYPE, 2) // Set price type to trades
  brokerCommand(SET_PRICETYPE, 1 /*or 0*/) // Set price type to ask/bid quote
  ```

* Support custom order text

  ```C++
  // Use brokerCommand(SET_ORDERTEXT, xxxx) to set a custom order text
  brokerCommand(SET_ORDERTEXT, "MyAlpacaOrder");  // "MyAlpacaOrder" will be added into ClientOrderId
  enterLong(5);
  ```

* Support Position retrieval

  ```C++
  // get position for specific Asset
  brokerCommand(GET_POSITION, "AAPL");
  ```

* Support Fractional Quantity
  To enable Fractional Quantity support, add the AlpacaFractionalLotAmount config in Zorro.ini or ZorroFix.ini
  ``` C++
  // AlpacaFractionalLotAmount = 0.001 in ZorroFix.ini
  #include <AlpacaBrokerCommands.h>
  ...
  asset("TLSA");
  brokerCommand(SET_ORDERTYPE, ORDERTYPE_DAY);  // Fractional qty only for Market and Day order type
  LotAmount = 5;
  enterLong();      // Buy 0.005 shares TLSA
  ```

* Generate AssetList file through custom brokerCommand
  
  ``` C++
  #include <AlpacaBrokerCommands.h>
  brokerCommand(CREATE_ASSETLIST, char *symbols);
  ```

  **symbols** - One or more symbols separated by a comma, or **0** for all tradeable symbols.
  An AssetAlpaca.csv file will be generated in the Log directory.

  ``` C++
  Example:
  // GenerateAlpacaAssetList.c
  #include <AlpacaBrokerCommands.h>
  function main() {
    brokerCommand(CREATE_ASSETLIST, "SPY,AAPL,MSFT,TSLA");  // Generate AssetsAlpaca.csv contains SPY, AAPL, MSFT, TSLA symbols
    //brokerCommand(CREATE_ASSETLIST, 0);   // Generate AssetsAplaca.csv contains all tradeable symbols /v2/assets endpoint. 
  }
  ```

* Get properties of current Asset
  ``` C++
  #include <AlpacaBrokerCommands.h>
  ....
  int fractionable = brokerCommand(IS_ASSET_FRACTIONABLE, Asset);
  int shortable = brokerCommand(IS_ASSET_SHORTABLE, Asset);
  int easyToBorrow = brokerCommand(IS_ASSET_EASY_TO_BORROW, Asset);
  int marginable = brokerCommand(IS_ASSET_EASY_TO_MARGINABLE, Asset);
  ```

* Set Historical Bar Adjustment 
  ``` C++
  #include <AlpacaBrokerCommands.h>
  ....
  brokerCommand(SET_ADJUSTMENT, ADJUSTMENT_RAW);

  // Valid Adjustment value are
  //  ADJUSTMENT_RAW      - 0
  //  ADJUSTMENT_SPLIT    - 1
  //  ADJUSTMENT_DEVEDEND - 2
  //  ADJUSTMENT_ALL      - 3
  ```

* Following Zorro Broker API functions have been implemented:

  * BrokerOpen
  * BrokerHTTP
  * BrokerLogin
  * BrokerTime
  * BrokerAsset
  * BrokerHistory2
  * BrokerBuy2
  * BrokerTrade
  * BrokerSell2
  * BrokerCommand
    * GET_COMPLIANCE
    * GET_MAXTICKS
    * GET_MAXREQUESTS
    * GET_LOCK
    * GET_POSITION
    * GET_PRICETYPE
    * GET_VOLTYPE
    * SET_ORDERTEXT
    * SET_SYMBOL
    * SET_ORDERTYPE
    * SET_PRICETYPE:
    * SET_DIAGNOSTICS:

## [Build From Source](BUILD.md)

## Bug Report

If you find any issues or have any suggestions, please report them in GitHub [issues](https://github.com/kzhdev/alpaca_zorro_plugin/issues).


