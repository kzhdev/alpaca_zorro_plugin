# AlpacaZorroPlugin

**[Alpaca](http://alpaca.markets)** is a moden commision-free brokrage for algorithmic trading. Alpaca API allows a trading algo to access real-time price, place orders and manage potofolio through either REST or streaming.

**AlpacaZorroPlugin** is a plugin for **[Zorro](https://zorro-project.com/)**, an institutional-grade development tool fro financial research and automatic traiding system.

## Windows Definder
After Windows Definder Updated to 1.339.981.0, zorro_websocket_proxy.exe detected as a **Trojan:Win32/Sabsik.FT.A!ml** incorrectly by Windows Definder on Windows Server 2019. zorro_websocket_proxy.exe has been send to Microsoft for analysis. It is totaly safe to use. To restore zorro_websocket_proxy.exe from quarantin, go to **Windows Security**, click **Virus & Thread Protection** -> **Thread history** ->  **Trojan:Win32/Sabsik.FT.A!ml** then click the **Restore** button.

## Install

To install the plugin, download the [latest release](https://github.com/kzhdev/alpaca_zorro_plugin/releases/download/v1.0.3/AlpacaZorroPlugin_v1.0.3.zip), unzip and place the Alpaca.dll file and the websocket_proxy folder into the **Plugin** folder under Zorro's root path.

## How to Use

* First generate a API Key in Alpaca website.
* In Zorro, select Alpaca.
* Enter the **API Key** in the **User ID** input box
* Enter the **Secret Key** in the **Password** input box.

## AlpacaPlugin Specific Config
Following optional Alpaca specific configurations are added since V1.0.0. These configurations can be added in Zorro.ini or ZorroFix.ini (preferred) file.

  ```text
  AlpacaDataSource = 1                  // 0 = Alpaca, 1 = Polygon. Default to 0.
  AlpacaPaidDataPlan = 1	            // 0 = False (Use Basic Data Plan), 1 = True (Use Pro Data Plan). Default to 1.
  AlpacaLogLevel = 0                    // 0 = OFF, 1 = ERROR, 2 = WARNING, 3 = INFO, 4 = DEBUG, 5 = TRACE. Default to 0.
  AlpacaUseWebsocket = 0                // 0 = Not use Websocket, 1 = Use Websocket. Default to 1.
  PolygonApiKey = "*************"       // Polygon ApiKey
  ```

  **AlpacaDataSource**
  Specify to use Alpaca MaketData or Polygon MarketData. By default, Alpaca MarketData will be used.

  **AlpacaPaidDataPlan**
  Specify which Alpaca Websocket enpoint will be used. By default, paid data plan will be used.

  **AlpacaLogLevel**
  By default, ApacaPlugin log is turned off unless "diag" mode is enabled. AlpacaPlugin logging can be enabled through this config.

  **AlpacaUseWebsocket**
  Specify receiving price update from Websocket or REST API.

  **PolygonApiKey**
  The Polygon ApiKey. If AlpacaDataSource set to 1, the PolygonApiKey must be provided through this config, otherwise Alpaca MarketData will be used.

## Features

* Support Alpaca MarketData V2 Websocket Real-Time Data. Multiple ZorroS instances can share one WebSocket connection through ZorroWebsocketProxy.<br/>
**NTOE:** ZorroWebsocketProxy has a lock-free design, it spins on one CPU core. High CPU usage is normal and expected.<br/>
Websocket can be turned off by AlpacaUseWebsocket config. If AlpacaUseWebsocket set to 0, last price will be polled from Snapshot REST endpoint.

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
  //  1 - ICO
  //  2 - GTC
  //  3 - FOK (default)
  //  4 - DAY
  //  5 - OPG
  //  6 - CLS
  //
  // NOTE: brokemand(SET_ORDERTYPE, 0) will be ignored, this is because Zorro always call brokerCommand(SET_ORDERTYPE, 0) before setting limit price.

  brokerCommand(SET_ORDERTYPE, 4);  // set TIF to Day
  OrderLimit = 100.00;
  enterShort(5);    // Sell 5 lot Day order at limit price 100.00

  OrderLimit = 0;   // set order type back to Market
  brokerCommand(SET_ORDERTYPE, 6);  // set TIF to CLS
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

* Support [Polygon](https://polygon.io) market data

  After Alpaca departured from Polygon, Alpaca ApiKey no longer works for Polygon. AlpacaZorroPlugin keeped the Polygon market data support. 
  **NOTE:** The Polygon ApiKey is movied into Zorro config file.
  **NOTE:** Polygon free plan can't be used as alternative market data source due to lack of last trade and last quote data.

* Generate AssetList file through custom borkerCommand
  
  ``` C++
  brokerCommand(2001, char *symbols);
  ```

  **symbols** - One or more symbols separated by comma. If symbols = **0**, all symbols will be included.
  An AssetAlpaca.csv file will be generated in the Log diredtory.

  ``` C++
  Exemple:
  // GenerateAlpacaAssetList.c
  function main() {
    brokerCommand(2001, "SPY,AAPL,MSFT,TSLA");  // Generate AssetsAlpaca.csv contains SPY, AAPL, MSFT, TSLA symbols
  }
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

If you find any issue or have any suggestion, please report in GitHub [issues](https://github.com/kzhdev/alpaca_zorro_plugin/issues).


---

Thanks to [JetBrains](https://jb.gg/OpenSource/?from=alpaca_zorro_plugin) for donating product licenses to help develop **AlpacaZorroPlugin**
