# AlpacaZorroPlugin

**[Alpaca](http://alpaca.markets)** is a modern commission-free brokerage for algorithmic trading. Alpaca API allows a trading algo to access real-time price, place orders, and manage portfolio through either REST or streaming.

**AlpacaZorroPlugin** is a plugin for **[Zorro](https://zorro-project.com/)**, an institutional-grade development tool for financial research and an automatic trading system.

## Install

To install the plugin, download the [latest release](https://github.com/kzhdev/alpaca_zorro_plugin/releases/download/v1.2.1/AlpacaZorroPlugin_v1.2.1.zip), unzip, place the "include/AlpacaBrokerCommand.h" file into the **include** folder under Zorro's root path, and place the "Plugin/Alpaca.dll" file and the "Plugin/websocket_proxy" folder into the **Plugin** folder under Zorro's root path.

## How to Use

* First generate an API Key on the Alpaca website.
* In Zorro, select Alpaca.
* Enter the **API Key** in the **User ID** input box
* Enter the **Secret Key** in the **Password** input box.

## AlpacaPlugin Specific Config
Following optional Alpaca-specific configurations are added since V1.0.0. These configurations can be added in Zorro.ini or ZorroFix.ini (preferred) file.

  ```text
  AlpacaDataSource = 1                  // 0 = Alpaca, 1 = Polygon. Default to 0.
  AlpacaPaidDataPlan = 1	              // 0 = False (Use Basic Data Plan), 1 = True (Use Pro Data Plan). Default to 1.
  AlpacaLogLevel = 0                    // 0 = OFF, 1 = ERROR, 2 = WARNING, 3 = INFO, 4 = DEBUG, 5 = TRACE. Default to 0.
  AlpacaUseWebsocket = 0                // 0 = Not use Websocket, 1 = Use Websocket. Default to 1.
  AlpacaFractionalLotAmount = 0.001     // The LotAmount for fractionable asset. Default to 1.
  PolygonApiKey = "*************"       // Polygon ApiKey
  ```

  **AlpacaDataSource**
  Specify to use Alpaca market data or Polygon market data. By default, Alpaca market data will be used.

  **AlpacaPaidDataPlan**
  Specify which Alpaca Websocket endpoint will be used. By default, the paid data plan will be used.

  **AlpacaLogLevel**
  By default, the ApacaPlugin log is turned off unless the "diag" mode is enabled. AlpacaPlugin logging can be enabled through this config.

  **AlpacaUseWebsocket**
  Specify receiving price updates from Websocket or REST API.

  **AlpacaFractionalLotAmount**
  The LotAmount for a fractionable asset. Set this config to a number less than 1 to enable fractional quantity. When generating AssetList.csv, the LotAmount of any fractionable asset is set to the value of this configuration.
  **NOTE:** Fractional quantity only for Market and Day order type

  **PolygonApiKey**
  The Polygon ApiKey. If AlpacaDataSource is set to 1, the PolygonApiKey must be provided through this config, otherwise, Alpaca MarketData will be used.

## Features

* Support Alpaca MarketData V2 Websocket Real-Time Data. Multiple ZorroS instances can share one WebSocket connection through ZorroWebsocketProxy.<br/>
**NTOE:** ZorroWebsocketProxy has a lock-free design, it spins on one CPU core. High CPU usage is normal and expected.<br/>
Websocket can be turned off by AlpacaUseWebsocket config. If AlpacaUseWebsocket is set to 0, the last price will be polled from the Snapshot REST endpoint.

* Support **Limit**, **Market** order types

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
  // Use borkerCommand(SET_ORDERTYPE, tif) to change TimeInForce.
  // Valid TimeInForce value is
  //  ORDERTYPE_IOC - ICO
  //  ORDERTYPE_GTC - GTC
  //  ORDERTYPE_FOK - FOK (default)
  //  ORDERTYPE_DAY - DAY
  //  ORDERTYPE_OPG - OPG
  //  ORDERTYPE_CLS - CLS
  //
  // NOTE: brokerCommand(SET_ORDERTYPE, 0) will be ignored, this is because Zorro always calls brokerCommand(SET_ORDERTYPE, 0) before setting the limit price.

  #include "AlpacaBrokerCommands.h"
  brokerCommand(SET_ORDERTYPE, ORDERTYPE_DAY);  // set TIF to Day
  OrderLimit = 100.00;
  enterShort(5);    // Sell 5 lot Day order at limit price 100.00

  OrderLimit = 0;   // set order type back to Market
  brokerCommand(SET_ORDERTYPE, ORDERTYPE_CLS);  // set TIF to CLS
  enteryLong(5);    // Buy 5 lot MarketOnClose order
  ```

* Support LastQuote/LastTrade price type

  ```C++
  // By default, it use ask/bid price mode
  // Use brokerCommand(SET_PRICETYPE, 2) to change price type to trades
  brokerCommand(SET_PRICETYPE, 2) // Set price type to trades
  brokerCommand(SET_PRICETYPE, 1 /*or 0*/) // Set price type to ask/bid quote
  ```

* Support custom order text

  ```C++
  // Use borkderCommand(SET_ORDERTEXT, xxxx) to set a custom order text
  brokerCommand(SET_ORDERTEXT, "MyAlpacaOrder");  // "MyAlpacaOrder" will be added into ClientOrderId
  enterLong(5);
  ```

* Support Position retrieval

  ```C++
  // get position for specific Asset
  brokerCommand(GET_POSITION, "AAPL");
  ```

* Support Fractional Quantity
  To enable Franction Quantity support, add the AlpacaFractionalLotAmount config in Zorro.ini or ZorroFix.ini
  ``` C++
  // AlpacaFractionalLotAmount = 0.001 in ZorroFix.ini
  #include "AlpacaBrokerCommands.h"
  ...
  asset("TLSA");
  brokerCommand(SET_ORDERTYPE, ORDERTYPE_DAY);  // Fractional qty only for Market and Day order type
  LotAmount = 5;
  enterLong();      // Buy 0.005 shares TLSA
  ```

* Support [Polygon](https://polygon.io) market data

  After Alpaca’s departure from Polygon, Alpaca ApiKey no longer works for Polygon. AlpacaZorroPlugin kept the Polygon market data support. 
  **NOTE:** The Polygon ApiKey is moved into the Zorro config file.
  **NOTE:** Polygon free plan can't be used as an alternative market data source due to lack of last trade and last quote data.

* Generate AssetList file through custom borkerCommand
  
  ``` C++
  #include "AlpacaBrokerCommands.h"
  brokerCommand(CREATE_ASSETLIST, char *symbols);
  ```

  **symbols** - One or more symbols separated by a comma, or **0** for all tradeable symbols.
  An AssetAlpaca.csv file will be generated in the Log directory.

  ``` C++
  Exemple:
  // GenerateAlpacaAssetList.c
  #include "AlpacaBrokerCommands.h"
  function main() {
    brokerCommand(CREATE_ASSETLIST, "SPY,AAPL,MSFT,TSLA");  // Generate AssetsAlpaca.csv contains SPY, AAPL, MSFT, TSLA symbols
    //brokerCommand(CREATE_ASSETLIST, 0);   // Generate AssetsAplaca.csv contains all tradeable symbols /v2/assets endpoint. 
  }
  ```

* Get properties of current Asset
  ``` C++
  #include "AlpacaBorkerCommads.h"
  ....
  int fractionable = borkerCommand(IS_ASSERT_FRACTIONABLE, Asset);
  int shortable = borkerCommand(IS_ASSERT_SHORTABLE, Asset);
  int easyToBorrow = borkerCommand(IS_ASSERT_EASY_TO_BORROW, Asset);
  int marginable = borkerCommand(IS_ASSERT_EASY_TO_MARGINABLE, Asset);
  ```

* Following Zorro Broker API functions has been implemented:

  * BrokerOpen
  * BrokerHTTP
  * BrokerLogin
  * BrokerTime
  * BrokerAsset
  * BrokerHistory2
    * Alpaca only support M1, M5, M15 and D1 bars.
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

## TO-DO List

* Add target and stop order support

## [Build From Source](BUILD.md)

## Bug Report

If you find any issues or have any suggestions, please report them in GitHub [issues](https://github.com/kzhdev/alpaca_zorro_plugin/issues).


---

Thanks to [JetBrains](https://jb.gg/OpenSource/?from=alpaca_zorro_plugin) for donating product licenses to help develop **AlpacaZorroPlugin**


