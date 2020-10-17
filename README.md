# AlpacaZorroPlugin

**[Alpaca](http://alpaca.markets)** is a moden commision-free brokrage for algorithmic trading. Alpaca API allows a trading algo to access real-time price, place orders and manage potofolio through either REST or streaming.

**AlpacaZorroPlugin** is a plugin for **[Zorro](https://zorro-project.com/)**, an institutional-grade development tool fro financial research and automatic traiding system.

## Install

To install the plugin, download the latest release and place the Alpaca.dll file into the Plugin folder under Zorro's root folder.

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

* Support **FOK**, **IOC**, **GTC** TimeInForce types. See [Understand Orders](https://alpaca.markets/docs/trading-on-alpaca/orders/#time-in-force) for more info.

    ```C++
    // By default, enterLong/enterShort places an order with FOK TimeInfoForce type
    ```

* Support custom order text

    ````C++
    // Use borkderCommand(SET_ORDERTEXT, xxxx) to set a custom order text
    brokerCommand(SET_ORDERTEXT, "MyAlpacaOrder");  // "MyAlpacaOrder" will be added into ClientOrderId
    enterLong(5);
    ````

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
    * SET_ORDERTEXT
    * SET_SYMBOL
    * SET_ORDERTYPE
    * SET_PRICETYPE:
    * SET_DIAGNOSTICS:

## TO-DO List

* Add target and stop order support
* Add Polygon market data support
* Create unit tests
* Add streaming support to lower number of API requests. There is an issue where Alpaca currently support only 1 websocket per account. For multiple Zorro-S intances to work, AlpacaProxyAgent needs to be used.

## [To Contribute](CONTRIBUTING.md)