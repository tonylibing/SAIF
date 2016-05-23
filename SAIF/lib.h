#ifndef __LIB_H__
#define __LIB_H__

#include "TDBAPI.h"
#include <iostream>
#include <algorithm>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
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
	string stockCode; //֤ȯ����
	string stockName; //֤ȯ���
	int    boardDay;  //��������
}StockTicker;


static vector<StockTicker> allStockTikers;
static int ErrNum = 0;
static char ErrCode[100][10] = {0};



string int2str(int n);
string array2str(const int* arr, int len);
bool isValidDay(int date);

//��ȡ����֤ȯ�Ĵ���
void GetAllStockTikers(vector<StockTicker>& vec, const string& fileName);

//��������
vector<TDBDefine_Code> GetCodeTable(THANDLE hTdb, char* szMarket);

//��¼
THANDLE logIn(const string& ipAddress, int port, const string& userName, const string& passWord);

void showTranscation(const TDBDefine_Transaction& trans);
void GetTickData(THANDLE hTdb, char* szCode, char* szMarket, int nDate);//�������̵�tick
void GetKData(THANDLE hTdb, char* szCode, char* szMarket, int nBeginDate, int nEndDate, int nCycle, int nUserDef, int nCQFlag, int nAutoComplete);

//��ȡָ���������ÿ�ʳɽ�����
vector<TDBDefine_Transaction> GetTransaction(THANDLE hTdb, char* szCode, char* szMarketKey, int nDate);

//��ȡ�������������е���ʳɽ�
map<int, vector<TDBDefine_Transaction>> GetAllTransactions(THANDLE hTdb, char* szCode, char* szMarketKey, int nDate);


#endif