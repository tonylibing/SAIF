#ifndef __LIB_H__
#define __LIB_H__

#include "TDBAPI.h"
#include <iostream>
#include <algorithm>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <ctime>
#include <map>
using namespace std;

#define ELEMENT_COUNT(arr) (sizeof(arr)/sizeof(*arr))
#define BIGGER  1000000
#define BIG     200000
#define MEDIUM  40000
const int OrderBase = 10000;
enum OrderType 
{OrderLow1,   Order1to2,   Order2to3, Order3to4, Order4to5, Order5to6, Order6to7, Order7to8, Order8to9, Order9to10,
Order10to20, Order20to30, Order30to40, Order40to50, Order50to60, Order60to70, Order70to80, Order80to90, Order90to100, OrderBig100
};
typedef struct Order
{
	OrderType orderType;
	vector<TDBDefine_Transaction> data;
	long long buyValue;
	long long sellValue;

} Order;

typedef struct StockTicker 
{
	string stockCode; //证券代码
	string stockName; //证券简称
	int    boardDay;  //上市日期
	string stockType; //证券类型 SH or SZ
}StockTicker;

typedef struct Stock
{
	TDBDefine_Code tdbCode;
	vector<TDBDefine_Transaction>  tdbTransaction;
	Order  order[4];
} Stock;

typedef struct InputParameter {
	int type;
	int startYear;
	int startMonth;
	int startDay;
	int endYear;
	int endMonth;
	int endDay;
	string stockCode;
	int cycleNumber;
} InputParameter;
static int ErrNum = 0;
static char ErrCode[100][10] = {0};


string changeStockCode(string stockCode);
string int2str(int n);
string array2str(const int* arr, int len);

//获取所有证券的代码
void GetAllStockTikers(vector<StockTicker>& vec, const string& fileName);

//请求代码表
vector<TDBDefine_Code> GetCodeTable(THANDLE hTdb, char* szMarket);
void StoreCodeTable(const vector<TDBDefine_Code> stockCodes, const string& fileName);


void showTranscation(const TDBDefine_Transaction& trans);
void GetTickData(THANDLE hTdb, char* szCode, char* szMarket, int nDate);//带买卖盘的tick


#endif
