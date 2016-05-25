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

enum OrderType {Bigger, Big, Medium, Low};
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
}StockTicker;

typedef struct Stock
{
	TDBDefine_Code tdbCode;
	vector<TDBDefine_Transaction>  tdbTransaction;
	Order  order[4];
} Stock;

static vector<StockTicker> allStockTikers;
static int ErrNum = 0;
static char ErrCode[100][10] = {0};



string int2str(int n);
string array2str(const int* arr, int len);
bool isValidDay(int date);
int getCurrentDay();

//获取所有证券的代码
void GetAllStockTikers(vector<StockTicker>& vec, const string& fileName);

//请求代码表
vector<TDBDefine_Code> GetCodeTable(THANDLE hTdb, char* szMarket);
void StoreCodeTable(const vector<TDBDefine_Code> stockCodes, const string& fileName);

//登录
THANDLE logIn(const string& ipAddress, int port, const string& userName, const string& passWord);

void showTranscation(const TDBDefine_Transaction& trans);
void GetTickData(THANDLE hTdb, char* szCode, char* szMarket, int nDate);//带买卖盘的tick
void GetKData(THANDLE hTdb, char* szCode, char* szMarket, int nBeginDate, int nEndDate, int nCycle, int nUserDef, int nCQFlag, int nAutoComplete);

//获取指定天的所有每笔成交数据
vector<TDBDefine_Transaction> GetTransaction(THANDLE hTdb, char* szCode, char* szMarketKey, int nDate);

//获取自上市日起所有的逐笔成交
map<int, vector<TDBDefine_Transaction>> GetAllTransactions(THANDLE hTdb, char* szCode, char* szMarketKey, int nDate);


#endif
