///////////////////////////////////////////////////////
// VC++ Header for DLL-based Zorro strategies
// set struct member alignment to 4 bytes
///////////////////////////////////////////////////////
#pragma once

// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.
//#define WINVER 0x0501		// Change this to the appropriate value to target other versions of Windows.
//#define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
//#define _WIN32_WINDOWS 0x0410 // Change this to the appropriate value to target Windows Me or later.
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

// Windows Header Files:
#include <windows.h>
#include <wtypes.h>
#include <stdio.h>
#include <cmath>

// Zorro specific header files
#pragma pack(push,4)
#include "trading.h"
#include "variables.h"
#pragma pack(pop)

#define DLLFUNC extern "C" __declspec(dllexport)
#pragma warning( disable: 4244 4800 4311 4996)

////////////////////////////////////////////////////////
#ifndef ZORRO_CPP
extern GLOBALS *g;

// function prototypes
#define F(x) (*x)
#define F0(x) (*x##0)
#define F1(x) (*x##1)
#define F2(x) (*x##2)
#define F3(x) (*x##3)
#define C extern
#include "functions.h"
#define CCC const_cast<char*>

// convenience definitions for overloaded functions ////////////////
inline TRADE* enterLong(int lots=0,var entry=0, var stop=0, var takeprofit=0, var trail=0, var trailslope=0, var traillock=0, var trailstep=0)
{ return enterLong0(lots,entry,stop,takeprofit,trail,trailslope,traillock,trailstep,0); }
inline TRADE* enterShort(int lots=0,var entry=0, var stop=0, var takeprofit=0, var trail=0, var trailslope=0, var traillock=0, var trailstep=0)
{ return enterShort0(lots,entry,stop,takeprofit,trail,trailslope,traillock,trailstep,0); }
template <typename FUNCTION> 
inline TRADE* enterLong(FUNCTION f=0,var v0=0,var v1=0,var v2=0,var v3=0,var v4=0,var v5=0,var v6=0,var v7=0)
{ return enterLong1((void*)f,v0,v1,v2,v3,v4,v5,v6,v7); }
template <typename FUNCTION> 
inline TRADE* enterShort(FUNCTION f=0,var v0=0,var v1=0,var v2=0,var v3=0,var v4=0,var v5=0,var v6=0,var v7=0)
{ return enterShort1((void*)f,v0,v1,v2,v3,v4,v5,v6,v7); }
inline void exitLong(const char* Name=0,var Limit=0,int nLots=0)
{ exitLong0(Name,Limit,nLots); }
inline void exitShort(const char* Name=0,var Limit=0,int nLots=0)
{ exitShort0(Name,Limit,nLots); }
inline int exitTrade(TRADE* tr,var Limit=0,int nLots=0)
{ return exitTrade0(tr,Limit,nLots); }
inline char* loop(void* p0=0,void* p1=0,void* p2=0,void* p3=0,void* p4=0,void* p5=0,void* p6=0,void* p7=0,void* p8=0,void* p9=0,void* p10=0,void* p11=0,void* p12=0,void* p13=0,void* p14=0,void* p15=0,void* p16=0,void* p17=0,void* p18=0,void* p19=0,
	void* p20=0,void* p21=0,void* p22=0,void* p23=0,void* p24=0,void* p25=0,void* p26=0,void* p27=0,void* p28=0,void* p29=0,void* p30=0,void* p31=0,void* p32=0,void* p33=0,void* p34=0,void* p35=0,void* p36=0,void* p37=0,void* p38=0,void* p39=0)
{ return loop0(p0,p1,p2,p3,p4,p5,p6,p7,p8,p9,p10,p11,p12,p13,p14,p15,p16,p17,p18,p19,
	p20,p21,p22,p23,p24,p25,p26,p27,p28,p29,p30,p31,p32,p33,p34,p35,p36,p37,p38,p39); }
inline char* loop(const char* p0 = 0, const char* p1 = 0, const char* p2 = 0, const char* p3 = 0, const char* p4 = 0, const char* p5 = 0, const char* p6 = 0, const char* p7 = 0, const char* p8 = 0, const char* p9 = 0, const char* p10 = 0, const char* p11 = 0, const char* p12 = 0, const char* p13 = 0, const char* p14 = 0, const char* p15 = 0, const char* p16 = 0, const char* p17 = 0, const char* p18 = 0, const char* p19 = 0,
	const char* p20 = 0, const char* p21 = 0, const char* p22 = 0, const char* p23 = 0, const char* p24 = 0, const char* p25 = 0, const char* p26 = 0, const char* p27 = 0, const char* p28 = 0, const char* p29 = 0, const char* p30 = 0, const char* p31 = 0, const char* p32 = 0, const char* p33 = 0, const char* p34 = 0, const char* p35 = 0, const char* p36 = 0, const char* p37 = 0, const char* p38 = 0, const char* p39 = 0)
{ return loop0(CCC(p0), CCC(p1), CCC(p2), CCC(p3), CCC(p4), CCC(p5), CCC(p6), CCC(p7), CCC(p8), CCC(p9), CCC(p10), CCC(p11), CCC(p12), CCC(p13), CCC(p14), CCC(p15), CCC(p16), CCC(p17), CCC(p18), CCC(p19,
		p20), CCC(p21), CCC(p22), CCC(p23), CCC(p24), CCC(p25), CCC(p26), CCC(p27), CCC(p28), CCC(p29), CCC(p30), CCC(p31), CCC(p32), CCC(p33), CCC(p34), CCC(p35), CCC(p36), CCC(p37), CCC(p38), CCC(p39));
}
inline var adviseLong(int Method=0,var Objective=0,var v0=NIL,var v1=NIL,var v2=NIL,var v3=NIL,var v4=NIL,var v5=NIL,var v6=NIL,var v7=NIL,var v8=NIL,var v9=NIL,var v10=NIL,var v11=NIL,var v12=NIL,var v13=NIL,var v14=NIL,var v15=NIL,var v16=NIL,var v17=NIL,var v18=NIL,var v19=NIL)
{ return adviseLong0(Method,Objective,v0,v1,v2,v3,v4,v5,v6,v7,v8,v9,v10,v11,v12,v13,v14,v15,v16,v17,v18,v19); }
inline var adviseLong(int Method,var Objective,var *Signals,long NumSignals)
{ return adviseLong1(Method,Objective,Signals,NumSignals); }
inline var adviseShort(int Method=0,var Objective=0,var v0=NIL,var v1=NIL,var v2=NIL,var v3=NIL,var v4=NIL,var v5=NIL,var v6=NIL,var v7=NIL,var v8=NIL,var v9=NIL,var v10=NIL,var v11=NIL,var v12=NIL,var v13=NIL,var v14=NIL,var v15=NIL,var v16=NIL,var v17=NIL,var v18=NIL,var v19=NIL)
{ return adviseShort0(Method,Objective,v0,v1,v2,v3,v4,v5,v6,v7,v8,v9,v10,v11,v12,v13,v14,v15,v16,v17,v18,v19); }
inline var adviseShort(int Method,var Objective,var *Signals,long NumSignals)
{ return adviseShort1(Method,Objective,Signals,NumSignals); }
inline void assetAdd(const char* Name) { assetAdd0(Name,0,0,0,0,0,0,0,0,0,0,0); }
inline void assetAdd(const char* Name,const char* BrokerSymbol) { assetAdd0(Name,0,0,0,0,0,0,0,0,0,0,BrokerSymbol); }
inline void assetAdd(const char* Name, var vPrice, var vSpread, var vRollLong, var vRollShort, var vPip, var vPipCost, var vMarginCost, var vLeverage, var vLotAmount, var vCommission, const char* sSymbol)
{ assetAdd0(Name, vPrice, vSpread, vRollLong, vRollShort, vPip, vPipCost, vMarginCost, vLeverage, vLotAmount, vCommission, sSymbol); }

inline int dataParse(int Handle,const char* Format,const char* FileName)
{ return dataParse0(Handle,Format,FileName); }
inline int dataParse(int Handle,const char* Format,const char* FileName,int Start,int Num=0)
{ return dataParse1(Handle,Format,FileName,Start,Num); }
inline int dataParse(int Handle,const char* Format,const char* FileName,const char* Filter)
{ return dataParse2(Handle,Format,FileName,Filter); }
inline void dataSet(int Handle,int Row,int Col,var Value) { dataSet0(Handle,Row,Col,Value); }
inline void dataSet(int Handle,int Row,int Col,int Value) { dataSet1(Handle,Row,Col,Value); }

inline vars series(var val=0,int size=0) { return series0(val,size); }
inline void set(long Flag1=0,long Flag2=0,long Flag3=0,long Flag4=0,long Flag5=0,long Flag6=0,long Flag7=0,long Flag8=0,long Flag9=0,long Flag10=0) 
{ return set0(Flag1,Flag2,Flag3,Flag4,Flag5,Flag6,Flag7,Flag8,Flag9,Flag10); }
inline int is(long Flag) { return is0(Flag); }
inline void reset(long Flag) { return reset0(Flag); }

inline BOOL peak(vars Data) { return peak0(Data); }
inline int peak(vars Data,int TimePeriod) { return peak1(Data,TimePeriod); }
inline BOOL valley(vars Data) { return valley0(Data); }
inline int valley(vars Data,int TimePeriod) { return valley1(Data,TimePeriod); }
inline BOOL crossOver(vars a,vars b) { return crossOver0(a,b); }
inline BOOL crossUnder(vars a,vars b) { return crossUnder0(a,b); }
inline BOOL crossOver(vars a,var b) { return crossOver1(a,b); }
inline BOOL crossUnder(vars a,var b) { return crossUnder1(a,b); }
inline BOOL crossOver(vars a,vars b,int TimePeriod) { return crossOver2(a,b,TimePeriod); }
inline BOOL crossUnder(vars a,vars b,int TimePeriod) { return crossUnder2(a,b,TimePeriod); }
inline BOOL crossOver(vars a,var b,int TimePeriod) { return crossOver3(a,b,TimePeriod); }
inline BOOL crossUnder(vars a,var b,int TimePeriod) { return crossUnder3(a,b,TimePeriod); }

inline CONTRACT* contract(int Type,int Days,var Strike) { return contract0(Type,Days,Strike); }
inline CONTRACT* contract(CONTRACT* Contract) { return contract1(Contract); }
inline CONTRACT* contract(TRADE* T) { return contract2(T); }
inline CONTRACT* contract(int N) { return contract3(N); }
inline var contractDays(CONTRACT* Contract) { return contractDays0(Contract); }
inline var contractDays(TRADE* Tr) { return contractDays1(Tr); }
inline var contractPrice(CONTRACT* Contract) { return contractPrice0(Contract); }
inline var contractPrice(TRADE* Tr) { return contractPrice1(Tr); }
inline CONTRACT* contractFind(int Type, int Days, var Find, int N) { return contractFind0(Type, Days, Find, N); }
inline CONTRACT* contractFind(int Type,int Days,var Find,int N, var MinStrike,var MaxStrike) { return contractFind1(Type,Days,Find,N, MinStrike,MaxStrike); }

inline char* strdate(const char* format,int offset) { return strdate0(format,offset); }
inline char* strdate(const char* format,var date) { return strdate1(format,date); }
inline long file_append(const char* name,const char* content) { return file_append0(name,content); }
inline long file_append(const char* name,void* content,long size) { return file_append1(name,content,size); }
inline var second(DATE Time) { return second1(Time); }

inline var random() { return random0(); }
inline var random(var limit) { return random1(limit); }
inline var sign(var a) { return sign0(a); }
inline int sign(int a) { return sign1(a); }
inline var ifelse(BOOL c,var a,var b) { return ifelse0(c,a,b); }
inline var ifelse(BOOL c,var a,int b) { return ifelse0(c,a,b); }
inline var ifelse(BOOL c,int a,var b) { return ifelse0(c,a,b); }
inline int ifelse(BOOL c,int a,int b) { return ifelse1(c,a,b); }
inline char* ifelse(BOOL c,char* a,char* b) { return ifelse2(c,a,b); }
inline char* ifelse(BOOL c,const char* a,const char* b) { return ifelse2(c,(char*)a,(char*)b); }
inline var clamp(var a,var l,var h) { return clamp0(a,l,h); }
inline var clamp(int a,int l,int h) { return clamp1(a,l,h); }
inline BOOL between(var a,var l,var h) { return between0(a,l,h); }
inline BOOL between(int a,int l,int h) { return between1(a,l,h); }

inline mat matSet(mat M,mat A) { return matSet0(M,A); }
inline mat matSet(mat M,int row,int col,mat A) { return matSet1(M,row,col,A); }
inline mat matSet(mat M,var c) { return matSet2(M,c); }
inline var markowitz(var* CovMatrix,var* Means,int N,var* Caps) { return markowitz0(CovMatrix,Means,N,Caps); }
inline var markowitz(var* CovMatrix,var* Means,int N,var Cap) { return markowitz1(CovMatrix,Means,N,Cap); }

inline char* strcon(TRADE* Tr) { return strcon0(Tr); }
inline char* strcon(CONTRACT* Ct) { return strcon1(Ct); }
inline char* strx(const char* In,const char* Orig,const char* Repl)
{ return strx0(In,Orig,Repl); } 
inline char* strx(char *Out,int Size,const char* In,const char* Orig,const char* Repl)
{ return strx1(Out,Size,In,Orig,Repl); } 

inline var LowPass(vars Data,int Cutoff) { return LowPass0(Data,Cutoff); }
inline var LowPass(var* Buffer,var Val,int Cutoff) { return LowPass(Buffer,Val,Cutoff); }
inline var ATR(int TimePeriod) { return ATR0(TimePeriod); }
inline var ATR(vars Open,vars High,vars Low,vars Close,int TimePeriod) { return ATR1(Open,High,Low,Close,TimePeriod); }
inline var MinusDI(int TimePeriod) { return MinusDI0(TimePeriod); }
inline var MinusDI(vars Open,vars High,vars Low,vars Close,int TimePeriod) { return MinusDI1(Open,High,Low,Close,TimePeriod); }
inline var MinusDM(int TimePeriod) { return MinusDM0(TimePeriod); }
inline var MinusDM(vars Open,vars High,vars Low,vars Close,int TimePeriod) { return MinusDM1(Open,High,Low,Close,TimePeriod); }
inline var PlusDI(int TimePeriod) { return PlusDI0(TimePeriod); }
inline var PlusDI(vars Open,vars High,vars Low,vars Close,int TimePeriod) { return PlusDI1(Open,High,Low,Close,TimePeriod); }
inline var PlusDM(int TimePeriod) { return PlusDM0(TimePeriod); }
inline var PlusDM(vars Open,vars High,vars Low,vars Close,int TimePeriod) { return PlusDM1(Open,High,Low,Close,TimePeriod); }
inline var EMA(vars Data,int TimePeriod) { return  EMA0(Data,TimePeriod); }
inline var EMA(vars Data,var alpha) { return EMA1(Data,alpha); }
inline var EMA(var In,var alpha) { return EMA2(In,alpha); }
inline var EMA(var In,int TimePeriod) { return EMA3(In,TimePeriod); }
inline var Stoch(int FastK_Period,int SlowK_Period,int SlowK_MAType,int SlowD_Period,int SlowD_MAType) 
{ return Stoch0(FastK_Period,SlowK_Period,SlowK_MAType,SlowD_Period,SlowD_MAType); }
inline var Stoch(vars Open,vars High,vars Low,vars Close,int FastK_Period,int SlowK_Period,int SlowK_MAType,int SlowD_Period,int SlowD_MAType) 
{ return Stoch1(Open,High,Low,Close,FastK_Period,SlowK_Period,SlowK_MAType,SlowD_Period,SlowD_MAType); }
inline var AGC(vars Data, var alpha) { return AGC0(Data,alpha); }
inline var AGC(vars Data, int Period) { return AGC1(Data,Period); }
inline var ALMA(vars Data, int TimePeriod, int sigma, var offset) { return ALMA0(Data,TimePeriod,sigma,offset); }
inline var ALMA(vars Data, int TimePeriod) { return ALMA1(Data,TimePeriod); }

inline int pySet(const char* Name, int n) { return pySet0(Name,n); }
inline int pySet(const char* Name, var d) { return pySet1(Name,d); }
inline int pySet(const char* Name, var *v, int elements) { return pySet2(Name,v,elements); }
inline int assetList(const char* Name) { return assetList0(Name); }
inline int assetList(const char* Name,const char* Select) { return assetList1(Name,Select); }

inline var slider(int N,int Pos=0,int Lower=0,int Upper=0,const char* Name=0,const char* Tooltip=0)
{ return slider0(N,Pos,Lower,Upper,Name,Tooltip); }
inline int panel(int Rows, int Cols, int Color, int Size) { return panel0(Rows, Cols, Color, Size); }
inline int panel(const char* Filename, int Color, int Size) { return panel1(Filename,Color,Size); }
inline void plot(const char* Name,var Val,int Type,int Color) { plot0(Name,Val,Type,Color); }
inline void plot(const char* Name,int Val,int Type,int Color) { plot0(Name,(var)Val,Type,Color); }
inline void plot(const char* Name,vars Data,int Type,int Color) { plot0(Name,Data[0],Type,Color); }

extern void _printf(const char* Format, ...);
#define printf _printf

#endif
