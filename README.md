# AlpacaZorroPlugin

**[Alpaca](http://alpaca.markets)** is a moden commision-free brokrage for algorithmic trading. Alpaca API allows a trading algo to access real-time price, place orders and manage potofolio through either REST or streaming.

**AlpacaZorroPlugin** is a plugin for **[Zorro](https://zorro-project.com/)**, an institutional-grade development tool fro financial research and automatic traiding system.

## Install

To install the plugin, download the [latest release](https://github.com/kzhdev/alpaca_zorro_plugin/releases/download/v0.2.3/AlpacaZorroPlugin_v0.2.4.zip) and place the Alpaca.dll file into the **Plugin** folder under Zorro's root path.

## How to Use

* First generate a API Key in Alpaca website.
* In Zorro, select Alpaca.
* Enter the **API Key** in the **User ID** input box
* Enter the **Secret Key** in the **Password** input box.

## Features

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

* Support [Polygon](https://polygon.io) market data (need live account)

  All Alpaca customers with live brokerage accounts can access various kinds of market data provided by Polygon. By default, the plugin uses Alpaca market data.
  
  **To Use Polygon:**
  In the User input, put both paper account API Key and live account API Key separated by '_' like following: **\<PaperAccountAPIKey or LiveAccountAPIKey>\_\<LiveAccountAPIKey>**

  **brokerCommand(2000, int usePolygon)** can also be used to switch the data source. When usePolygon = **0**, Alpaca market data will be used. Othersise, Polygon market data will be used. Zorro retrieves historical data right after logged in. User needs aware that after switching market data source, the history data and live data are came from different source.

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
* Add streaming support to lower number of API requests. There is an issue where Alpaca currently support only 1 websocket per account. For multiple Zorro-S intances to work, AlpacaProxyAgent needs to be used.

## [To Contribute](CONTRIBUTING.md)
