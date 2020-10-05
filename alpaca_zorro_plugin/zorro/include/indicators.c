// Useful Indicators ////////////////////////////
// (c) oP group 2010
/////////////////////////////////////////////////
#ifndef indicators_c
#define indicators_c

#ifdef INDICATORS_H
// Highest High, lowest Low
var HH(int Period,int Offset);
var LL(int Period,int Offset); 

// Min/Max based volatility
var VolatilityMM(var* Data,int TimePeriod,int EMAPeriod);

// Chaikin volatility
var VolatilityC(int TimePeriod,int EMAPeriod);

// SD based volatility
var Volatility(var* Data,int Period);

// ATR based on SMA
var ATRS(int Period);

// Zorro's Market Meanness Index
var MMI(vars Data,int TimePeriod);

// Bill Williams Fractal Indicator
var FractalLow(var* Data,int Period);
var FractalHigh(var* Data,int Period);

// Donchian Channel, Donchian Oscillator
var DChannel(int Period);
var DCOsc(var* Data,int Period);

// Bollinger Bands Oscillator
var BBOsc(var* Data,int Period,var NbDev,int MAType);

// Trend Strength Index, from Engineering Returns 
var TSI(var* Data,int Period);

// High, Low, Open, Close of a trading day in a certain time zone
var dayHigh(int zone,int day);
var dayLow(int zone,int day);
var dayClose(int zone,int day);
var dayOpen(int zone,int day);
var dayPivot(int zone,int day);

// Filters 
var FIR3(var* Data);
var FIR4(var* Data);
var FIR6(var* Data);

// normalize a value to the -1..+1 range
var Normalize(var* Data,int Period);

// Fisher Transform
var Fisher(var* Data);

// Fisher Transform with normalizing
var FisherN(var* Data,int Period);

// Inverse Fisher Transform
var FisherInv(var* Data);

// 3-pole Butterworth filter, for super-smoothing
var Butterworth(var *Data,int Cutoff);

// Ehler's smoothing filter, 2-pole Butterworth * SMA
var Smooth(var *Data,int Cutoff);

// Ehler's roofing filter
var Roof(var* Data,int CutOffLow,int CutOffHigh);

// Automatic gain control
//var AGC(var *Data, int Period);
var AGC(var *Data, var alpha);

// 2-pole Highpass filter
var HighPass2(var* Data,int CutoffPeriod); 

// Predictive Stochastic
var StochEhlers(var* Data,int TimePeriod,int CutOffLow,int CutOffHigh);

// Smoothed Momentum
var SMom(var *Data,int Period,int Cutoff);

// Zero-lag Moving Average by John Ehlers
extern var rError, rEMA;
var ZMA(var* Data,int Period);

// Center Of Gravity Oscillator by John Ehlers
var CGOsc(var* Data,int Period);

// Relative Vigor Index by John Ehlers
var RVI(int Period);

// Hull Moving Average by Alan Hull
var HMA(var* Data,int Period);

// return the number of rising/falling bars
int NumRiseFall(var* Data,int Period);
int NumWhiteBlack(var vBody,int Offset,int Period);
int NumInRange(var* Low,var* High,var vMin,var vMax,int Period);

var ProfitFactor(var* Data,int Length);

// Spearman trend indicator, S&C 2/2011
var Spearman(var* Data,int Period);

// Fractal Dimension
var FractalDimension(var* Data,int Period);

// Hurst Exponent
var Hurst(var* Data,int Period);

// statistical moment (1 = mean, 2 = variance, 3 = skewness, 4 = kurtosis)
extern var rMean;
var Moment(var* Data,int Period,int n);

// Alligator
extern var rRed, rGreen, rBlue;
var Alligator(var* Data);

// Awesome Oscillator
var AO(var* Data);

// Accelerator Oscillator: AO - SMA(AO,5)
var AC(var* Data);

// Accumulation / Distribution Oscillator
var ADO();

// Ehler's universal oscillator, from S&C 1/2015
var UO(var *Data,int Cutoff);

// Ehlers' Decycler, S&C 9/2015
var Decycle(var* Data,int Period);

// Haikin Ashi
var HAClose();
var HAOpen();
var HAHigh();
var HALow();

// Laguerre filter
var Laguerre(var *Data, var alpha);

var Coral(var *Data);

var IBS();

var ALMA(var *Data, int TimePeriod, int sigma, var offset);

var SIROC(var* Data, int TimePeriod, int EMAPeriod); 

var DPO(var* Data, int TimePeriod);

var Keltner(var* Data, int TimePeriod, var Factor);

var SAR(var AFstep,var AFmin,var AFmax);

extern var rPeak,rSlope,rSign,rLength;
var ZigZag(vars Data,var Depth,int Length,long Color);

extern var rTenkan, rKijun, rSenkouA, rSenkouB;
var Ichimoku(int PeriodTenkan,int PeriodKijun,int PeriodSenkou,int Offset);
var Chikou(int Shift);

var ChandelierLong(int TimePeriod,var Multipl);
var ChandelierShort(int TimePeriod,var Multipl);

var ShannonGain(var* Data,int Period);
var ShannonEntropy(var *Data,int Length,int PatternSize);

var CI(int TimePeriod);

var move(vars Data,int Length,int MovePeriod,var Percent);

var season(vars Data,int TimePeriod,int Horizon,int Mode);

// currency strength
void ccyReset();
void ccySet(var Strength);
var ccyStrength(char* Currency);
char* ccyMax();
char* ccyMin();

// Wave generators for filter testing
var genSine(var Period1, var Period2);
var genSquare(var Period1, var Period2);
var genNoise();

#ifdef OVERLOADED
// overloaded indicators 
var ATR(var *Open,var *High,var *Low,var *Close,int TimePeriod);
var EMA(var* Data,var alpha);
var MinusDI(var *Open,var *High,var *Low,var *Close,int TimePeriod);
var PlusDI(var *Open,var *High,var *Low,var *Close,int TimePeriod);
var MinusDM(var *Open,var *High,var *Low,var *Close,int TimePeriod);
var PlusDM(var *Open,var *High,var *Low,var *Close,int TimePeriod);
var Stoch(var *Open,var *High,var *Low,var *Close,
	int FastK_Period,int SlowK_Period,int SlowK_MAType,int SlowD_Period,int SlowD_MAType);
var StdDev(var* Data,int TimePeriod);
var Variance(var* Data,int TimePeriod);
var ALMA(var *Data, int TimePeriod);

var Ichimoku() { return Ichimoku(0,0,0,0); }
var Chikou() { return Chikou(26); }
var SAR(var AFstep,var AFmax) { return SAR(AFstep,AFstep,AFmax); }
var Fisher(var* Data,int Period) { return FisherN(Data,Period); }
var AGC(var *Data, int Period);
var HH(int Period) { return HH(Period,0); }
var LL(int Period) { return LL(Period,0); }

var genSine(var Period) { return genSine(Period,Period); }
var genSquare(var Period) { return genSquare(Period,Period); }
#endif

#else
// Helper functions and definitions /////////////

// percentage of a variable within a band
var percent(var a,var l,var h) { 
	if(l == h) return 50.;
	return 100.*(a-l)/(h-l); 
}

// clip away the center range of a percent value
var percentClip(var a,var border)
{
// at the lower end, return the difference to 0
	if(a < border) return a-border; 
// at the upper end, return the difference to 100
	if(a > 100.-border) return a-(100.-border);
	return 0.; // center area
} 

// convert a smoothing factor to a period
//inline int smoothT(var a) { return (int)(2./a-1.); }
inline var smoothF(int period) { return 2./(period+1); }

//inline var deg(var x) { return (180./PI)*x; }

// Indicators /////////////////////////////////////////
int g_count;

// Highest High
var HH(int Period,int Offset) 
{ 
	var vHH = 0.;
	for(g_count = Offset; g_count < Period+Offset; g_count++)
		vHH = Max(vHH,priceHigh(g_count));
	return vHH;
}

//Lowest Low
var LL(int Period,int Offset) 
{ 
	var vLL = 999999.;
	for(g_count = Offset; g_count < Period+Offset; g_count++) {
		var Price = priceLow(g_count);
		if(Price > 0.) vLL = Min(vLL,Price);
	}
	if(vLL > 999998.) vLL = 0.;
	return vLL;
}

// Simple ATR based on SMA
var ATRS(int Period)
{
	Period = Max(1,Period);
	var vATRS =  0;
	for(g_count = 0; g_count < Period; g_count++)
		vATRS += priceHigh(g_count)-priceLow(g_count);
	return vATRS/Period;
}

// Zorro's Market Meanness Index
var MMI(var* Data,int TimePeriod)
{
	TimePeriod = Min(TimePeriod,1000);
	var m = Median(Data,TimePeriod);
	TimePeriod = Min(TimePeriod,g->nBar);
	if(TimePeriod <= 2) return 75;
	int i, nh=0, nl=0;
	for(i=1; i<TimePeriod; i++) {
		if(Data[i] > m && Data[i] > Data[i-1])
			nl++;
		else if(Data[i] < m && Data[i] < Data[i-1])
			nh++;
	}
	return 100.*(nl+nh)/(TimePeriod-1);
}

// Bill Williams Fractal Indicator
var FractalLow(var* Data,int Period)
{
	if(Period < 3) return *Data;
	int Center = (Period-1)/2;
	if(MinIndex(Data,Period) == Center)
		return Data[Center];
	else return 0.;
}

var FractalHigh(var* Data,int Period)
{
	if(Period < 3) return *Data;
	int Center = (Period-1)/2;
	if(MaxIndex(Data,Period) == Center)
		return Data[Center];
	else return 0.;
}

// Donchian Channel
var DChannel(int Period)
{
	rRealUpperBand = HH(Period,0);
	rRealLowerBand = LL(Period,0);
	return rRealUpperBand;
}

// Donchian Oscillator
var DCOsc(var* Data,int Period)
{
	DChannel(Period);
	return percent(Data[0],rRealLowerBand,rRealUpperBand);
}

// Bollinger Bands Oscillator
var BBOsc(var* Data,int Period,var NbDev,int MAType)
{
	BBands(Data,Period,NbDev,NbDev,MAType);
	return percent(Data[0],rRealLowerBand,rRealUpperBand);
}

// Trend Strength Index, from Engineering Returns 
var TSI(var* Data,int Period)
{
	var* ratio = series(abs(Data[0]-Data[Period])/ATR(Period),0);
	var* TSI = series(SMA(series(SMA(ratio,Period),0),10*Period),0);
	SETSERIES(Data,Period);
	return *TSI;
}

// Accumulation/distribution indicator
var ADO() 
{
	return (2*priceClose(0)-priceHigh(0)-priceLow(0))
		/Max(0.0001,priceHigh(0)-priceLow(0));
}

int skipWeekend(int day) 
{
	int d = dow(0)-day;
	while(d < 0) d += 7;
	if(d == SATURDAY) day += 1;
	if(d == SUNDAY) day += 2;
	return day;
}

// High, Low, Open, Close of a trading day in a certain time zone
var dayHigh(int zone,int day)
{
	if(is(INITRUN)) return 0;
	day = skipWeekend(day);
	int evening = timeOffset(zone,day,g->nEndMarket/100,g->nEndMarket%100);
	int morning = timeOffset(zone,day,g->nStartMarket/100,g->nStartMarket%100);
	if(evening >= morning) return priceHigh(evening); 
	var vHH = 0;
	for(g_count = evening; g_count <= morning; g_count++)
		vHH = Max(vHH,priceHigh(g_count));
	return vHH;
}

var dayLow(int zone,int day)
{
	if(is(INITRUN)) return 0;
	day = skipWeekend(day);
	int evening = timeOffset(zone,day,g->nEndMarket/100,g->nEndMarket%100);
	int morning = timeOffset(zone,day,g->nStartMarket/100,g->nStartMarket%100);
	if(evening >= morning) return priceLow(evening);
	var vLL = 999999;
	for(g_count = evening; g_count <= morning; g_count++)
		vLL = Min(vLL,priceLow(g_count));
	return vLL;
}

var dayClose(int zone,int day)
{
	if(is(INITRUN)) return 0;
	day = skipWeekend(day);
	return priceClose(timeOffset(zone,day,g->nEndMarket/100,g->nEndMarket%100)); 
}

var dayOpen(int zone,int day)
{
	if(is(INITRUN)) return 0;
	day = skipWeekend(day);
	return priceOpen(timeOffset(zone,day,g->nStartMarket/100,g->nStartMarket%100)); 
}

var dayPivot(int zone,int day)
{
	if(is(INITRUN)) return 0;
	return (dayHigh(zone,day)+dayLow(zone,day)+dayClose(zone,day))/3;
}

//"Internal Bar Strength"
var IBS()
{
	var Low = priceLow(0);

	if(priceHigh(0) == Low) return 1.;
	return (priceClose(0) - Low)/(priceHigh(0) - Low);
}

// Filters /////////////////////////////////////////////

var FIR3(var* Data)
{
	return (Data[0]+2.*Data[1]+Data[2])/4.;
}

var FIR4(var* Data)
{
	return (Data[0]+2.*Data[1]+2.*Data[2]+Data[3])/6.;
}

var FIR6(var* Data)
{
	return (Data[0]+2.*Data[1]+3.*Data[2]+3.*Data[3]+2.*Data[4]+Data[5])/12.;
}

// normalize a value to the -1..+1 range
var Normalize(var* Data,int Period)
{
	Period = Max(2,Period);
	var vMin = MinVal(Data,Period);
	var vMax = MaxVal(Data,Period);
	if(vMax>vMin) 
		return 2.*(*Data-vMin)/(vMax-vMin) - 1.;
	else return 0.;
}

// Fisher Transform
var Fisher(var* Data)
{
	var v = Clamp(Data[0],-0.998,0.998);
	return 0.5*log((1.+v)/(1.-v));
}

// Normalized Fisher transform
var FisherN(var* Data,int Period)
{
	var *Value = series(*Data,2), *FN = series(*Data,2);
	Value[0] = 0.33*Normalize(Data,Period) + 0.67*Value[1];
	return FN[0] = Fisher(Value) + 0.5*FN[1];
}

// Inverse Fisher Transform
var FisherInv(var* Data)
{
	var Exp = exp(2.*Data[0]);
	return (Exp-1.)/(Exp+1.);
}

// 3-pole Butterworth filter
var Butterworth(var *Data,int Cutoff)
{
	if(!Cutoff) Cutoff = 8;
	var a = exp(-PI / Cutoff);
	var b = 2*a*cos(1.738*PI / Cutoff);
	var c = a*a;
	var c1 = b + c;
	var c2 = -(c + b*c);
	var c3 = c*c;
	var c0 = 1 - c1 - c2 - c3;

	var* Filt = series(*Data,4);
	SETSERIES(Data,0);
	return Filt[0] = c0*Data[0] + c1*Filt[1] + c2*Filt[2] + c3*Filt[3];
}

// Automatic gain control
var AGC1(var *Data, int Period);
var AGC(var *Data, var alpha)
{
	if(alpha > 1.) return AGC1(Data,(int)alpha);	// for calling with optimize()
	vars G = series(Data[0],2);
	G[0] = Max(abs(Data[0]),alpha*G[1]);
	SETSERIES(Data,0);
	if(G[0] == 0.) return 0.;
	return Data[0]/G[0];
}

var AGC1(var *Data, int Period)
{
	var a = 0.991;
	if(Period > 0) a = pow(0.841,2./Period);
	return AGC(Data,a);
}

// Highpass filter by John F. Ehlers, converted by DdlV
// http://www.mesasoftware.com/technicalpapers.htm
var HighPass2(var* Data,int CutoffPeriod) 
{
// alpha1 = (Cosine(.707*360 / 48) + Sine (.707*360 / 48) - 1) / Cosine(.707*360 / 48);
// HP = (1 - alpha1 / 2)*(1 - alpha1 / 2)*(Close - 2*Close[1] + Close[2]) + 2*(1 - alpha1)*HP[1] - (1 - alpha1)*(1 - alpha1)*HP[2];
	if(!CutoffPeriod) CutoffPeriod = 48;
	var a	= (0.707*2*PI)/CutoffPeriod;
	var alpha1 = 1.+(sin(a)-1.)/cos(a);
	var b	= 1.-alpha1/2.;
	var c	= 1.-alpha1;
	
	var* HP = series(0,3);
	SETSERIES(Data,0);
	return HP[0] = b*b*(Data[0]-2*Data[1]+Data[2])
		+2*c*HP[1]
		-c*c*HP[2];
}

// Ehlers' Decycler, S&C 9/2015
var Decycle(var* Data,int Period)
{
	return Data[0]-HighPass2(Data,Period);
}

// Ehlers' smoothing filter, 2-pole Butterworth * SMA
var Smooth(var *Data,int Cutoff)
{
	if(!Cutoff) Cutoff = 10;
	var f = (1.414*PI) / Cutoff;
	var a = exp(-f);
	var c2 = 2*a*cos(f);
	var c3 = -a*a;
	var c1 = 1 - c2 - c3;

	var* Filt = series(*Data,4);
	SETSERIES(Data,0);
	return Filt[0] = c1*(Data[0]+Data[1])*0.5 + c2*Filt[1] + c3*Filt[2];
}

var Roof(var* Data,int CutOffLow,int CutOffHigh)
{
	vars HP = series(HighPass2(Data,CutOffHigh),2);
	var R = Smooth(HP,CutOffLow);
	SETSERIES(Data,0);
	return R;
}

// Ehler's universal oscillator, from S&C 1/2015
var UO(var *Data,int Cutoff)
{
	vars WhiteNoise = series((Data[0]-Data[2])/2.,2);
	vars SWN = series(Smooth(WhiteNoise,Cutoff),2);
	var A = AGC(SWN,0.991);
	SETSERIES(Data,0);
	return A;
}

// Predictive Stochastic by John F. Ehlers, converted by DdlV
// http://www.mesasoftware.com/technicalpapers.htm
var StochEhlers(var* Data,int TimePeriod,int CutOffLow,int CutOffHigh) 
{
	if(!TimePeriod) TimePeriod = 20;
	if(!CutOffLow) CutOffLow = 10;
	if(!CutOffHigh) CutOffHigh = CutOffLow;
	checkLookBack(TimePeriod);

	var* HP = series(HighPass2(Data,CutOffHigh),3);
	var* RS = series(Butterworth(HP,CutOffLow),0);
	
	var Highest = MaxVal(RS,TimePeriod);
	var Lowest = MinVal(RS,TimePeriod);
	var* SE = series(0,3);
	if(Highest == Lowest)
		SE[0] = 0;
	else
		SE[0] = (RS[0]-Lowest)/(Highest-Lowest);

	var B = Butterworth(SE,CutOffLow);
	SETSERIES(Data,TimePeriod);
	return B;
}

// Smoothed Momentum
var SMom(var *Data,int Period,int Cutoff)
{
	checkLookBack(Period);
	var *Diff = series(Data[0]-Data[Period],2);
	SETSERIES(Data,Period);
	return Butterworth(Diff,Cutoff);
}

// Zero-lag Moving Average by John Ehlers
var rError;
var rEMA;
var ZMA(var* Data,int Period)
{
	Period = Max(Period,1);
	var a = smoothF(Period);
	rEMA = EMA1(Data,a); 
	var *vZMA = series(*Data,2);
	rError = 1000000;
	var Gain,GainLimit=5,BestGain=0;
	for(Gain=-GainLimit; Gain<GainLimit; Gain += 0.1)
	{
		vZMA[0] = a*(rEMA + Gain*(Data[0]-vZMA[1])) + (1-a)*vZMA[1];
		var Error = Data[0] - vZMA[0];
		if(abs(Error) < rError) {
			rError = abs(Error);
			BestGain = Gain;
		}
	}
	SETSERIES(Data,Period);
	return vZMA[0] = a*(rEMA + BestGain*(Data[0]-vZMA[1])) + (1-a)*vZMA[1];
}

// Center Of Gravity Oscillator
var CGOsc(var* Data,int Period)
{
	var Num=0., Denom=0.;
	for(g_count = 0; g_count < Period; g_count++) {
		Num += (g_count+1)*Data[g_count];
		Denom += Data[g_count];
	}
	SETSERIES(Data,Period);
	if(Denom > 0.)
		return -Num/Denom + 0.5*(Period+1);
	else
		return 0.;	
}

// Relative Vigor Index
var RVI(int Period)
{
	var *CO = series(FIR4(series(priceClose(0)-priceOpen(0),0)),0);
	var *HL = series(FIR4(series(priceHigh(0)-priceLow(0),0)),0);
	var Denom = SMA(HL,Period);
	if(Denom == 0.) return 0.;
	return SMA(CO,Period) / Denom;
}

// Hull Moving Average
var HMA(var* Data,int n)
{
	int Period = sqrt((var)n)+0.5;
	var H = WMA(series(2*WMA(Data,0.5*n+0.5)-WMA(Data,n),0),Period);
	SETSERIES(Data,Period);
	return H;
}

// Alligator
var rRed, rGreen, rBlue;
var Alligator(var* Data)
{
	rBlue = SMA(Data+5,13);
	rRed = SMA(Data+2,8);
	rGreen = SMA(Data,5);
	SETSERIES(Data,13);
	return rRed;
}

// Awesome Oscillator
var AO(var* Data) 
{ 
	var R = SMA(Data,5)-SMA(Data,34); 
	SETSERIES(Data,34);
	return R;
}

// Haiken Ashi
var HAClose() { return (priceOpen(0)+priceHigh(0)+priceLow(0)+priceClose(0))/4; }
var HAOpen() { return (priceOpen(1)+priceClose(1))/2; }
var HAHigh() { return Max(priceHigh(0),Max(HAOpen(),HAClose())); }
var HALow() { return Min(priceLow(0),Min(HAOpen(),HAClose())); }

// Accelerator Oscillator: AO - SMA(AO,5)
var AC(var* Data) 
{ 
	SETSERIES(Data,5);
	return (4*AO(Data)-AO(Data+1)-AO(Data+2)-AO(Data+3)-AO(Data+4))/5.;
}

// Laguerre filter
var Laguerre(var *Data, var alpha)
{
   vars L = series(Data[0],8);
	SETSERIES(Data,0);

   L[0] = alpha*Data[0] + (1-alpha)*L[1];
   L[2] = -(1-alpha)*L[0] + L[1] + (1-alpha)*L[2+1];
   L[4] = -(1-alpha)*L[2] + L[2+1] + (1-alpha)*L[4+1];
   L[6] = -(1-alpha)*L[4] + L[4+1] + (1-alpha)*L[6+1];
   return (L[0]+2*L[2]+2*L[4]+L[6])/6;
}

var Coral(vars Data)
{
	return T3(Data,60,0.4);
}

// ALMA, Arnaud Legoux Moving Average 
// © 2013 acidburn @ Zorro Trader Forum
var ALMA(var *Data, int Period, int sigma, var offset)
{
	checkLookBack(Period);
	if(sigma == 0) sigma = 6;
	if(offset == 0.) offset = 0.85;
	Period = Max(Period,2);
	var m = floor(offset * (Period - 1));
	var s = Period / sigma;
	var alma = 0., wSum = 0.;
	int i;

	for (i = 0; i < Period; i++) {
		var w = exp(-((i - m) * (i - m)) / (2 * s * s));
		alma += Data[Period - 1 - i] * w;
		wSum += w;
	}
	SETSERIES(Data,Period);	
	return alma / wSum;
}

var ALMA1(var *Data, int TimePeriod) { 
	return ALMA(Data,TimePeriod,0,0.); 
}

var SIROC(var* Data, int TimePeriod, int EMAPeriod) 
{
	if(!EMAPeriod) EMAPeriod = 13;
	checkLookBack(TimePeriod+EMAPeriod);
	var PrevEMA = EMA(Data+TimePeriod,EMAPeriod);
	if(PrevEMA == 0.) return 0.;
	return 100.*(EMA(Data,EMAPeriod)-PrevEMA)/PrevEMA; 
}

var DPO(var* Data, int TimePeriod) 
{
	int HalfPeriod = TimePeriod/2 + 1;
	checkLookBack(TimePeriod+HalfPeriod);
	return Data[0] - SMA(Data+HalfPeriod,TimePeriod); 
}

var Keltner(var* Data, int TimePeriod, var Factor)
{
	rRealMiddleBand = SMA(Data,TimePeriod);
	var Band = Factor * ATRS(TimePeriod);
	rRealUpperBand = rRealMiddleBand + Band;
	rRealLowerBand = rRealMiddleBand - Band;
	return rRealMiddleBand;
}

var rSign;
var SAR(var AFstep,var AFmin,var AFmax)
{
#ifndef NIX //_DEBUG
#define _newHigh	v[0]
#define _prevHigh	v[1]
#define _newLow	v[2]
#define _prevLow	v[3]
#define _isLong	v[4]
#define _af			v[5]
#define _ep			v[6]
#define _sar		v[7]
	var* v = series(0,-8); // static series
#else
	static var _newHigh,_prevHigh,_newLow,_prevLow,
		_isLong,_af,_ep,_sar;
#endif

	_prevLow = _newLow;
	_prevHigh = _newHigh;
	_newLow  = priceLow(0);
	_newHigh = priceHigh(0);
	var result = 0.;

	if(is(FIRSTRUN)) {
		_isLong = priceClose(0) - priceOpen(0);
		_prevLow = _newLow;
		_prevHigh = _newHigh;
		if(_isLong > 0.) {
			_ep  = _newHigh;
			_sar = _newLow;
		} else {
			_ep  = _newLow;
			_sar = _newHigh;
		}
		_af = AFmin;
		return _sar;
	}
	
  if(_isLong > 0.)
  {  
// Switch to short if the low penetrates the SAR value.
		if(_newLow <= _sar)
		{
// Switch and override the SAR with the _ep
// Make sure the SAR is within yesterday's and today's range.
			_isLong = -1.;
			_sar = Max(_ep,Max(_prevHigh,_newHigh));
			result = _sar;
			
// Adjust _af and _ep, Calculate the new SAR
			_af = AFmin;
			_ep = _newLow;
			_sar += _af * (_ep - _sar);
			
// Make sure the new SAR is within yesterday's and today's range.
			_sar = Max(_sar,Max(_prevHigh,_newHigh));
		}
		else // No switch
		{
// Output the SAR (was calculated in the previous iteration)
			result = _sar;
			
// Adjust _af and _ep
			if(_newHigh > _ep) {
				_ep = _newHigh;
				_af = Min(_af+AFstep,AFmax);
			}
			
// Calculate the new SAR
			_sar += _af * (_ep - _sar);
	
// Make sure the new SAR is within yesterday's and today's range.
			_sar = Min(_sar,Min(_prevLow,_newLow));
		}
	}
	else
	{
		if(_newHigh >= _sar)
		{
			_isLong = 1.;
			_sar = Min(_ep,Min(_prevLow,_newLow));
			result = _sar;
			_af = AFmin;
			_ep = _newHigh;
			_sar += _af * (_ep - _sar);
			_sar = Min(_sar,Min(_prevLow,_newLow));
		}
		else
		{
			result = _sar;
			if(_newLow < _ep) {
				_ep = _newLow;
				_af = Min(_af+AFstep,AFmax);
			}
			_sar += _af * (_ep - _sar);
			_sar = Max(_sar,Max(_prevHigh,_newHigh));
		}
	}
	rSign = _isLong;
	return result;
}

var rLength,rPeak,rSlope;
var ZigZag(var* Data,var Depth,int Length,long Color)
{
#ifndef _DEBUG
	#define _Dir		v[0]	
	#define _LastBar	v[1]	
	#define _HighBar	v[2]	
	#define _LowBar	v[3]	
	#define _LastPrice v[4]	
	#define _HighPrice v[5]
	#define _LowPrice	v[6]
	#define _Length	v[7]
	#define _Slope		v[8]
	vars v = series(0,-9);
#else
	static var _Dir,_LastBar,_HighBar,_LowBar,
		_LastPrice,_HighPrice,_LowPrice,_Slope,_Length;
#endif
	SETSERIES(Data,0);
	var Price = Data[0];

	if(is(FIRSTRUN)) {
		_HighPrice = Price;
		_LowPrice = Price;
		_LastPrice = Price;
		_LastBar = _HighBar = _LowBar = g->nBar;
		_Dir = 0;
		if(Color) {
			plotGraph("ZigZag",0,_LastPrice,LINE,Color);
			plotGraph("ZigZag",0,_LastPrice,LINE+END,Color);
		}
		rLength = 1;
		return rSign = rPeak = rSlope = 0;
	}

// check for up-swing > threshold and down-swing > threshold
	if(_Dir >= 0) 
	{
		if(Price > _HighPrice) { // new high pivot
			/*_LowPrice =*/ _HighPrice = Price;
			/*_LowBar =*/ _HighBar = g->nBar;
		}
// store first low pivot after the high pivot
		else if(Price < _LowPrice) { 
			_LowPrice = Price;
			_LowBar = g->nBar;
		}
		if(_HighPrice - _LastPrice >= Depth
			&& _HighPrice - Price >= Depth
			&& g->nBar - _HighBar >= Length)
		{				
// Up-swing ended, now move down
			if(Color) {
				plotGraph("ZigZag",g->nBar-_LastBar,_LastPrice,LINE,Color);
				plotGraph("ZigZag",g->nBar-_HighBar,_HighPrice,LINE+END,Color);
			}
// store the slope of the up-swing
			_Length = _HighBar-_LastBar;
			_Slope = (_HighPrice-_LastPrice)/_Length;
// The high becomes a new pivot, and _Dir changes to a down-swing
			_LowBar = _LastBar = _HighBar;
			_LowPrice = _LastPrice = _HighPrice;
			_Dir = -1;
		}
	}

	if(_Dir <= 0) // can happen in the same bar
	{
		if(Price < _LowPrice) {
			_LowPrice = /*_HighPrice =*/ Price;
			_LowBar = /*_HighBar =*/ g->nBar;
		}
		else if(Price > _HighPrice) {
			_HighPrice = Price;
			_HighBar = g->nBar;
		}
		if(_LastPrice - _LowPrice > Depth
			&& Price - _LowPrice > Depth
			&& g->nBar - _LowBar >= Length)
		{
			if(Color) {
				plotGraph("ZigZag",g->nBar-_LastBar,_LastPrice,LINE,Color);
				plotGraph("ZigZag",g->nBar-_LowBar,_LowPrice,LINE+END,Color);
			}				
			_Length = _LowBar-_LastBar;
			_Slope = (_LowPrice-_LastPrice)/_Length;
			_HighBar = _LastBar = _LowBar;
			_HighPrice = _LastPrice = _LowPrice;
			_Dir = 1;

// fix high price in case the current price was already a high pivot
			if(Price > _HighPrice) {
				_HighPrice = Price;
				_HighBar = g->nBar;
			}
		} 
	}

	rLength = _Length;
	rPeak = g->nBar-_LastBar;
	rSign = _Dir;
	return rSlope = _Slope;	
}

var rTenkan, rKijun, rSenkouA, rSenkouB;
var Ichimoku(int PeriodTenkan,int PeriodKijun,int PeriodSenkou,int Offset)
{
	if(!PeriodTenkan) PeriodTenkan = 9;
	if(!PeriodKijun) PeriodKijun = 26;
	if(!PeriodSenkou) PeriodSenkou = 52;
	
	checkLookBack(Max(PeriodSenkou,PeriodKijun)+Offset);
	if(g->nTimeFrame == 1) {
		rTenkan = (HH(PeriodTenkan,Offset)+LL(PeriodTenkan,Offset))/2;
		rKijun = (HH(PeriodKijun,Offset)+LL(PeriodKijun,Offset))/2;
		rSenkouA = (HH(PeriodTenkan,Offset+PeriodKijun)+LL(PeriodTenkan,Offset+PeriodKijun)
			+ HH(PeriodKijun,Offset+PeriodKijun)+LL(PeriodKijun,Offset+PeriodKijun))/4;
		rSenkouB = (HH(PeriodSenkou,Offset+PeriodKijun)+LL(PeriodSenkou,Offset+PeriodKijun))/2;
	} else {
		var *High = series(priceHigh(0),0), *Low = series(priceLow(0),0);
		rTenkan = (MaxVal(High+Offset,PeriodTenkan)+MinVal(Low+Offset,PeriodTenkan))/2;
		rKijun = (MaxVal(High+Offset,PeriodKijun)+MinVal(Low+Offset,PeriodKijun))/2;
		rSenkouA = (MaxVal(High+Offset+PeriodKijun,PeriodTenkan)+MinVal(Low+Offset+PeriodKijun,PeriodTenkan)
			+ MaxVal(High+Offset+PeriodKijun,PeriodKijun)+MinVal(Low+Offset+PeriodKijun,PeriodKijun))/4;
		rSenkouB = (MaxVal(High+Offset+PeriodKijun,PeriodSenkou)+MinVal(Low+Offset+PeriodKijun,PeriodSenkou))/2;
	}
	return rTenkan;
}

var Chikou(int Shift)
{
	if(!Shift) Shift = 26;
	checkLookBack(Shift);
	return priceClose(Shift);
}

var ChandelierLong(int TimePeriod,var Multiplier)
{
	if(!TimePeriod) TimePeriod = 22;
	if(0. == Multiplier) Multiplier = 3.;
	return HH(TimePeriod,0) - ATR(TimePeriod)*Multiplier;
}

var ChandelierShort(int TimePeriod,var Multiplier)
{
	if(!TimePeriod) TimePeriod = 22;
	if(0. == Multiplier) Multiplier = 3.;
	return LL(TimePeriod,0) + ATR(TimePeriod)*Multiplier;
}


///////////////////////////////////////////////////////////////////
// statistics

// return the number of rising/falling bars
int NumRiseFall(var* Data,int Period)
{
	checkLookBack(Period);
	int i,j;
	for(i=0; i<Period; i++)
		if(Data[i]>=Data[i+1]) break;
	for(j=0; j<Period; j++)
		if(Data[j]<=Data[j+1]) break;
	return j-i;
}

// return Number of white and black candles
int NumWhiteBlack(var vBody,int Offset,int Period)
{
	checkLookBack(Period+Offset);
	int i,j;
	for(i=0; i<Period; i++)
		if(priceClose(i+Offset)+vBody > priceOpen(i+Offset)) break;
	for(j=0; j<Period; j++)
		if(priceClose(j+Offset)-vBody < priceOpen(j+Offset)) break;
	return j-i;
}

int NumInRange(var* Low,var* High,var vMin,var vMax,int Period)
{
	int i,n;
	for(i=0,n=0; i<Period; i++)
		if(High[i] <= vMax && Low[i] >= vMin) n++;
	return n;
}

var ProfitFactor(var* Data,int Length)
{
	var Win = 0.,Loss = 0.;
	for(int i=1; i<Length; i++) {
		if(Data[i-1] > Data[i])
			Win += Data[i-1]-Data[i];
		else
			Loss += Data[i]-Data[i-1];
	}
	if(Loss == 0.) 
		return Win == 0.? 1. : 10.;
	return Clamp(Win/Loss,0.1,9.999);
}

// Spearman trend indicator, S&C 2/2011
var Spearman(var* Data,int Period)
{
	Period = Clamp(Period,2,256);
	checkLookBack(Period);
	int* Idx = sortIdx(Data,Period);
	var sum = 0;
	int i;
	for(i=0,g_count=Period-1; i < Period; i++,g_count--)
		sum += (g_count-Idx[i])*(g_count-Idx[i]);
	return 1. - 6.*sum/(Period*(Period*Period-1.));
}

// Fractal Dimension
var FractalDimension(var* Data,int Period)
{
	checkLookBack(2*Period);
	Period &= ~1;
	int Period2 = Max(1,Period/2);
	var N1 = (MaxVal(Data,Period2)-MinVal(Data,Period2))/Period2;
	var N2 = (MaxVal(Data+Period2,Period2)-MinVal(Data+Period2,Period2))/Period2;
	var N3 = (MaxVal(Data,Period)-MinVal(Data,Period))/Period;
	if(N1+N2 <= 0. || N3 <= 0.) return 1.;
	return (log(N1+N2)-log(N3))/log(2.);
}

// Hurst exponent
var Hurst(var* Data,int Period)
{
	Period = Max(20,Period);
	vars Hurst = series(2. - FractalDimension(Data,Period),2);
	var R = Clamp(Smooth(Hurst,Period/10),0.,1.);
	SETSERIES(Data,Period);
	return R;
}


// statistical moment (1 = mean, 2 = variance, 3 = skewness, 4 = kurtosis)
var rMean;
var Moment(var* Data,int Period,int n)
{
	Period = Max(2,Period);
	checkLookBack(Period);
	rMean = 0.;
	for(int i=0; i<Period; i++)
		rMean += Data[i];
	rMean /= Period;
	if(n <= 1) return rMean;
	var variance=0.,sum = 0.;
	int i,j;
	for(i=0; i<Period; i++) {
		var f = Data[i]-rMean;
		variance += f*f;
		for(j=2; j<=n; j++)
			f *= Data[i]-rMean;
		sum += f;
	}
	variance /= (Period-1);
	if(n==2) return variance;
	sum /= variance*(Period-1);
	var stdev = sqrt(variance);
	for(j=3; j<=n; j++)
		sum /= stdev;
	return sum;
}

var VolatilityMM(var* Data,int TimePeriod,int EMAPeriod)
{
	var Range = MaxVal(Data,TimePeriod)-MinVal(Data,TimePeriod);
	if(!EMAPeriod) return Range;
	var R = EMA(series(Range,0),EMAPeriod);
	SETSERIES(Data,TimePeriod);
	return R;
}

var VolatilityC(int TimePeriod,int EMAPeriod)
{
	if(!TimePeriod) TimePeriod = 10;
	if(!EMAPeriod) EMAPeriod = 10;
	checkLookBack(TimePeriod+g->nUnstablePeriod);
	vars Range = series(priceHigh(0)-priceLow(0),0);
	var EMA_0 = EMA(Range,EMAPeriod);
	var EMA_N = EMA(Range+TimePeriod,EMAPeriod);
	return 100.*(EMA_0-EMA_N)/Max(1.,EMA_N);
}

var Volatility(var* Data,int TimePeriod)
{
	checkLookBack(TimePeriod*TIMEFRAME);
	vars LogReturn = series(log(Data[0]/Data[1]),0);
	var Variance = Moment(LogReturn,TimePeriod,2);
	return sqrt(Variance*252*DAYFRAME);
}

// Gain based on shannon probability
// Algorithm from http://www.johncon.com/john/correspondence/020213233852.26478.html
var ShannonGain(var* Data,int Period)
{
	Period = Max(Period,2);
	checkLookBack(Period+1);
	var sumx = 0., sumx2 = 0.;
	int j;
	for (j=0; j<Period; j++) {
		var G = 0.0000001;
		if(Data[j+1] != 0. && Data[j+1] != Data[j]) 
			G = log(Data[j]/Data[j+1]); // gain
		sumx += G;
		sumx2 += G*G;
	}
	var avgx = sumx/Period; // average gain
	var rmsx = sqrt(sumx2/Period); // root-mean-square of gains
	var P = ((avgx/rmsx)+1)/2.0; // shannon probability
	//return pow(1+rmsx,P)*pow(1-rmsx,1-P); // gain
	return P*log(1+rmsx) + (1-P)*log(1-rmsx); // logarithmic gain
}

var log2(var X) { return log(X)/0.6931; } //log(2)
 
var ShannonEntropy256(char *S,int Length)
{
	static var Hist[256];
	memset(Hist,0,256*sizeof(var));
	var Step = 1./Length;
	for(int i=0; i<Length; i++) 
		Hist[S[i]] += Step;
	var H = 0;
	for(int i=0;i<256;i++){
		if(Hist[i] > 0.)
			H -= Hist[i]*log2(Hist[i]);
	}
	return H;
}

var ShannonEntropy(var *Data,int Length,int PatternSize)
{
	int i,j;
	int Size = Length-PatternSize-1;
	if(!Data || Size <= 0) return 0.; 
	char* S = (char*)mem_alloc(Size,"ENT");
	if(!S) return 0.;
	for(i=0; i<Size; i++) {
		int C = 0;
		for(j=0; j<PatternSize; j++) {
			if(Data[i+j] > Data[i+j+1])
				C += 1<<j;
		}
		S[i] = C;
	}
	var H = ShannonEntropy256(S,Size);
	mem_free(S);
	return H;
} 

var CI(int TimePeriod)
{
	checkLookBack(TimePeriod);
	var Sum = 0;
	for(int i=0; i<TimePeriod; i++)
		Sum += priceHigh(i)-priceLow(i);
	Sum /= HH(TimePeriod,0)-LL(TimePeriod,0);
	return 100.*log10(Sum)/log10((double)TimePeriod);
}

var move(vars Data,int Length,int Horizon,var Percent)
{
	if(Length <= Horizon) return 0;
	checkLookBack(Length);
	vars Moves = series(0,0);
	for(int i=0; i<Length-Horizon; i++)
		Moves[i] = 100.*(Data[i]-Data[i+Horizon])/Data[i+Horizon];
	var Result = Percentile(Moves,Length-Horizon,Percent);
	return Result;
}

inline int seasonMode(int i,int Mode)
{
	switch(Mode) {
		case 1: return hour(i);
		case 2: return dow(i);
		case 3: return tdm(i);
		case 4: return month(i);
	}
	return 0;
}

var season(vars Data,int Length,int Horizon,int Mode)
{
	checkLookBack(Length);
	int i = 0, M = seasonMode(0,Mode);
	var Val0 = 0., Val1 = 0.; // initial and final value
	if(Mode == 3) Length -= 31; // tdm goes back to the first day of the month
	while(i < Length) {
		for(; i < Length && seasonMode(i,Mode) == M; i++); // go back to previous day
		for(; i < Length && seasonMode(i,Mode) != M; i++); // go back to same day previous week/month
		for(; i < Length && seasonMode(i,Mode) == M; i++) { // collect data of day (Val1) and day+Horizon (Val0)
			if(i >= Horizon)
				Val1 += Data[i], Val0 += Data[i-Horizon];
		}
	}
	if(Val1 == 0.) return 0.;
	return 100.*(Val0-Val1)/Val1; // percent change from day to day+Horizon
}



//////////////////////////////////////////////////////////////////
// Currency strength

#define MAXCURRENCIES	32
var CStren[MAXCURRENCIES],CStrenSum[MAXCURRENCIES];
int CNum[MAXCURRENCIES];
char CNames[MAXCURRENCIES*4];
int NumCurrencies = 0;

void ccyReset()
{
	for(int n=0; n<MAXCURRENCIES; n++) {
		CStren[n] = 0.;
		CStrenSum[n] = 0.;
		CNum[n] = 0;
	}
}

int ccyIdx(char* name)
{
	char Ccy[4];
	memcpy(Ccy,name,4);
	Ccy[3] = 0; 
	for(int n=0; n<MAXCURRENCIES; n++) {
		if(!CNames[n*4]) { // not yet stored
			strcpy(CNames+n*4,Ccy);
			NumCurrencies++;
			return n;
		}
		if(strstr(CNames+n*4,Ccy))
			return n;
	}
	return 0;
}

void ccySet(var Strength)
{
	if(!isForex(g->asset)) return; 
	int n = ccyIdx(g->asset->sName);
	CStrenSum[n] += Strength; 
	CNum[n]++;
	CStren[n] = CStrenSum[n]/CNum[n];
	g->asset->vStrength = CStren[n];
	
	n = ccyIdx(g->asset->sName+4); // counter currency
	CStrenSum[n] -= Strength; 
	CNum[n]++;
	CStren[n] = CStrenSum[n]/CNum[n];
	g->asset->vStrength -= CStren[n];
}

var ccyStrength(char* Currency)
{
	if(!Currency || strlen(Currency) < 3) return 0;
	if(Currency[3] == '/') // 
		return CStren[ccyIdx(Currency)] - CStren[ccyIdx(Currency+4)];  
	else return CStren[ccyIdx(Currency)];
}

char* ccyMax()
{
	var MaxStren = 0.;
	static char Ccy[8] = "";
	FOR_ASSETS(a)
		if(a->vStrength > MaxStren) {
			MaxStren = a->vStrength;
			strcpy_s(Ccy,a->sName);
		}
	}
	return Ccy;
}

char* ccyMin()
{
	var MaxStren = 0.;
	static char Ccy[8] = "";
	FOR_ASSETS(a)
		if(a->vStrength < MaxStren) {
			MaxStren = a->vStrength;
			strcpy_s(Ccy,a->sName);
		}
	}
	return Ccy;
}

//////////////////////////////////////////////////////////////////
// test functions

// Sine generator for filter testing
var genSine(var Period1, var Period2) 
{
	if(Period1 <= 1.) Period1 = 1.;
	if(Period2 == 0.) Period2 = Period1;
	var k = (Period2-Period1)/g->numBars;
	if(k == 0.) // constant frequency
		return 0.5 + 0.5*sin((2.*PI*g->nBar)/Period1);
	else // hyperbolic chirp
		return 0.5 + 0.5*sin(2*PI*log(1+k*g->nBar/Period1)/k);
}

// Square wave generator for filter testing
var genSquare(var Period1, var Period2) 
{
	return (genSine(Period1,Period2) < 0.5)? 0. : 1.;
}

// Noise generator for filter testing
var genNoise() 
{
	return random();
}

#endif //INDICATORS_H
#endif