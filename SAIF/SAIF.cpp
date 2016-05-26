// SAIF.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "TDBAPI.h"
#include <string>
#include <vector>
#include "lib.h"
#include <map>
#include <Windows.h>
#include <iostream>
using namespace std;
static fstream fcout("result.txt", fstream::out);
OrderType checkOrderType(int value)
{
	if (value < 1*OrderBase)
		return OrderLow1;
	else if (value>=OrderBase && value<2*OrderBase)
		return Order1to2;
	else if (value>=2*OrderBase && value<3*OrderBase)
		return Order2to3;
	else if (value>=3*OrderBase && value<4*OrderBase)
		return Order3to4;
	else if (value>=4*OrderBase && value<5*OrderBase)
		return Order4to5;
	else if (value>=5*OrderBase && value<6*OrderBase)
		return Order5to6;
	else if (value>=6*OrderBase && value<7*OrderBase)
		return Order6to7;
	else if (value>=7*OrderBase && value<8*OrderBase)
		return Order7to8;
	else if (value>=8*OrderBase && value<9*OrderBase)
		return Order8to9;
	else if (value>=9*OrderBase && value<10*OrderBase)
		return Order9to10;
	else if (value>=10*OrderBase && value<20*OrderBase)
		return Order10to20;
	else if (value>=20*OrderBase && value<30*OrderBase)
		return Order20to30;
	else if (value>=30*OrderBase && value<40*OrderBase)
		return Order30to40;
	else if (value>=40*OrderBase && value<50*OrderBase)
		return Order40to50;
	else if (value>=50*OrderBase && value<60*OrderBase)
		return Order50to60;
	else if (value>=60*OrderBase && value<70*OrderBase)
		return Order60to70;
	else if (value>=70*OrderBase && value<80*OrderBase)
		return Order70to80;
	else if (value>=80*OrderBase && value<90*OrderBase)
		return Order80to90;
	else if (value>=90*OrderBase && value<100*OrderBase)
		return Order90to100;
	else
		return OrderBig100;
}
void calBuyAndSellValue(Order& order)
{
	vector<TDBDefine_Transaction>& vec = order.data; 
	order.buyValue = 0;
	order.sellValue = 0;
	for(vector<TDBDefine_Transaction>::iterator iter = vec.begin(); iter != vec.end(); iter++)
	{
		if(iter->chBSFlag == 'B')
		{
			order.buyValue = order.buyValue + (iter->nTradePrice / 10000) * iter->nTradeVolume; 
		}
		else if(iter->chBSFlag == 'S')
		{
			order.sellValue = order.sellValue + (iter->nTradePrice / 10000) * iter->nTradeVolume; 
		}
	}
}
void anslyseAllTransactions(map<int, vector<TDBDefine_Transaction>>& allTransMap, const string& windName)
{
	map<int, vector<TDBDefine_Transaction>>::iterator iter = allTransMap.begin();
	
	for(;iter != allTransMap.end(); iter++)
	{
		Order bigger, big, medium, low;
		vector<Order> orders(20);
		int key = iter->first;
		vector<TDBDefine_Transaction> value = iter->second;

		for(vector<TDBDefine_Transaction>::iterator iter2 = value.begin(); iter2 != value.end(); iter2++)
		{
			OrderType orderType = checkOrderType(iter2->nTradeVolume);
			orders.at(orderType).orderType = orderType;
			(orders.at(orderType).data).push_back(*iter2);
		}
		fcout<<windName<<";"<<key;
		for(int i = 0; i < 20; i++)
		{
			calBuyAndSellValue(orders.at(i));
			fcout<<";"<<orders.at(i).buyValue<<";"<<orders.at(i).sellValue;
		}
		fcout<<endl;
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	string ipAddress = "114.80.154.34";
	int    port      = 6261;
	string userName  = "TD1798799001";
	string passWord  = "19860968";

	THANDLE hTdb = logIn(ipAddress, port, userName, passWord);
	fcout<<"股票名字;日期;<1万;1万~2万;2万~3万;3万~4万;4万~5万;5万~6万;6万~7万;7万~8万;8万~9万;9万~10万;10万~20万;20万~30万;30万~40万;40万~50万;50万~60万;60万~70万;70万~80万;80万~90万;90万~100万;>100万"<<endl;
	//GetAllStockTikers(allStockTikers, "ticker list.csv");
	if (hTdb)
	{
		vector<TDBDefine_Code> shStocks = GetCodeTable(hTdb, "SH-2-0");
		vector<TDBDefine_Code> szStocks = GetCodeTable(hTdb, "SZ-2-0");

		vector<TDBDefine_Code> allStocks;
		allStocks.insert(allStocks.end(), shStocks.begin(), shStocks.end());
		allStocks.insert(allStocks.end(), szStocks.begin(), szStocks.end());
		vector<TDBDefine_Code>().swap(shStocks);
		vector<TDBDefine_Code>().swap(szStocks);
		StoreCodeTable(allStocks, "stockCodeTable.txt");

		map<int, vector<TDBDefine_Transaction>> allTransMap;
		for (auto iter = allStocks.begin(); iter != allStocks.end(); iter++)
		{
			allTransMap = GetAllTransactions(hTdb, iter->chWindCode,  iter->chMarket, getCurrentDay());
			anslyseAllTransactions(allTransMap, iter->chWindCode);
			allTransMap.clear();
		}
		/*GetKData(hTdb, "600000.SH", "SH-2-0", 20160520, 20160520, CYC_DAY, 0, 0, 0);
		map<int, vector<TDBDefine_Transaction>> allTransMap = GetAllTransactions(hTdb, "600000.SH", "SH-2-0", 20160520);
		anslyseAllTransactions(allTransMap);*/
		//GetTickData(hTdb, "600000.SH", "SH-2-0", 20160517);
		//GetTransaction(hTdb, "600000.SH", "SH-2-0", 20160310);
	}
	TDB_Close(hTdb);
	return 0;
}