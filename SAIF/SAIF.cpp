// SAIF.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "TDBAPI.h"
#include <string>
#include <vector>
#include "lib.h"
#include <map>
#include <set>
#include <tuple>

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
	string resultName = string(".\\task1\\") + int2str(year*100 + month) + string(".txt");
	fcout.open(resultName, fstream::out);
	writeFileHeaderForTask1(fcout);
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
			anslyseAllTransactionsByMonth(hTdb, inputParameter.startYear, inputParameter.startMonth);
		}
		else if (inputParameter.type == 3)
		{
			string resultName = string(".\\task1\\") + inputParameter.stockCode + int2str(inputParameter.startYear*100 + inputParameter.startMonth) + string(".txt");
			fcout.open(resultName, fstream::out);
			writeFileHeaderForTask1(fcout);

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
		else if (inputParameter.type == 1)
		{
			vector<pair<int, int>> timeVec = timeRange(inputParameter.startYear, inputParameter.startMonth, inputParameter.endYear, inputParameter.endMonth);
			for(int i = 0; i < timeVec.size(); i++)
			{
				cout<<i<<" "<<timeVec.at(i).first<<" "<<timeVec.at(i).second<<endl;
				anslyseAllTransactionsByMonth(hTdb, timeVec.at(i).first, timeVec.at(i).second);
			}
		}
		else if (inputParameter.type == 4)
		{
			int date = inputParameter.startYear * 10000 + inputParameter.startMonth * 100 + inputParameter.startDay;
			string resultName = string(".\\task1\\") + string("transactionData") + int2str(date) + string(".txt");
			fcout.open(resultName, fstream::out);
			writeFileHeaderForTask1(fcout);

			vector<TDBDefine_Transaction> vec;
			map<int, vector<TDBDefine_Transaction>> allTransMap;
			for (auto iter = allStockTikers.begin(); iter != allStockTikers.end(); iter++) 
			{
				vec = GetTransaction(hTdb, (char*)(iter->stockCode).c_str(), (char*)(iter->stockType).c_str(), date);
				allTransMap[date] = vec;
				anslyseAllTransactions(allTransMap, iter->stockCode);
				allTransMap.clear();
			}
			fcout.close();
		}
		else if (inputParameter.type == 5)
		{
			char filename[100] = {'\0'};
			sprintf(filename, ".\\task2\\kLineData%d%02d%02d-%d%02d%02d(%dminutes).txt", inputParameter.startYear, inputParameter.startMonth, inputParameter.startDay, inputParameter.endYear, inputParameter.endMonth, inputParameter.endDay, inputParameter.cycleNumber);
			fcout.open(string(filename), fstream::out);
			fcout<<"WindCode;Date;Time;Open;Close;High;Low;Volume;Vmap"<<endl;

			int endYear  = inputParameter.endYear;
			int endMonth = inputParameter.endMonth;
			int endDay   = inputParameter.endDay;

			int startYear  = inputParameter.startYear;
			int startMonth = inputParameter.startMonth;
			int startDay   = inputParameter.startDay;

			int cycleNumber = inputParameter.cycleNumber;

			vector<pair<int, int>> timeVec = timeRange2(startYear, startMonth, startDay, endYear, endMonth, endDay);
			
			for(auto dayIter = timeVec.begin(); dayIter != timeVec.end(); dayIter++)
			{
				for (auto iter = allStockTikers.begin(); iter != allStockTikers.end(); iter++)
				{
					vector<TDBDefine_KLine> kDataVec= GetKData(hTdb, (char*)(iter->stockCode).c_str(), (char*)(iter->stockType).c_str(), dayIter->first, dayIter->second, CYC_MINUTE, cycleNumber, 0, 1);
					printf("stock: %s, start: %d, end: %d, size: %d\n", (iter->stockCode).c_str(), dayIter->first, dayIter->second, kDataVec.size());
					for (auto iter1 = kDataVec.begin(); iter1 != kDataVec.end(); iter1++) 
					{
						const TDBDefine_KLine& kline = *iter1;
						double vMap = (kline.iVolume == 0) ? 0 : ((double)kline.iTurover/(double)kline.iVolume);
						fcout<<kline.chWindCode<<";"<<kline.nDate<<";"<<kline.nTime<<";"<<kline.nOpen<<";"<<kline.nClose<<";"<<kline.nHigh<<";"<<kline.nLow<<";"<<kline.iVolume<<";"<<vMap<<endl;
					}
				}

			}
			fcout.close();
		}
		else if (inputParameter.type == 6)
		{
			map<int, vector<TDBDefine_Transaction>> allTransMap;
			for (auto stockIter = allStockTikers.begin(); stockIter != allStockTikers.end(); stockIter++)
			{	
				if(stockIter->stockCode != "600602.SH")
					continue;
				vector<TDBDefine_Transaction> transVec = GetTransaction(hTdb, (char*)(stockIter->stockCode).c_str(), (char*)(stockIter->stockType).c_str(), 20160627);
				vector<TDBDefine_Transaction> first15;
				vector<TDBDefine_Transaction> first30;
				vector<TDBDefine_Transaction> last15;
				vector<TDBDefine_Transaction> last30;
				for(vector<TDBDefine_Transaction>::iterator iter = transVec.begin(); iter != transVec.end(); iter++)
				{
					//if(iter->chBSFlag != 'B')
					//	continue;
					const int FLAG = 100000;
					int hourAndMinute = iter->nTime / 100000;
					if( iter->nTime >= 1445 * FLAG)
					{
						last15.push_back(*iter);
					}
					if ( iter->nTime >= 1430 * FLAG)
					{
						last30.push_back(*iter);

					}
					if (iter->nTime >= 925 * FLAG && iter->nTime <= 945 * FLAG)
					{
						first15.push_back(*iter);
					}
					if (iter->nTime >= 925 * FLAG && iter->nTime <= 1000 * FLAG)
					{
						first30.push_back(*iter);
					}
				}

				double first30Res = 0;
				for_each(first30.begin(), first30.end(), [&first30Res](const TDBDefine_Transaction& element) {
						if(element.chBSFlag == 'B')
							first30Res += (element.nTradeVolume * ((double)element.nTradePrice / 10000));
						else if(element.chBSFlag == 'S')
							first30Res -= (element.nTradeVolume * ((double)element.nTradePrice / 10000));
					});

				double last30Res = 0;
				for_each(last30.begin(), last30.end(), [&last30Res](const TDBDefine_Transaction& element) {
						if(element.chBSFlag == 'B')
							last30Res += (element.nTradeVolume * ((double)element.nTradePrice / 10000));
						else if(element.chBSFlag == 'S')
							last30Res -= (element.nTradeVolume * ((double)element.nTradePrice / 10000));
					});
				cout<<(long long)first30Res<<" "<<(long long)last30Res<<endl;

			}
		}
	}
	cout<<string(50, '*')<<endl<<"                  运行完成!          "<<endl<<string(50, '*')<<endl;
	TDB_Close(hTdb);
	return 0;
}