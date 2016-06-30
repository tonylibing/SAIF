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

static fstream fcout;
static vector<StockTicker> allStockTikers;

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
void anslyseAllTransactionsByMonth(THANDLE& hTdb, int year, int month)
{
	string resultName = string(".\\result\\") + int2str(year*100 + month) + string(".txt");
	fcout.open(resultName, fstream::app|fstream::out);
	fcout<<"股票名字;日期;<1万买;<1万卖;1万~2万买;1万~2万卖;2万~3万买;2万~3万卖;3万~4万买;3万~4万卖;4万~5万买;4万~5万卖;5万~6万买;5万~6万卖;6万~7万买;6万~7万卖;7万~8万买;7万~8万卖;8万~9万买;8万~9万卖;9万~10万买;9万~10万卖;";
	fcout<<"10万~20万买;10万~20万卖;20万~30万买;20万~30万卖;30万~40万买;30万~40万卖;40万~50万买;40万~50万卖;50万~60万买;50万~60万卖;60万~70万买;60万~70万卖;70万~80万买;70万~80万卖;80万~90万买;80万~90万卖;90万~100万买;90万~100万卖;>100万买;>100万卖"<<endl;
	map<int, vector<TDBDefine_Transaction>> allTransMap;

	for (auto iter = allStockTikers.begin(); iter != allStockTikers.end(); iter++) 
	{ 
		allTransMap = GetAllTransactions(hTdb, (char*)(iter->stockCode).c_str(), (char*)(iter->stockType).c_str(), year, month);
		anslyseAllTransactions(allTransMap, iter->stockCode);
		allTransMap.clear();
	}
	fcout.close();
}

int _tmain(int argc, _TCHAR* argv[])
{
	string ipAddress = "114.80.154.34";
	int    port      = 6261;
	string userName  = "TD1798799008";
	string passWord  = "33773595";
	THANDLE hTdb = logIn(ipAddress, port, userName, passWord);

	GetAllStockTikers(allStockTikers, "ticker list.csv");

	InputParameter inputParameter = readInput();

	if(hTdb)
	{
		if(inputParameter.type == 2)
		{
			for (auto iter = allStockTikers.begin(); iter != allStockTikers.end(); iter++)
			{
				anslyseAllTransactionsByMonth(hTdb, inputParameter.startYear, inputParameter.startMonth);
			}
		}
		else if (inputParameter.type == 3)
		{
			string resultName = string(".\\result\\") + inputParameter.stockCode + int2str(inputParameter.startYear*100 + inputParameter.startMonth) + string(".txt");
			fcout.open(resultName, fstream::out);
			fcout<<"股票名字;日期;<1万买;<1万卖;1万~2万买;1万~2万卖;2万~3万买;2万~3万卖;3万~4万买;3万~4万卖;4万~5万买;4万~5万卖;5万~6万买;5万~6万卖;6万~7万买;6万~7万卖;7万~8万买;7万~8万卖;8万~9万买;8万~9万卖;9万~10万买;9万~10万卖;";
			fcout<<"10万~20万买;10万~20万卖;20万~30万买;20万~30万卖;30万~40万买;30万~40万卖;40万~50万买;40万~50万卖;50万~60万买;50万~60万卖;60万~70万买;60万~70万卖;70万~80万买;70万~80万卖;80万~90万买;80万~90万卖;90万~100万买;90万~100万卖;>100万买;>100万卖"<<endl;
			bool flag = false;
			for (auto iter = allStockTikers.begin(); iter != allStockTikers.end(); iter++)
			{
				if(iter->stockCode == inputParameter.stockCode) 
				{
					flag = true;
				}
				if(flag) 
				{
					map<int, vector<TDBDefine_Transaction>> allTransMap;

					allTransMap = GetAllTransactions(hTdb, (char*)(iter->stockCode).c_str(), (char*)(iter->stockType).c_str(), inputParameter.startYear, inputParameter.startMonth);
					anslyseAllTransactions(allTransMap, iter->stockCode);
					allTransMap.clear();
				}
			}
			fcout.close();
		}
		else
		{
			vector<pair<int, int>> timeVec = timeRange(inputParameter.startYear, inputParameter.startMonth, inputParameter.endYear, inputParameter.endMonth);
			for(int i = 0; i < timeVec.size(); i++)
			{
				cout<<i<<" "<<timeVec.at(i).first<<" "<<timeVec.at(i).second<<endl;
				anslyseAllTransactionsByMonth(hTdb, timeVec.at(i).first, timeVec.at(i).second);
			}
		}
	}
	cout<<string(50, '*')<<endl<<"                  运行完成!          "<<endl<<string(50, '*')<<endl;
	TDB_Close(hTdb);
	return 0;
}