// SAIF.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "TDBAPI.h"
#include <string>
#include <vector>
#include "lib.h"
#include <map>
#include <iostream>
using namespace std;
static fstream fcout("result.txt", fstream::out);

void anslyseAllTransactions(map<int, vector<TDBDefine_Transaction>>& allTransMap, const string& windName)
{
	map<int, vector<TDBDefine_Transaction>>::iterator iter = allTransMap.begin();
	
	for(;iter != allTransMap.end(); iter++)
	{
		Order bigger, big, medium, low;
		int key = iter->first;
		vector<TDBDefine_Transaction> value = iter->second;

		for(vector<TDBDefine_Transaction>::iterator iter2 = value.begin(); iter2 != value.end(); iter2++)
		{
			if(Bigger == checkOrderType(iter2->nTradeVolume))
			{
				bigger.orderType = Bigger;
				bigger.data.push_back(*iter2);
			}
			else if(Big == checkOrderType(iter2->nTradeVolume))
			{
				big.orderType = Big;
				big.data.push_back(*iter2);
			}
			else if(Medium == checkOrderType(iter2->nTradeVolume))
			{
				medium.orderType = Medium;
				medium.data.push_back(*iter2);
			}
			else
			{
				low.orderType = Low;
				low.data.push_back(*iter2);
			}
		}

		calBuyAndSellValue(bigger);
		calBuyAndSellValue(big);
		calBuyAndSellValue(medium);
		calBuyAndSellValue(low);
		
		fcout<<windName<<";";
		fcout<<key<<";"<<bigger.buyValue<<";"<<bigger.sellValue<<";"<<big.buyValue<<";"<<big.sellValue<<";"<<medium.buyValue<<";"<<medium.sellValue<<";"<<low.buyValue<<";"<<low.sellValue<<endl;
	}
}
int _tmain(int argc, _TCHAR* argv[])
{
	string ipAddress = "114.80.154.34";
	int    port      = 6261;
	string userName  = "TD1798799001";
	string passWord  = "19860968";

	THANDLE hTdb = logIn(ipAddress, port, userName, passWord);
	//GetAllStockTikers(allStockTikers, "ticker list.csv");
	if (hTdb)
	{
		GetCodeTable(hTdb, "SH-2-0");
		GetCodeTable(hTdb, "SZ-2-0");

		for (auto iter = allStocks.begin(); iter != allStocks.end(); iter++)
		{
			string chWindCode = (iter->tdbCode).chWindCode;
			map<int, vector<TDBDefine_Transaction>> allTransMap;
			if (chWindCode.find(".SH") != string::npos)
			{
				allTransMap = GetAllTransactions(hTdb, iter->chWindCode, "SH-2-0", 20160523);
			}
			else
			{
				allTransMap = GetAllTransactions(hTdb, iter->chWindCode, "SZ-2-0", 20160523);
			}
			iter->tdbTransactionMap = allTransMap;
			//anslyseAllTransactions(allTransMap, iter->chWindCode);
		}
		task1();
		/*GetKData(hTdb, "600000.SH", "SH-2-0", 20160520, 20160520, CYC_DAY, 0, 0, 0);
		map<int, vector<TDBDefine_Transaction>> allTransMap = GetAllTransactions(hTdb, "600000.SH", "SH-2-0", 20160520);
		anslyseAllTransactions(allTransMap);*/
		//GetTickData(hTdb, "600000.SH", "SH-2-0", 20160517);
		//GetTransaction(hTdb, "600000.SH", "SH-2-0", 20160310);
	}
	TDB_Close(hTdb);
	return 0;
}