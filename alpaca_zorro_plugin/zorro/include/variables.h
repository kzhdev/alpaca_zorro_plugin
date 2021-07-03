// Zorro predefined variables //////////////////////////
// redefine variables to less scary names //////////////

// deprecated (see also legacy.h)
#define Broker			g->sBroker
#define Account		g->sAccount
#define AccountName	g->sAccountName
#define LoopNum1	g->nLoop[0]
#define LoopNum2	g->nLoop[1]
#define FactorList	g->sFactorList
#define Symbol		g->asset->sSymbol

// g struct
#define PlotLabels g->nPlotLabels
#define PlotScale g->nPlotScale
#define PlotText	g->nPlotText
#define PlotBorder g->nPlotBorder
#define PlotWidth g->nPlotWidth
#define PlotHeight1 g->nPlotHeight1
#define PlotHeight2 g->nPlotHeight2
#define PlotDate	g->nPlotDate
#define PlotStart	g->nPlotStart
#define PlotBars	g->nPlotBars
#define PlotPeriod g->nPlotPeriod
#define ColorDn	g->dwColorDn
#define ColorUp	g->dwColorUp
#define ColorCandle	g->dwColorCandle
#define ColorEquity	g->dwColorEquity
#define ColorDD	g->dwColorDD
#define ColorWin	g->dwColorWin
#define ColorLoss	g->dwColorLoss
#define ColorBars	g->dwColorBars
#define ColorPanel g->dwColorPanel

#define PIP			g->asset->vPIP
#define PIPCost	g->asset->vPIPCost
#define MarginCost g->asset->vMarginCost
#define Leverage	g->asset->vLeverage
#define LotAmount	g->asset->vLotAmount
#define LotLimit	g->asset->nMaxLots
#define Spread		g->asset->vSpread
#define RollLong	g->asset->vRollLong
#define RollShort	g->asset->vRollShort
#define WinPayout g->asset->vWinPayout
#define LossPayout g->asset->vLossPayout
#define Slippage	g->vSlippage
#define Centage	g->asset->nCentage
#define Commission g->asset->vCommission
#define InitialPrice	g->asset->vInitialPrice
#define AskPrice	g->asset->vPrice
#define TimeAsk	g->asset->tAsk
#define TimeBid	g->asset->tBid
#define HedgeRatio g->asset->vFactor
#define Hedge		g->nHedge
#define Optimize	g->nTrainMode
#define Fill		g->nFill
#define Now			g->tNow
#define TrainMode	g->nTrainMode
#define TradeMode	g->nTradeMode
#define PlotMode	g->nPlotMode

#define Lots		g->vLots
#define Amount		g->vAmount
#define Margin		g->vMargin
#define Risk		g->vRisk
#define Entry		g->vEntry
#define Stop		g->vStop
#define StopFactor g->vStopFactor
#define StopPool	g->vStopPool
#define SpreadFactor g->vSpreadFactor
#define OrderLimit g->vOrderLimit
#define OrderDuration g->vOrderDuration
#define OrderDelay g->vOrderDelay
#define Trail		g->vTrail
#define TrailSlope g->vTrailSlope
#define TrailStep g->vTrailStep
#define TrailLock g->vTrailLock
#define TrailSpeed g->vTrailSpeed
#define TakeProfit g->vTakeProfit
#define LifeTime	g->nLifeTime
#define EntryTime	g->nEntryTime
#define EntryDelay g->vOrderDelay
#define MaxLong	g->nMaxLong
#define MaxShort	g->nMaxShort

#define LookBack	g->nLookBack
#define UnstablePeriod	g->nUnstablePeriod
#define DataSkip	g->nDataSkip
#define DataSplit	g->nDataSplit
#define DataSlope	g->vDataSlope
#define DataHorizon	g->nDataHorizon
#define BarPeriod g->vBarPeriod
#define BarOffset	g->nBarOffset
#define BarZone	g->nBarZone
#define HistoryZone	g->nHistoryZone
#define BrokerZone	g->nBrokerZone
#define FrameOffset g->nFrameOffset
#define TimeFrame	g->nTimeFrame
#define NumSampleCycles	g->numSampleCycles
#define NumOptCycles	g->numOptCycles
#define NumTrainCycles	g->numOptCycles
#define NumWFOCycles	g->numWFOCycles
#define NumTotalCycles g->numTotalCycles
#define SampleOffset	g->nSampleOffset
#define SampleCycle	g->nSampleCycle
#define TrainCycle	g->nOptCycle
#define WFOCycle	g->nWFOCycle
#define WFOBar		(g->nBar-g->nWFOStart)
#define WFOPeriod	g->nWFOPeriod
#define TotalCycle	g->nTotalCycle
#define NumDays	g->numDays
#define NumYears	g->numYears
#define StartDate	g->nStartDate
#define EndDate	g->nEndDate
#define StartWeek g->nStartWeek
#define EndWeek	g->nEndWeek
#define StartMarket g->nStartMarket
#define EndMarket	g->nEndMarket
#define Holidays	g->nHolidays
#define UpdateDays g->nUpdateDays
#define ReTrainDays g->nReTrainDays
#define GapDays	g->nGapDays
#define MaxBars	g->nMaxBars
#define NumBars	g->numBars
#define NumTicks	g->numTicks
#define NumMinutes g->numMinutes
#define Bar			g->nBar
#define StartBar	g->nFrameStart
#define EndBar		g->nFrameEnd
#define Day			g->nDay
#define SelectCore	g->nSelectCore
#define TradesPerBar g->nTradesPerBar
#define MinutesPerDay g->nMinutesPerDay
#define TimeStep	g->nTimeStep
#define LogNumber	g->nLogNumber
#define MaxRequests	g->vMaxRequests
#define HWnd		g->hWnd

#define Itor1		(g->nLoop[0]-1)
#define Itor2		(g->nLoop[1]-1)
#define Loop1		g->pLoopPar[0]
#define Loop2		g->pLoopPar[1]
#define Cycle1		g->nLoopCycle[0]
#define Cycle2		g->nLoopCycle[1]
#define NumLoops1	g->numLoops[0]
#define NumLoops2	g->numLoops[1]
#define SelectWFO	g->nSelectWFO
#define NumParameters g->numParTrain
#define ParCycle	g->nParCycle
#define StepCycle	g->nStepCycle
#define StepNext	g->bDoStep
#define LogTrainCycle g->nLogCycle
#define Pin			g->nPin
#define NumAssetsListed g->nAssets
#define NumAssetsUsed g->numAssets
#define Parameters (g->pParameters)
#define BestResult g->vObjectiveBest

#define Detrend	g->nDetrend
#define FuzzyRange g->vFuzzyRange
#define FuzzyLevel g->vFuzzyLevel
#define MonteCarlo g->nMonteCarlo
#define Confidence g->nConfidence

#define Train		is(TRAINMODE)
#define Test		is(TESTMODE)
#define Live		is(TRADEMODE)
#define ReTrain	(is(PROCESS) && is(TRAINMODE))
#define ReTest		(is(PROCESS) && is(TESTMODE))
#define Init		is(INITRUN)
#define Define		g->sDefine
#define Command	g->nCommand
#define NumCores	g->numCores
#define Core		g->nCore
#define Verbose	g->nVerbose
#define SaveMode	g->nSaveMode
#define BarMode	g->nBarMode
#define BoxState	g->nBoxState
#define TickTime	g->nTickTime
#define TockTime	g->nTockTime
#define TickSmooth g->nTickAvg
#define TickFix	g->nTimeFix
#define Outlier	g->vOutlier
#define CBIScale	g->vCBIScale
#define PriceJump g->nPriceEvent
#define DayOffset	g->tDayOffset
#define ExpiryTime g->nExpiryTime
#define TradeRate g->nTradeRate
#define ExitCode	g->nExitCode

#define Capital	g->vCapital
#define Interest	g->vInterest
#define Balance	g->vBalance
#define Equity		g->vEquity
#define MarginVal	g->vMarginVal
#define MarginTotal g->vMarginSum
#define RiskVal	g->vRiskSum
#define RiskTotal	g->vRiskSum
#define MarginMax	g->w.vMarginMax
#define RiskMax	g->w.vRiskMax
#define TradeVal	(g->vWinVal-g->vLossVal)

#define Assets			(g->pAssets)
#define ThisAsset		g->asset
#define Asset			(g->asset->sName)
#define AssetBar		g->asset->nBar
#define AssetFrame	g->asset->nFrame
#define AssetZone		g->asset->nZone
#define AssetMarket	g->asset->nMarket
#define AssetVar		(g->asset->Skill)
#define AssetStr		(string)&(g->asset->Skill)
#define AssetInt		((int*)AssetVar)
#define AssetFloat	((float*)AssetVar)
#define AssetFactor	g->vAssetFactor
#define LotsPool		g->asset->nNetSum
#define LotsVirtual	g->asset->nVirtualSum
#define LotsPhantom	g->asset->nPhantomSum
#define SymbolTrade	g->asset->sSymbol
#define SymbolLive	g->asset->sSymbolLive
#define SymbolHist	g->asset->sSymbolHist
#define Component		g->statLong->nComponent
#define SeriesBuffer	g->pSeriesBuffer

#define AssetMode		g->asset->flags
#define AssetO			(g->asset->pOpen)
#define AssetH			(g->asset->pHigh)
#define AssetL			(g->asset->pLow)
#define AssetC			(g->asset->pClose)
#define AssetP			(g->asset->pPrice)

#define PatternCount	g->vAdvise[0]
#define PatternRate	g->vAdvise[1]
#define PatternPF		g->vAdvise[2]

#define AssetList		g->sAssetList
#define Factors		g->sFactorList
#define Algo			(g->sAlgo)
#define Script			g->sScript
#define History		g->sHistory
#define WebFolder		g->sWebFolder
#define ZorroFolder	g->sZorroFolder
#define Curves			g->sCurves
#define AssetBox		g->sAssetBox

#define WinLong		g->statLong->vWin
#define WinShort		g->statShort->vWin
#define WinTotal		g->w.vWin
#define PipsTotal		g->w.vPips
#define LossLong		g->statLong->vLoss
#define LossShort		g->statShort->vLoss
#define LossTotal		g->w.vLoss
#define WinValLong	g->statLong->vWinVal
#define WinValShort	g->statShort->vWinVal
#define WinValTotal	g->vWinVal
#define LossValLong	g->statLong->vLossVal
#define LossValShort	g->statShort->vLossVal
#define LossValTotal	g->vLossVal
#define NumWinLong	g->statLong->numWin
#define NumWinShort	g->statShort->numWin
#define NumWinTotal	g->w.numWin
#define NumLossLong	g->statLong->numLoss
#define NumLossShort	g->statShort->numLoss
#define NumLossTotal	g->w.numLoss
#define WinStreakLong	g->statLong->nWinStreak
#define WinStreakShort	g->statShort->nWinStreak
#define WinStreakTotal	g->w.nWinStreak
#define LossStreakLong	g->statLong->nLossStreak
#define LossStreakShort	g->statShort->nLossStreak
#define LossStreakTotal	g->w.nLossStreak
#define WinStreakValLong	g->statLong->vWinStreakVal
#define WinStreakValShort	g->statShort->vWinStreakVal
#define WinStreakValTotal	g->w.vWinStreakVal
#define LossStreakValLong	g->statLong->vLossStreakVal
#define LossStreakValShort	g->statShort->vLossStreakVal
#define LossStreakValTotal	g->w.vLossStreakVal
#define NumWinningLong	g->statLong->numWinning
#define NumWinningShort	g->statShort->numWinning
#define NumLosingLong	g->statLong->numLosing
#define NumLosingShort	g->statShort->numLosing
#define NumShortTotal	g->numShort
#define NumLongTotal		g->numLong
#define NumOpenLong		(NumWinningLong+NumLosingLong)
#define NumOpenShort		(NumWinningShort+NumLosingShort)
#define NumOpenTotal		(NumShortTotal+NumLongTotal)
#define NumOpenPhantom	g->numPhantom
#define NumPendingLong	g->statLong->numPending
#define NumPendingShort	g->statShort->numPending
#define NumPendingTotal	g->numPending
#define NumRejected		g->numRejected
#define NumTradesTotal	g->numTrades
#define WinMaxLong		g->statLong->vWinMax
#define WinMaxShort		g->statShort->vWinMax
#define WinMaxTotal		g->w.vWinMax
#define LossMaxLong		g->statLong->vLossMax
#define LossMaxShort		g->statShort->vLossMax
#define LossMaxTotal		g->w.vLossMax
#define BalanceLong		(WinLong-LossLong)
#define BalanceShort		(WinShort-LossShort)
#define EquityLong		(BalanceLong+WinValLong-LossValLong)
#define EquityShort		(BalanceShort+WinValShort-LossValShort)
#define ProfitClosed		(WinLong-LossLong+WinShort-LossShort)
#define ProfitOpen		(WinValLong-LossValLong+WinValShort-LossValShort)
#define ProfitTotal		(WinTotal-LossTotal+WinValTotal-LossValTotal)
#define OptimalFLong		g->statLong->vOptimalF
#define OptimalFShort	g->statShort->vOptimalF
#define OptimalF			g->statShort->vOptimalF2
#define OptimalFRatio	g->vOptimalFRatio
#define ObjectiveLong	g->statLong->vTrainPF
#define ObjectiveShort	g->statShort->vTrainPF
#define TrainPF			g->statShort->vTrainPF
#define AlgoVar2			(g->statLong->Skill)
#define AlgoVar			(g->statShort->Skill)
#define ResultLong		(g->statLong->Result)
#define ResultShort		(g->statShort->Result)
#define ThisPerformance	(g->w)
//#define DailyLong		(g->statLong->pCurve)
//#define DailyShort		(g->statShort->pCurve)
//#define DailyTotal		(g->pCurve)

#define DrawDownMax		g->w.vDrawDown
#define MAE					g->w.vEquityDown
#define MarginMax			g->w.vMarginMax
#define RiskMax			g->w.vRiskMax
#define SpreadCost		g->w.vSpreadCost
#define SlippageCost		(g->w.vSlippageWin+g->w.vSlippageLoss)
#define RollCost			g->w.vRollCost
#define CommissionCost	g->w.vCommissionCost
#define ReturnMean		g->w.vMean
#define ReturnStdDev		g->w.vStdDev
#define ReturnSemiDev	g->w.vSemiDev
#define ReturnUlcer		g->w.vUlcer
#define ReturnR2			g->w.vR2
#define ReturnCBI			g->w.vCBI
#define DrawDownBars		g->w.numDrawDownBars
#define DrawDownBarsMax	g->w.numDrawDownMax
#define LossStreakMax	g->w.numLossStreakMax
#define NumOpenMax		g->w.numTradesMax
#define InMarketBars		g->w.numMarketBars
#define InMarketSum		g->w.numMarketTotal
#define Results			(g->pResults)
#define ResultsDaily		(g->pCurve)
#define DatesDaily		(g->pTime)
#define Bars				(g->bars)
#define Shape				(g->pShape)
#define ScholzBrake		g->vScholzBrake

#define ThisTrade			g->tr
#define TradePriceOpen	((var)g->tr->fEntryPrice)
#define TradeSpread		((var)g->tr->fSpread)
#define TradePriceClose	((var)g->tr->fExitPrice)
#define TradeResult		g->tr->fResult
#define TradeProfit		((var)g->tr->fResult)
#define TradeStopLimit	g->tr->fStopLimit
#define TradeStopDiff	g->tr->fStopDiff
#define TradeProfitLimit g->tr->fProfitLimit
#define TradeTrailLimit g->tr->fTrailLimit
#define TradeTrailDiff	g->tr->fTrailDiff
#define TradeTrailSlope g->tr->fTrailSlope
#define TradeTrailStep	g->tr->fTrailStep
#define TradeTrailLock	g->tr->fTrailLock
#define TradeEntryLimit g->tr->fEntryLimit
#define TradeUnits		((var)(g->tr->fUnits*g->tr->nLots))
#define TradeLife			g->tr->nExitTime
#define TradeTime			(g->nBar-g->tr->nBarOpen)
#define TradeDate			g->tr->tEntryDate
#define TradeExitDate	g->tr->tExitDate
#define TradeBars			(g->nBar-g->tr->nBarOpen)
#define TradeLots			g->tr->nLots
#define TradeLotsTarget	g->tr->nLotsTarget
#define TradeID			g->tr->nID
#define TradeBarOpen		g->tr->nBarOpen
#define TradeBarClose	g->tr->nBarClose
#define TradeMAE			((var)g->tr->fMAE)
#define TradeMFE			((var)g->tr->fMFE)
#define TradeContract	g->tr->nContract
#define TradeStrike		((var)g->tr->fStrike)
#define TradeUnderlying	((var)g->tr->fUnl)
#define TradeRoll			g->tr->fRoll
#define TradeVar			(g->tr->Skill)
#define TradeStr			(string)&(g->tr->Skill)
#define TradeParameter	(g->tr->fArg)
#define TradeDir			ifelse(TradeIsShort,-1,1)
#define TradeStatus		((STATUS*)(g->tr->algo))
#define TradeAlgo			TradeStatus->sAlgo
#define TradeAsset		TradeStatus->asset->sName
#define TradeMarginCost	g->tr->fMarginCost
#define TradeCommission	g->tr->fCommission

// for() macros
#define current_trades	forTrade(20); g->nItor; forTrade(21) // asset/algo trades only
#define open_trades		forTrade(0); g->nItor; forTrade(1) // open trades only
#define closed_trades	forTrade(32+128); g->nItor; forTrade(32+128+1) // closed trades in reverse direction
#define last_trades		forTrade(32+20); g->nItor; forTrade(32+20+1) // all asset/algo trades
#define all_trades		forTrade(2); g->nItor; forTrade(3) // all trades forward
#define past_trades		forTrade(32); g->nItor; forTrade(32+1) // all trades backwards
#define break_trades		if(!forTrade(64)) break	// abort loop
#define return_trades	if(!forTrade(64)) return	// abort loop and return
#define used_assets		forAsset(0); g->nItor; forAsset(1)
#define listed_assets	forAsset(2); g->nItor; forAsset(1+2)
#define break_assets		if(!forAsset(64)) break	// abort loop
#define all_algos			forStatus(0); g->nItor; forStatus(1)
#define break_algos		if(!forStatus(64)) break	// abort loop
#define Itor				(g->nItor-1)

#define setf(mode,flag)	mode |= (flag)
#define resf(mode,flag)	mode &= ~(flag)
#define isf(mode,flag)	(mode & (flag))
#define swap(a,b)			g->vTemp = a; a = b; b = g->vTemp

#define TradeFlag(flag)	((g->tr->flags&(flag)) != 0)
#define TradeIs(flag)	((g->tr->flags&(flag)) == (flag))
#define TradeIsOpen		TradeFlag(TR_OPEN)
#define TradeIsPending	TradeFlag(TR_WAITBUY)
#define TradeIsUnfilled	TradeFlag(TR_NOTFILLED)
#define TradeIsMissed	TradeIs(TR_EVENT|TR_NOTFILLED)
#define TradeIsClosed	(!TradeIsOpen && TradeFlag(TR_STOPPED|TR_PROFIT|TR_TIME|TR_SOLD|TR_EXIT|TR_REMOVED|TR_REVERSED|TR_EXPIRED))
#define TradeIsShort		TradeFlag(TR_SHORT)
#define TradeIsLong		(!TradeFlag(TR_SHORT))
#define TradeIsPhantom	TradeFlag(TR_PHANTOM)
#define TradeIsPool		((g->tr->flags&(TR_PHANTOM|TR_NET)) == TR_NET)
#define TradeIsVirtual	TradeIs(TR_PHANTOM|TR_NET)
#define TradeIsNewBar	(g->tr->nBarClose != g->nBar)
#define TradeIsStop		TradeIs(TR_STOPPED)
#define TradeIsProfit	TradeIs(TR_PROFIT)
#define TradeIsEntry		TradeIs(TR_EVENT|TR_WAITBUY)
#define TradeIsExpired	TradeFlag(TR_EXPIRED)
#define TradeIsContract	((g->tr->nContract&(PUT|CALL|FUTURE)) != 0)
#define TradeIsCall		((g->tr->nContract&CALL) != 0)
#define TradeIsPut		((g->tr->nContract&PUT) != 0)
#define TradeComboLeg	((g->tr->nContract&COMBOLEG))>>9)
#define TradeIsAsset		(TradeStatus->asset == g->PrevAsset)

#define Contracts		(g->asset->pContracts)
#define NumContracts g->asset->numContracts
#define ContractRow	(g->asset->nContractRow+g->asset->nContractOffs)
#define ContractHandle	(g->asset->nHandle)
#define ThisContract	g->contract
#define ThisCombo		g->combo
#define ContractAsk	((var)(ThisContract->fAsk))
#define ContractBid	((var)(ThisContract->fBid))
#define ContractUnl	((var)(ThisContract->fUnl))
#define ContractVal	((var)(ThisContract->fVal))
#define ContractVol	((var)(ThisContract->fVol))
#define ContractStrike ((var)(ThisContract->fStrike))
#define ContractExpiry ThisContract->Expiry
#define ContractType	ThisContract->Type
#define OrderRow		g->nOrderRow
#define Multiplier	g->asset->nMultiplier
#define Exchange		g->asset->sExchange

#define Population	g->nPopulation
#define Generations	g->nGenerations
#define MutationRate	g->nMutationRate
#define CrossoverRate g->nCrossoverRate

#define rAroonDown	g->vAroonDown
#define rAroonUp 		g->vAroonUp
#define rRealUpperBand 	g->vRealUpperBand
#define rRealMiddleBand	g->vRealMiddleBand
#define rRealLowerBand	g->vRealLowerBand
#define rInPhase		g->vInPhase
#define rQuadrature	g->vQuadrature
#define rSine			g->vSine
#define rLeadSine		g->vLeadSine
#define rMACD			g->vMACD
#define rMACDSignal	g->vSignal
#define rSignal		g->vSignal
#define rMACDHist		g->vMACDHist
#define rMAMA			g->vMAMA
#define rFAMA			g->vFAMA
#define rMin			g->vMin
#define rMax			g->vMax
#define rMinIdx		g->vMinIdx
#define rMaxIdx		g->vMaxIdx
#define rSlowK			g->vSlowK
#define rSlowD			g->vSlowD
#define rFastK			g->vFastK
#define rFastD			g->vFastD
#define rDominantPeriod	g->vDominantPeriod
#define rDominantPhase	g->vDominantPhase
#define rRed			g->vRed
#define rGreen			g->vGreen
#define rBlue			g->vBlue
#define rError			g->vError
#define rEMA			g->vEMA
#define rPeak			g->vPeak
#define rSlope			g->vSlope
#define rSign			g->vSign
#define rLength		g->vLength
#define rMean			g->vMean
#define rMomentum		g->vMomentum
#define rTenkan		g->vTenkan
#define rKijun			g->vKijun
#define rSenkouA		g->vSenkouA
#define rSenkouB		g->vSenkouB
#define rHHBar			g->nHHBar
#define rLLBar			g->nLLBar

#define YMDHMS	"%y%m%d %H:%M:%S"
#define HMS	"%H:%M:%S"
#define YMD	"%Y%m%d"
#define OHLC seriesO(),seriesHigh(),seriesL(),seriesC()
#define TICKPERIOD	ifelse(Live,1/60.,1)
#define _POS(s)		debuginfo((char*)s)
