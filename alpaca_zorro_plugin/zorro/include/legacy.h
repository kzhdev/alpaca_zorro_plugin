// legacy definitions - include this file when you need them

#ifndef _LEGACY
#define _LEGACY
#include <default.c>

#define Slider1	g->vSlider[1]
#define Slider2	g->vSlider[2]
#define Slider3	g->vSlider[3]
#define CVolatility VolatilityC
#define loadHistory	assetHistory
#define DDScale	g->vCBIScale
#define OptCycle	g->nOptCycle
#define TRADING	TRADED
#define SHORTING	SHORTED
#define TR_BID		(1<<0)
#define NumTrades		g->numTrades
#define ExitTime	g->nExitTime
#define TradeExitTime	g->tr->nExitTime
#define Weekend	g->nWeekend
#define assetSwitch	assetSelect



// helper functions for limiting the open trades
TRADE* reverseLong(int MaxTrades)
{
// update the stops and profit targets of open trades
	var Price = priceClose();
	if(Stop > 0 && Stop < Price/2) // stop distance 
		exitLong(0,Price-Stop);
	else if(Stop >= Price/2) // stop limit
		exitLong(0,Stop); 
	if(TakeProfit > 0 && TakeProfit < Price/2) 
		exitLong(0,-(Price+TakeProfit));
	else if(TakeProfit >= Price/2) 
		exitLong(0,-TakeProfit);

// if MaxTrades is not reached, open a new trade
	if(NumOpenLong < MaxTrades || is(TRAINMODE))
		return enterLong();
// otherwise, close any opposite trades
	else if(Hedge <= 1)
		exitShort();
	return 0;
}

TRADE* reverseShort(int MaxTrades)
{
	var Price = priceClose();
	if(Stop > 0 && Stop < Price/2) 
		exitShort(0,Price+Stop); 
	else if(Stop >= Price/2)
		exitShort(0,Stop); 
	if(TakeProfit > 0 && TakeProfit < Price/2) 
		exitShort(0,-(Price-TakeProfit));
	else if(TakeProfit >= Price/2) 
		exitShort(0,-TakeProfit);

	if(NumOpenShort < MaxTrades || is(TRAINMODE))
		return enterShort();
	else if(Hedge <= 1)
		exitLong();
	return 0;
}

TRADE* reverseLong(int MaxTrades,void* f)
{
	reverseLong(0);
	if(NumOpenLong < MaxTrades)
		return enterLong(f);
	return 0;
}

TRADE* reverseShort(int MaxTrades,void* f)
{
	reverseShort(0);
	if(NumOpenShort < MaxTrades)
		return enterShort(f);
	return 0;
}

#endif
