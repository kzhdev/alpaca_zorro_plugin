// options / futures utilities ///////////////////////////////
#include <r.h>
//#define DO_LOG

void contractPrint(CONTRACT* c,int To)
{
	if(!c) return; 
	print(To,"\n%s,%s%s%s,%i,%s,%s,%s,%s,%s,%s,%s",
		ifelse(is(TRADEMODE),(char *)c,strdate("%Y-%m-%d",c->time)),
		ifelse(c->Type&FUTURE,"Future",""),
		ifelse(c->Type&PUT,"Put",""),
		ifelse(c->Type&CALL,"Call",""),
		c->Expiry,
		sftoa(c->fStrike,5),
		sftoa(c->fUnl,5),
		sftoa(c->fAsk,4),
		sftoa(c->fBid,4),
		sftoa(c->fVal,3),
		sftoa(c->fVol,3),
		ifelse(is(TRADEMODE),strcon(c),""));
}

void contractPrint(CONTRACT* c) { set(LOGFILE); contractPrint(c,TO_FILE); }

// print contracts to the log for testing
void contractPrint(int Handle, int To)
{
	set(LOGFILE);
	if(is(TRADEMODE))
		print(To,"\nClass,Type,Expiry,Strike,Underlying,Ask,Bid,Multiplier,Vol,Symbol");
	else
		print(To,"\nDate,Type,Expiry,Strike,Underlying,Ask,Bid,Val,Vol");
	int i = 0;
	if(Handle > 0) {
		CONTRACT* c;
		while(c = (CONTRACT*)dataStr(Handle,i++,0))
			contractPrint(c,To);
	} else if(!Handle) { // print current contract chain
		for(i=0; i < NumContracts; i++)
			contractPrint(Contracts + i,To);
	}
}

void contractPrint() { contractPrint(0,TO_CSV); }

// Quantlib Interface //////////////////////////////////////////

int RQLInitialized = 0;

int initRQL()
{
	if(!Rstart(0,2)) { 
		quit("Error - can't start R!"); 
		return 0; 
	}
	Rx("Installed <- require('RQuantLib')");
	RQLInitialized = 1;
	if(!Ri("Installed")) {
		RQLInitialized = -1;
		quit("\nError - RQuantLib not installed!");
		return 0;
	}
	return 1;
}

var contractVal(CONTRACT* c,var Price,var HistVol,var Dividend,var RiskFree,
	var* Delta,var* Gamma,var* Vega,var* Theta,var* Rho,...)
{	
	if(!RQLInitialized) initRQL();
	if(RQLInitialized != 1) return 0;
	if(!c) return 0;
	if(!(c->Type&(PUT|CALL))) return 0;

	Rset("Strike",(double)c->fStrike);
	Rset("Expiry",contractDays(c)/365.25);
	Rset("Price",Price);
	Rset("Volatility",HistVol);
	Rset("Dividend",Dividend);
	Rset("RiskFree",RiskFree);
	if(c->Type&PUT)
		Rset("Type","put");
	else
		Rset("Type","call");
	if(c->Type&BINARY) {
		if(c->Type&EUROPEAN)
			Rset("ExcType","european");
		else
			Rset("ExcType","american");
		Rx("Option <- BinaryOption('cash',Type,ExcType,Price,Price,Dividend,RiskFree,Expiry,Volatility,Strike)");
	} else {
		if(c->Type&EUROPEAN)
			Rx("Option <- EuropeanOption(Type,Price,Strike,Dividend,RiskFree,Expiry,Volatility)");
		else
			Rx("Option <- AmericanOption(Type,Price,Strike,Dividend,RiskFree,Expiry,Volatility,engine='CrankNicolson')");
	}
	var Value = Rd("Option$value");
	if(Delta) *Delta = Rd("Option$delta");
	if(Gamma) *Gamma = Rd("Option$gamma");
	if((Vega || Theta || Rho) && !(c->Type&EUROPEAN)) // calculate Vega etc. always with Black-Scholes
		Rx("Option <- EuropeanOption(Type,Price,Strike,Dividend,RiskFree,Expiry,Volatility)");
	if(Vega) *Vega = Rd("Option$vega");
	if(Theta) *Theta = Rd("Option$theta");
	if(Rho) *Rho = Rd("Option$rho");
#ifdef DO_LOG
	printf("\n%s: %i(%.2f) %.2f %.2f %.2f => %.2f",
		strdate("%Y-%m-%d",c->time),c->Expiry,contractDays(c)/365.25,
		(var)c->fStrike,Price,HistVol,Value);
#endif	
	return Value;
}

var contractVol(CONTRACT* c,var Price,var HistVol,var Value,var Dividend,var RiskFree)
{	
	if(!RQLInitialized) initRQL();
	if(RQLInitialized != 1) return 0;
	var Strike = (double)c->fStrike;
	var Expiry = contractDays(c)/365.25;
	if(!c) return 0;
	if(!(c->Type&(PUT|CALL))) return 0;
	if((c->Type&PUT) && Price+Value <= Strike) return 0.;	// otherwise, RQL crash
	if((c->Type&CALL) && Price-Value >= Strike) return 0.;
	if(c->fStrike > 2*Price || Price > 2*c->fStrike) return 0.;

	string Type = ifelse(c->Type&PUT,"put","call");
	Rset("Type",Type);
	Rset("Strike",Strike);
	Rset("Expiry",Expiry);
	Rset("Price",Price);
	Rset("Value",Value);
	Rset("Volatility",HistVol);
	Rset("Dividend",Dividend);
	Rset("RiskFree",RiskFree);
	//printf("\nIV: %s %.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f",
		//Type,Value,Price,Strike,Dividend,RiskFree,Expiry,HistVol);
		//Rd("Value"),Rd("Price"),Rd("Strike"),Rd("Dividend"),Rd("RiskFree"),Rd("Expiry"),Rd("Volatility"));

	if((c->Type&EUROPEAN))// || Expiry < 0.02)
		Rx("Vol <- EuropeanOptionImpliedVolatility(Type,Value,Price,Strike,Dividend,RiskFree,Expiry,Volatility)");
	else
		Rx("Vol <- AmericanOptionImpliedVolatility(Type,Value,Price,Strike,Dividend,RiskFree,Expiry,Volatility)");

	var Vol = Rd("Vol");
#ifdef DO_LOG
	static int warned = 0;
	if(Vol == 0. || (!warned++ && (Verbose&4)))
		printf("\nVol %.2f: %s %.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f",
			//Rd("Vol"),Type,Rd("Value"),Rd("Price"),Rd("Strike"),Rd("Dividend"),Rd("RiskFree"),Rd("Expiry"),Rd("Volatility"));
			Vol,Type,Value,Price,Strike,Dividend,RiskFree,Expiry,HistVol);
#endif
	return Vol;
}

var contractIntrinsic(CONTRACT* c,var Price)
{
	if(!c) return 0;
	if(c->Type&CALL) return Price - c->fStrike;
	else if(c->Type&PUT) return c->fStrike - Price;
	else return 0;
}

var contractIntrinsic(TRADE* tr,var Price)
{
	if(tr->nContract&CALL) return Price - tr->fStrike;
	else if(tr->nContract&PUT) return tr->fStrike - Price;
	else return 0;
}

var contractProfit(CONTRACT* c,var Price,var Premium)
{
	if(!c) return 0;
	if(Premium < 0) // contract was bought
		return max(0,contractIntrinsic(c,Price)) + Premium;
	else // contract was sold
		return min(0,-contractIntrinsic(c,Price)) + Premium;
}

var contractProfitN(CONTRACT* C,int N,var Price)
{
	if(!C || !N) return 0;
	return abs(N)*contractProfit(C,Price,ifelse(N > 0,-C->fAsk,C->fBid));
}

var contractStrike(int Type,int Days,var Price,var HistVol,var RiskFree,var Delta)
{
	if(!Live && Verbose > 0 && (!Type || Price == 0. || abs(Delta) > 1)) { 
		printf("\nError 011: contractStrike(%i,%i,%.2f,..,%.3f)",
		Type,Days,Price,Delta); 
		quit("");
		return 0;
	} 
//	K = S0 * exp(-qnorm(delta) * sigma * sqrt(T) + ((sigma^2)/2) * T)
	var T = Days/365.25;
	var D1 = qnorm(abs(Delta)); // * exp(RiskFree*T));
	if(Type&CALL) D1 = -D1;
	return Price * exp(D1*HistVol*sqrt(T) + (RiskFree+HistVol*HistVol/2.)*T);
}

var contractDelta(int Type,int Days,var Price,var HistVol,var RiskFree,var Strike)
{
// http://web.math.ku.dk/~rolf/EasyDelta.pdf
//	delta = cdf((ln(S0/K) + (r+(sigma^2)/2)*T) /(sigma * sqrt(T)))
	if(!Live && Verbose > 0 && (!Type || Price == 0. || Strike == 0.))
	{ printf("\nError 011: contractDelta"); quit(""); } 
	var T = Days/365.25;
	var D1 = (log(Price/Strike)+(RiskFree+HistVol*HistVol/2.)*T) / (HistVol*sqrt(T));
	var Delta = cdf(D1); // *exp(-RiskFree*T);
	//printf("\nP %.0f S %.0f T %.3f D1 %.3f",Price,Strike,T,D1);
	if(Type&CALL) return Delta;
	else return Delta-1.;
}

var contractVega(int Type,int Days,var Price,var HistVol,var RiskFree,var Strike)
{
// http://www.iotafinance.com/en/Formula-Vega-of-an-option.html
//	vega = Price* 1/sqrt(2pi) * exp(-D1^2/2) * sqrt(T)
// D1 = (ln(S0/K) + (r+(sigma^2)/2)*T)/(sigma * sqrt(T))) 
	if(!Live && Verbose > 0 && (!Type || Price == 0. || Strike == 0.))
	{ printf("\nError 011: contractVega"); quit(""); } 
	var T = Days/365.25;
	var D1 = (log(Price/Strike)+(RiskFree+HistVol*HistVol/2.)*T) / (HistVol*sqrt(T));
	return Price * 0.3989 /* = 1./sqrt(2.*PI)*/ * exp(-(D1*D1)/2.) * sqrt(T);
}

var contractUnderlying()
{
	if(Contracts) {
		if(Contracts->fUnl == 0) 
			contractPrice(Contracts);
		if(Contracts->fUnl > 0) 
			return Contracts->fUnl;
	}
	return priceClose(0);
}

// open a duplicate of a closed contract
TRADE* contractRoll(TRADE* tr,int Days,var Strike,function TMF)
{
	if(Strike == 0.) Strike = tr->fStrike;
	CONTRACT* c = contract(tr->nContract&(FUTURE|PUT|CALL),Days,Strike);
	var Price = contractPrice(c);
	if(Price == 0.) {
		printf("#\nCan't roll - no contract at strike %.2f",Strike);
		return 0;
	}
	Lots = tr->nLots;
	Stop = tr->fStopLimit*Price/tr->fEntryPrice;
	TakeProfit = tr->fProfitLimit*Price/tr->fEntryPrice;
	if(tr->flags&TR_SHORT)
		return enterShort(TMF);
	else
		return enterLong(TMF);	
}

TRADE* contractRoll(TRADE* tr,int Days)
{
	return contractRoll(tr,Days,0,0);
}

// get rid of underlying when exercised
int contractSellUnderlying()
{
	if(!Live) return 0;
	int OpenLots = brokerCommand(GET_POSITION,(DWORD)Asset);
	if(OpenLots > 0) {
		contract(0); // select underlying
		TRADE* SellUnl = enterShort(OpenLots);
		if(SellUnl) cancelTrade((DWORD)SellUnl);
	}
	return OpenLots;
}


var contractMargin(CONTRACT* c, int AssetType)
{
	if (!c || !AssetType) return 0;
	var Unl = c->fUnl;
	if (Unl == 0.) return 0;
	var Price = c->fBid;
	if (Price == 0. || Unl == 0.) contractPrice(c);
	var Strike = c->fStrike;
	var OTM = max(0,-contractIntrinsic(c, Unl));
	if (c->Type&CALL) {
		if (AssetType == 3) // stock; Price + Max((20% Underlying - OTM Amount), 10% Underlying)
			return Price + max(0.2*Unl - OTM, 0.1*Unl);
		else if (AssetType == INDEX) // Price + Max((15% Underlying - OTM Amount), 10% Underlying)
			return Price + max(0.15*Unl - OTM, 0.1*Unl);
		else if (AssetType == FOREX) // currency; Price + Max((4% Underlying - OTM Amount), 0.75% Underlying))
			return Price + max(0.04*Unl - OTM, 0.0075*Unl);
	}
	else {	// put
		if (AssetType == 3) // Price + Max((20% Underlying - OTM Amount), 10% Strike)
			return Price + max(0.2*Unl - OTM, 0.1*Strike);
		else if (AssetType == INDEX) // Price + Max((15% Underlying - OTM Amount), 10% Strike)
			return Price + max(0.15*Unl - OTM, 0.1*Strike);
		else if (AssetType == FOREX) // Price + Max((4% Underlying - OTM Amount), 0.75% Underlying)
			return Price + max(0.04*Unl - OTM, 0.0075*Unl);
	}
	return 0;
}

///////////////////////////////////////////////////////////

// define a combo of up to 4 contracts
int combo(CONTRACT* C1,int N1,CONTRACT* C2,int N2,CONTRACT* C3,int N3,CONTRACT* C4,int N4)
{
	g->nComboLeg = 0;
	if((N1 && !C1) || (N2 && !C2) || (N3 && !C3) || (N4 && !C4)) {
		memset(ThisCombo,0,sizeof(COMBO));
		return 0;
	}
	ThisCombo->Leg[0] = C1;
	ThisCombo->N[0] = N1;
	ThisCombo->Leg[1] = C2;
	ThisCombo->N[1] = N2;
	ThisCombo->Leg[2] = C3;
	ThisCombo->N[2] = N3;
	ThisCombo->Leg[3] = C4;
	ThisCombo->N[3] = N4;
	return comboLegs();
}

#define comboContract(n)	ThisCombo->Leg[n-1]
#define comboSign(n)			ThisCombo->N[n-1]

var comboStrike(int N)
{
	if(1 > N || N > 4) return 0;
	if(!ThisCombo->Leg[N-1] || !ThisCombo->N[N-1]) return 0;
	return ThisCombo->Leg[N-1]->fStrike;
}

var comboRisk(int Sign)
{
	var MaxRisk = 0,CRisk = 0,AvgStrike = 0;
	COMBO *Combo = ThisCombo;
// all contracts at almost zero
	int i, NLots = 0;
	for(i=0; i<4; i++) {
		if(!Combo->N[i]) break;
		NLots += abs(Combo->N[i]);
		AvgStrike += abs(Combo->N[i])*Combo->Leg[i]->fStrike;
		MaxRisk += contractProfitN(Combo->Leg[i],Sign*Combo->N[i],0.001);
	}
	if(!NLots) return 0;
// all contracts at avg strike 
	AvgStrike /= NLots;
	CRisk = 0;
	for(i=0; i<4; i++) {
		if(!Combo->N[i]) break;
		CRisk += contractProfitN(Combo->Leg[i],Sign*Combo->N[i],AvgStrike);
	}
	MaxRisk = min(MaxRisk,CRisk);
// all contracts at (2*strike)
	CRisk = 0;
	for(i=0; i<4; i++) {
		if(!Combo->N[i]) break;
		CRisk += contractProfitN(Combo->Leg[i],Sign*Combo->N[i],2*AvgStrike);
	}
	MaxRisk = min(MaxRisk,CRisk);
	return -MaxRisk;
}

var comboRisk() { return comboRisk(1); }

var comboProfit(var Price,int Sign)
{
	int i = comboLegs();
	var Profit = 0;
	for(; i>0; i--) {
		int N = Sign*comboLeg(i);
		CONTRACT* C = comboContract(i);
		Profit += contractProfitN(C,N,Price);
	}
	return Profit;
}

var comboPremium(int Sign)
{
	COMBO *Combo = ThisCombo;
	var Premium = 0;
	int i;
	for(i=0; i<4; i++) {
		if(Combo->N[i] && Combo->Leg[i]) {
			int N = Sign*Combo->N[i];
			if(Combo->Leg[i]->fAsk == 0. && Combo->Leg[i]->fBid == 0.)
				contractPrice(Combo->Leg[i]);
			Premium += N*ifelse(N > 0,Combo->Leg[i]->fAsk,Combo->Leg[i]->fBid);
		}
	}
	return -Premium;
}

int comboType()
{
	if(!comboContract(1))
		return 0;
	else if(!comboContract(2)) // put or call
		return comboContract(1)->Type;
	else if(!comboContract(3)) // Spread (call-call,put-put) or Strangle (call-put)
		return 4 + ((comboContract(1)->Type)&(CALL|PUT))
		+ ((comboContract(2)->Type)&(CALL|PUT));
	else if(!comboContract(4)) // Butterfly (call-call-call,put-put-put)
		return 12 + ((comboContract(1)->Type)&(CALL|PUT));
	else // we assume Condor
		return 20;
}

// Margin calculation from
// https://www.interactivebrokers.com/en/index.php?f=26660&hm=us&ex=us&rgt=1&rsk=0&pm=1&rst=101004100808
var comboMargin(int Sign, int AssetType)
{
	int i, ComboType = comboType();
	bool IsLong = Sign > 0;
	int Legs = comboLegs();
	if (!Legs) return 0;
	var Price = max(0,-comboPremium(Sign));
	switch (ComboType) {
	case 1:	// call
	case 2: 	// put
		if(IsLong) return Price;
		else return contractMargin(comboContract(1), AssetType);
	case 5: { // call spread (call-call), max(Strike Long - Strike Short,0)
		var Diff = comboStrike(1) - comboStrike(2);
		if (comboLeg(1) < 0) Diff = -Diff;
		return max(0, Diff) / 2;
		}
	case 6: { // put spread (put-put), max(Strike Short - Strike Long,0)
		var Diff = comboStrike(1) - comboStrike(2);
		if (comboLeg(1) > 0) Diff = -Diff;
		return max(0, Diff) / 2;
		}
	case 7: // Strangle (call-put)
		if(IsLong)  // Margined as two long options
			return Price/2; 
		else { // If Margin Put > Margin Call, then Margin Put + Price Call; else Margin Call + Price Put
			var Margin1 = contractMargin(comboContract(1), AssetType),
				Margin2 = contractMargin(comboContract(2), AssetType);
			var Price1 = comboContract(1)->fBid,
				Price2 = comboContract(2)->fBid;
			if (Margin1 > Margin2) return (Margin1 + Price2) / 2;
			else return (Margin2 + Price1) / 2;
		}
	case 13: // Butterfly call (long-2*short-long)
	case 14: { // Butterfly put (long-2*short-long)
		if(IsLong) return Price/4; 
		var HighStrike = max(comboStrike(1), comboStrike(3));
		var LowStrike = min(comboStrike(1), comboStrike(3));
		var MiddleStrike = comboStrike(2);
		if (ComboType == 14) // put; max(Highest Strike - Middle Strike, 0) + max(Lowest Strike - Middle Strike, 0)
			return (max(HighStrike - MiddleStrike, 0) + max(LowStrike - MiddleStrike, 0))/4;
		else // call; max(Middle Strike - High Strike, 0) + max(Middle Strike - Lowest Strike, 0)
			return (max(MiddleStrike - HighStrike, 0) + max(MiddleStrike - LowStrike, 0))/4;
		}
	case 20: { // Condor (short put, long put, short call, long call); Short Put Strike - Long Put Strike
		var FirstPutStrike = 0;
		for(i = 4; i > 0; i--)
			if((comboContract(i)->Type&PUT)) {
				if (FirstPutStrike == 0) FirstPutStrike = comboStrike(i);
				else return abs(FirstPutStrike - comboStrike(i))/4;
			}
		}
	case 0:
	default: {
		// unknown combo, treat as single contracts	
		Price = 0;
		for(i = Legs; i > 0; i--) {
			if(comboSign(i) > 0)
				Price += comboSign(i)*comboContract(i)->fAsk;
			else 
				Price += -comboSign(i)*contractMargin(comboContract(i),AssetType);
		}
		return Price/Legs;
		}
	}
}

void comboPrint(int To)
{
	if(!comboLegs()) return;
	set(LOGFILE);
	if(is(TRADEMODE))
		print(To,"\nClass,Type,Expiry,Strike,Underlying,Ask,Bid,Multiplier,Vol,Symbol");
	else
		print(To,"\nDate,Type,Expiry,Strike,Underlying,Ask,Bid,Val,Vol");
	int i;
	for(i=1; i <= comboLegs(); i++)
		contractPrint(comboContract(i),To);
}

///////////////////////////////////////////////////////////////////////
#define CDIVIDEND	0.01
#define CRISKFREE	0.01
#define CPOINTS	300

void plotContract(int N,CONTRACT* C,var HistVol,var Min,var Max,int Days,int Modus)
{
	static var Values[CPOINTS];
	PlotScale = 10;
	PlotLabels = CPOINTS/30;
	var Step = (Max-Min)/CPOINTS;
	Min = round(Min);
	if(!Multiplier) Multiplier = 100;
	int Exp = 0, ExpO;
	if(C) {
		ExpO = C->Expiry;
		Exp = contractDays(C);
		if(!Days) Days = max(Exp/2,1);
		if(Modus == 2) // set shorter expiry
			C->Expiry = max(Exp-Days,1);
	}
	string Title = "At Expiry"; 
	int Type = LINE, Color = BLUE;
	if(Modus == 2) {
		Title = strf("At %i Days",Days); Color = RED;
	} else if(Modus == 3) {
		Title = "Delta"; Type |= AXIS2; Color = GREEN;
	}

	int i;
	for(i=0; i<CPOINTS; i++)
	{
		if(N == 0 || C == 0 || Modus == 0) {
			Values[i] = 0;
			plotBar("#Zero",i,0,0,LINE,BLACK);
			continue;
		} 
		var Price = Min + i*Step;
		if(Price > Max) break;
		if(Modus == 1 || (Modus == 2 && Days >= Exp))
			Values[i] += Multiplier*contractProfitN(C,N,Price);
		else if(Modus == 2)
			Values[i] += Multiplier*N*(contractVal(C,Price,HistVol,CDIVIDEND,CRISKFREE) 
				- ifelse(N>0,C->fAsk,C->fBid));
		else if(Modus == 3)
			Values[i] += N*contractDelta(C->Type,Exp,Price,HistVol,CRISKFREE,C->fStrike);
		plotBar(Title,i,Price,Values[i],Type,Color);
	}
	if(C) C->Expiry = ExpO;
}

void plotCombo(var HistVol,var Min,var Max,int Days,int Modus)
{
	int i;
	plotContract(0,0,0,0,0,0,0); 	// reset arrays
	for(i=0; i<comboLegs(); i++)
		plotContract(comboLeg(i+1),comboContract(i+1),HistVol,Min,Max,Days,Modus);
}

///////////////////////////////////////////////////////////////////////

var dataFromCSV(int Handle,const char* Format, const char* Filename,int Column,int Offset)
{
	if(dataFind(Handle,0) < 0) { // data array not yet loaded
		if(!dataParse(Handle,Format,Filename)) return 0;
	}
	int Row = dataFind(Handle,wdate(0)-Offset/1440.);
	return dataVar(Handle,Row,Column); 
}

var dataFromCSV(int Handle, const char* Format, const char* Filename,int Column)
{
	return dataFromCSV(Handle,Format,Filename,Column,0);
}

var dataFromQuandl(int Handle,const char* Format,const char* Code,int Column)
{
	string Filename = strxc(Code,'/','-');
	Filename = strxc(Filename,':','_');
	if(dataFind(Handle,0) < 0) { // data array not yet loaded
		dataDownload(Code,FROM_QUANDL,12*60);
		if(!dataParse(Handle,Format,Filename)) return 0;
	}
	if(is(TRADEMODE) && !is(LOOKBACK)) {
		strcat(Filename,"1");
		int Rows = dataDownload(Code,FROM_QUANDL|1,60);
		if(Rows) dataParse(Handle,Format,Filename);	// add new record to the end of the array
		return dataVar(Handle,-1,Column);
	} else {
		int Row = dataFind(Handle,wdate(0)-16./24); // adjust for US market opening time
		return dataVar(Handle,Row,Column); 
	}
}

// US treasury 3-months interest rate
var yield() { 
	return dataFromQuandl(H_YIELD,"%Y-%m-%d,f","FRED/DTB3",1); 
}

var yieldCSV() { 
	return dataFromCSV(H_YIELD,"%Y-%m-%d,f","FRED-DTB3",1,16*60); 
}

// empirical volatility for options
var VolatilityOV(int Days)
{
	var a = 1, alpha = 0.92, numer = 0, denom = 0;
	int i;
	for(i=1; i<=Days; i++) {
		var SV = .627 * 19.11 * log(dayHigh(ET,i)/max(1,dayLow(ET,i))); // 19.11 = sqrt(365.25)
		numer += a*SV;
		denom += a;
		a *= alpha;
	}
	return numer/denom;
}

// return the date of the Nth given weekday of the given month 
var nthDay(int Year,int Month,int Dow,int N)
{
	var Prev = Now;
	Now = wdatef("%Y %m",strf("%i %i",Year,Month));
	while(N > 0) {
		Now += 1.; // next day
		if(dow(NOW) == Dow) N--;
	}
	var Result = Now;	Now = Prev;
	return Result;
}

///////////////////////////////////////////////////////


var COT(int Handle,string Code,int Field) {
// Date (0),Open Interest (1),Producer/Merchant/Processor/User Longs(2),Producer/Merchant/Processor/User Shorts(3),
// Swap Dealer Longs(4),Swap Dealer Shorts(5),Swap Dealer Spreads,Money Manager Longs(6),Money Manager Shorts(7),Money Manager Spreads,
// Other Reportable Longs,Other Reportable Shorts,Other Reportable Spreads,
// Total Reportable Longs,Total Reportable Shorts,Non Reportable Longs,Non Reportable Shorts
// 2018-08-07,64923.0,33838.0,13985.0,0.0,6368.0,17847.0,193.0,5713.0,1097.0,133.0,4106.0,3.0,32.0,50383.0,33290.0,14540.0,31633.0
	const char* COT_Format = "%Y-%m-%d,f1,f2,f3,f4,f5,,f6,f7";	
	const char* COT_Code = strf("CFTC/%s_F_ALL",Code);
	return dataFromQuandl(Handle,COT_Format,COT_Code,Field);
}

// Net Commercial Position = Total Reportable Longs - Total Reportable Shorts
int COT_CommercialPos(int Handle,string Code) {
	return COT(Handle,Code,2)-COT(Handle,Code,3)+COT(Handle,Code,4)-COT(Handle,Code,5);
}

int COT_CommercialIndex(int Handle,string Code,int TimePeriod) {
	return 0.5 * normalize(COT_CommercialPos(Handle,Code),TimePeriod) + 50;
}

int COT_OpenInterest(int Handle,string Code) {
	return COT(Handle,Code,1);
}

