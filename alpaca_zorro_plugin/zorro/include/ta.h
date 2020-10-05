///////////////////////////////////////////////
// Technical Analysis Functions by ta-lib.org
// Function descriptions & algorithms on http://tadoc.org
///////////////////////////////////////////////

extern var rMomentum;

// Types of Moving Average ///////////////////////////////
#define MAType_SMA	0
#define MAType_EMA	1
#define MAType_WMA	2
#define MAType_DEMA	3
#define MAType_TEMA	4
#define MAType_TRIMA 5
#define MAType_KAMA	6
#define MAType_MAMA	7
#define MAType_T3	8

// TA-Lib functions /////////////////////////////////////
var ADX(int TimePeriod);
var ADXR(int TimePeriod);
var Amplitude(var* Data,int TimePeriod);
var APO(var* Data,int FastPeriod,int SlowPeriod,int MAType);
extern var rAroonDown;
extern var rAroonUp;
var Aroon(int TimePeriod);
var AroonOsc(int TimePeriod);
var ATR(int TimePeriod);
var AvgPrice();
extern var rRealUpperBand;
extern var rRealMiddleBand;
extern var rRealLowerBand;
var BBands(var* Data,int TimePeriod,var NbDevUp,var NbDevDn,int MAType);
var Beta(var* Data,var* Data2,int TimePeriod);
var BOP();
var CCI(int TimePeriod);
int CDL2Crows();
int CDL3BlackCrows();
int CDL3Inside();
int CDL3LineStrike();
int CDL3Outside();
int CDL3StarsInSouth();
int CDL3WhiteSoldiers();
int CDLAbandonedBaby(var Penetration);
int CDLAdvanceBlock();
int CDLBeltHold();
int CDLBreakaway();
int CDLClosingMarubozu();
int CDLConcealBabysWall();
int CDLCounterAttack();
int CDLDarkCloudCover(var Penetration);
int CDLDoji();
int CDLDojiStar();
int CDLDragonflyDoji();
int CDLEngulfing();
int CDLEveningDojiStar(var Penetration);
int CDLEveningStar(var Penetration);
int CDLGapSideSideWhite();
int CDLGravestoneDoji();
int CDLHammer();
int CDLHangingMan();
int CDLHarami();
int CDLHaramiCross();
int CDLHignWave();
int CDLHikkake();
int CDLHikkakeMod();
int CDLHomingPigeon();
int CDLIdentical3Crows();
int CDLInNeck();
int CDLInvertedHammer();
int CDLKicking();
int CDLKickingByLength();
int CDLLadderBottom();
int CDLLongLeggedDoji();
int CDLLongLine();
int CDLMarubozu();
int CDLMatchingLow();
int CDLMatHold(var Penetration);
int CDLMorningDojiStar(var Penetration);
int CDLMorningStar(var Penetration);
int CDLOnNeck();
int CDLPiercing();
int CDLRickshawMan();
int CDLRiseFall3Methods();
int CDLSeperatingLines();
int CDLShootingStar();
int CDLShortLine();
int CDLSpinningTop();
int CDLStalledPattern();
int CDLStickSandwhich();
int CDLTakuri();
int CDLTasukiGap();
int CDLThrusting();
int CDLTristar();
int CDLUnique3River();
int CDLUpsideGap2Crows();
int CDLXSideGap3Methods();
var CMO(var* Data,int TimePeriod);
var Correlation(var* Data,var* Data2,int TimePeriod);
var Covariance(var* Data,var* Data2,int TimePeriod);
var DEMA(var* Data,int TimePeriod);
var DX(int TimePeriod);
var EMA(var* Data,int TimePeriod);
var HTDcPeriod(var* Data);
var HTDcPhase(var* Data);
extern var rInPhase;
extern var rQuadrature;
var HTPhasor(var* Data);
extern var rSine;
extern var rLeadSine;
var HTSine(var* Data);
var HTTrendline(var* Data);
int HTTrendMode(var* Data);
var KAMA(var* Data,int TimePeriod);
var LinearReg(var* Data,int TimePeriod);
var LinearRegAngle(var* Data,int TimePeriod);
var LinearRegIntercept(var* Data,int TimePeriod);
var LinearRegSlope(var* Data,int TimePeriod);
var MovingAverage(var* Data,int TimePeriod,int MAType);
extern var rMACD;
extern var rMACDSignal;
extern var rMACDHist;
var MACD(var* Data,int FastPeriod,int SlowPeriod,int SignalPeriod);
var MACDExt(var* Data,int FastPeriod,int FastMAType,int SlowPeriod,int SlowMAType,int SignalPeriod,int SignalMAType);
var MACDFix(var* Data,int SignalPeriod);
extern var rMAMA;
extern var rFAMA;
var MAMA(var* Data,var FastLimit,var SlowLimit);
var MovingAverageVariablePeriod(var* Data,var* Data2,int MinPeriod,int MaxPeriod,int MAType);
var MaxVal(var* Data,int TimePeriod);
int MaxIndex(var* Data,int TimePeriod);
var MedPrice();
var MidPoint(var* Data,int TimePeriod);
var MidPrice(int TimePeriod);
var MinVal(var* Data,int TimePeriod);
int MinIndex(var* Data,int TimePeriod);
extern var rMin;
extern var rMax;
var MinMax(var* Data,int TimePeriod);
extern int rMinIdx;
extern int rMaxIdx;
int MinMaxIndex(var* Data,int TimePeriod);
var MinusDI(int TimePeriod);
var MinusDM(int TimePeriod);
var Mom(var* Data,int TimePeriod);
var NATR(int TimePeriod);
//var OBV(var* Data,var* Data2,int TimePeriod);
var PlusDI(int TimePeriod);
var PlusDM(int TimePeriod);
var PPO(var* Data,int FastPeriod,int SlowPeriod,int MAType);
var ROC(var* Data,int TimePeriod);
var ROCP(var* Data,int TimePeriod);
var ROCR(var* Data,int TimePeriod);
var ROCL(var* Data,int TimePeriod);
var ROCR100(var* Data,int TimePeriod);
var RSI(var* Data,int TimePeriod);
//var SAR(var Acceleration,var Maximum);
//var SARExt(var StartValue,var OffsetOnReverse,var AccelerationInitLong,var AccelerationLong,var AccelerationMaxLong,var AccelerationInitShort,var AccelerationShort,var AccelerationMaxShort);
var SMA(var* Data,int TimePeriod);
var StdDev(var* Data,int TimePeriod,var NbDev);
extern var rSlowK;
extern var rSlowD;
var Stoch(int FastK_Period,int SlowK_Period,int SlowK_MAType,int SlowD_Period,int SlowD_MAType);
extern var rFastK;
extern var rFastD;
var StochF(int FastK_Period,int FastD_Period,int FastD_MAType);
var StochRSI(var* Data,int TimePeriod,int FastK_Period,int FastD_Period,int FastD_MAType);
var Sum(var* Data,int TimePeriod);
var T3(var* Data,int TimePeriod,var VFactor);
var TEMA(var* Data,int TimePeriod);
var TrueRange();
var Trima(var* Data,int TimePeriod);
var Trix(var* Data,int TimePeriod);
var TSF(var* Data,int TimePeriod);
var TypPrice();
var UltOsc(int TimePeriod1,int TimePeriod2,int TimePeriod3);
var Variance(var* Data,int TimePeriod,var NbDev);
var WCLPrice();
var WillR(int TimePeriod);
var WMA(var* Data,int TimePeriod);
