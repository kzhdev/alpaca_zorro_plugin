///////////////////////////////////////////////////////////////////
// Some self-defined candlestick patterns (alternatively use CDL_ functions)

#define open priceOpen
#define close priceClose
#define high priceHigh
#define low priceLow

#define candle_crossover(a,b,idx) (a(idx)>b(idx) && a(-labs(idx)-1)<b(-labs(idx)-1))

#define white(idx) (open(idx)<close(idx))
#define black(idx) (open(idx)>close(idx))

int candle_engulfing_white(int idx)
{
	idx = -labs(idx);	// make index negative
	if (white(idx) && open(idx)<low(idx-1) && close(idx)>high(idx-1))  
		return 1;
	else return 0;
}

int candle_engulfing_black(int idx)
{
	idx = -labs(idx);
	if (black(idx) && open(idx)>high(idx-1) && close(idx)<low(idx-1))  
		return 1;
	else return 0;
}

int candle_harami_white(int idx)
{
	idx = -labs(idx);	
	if (white(idx) && high(idx)<open(idx-1) && low(idx)>close(idx-1))  
		return 1;
	else return 0;
}

int candle_harami_black(int idx)
{
	idx = -labs(idx);	
	if (black(idx) && high(idx)<close(idx-1) && low(idx)>open(idx-1))  
		return 1;
	else return 0;
}

int candle_doji(int idx)
{
	double threshold = 0.1*(high(idx)-low(idx));
	if (fabs(open(idx)-close(idx)) < threshold)
		return 1;
	else return 0;
}

int candle_marubozu_white(int idx)
{
	double threshold = 0.1*(high(idx)-low(idx));
	if (white(idx) && (open(idx)>(high(idx)-threshold) && close(idx)<(low(idx)+threshold)))
		return 1;
	else return 0;
}

int candle_marubozu_black(int idx)
{
	double threshold = 0.1*(high(idx)-low(idx));
	if (black(idx) && (close(idx)>(high(idx)-threshold) && open(idx)<(low(idx)+threshold)))
		return 1;
	else return 0;
}

