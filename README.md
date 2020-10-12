# alpaca_zorro_plugin

**[Alpaca](http://alpaca.markets)** is a moden commision-free brokrage for algorithmic trading. Alpaca API allows a trading algo to access real-time price, place orders and manage potofolio through either REST or streaming.

**AlpacaZorroPlugin** is a plugin for **[Zorro](https://zorro-project.com/)**, an institutional-grade development tool fro financial research and automatic traiding system.

## Install
To install the plugin, download the latest release and place the Alpaca.dll file into the Plugin folder under Zorro's root folder.

## Use the Plgin
First generate a API Key in Alpaca website. In Zorro, select Alpaca. Enter the API Key in the "User ID" input box and enter the Secret Key in the "Password" input box.

## Plugin Capabilities
Following Zorro Broker API functions has been implemented:
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
   * GET_BROKERZONE
   * GET_MAXTICKS
   * GET_MAXREQUESTS
   * GET_LOCK
   * GET_POSITION
   * SET_ORDERTEXT
   * SET_SYMBOL
   * SET_MULTIPLIER
   * SET_ORDERTYPE
   * SET_PRICETYPE:
   * GET_UUID
   * SET_UUID
   * SET_DIAGNOSTICS:

## TO-DO List
* Add stop order support
* Support streaming to lower number of API requests.

## For Contributer
### Build
1. Download alpaca_zorro_plugin:

   ```git clone https://github.com/kzhdev/alpaca_zorro_plugin.git```
1. Download dependencies:

   ```cd alpaca_zorro_plugin```
   
   ```git submodule update --init```
1. Open the project in Visual Studio
### Bug Report
If you find any issue or have any suggestion, please report in GitHub [issues](https://github.com/kzhdev/alpaca_zorro_plugin/issues).