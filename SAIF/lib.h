#include "TDBAPI.h"
#include <iostream>
#include <string>
#include <vector>
#include <map>
using namespace std;

static int ErrNum = 0;
static char ErrCode[100][10] = {0};
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

string int2str(int n);
string array2str(const int* arr, int len);
bool isValidDay(int date);

//登录
THANDLE logIn(const string& ipAddress, int port, const string& userName, const string& passWord);

void showTranscation(const TDBDefine_Transaction& trans);
void GetTickData(THANDLE hTdb, char* szCode, char* szMarket, int nDate);//带买卖盘的tick
void GetKData(THANDLE hTdb, char* szCode, char* szMarket, int nBeginDate, int nEndDate, int nCycle, int nUserDef, int nCQFlag, int nAutoComplete);

//获取指定天的所有每笔成交数据
vector<TDBDefine_Transaction> GetTransaction(THANDLE hTdb, char* szCode, char* szMarketKey, int nDate);

//获取自上市日起所有的逐笔成交
map<int, vector<TDBDefine_Transaction>> GetAllTransactions(THANDLE hTdb, char* szCode, char* szMarketKey, int nDate);