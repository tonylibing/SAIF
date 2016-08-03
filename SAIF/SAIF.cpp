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
void anslyseAllTransactions(map<int, vector<TDBDefine_Transaction>>& allTransMap, const string& windName, fstream& fcout)
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
		anslyseAllTransactions(allTransMap, changeStockCode(iter->stockCode));
		allTransMap.clear();
	}
	fcout.close();
}

void anslyseAllTransactionsByMonthTask3(THANDLE& hTdb, int year, int month)
{
	fstream fcoutLast15, fcoutLast30;
	char last15ResultName[100] = {'\0'};
	sprintf(last15ResultName, ".\\task3\\last15\\%d%02d.txt", year, month);
	fcoutLast15.open(last15ResultName, fstream::out);
	writeFileHeaderForTask1(fcoutLast15);

	char last30ResultName[100] = {'\0'};
	sprintf(last30ResultName, ".\\task3\\last30\\%d%02d.txt", year, month);
	fcoutLast30.open(last30ResultName, fstream::out);
	writeFileHeaderForTask1(fcoutLast30);

	map<int, vector<TDBDefine_Transaction>> allTransMap;

	map<int, vector<TDBDefine_Transaction>> last15transMap;
	map<int, vector<TDBDefine_Transaction>> last30transMap;

	for (auto iter = allStockTikers.begin(); iter != allStockTikers.end(); iter++) 
	{ 
		allTransMap = GetAllTransactions(hTdb, (char*)(iter->stockCode).c_str(), (char*)(iter->stockType).c_str(), year, month);

		//获取last15和last30的数据
		for(auto mapIter = allTransMap.begin(); mapIter != allTransMap.end(); mapIter++)
		{
			vector<TDBDefine_Transaction> last15;  //日
			vector<TDBDefine_Transaction> last30;  //日

			getLast15And30(mapIter->second, last15, last30);
			last15transMap[mapIter->first] = last15;
			last30transMap[mapIter->first] = last30;

			cout<<mapIter->first<<" "<<last15.size()<<" "<<last30.size()<<endl;
		}
		anslyseAllTransactions(last15transMap, changeStockCode(iter->stockCode), fcoutLast15);
		anslyseAllTransactions(last30transMap, changeStockCode(iter->stockCode), fcoutLast30);

		allTransMap.clear();
		last15transMap.clear();
		last30transMap.clear();
	}
	fcoutLast15.close();
	fcoutLast30.close();
}




void doTask1ByDay(THANDLE& hTdb, const vector<StockTicker>& allStockTikers, int date)
{
	string resultName = string(".\\task1\\") + string("transactionData") + int2str(date) + string(".txt");
	//fstream fcout;
	fcout.open(resultName, fstream::out);
	writeFileHeaderForTask1(fcout);

	vector<TDBDefine_Transaction> vec;
	map<int, vector<TDBDefine_Transaction>> allTransMap;
	for (auto iter = allStockTikers.begin(); iter != allStockTikers.end(); iter++) 
	{
		vec = GetTransaction(hTdb, (char*)(iter->stockCode).c_str(), (char*)(iter->stockType).c_str(), date);
		TDBDefine_Transaction tmp = vec[6604];
		allTransMap[date] = vec;
		anslyseAllTransactions(allTransMap, changeStockCode(iter->stockCode));
		allTransMap.clear();
	}
	fcout.close();
	printf("Finish task1: day=%d!\n", date);
}

void doTask2ByDay(THANDLE& hTdb, const vector<StockTicker>& allStockTikers, int date, int cycleNumber)
{
	char filename[100] = {'\0'};
	sprintf(filename, ".\\task2\\kLineData%d(%dminutes).txt", date, cycleNumber);
	fcout.open(string(filename), fstream::out);
	fcout<<"WindCode;Date;Time;Open;Close;High;Low;Volume;Vmap"<<endl;

	for (auto iter = allStockTikers.begin(); iter != allStockTikers.end(); iter++)
	{
		vector<TDBDefine_KLine> kDataVec= GetKData(hTdb, (char*)(iter->stockCode).c_str(), (char*)(iter->stockType).c_str(), date, date, CYC_MINUTE, cycleNumber, 0, 1);
		printf("stock: %s, start: %d, end: %d, size: %d\n", (iter->stockCode).c_str(), date, date, kDataVec.size());
		for (auto iter1 = kDataVec.begin(); iter1 != kDataVec.end(); iter1++) 
		{
			const TDBDefine_KLine& kline = *iter1;
			double vMap = (kline.iVolume == 0) ? 0 : ((double)kline.iTurover/(double)kline.iVolume);
			fcout<<changeStockCode(kline.chWindCode)<<";"<<kline.nDate<<";"<<kline.nTime<<";"<<kline.nOpen<<";"<<kline.nClose<<";"<<kline.nHigh<<";"<<kline.nLow<<";"<<kline.iVolume<<";"<<vMap<<endl;
		}
	}

	fcout.close();
		printf("Finish task2: day=%d!\n", date);
}


void doTask3ByDay(THANDLE& hTdb, const vector<StockTicker>& allStockTikers, int date)
{
	fstream fcoutLast15, fcoutLast30;
	char last15ResultName[100] = {'\0'};
	sprintf(last15ResultName, ".\\task3\\last15\\last15-%d.txt", date);
	fcoutLast15.open(last15ResultName, fstream::out);
	writeFileHeaderForTask1(fcoutLast15);

	char last30ResultName[100] = {'\0'};
	sprintf(last30ResultName, ".\\task3\\last30\\last30-%d.txt", date);
	fcoutLast30.open(last30ResultName, fstream::out);
	writeFileHeaderForTask1(fcoutLast30);

	vector<TDBDefine_Transaction> vec;
	map<int, vector<TDBDefine_Transaction>> last15transMap;
	map<int, vector<TDBDefine_Transaction>> last30transMap;
	for (auto iter = allStockTikers.begin(); iter != allStockTikers.end(); iter++) 
	{
		vec = GetTransaction(hTdb, (char*)(iter->stockCode).c_str(), (char*)(iter->stockType).c_str(), date);
		vector<TDBDefine_Transaction> last15;  //日
		vector<TDBDefine_Transaction> last30;  //日

		getLast15And30(vec, last15, last30);
		last15transMap[date] = last15;
		last30transMap[date] = last30;

		anslyseAllTransactions(last15transMap, changeStockCode(iter->stockCode), fcoutLast15);
		anslyseAllTransactions(last30transMap, changeStockCode(iter->stockCode), fcoutLast30);

		vec.clear();
		last15transMap.clear();
		last30transMap.clear();
	}
	fcoutLast15.close(); fcoutLast30.close();
	printf("Finish task3: day=%d!\n", date);
}



int _tmain(int argc, _TCHAR* argv[])
{
	string ipAddress = "114.80.154.34";
	int    port      = 6261;
	string userName  = "TD1056611005";
	string passWord  = "37264673";
	THANDLE hTdb = logIn(ipAddress, port, userName, passWord);

	//GetAllStockTikers(allStockTikers, "ticker list.csv");
	fstream fcin("ticker_list.txt", fstream::in);
	if (!fcin.is_open())
	{
		cout<<"Cann't open ticker_list.txt"<<endl;
		return -1;
	}
	string line;
	while (getline(fcin, line))
	{
		StockTicker stockTiker;
		stockTiker.stockCode = line;
		allStockTikers.push_back(stockTiker);
	}
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
					anslyseAllTransactions(allTransMap, changeStockCode(iter->stockCode));
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
			int endYear  = inputParameter.endYear;
			int endMonth = inputParameter.endMonth;
			int endDay   = inputParameter.endDay;

			int startYear  = inputParameter.startYear;
			int startMonth = inputParameter.startMonth;
			int startDay   = inputParameter.startDay;

			vector<int> timeVec = timeRange(startYear, startMonth, startDay, endYear, endMonth, endDay);

			for(auto dayIter = timeVec.begin(); dayIter != timeVec.end(); dayIter++)
			{
				doTask1ByDay(hTdb, allStockTikers, *dayIter);
			}
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
						fcout<<changeStockCode(kline.chWindCode)<<";"<<kline.nDate<<";"<<kline.nTime<<";"<<kline.nOpen<<";"<<kline.nClose<<";"<<kline.nHigh<<";"<<kline.nLow<<";"<<kline.iVolume<<";"<<vMap<<endl;
					}
				}

			}
			fcout.close();
		}
		else if (inputParameter.type == 6)
		{
			vector<pair<int, int>> timeVec = timeRange(inputParameter.startYear, inputParameter.startMonth, inputParameter.endYear, inputParameter.endMonth);
			for(int i = 0; i < timeVec.size(); i++)
			{
				cout<<i<<" "<<timeVec.at(i).first<<" "<<timeVec.at(i).second<<endl;
				anslyseAllTransactionsByMonthTask3(hTdb, timeVec.at(i).first, timeVec.at(i).second);
			}
		}
		else if (inputParameter.type == 7)
		{	
			int endYear  = inputParameter.endYear;
			int endMonth = inputParameter.endMonth;
			int endDay   = inputParameter.endDay;

			int startYear  = inputParameter.startYear;
			int startMonth = inputParameter.startMonth;
			int startDay   = inputParameter.startDay;

			vector<int> timeVec = timeRange(startYear, startMonth, startDay, endYear, endMonth, endDay);

			for(auto dayIter = timeVec.begin(); dayIter != timeVec.end(); dayIter++)
			{
				doTask3ByDay(hTdb, allStockTikers, *dayIter);
			}

		}
		else if (inputParameter.type == 8)
		{
			int endYear  = inputParameter.endYear;
			int endMonth = inputParameter.endMonth;
			int endDay   = inputParameter.endDay;

			int startYear  = inputParameter.startYear;
			int startMonth = inputParameter.startMonth;
			int startDay   = inputParameter.startDay;

			int cycleNumber = inputParameter.cycleNumber;

			vector<int> timeVec = timeRange(startYear, startMonth, startDay, endYear, endMonth, endDay);

			for(auto dayIter = timeVec.begin(); dayIter != timeVec.end(); dayIter++)
			{
				doTask1ByDay(hTdb, allStockTikers, *dayIter);
				doTask2ByDay(hTdb, allStockTikers, *dayIter, cycleNumber);
				doTask3ByDay(hTdb, allStockTikers, *dayIter);
			}
		}
	}
	cout<<string(50, '*')<<endl<<"                  运行完成!          "<<endl<<string(50, '*')<<endl;
	TDB_Close(hTdb);
	return 0;
}