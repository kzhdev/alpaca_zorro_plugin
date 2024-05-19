#pragma once

//////////////////////////////////////////////////////////////////////
// 
// Alpaca Plugin Custom BrokerCommand Definitions
//
//////////////////////////////////////////////////////////////////////
#include "include/default.c"

#define ORDERTYPE_IOC 1
#define ORDERTYPE_GTC 2
#define ORDERTYPE_FOK 3
#define ORDERTYPE_DAY 4
#define ORDERTYPE_OPG 5
#define ORDERTYPE_CLS 6

#define ADJUSTMENT_RAW 0
#define ADJUSTMENT_SPLIT 1
#define ADJUSTMENT_DEVIDEND 2
#define ADJUSTMENT_ALL 3

#define CREATE_ASSETLIST 2001
#define IS_ASSET_FRACTIONABLE 2002
#define IS_ASSET_SHORTABLE 2003
#define IS_ASSET_EASY_TO_BORROW 2004
#define IS_ASSET_MARGINABLE 2005
#define SET_LOG_TYPE 2006
#define SET_LOG_LEVEL 2007
#define SET_ADJUSTMENT 2008