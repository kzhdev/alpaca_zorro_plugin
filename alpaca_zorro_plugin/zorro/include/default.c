/////////////////////////////////////////////////////////////////////
// Default trading headers and functions
// (c) oP group 2018
/////////////////////////////////////////////////////////////////////
#ifndef WIN32
#ifndef default_c
#define default_c

#define PRAGMA_ZERO	// initialize local variables to zero
#define PRAGMA_POINTER // no pointer autodetection

#include <litec.h>	// common language header
#include <trading.h> // trading-specific structs
#include <variables.h> // trading variable re-definitions
#include <functions.h> // trading and technical analysis functions

extern GLOBALS* g; // global variables struct

/////////////////////////////////////////////////////////////////////
//dummy function to preset external structs for CS_GetVar() and sizeof()
void lcStructs()
{
	RECT* r = 0; r->left = 0;
	T1* t1 = 0; t1->time = 0;
	T2* t2 = 0; t2->time = 0;
	T6* t6 = 0; t6->time = 0;
	BAR* b = 0; b->time_base = 0;
	DATA* d = 0; d->Data = 0;
	DATASET* ds = 0; ds->fData = 0;
	MATRIX* m = 0; m->dat = 0;
	CONTRACT* c = 0; c->time = 0;
	COMBO* co = 0; co->Leg[0] = 0;
	PARAMETER* pa = 0; pa->Value = 0;
}

// lite-C internal functions
int compareStrings(char* p1,char* p2);
void paraNumber(long type,long num);

// overloaded functions
var abs(var a);
int abs(int a);
var max(var a,var b);
int max(int a,int b);
var min(var a,var b);
int min(int a,int b);
int watch(string text,...);

var round(var val,var step) { return roundto(val,step); }
var round(var val) { return roundto(val,1.); }
var ifelse(BOOL c,var a,int b) { return ifelse(c,a,(var)b); }
var ifelse(BOOL c,int a,var b) { return ifelse(c,(var)a,b); }
string strdate(string format) { return strdate(format,0); }
var strvar(string str,string name) { return strvar(str,name,0); }
var optimize(var val,var start,var end,var step) { return optimize(val,start,end,step,0); }
var optimize(var val,var start,var end) { return optimize(val,start,end,0,0); }
int DominantPeriod(int Period) {	return DominantPeriod(series(price()),Period); }
int DominantPhase(int Period) {	return DominantPhase(series(price()),Period); }
void assetAdd(string Name) { assetAdd(Name,0,0,0,0,0,0,0,0,0,0,0); }
void assetAdd(string Name,string BrokerSymbol) { assetAdd(Name,0,0,0,0,0,0,0,0,0,0,BrokerSymbol); }
void plot(string name,int val,int type,int color) { plot(name,(var)val,type,color); }
void plot(string name,vars data,int type,int color) { plot(name,data[0],type,color); }
int memory() { return memory(0); }
var Ichimoku() { return Ichimoku(0,0,0,0); }
var Chikou() { return Chikou(26); }
var SAR(var AFstep,var AFmax) { return SAR(AFstep,AFstep,AFmax); }
var Fisher(var* Data,int Period) { return FisherN(Data,Period); }
var HH(int Period) { return HH(Period,0); }
var LL(int Period) { return LL(Period,0); }
int contractCheck(TRADE* tr) { return contractCheck(tr,3); }
var diff(var x) { return diff(x,1); }

var genSine(var Period) { return genSine(Period,Period); }
var genSquare(var Period) { return genSquare(Period,Period); }

/////////////////////////////////////////////////////////////////////
// user functions

// optimizing objective based on PRR
var objective()
{
	if(NumWinTotal < 2 || NumWinTotal+NumLossTotal < 5) 
		return 0.;	// needs at least 5 trades
	var wFac = 1./sqrt(1.+NumWinTotal); 
	var lFac = 1./sqrt(1.+NumLossTotal);
	var Win = WinTotal, Loss = LossTotal;
// remove possible outlier wins/losses
	if(NumLossTotal > 1) {
		Loss -= (NumLossTotal-1)*LossMaxTotal/NumLossTotal;
		Win -= (NumWinTotal-1)*WinMaxTotal/NumWinTotal;
	}
// return PRR
	return (1.-wFac)/(1.+lFac)*(1.+Win)/(1.+Loss);
}

// helper function for printing the date
string datetime()
{
	return strf("%02d.%02d.%02d %02d:%02d:%02d",
		day(),month(),year()-2000,hour(),minute(),(int)second());
}

int scriptVersion() { return SCRIPT_VERSION; }

#endif
#endif //ndef WIN32

