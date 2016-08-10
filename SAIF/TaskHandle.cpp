#include "StdAfx.h"
#include "TaskHandle.h"


TaskHandle::TaskHandle(THANDLE& hTdb, 
	NetWorkUtility* pNetworkInstance, 
	InputParameter& inputParameter):hTdb(hTdb), pNetWorkInstance(pNetworkInstance), inputParameter(inputParameter)
{
	fstream fcin("ticker_list.txt", fstream::in);
	if (!fcin.is_open())
	{
		LOG_INFO<<"Cann't open ticker_list.txt";
		exit(-1);
	}
	string line;
	while (getline(fcin, line))
	{
		StockTicker stockTiker;
		stockTiker.stockCode = line;
		allStockTikers.push_back(stockTiker);
	}
	LOG_INFO<<"stock number is "<<allStockTikers.size();
}
TaskHandle::~TaskHandle(void)
{
}

void TaskHandle::writeFileHeaderForTask1(fstream& fcout)
{
	fcout<<"股票名字;日期;<1万买;<1万卖;1万~2万买;1万~2万卖;2万~3万买;2万~3万卖;3万~4万买;3万~4万卖;4万~5万买;4万~5万卖;5万~6万买;5万~6万卖;6万~7万买;6万~7万卖;7万~8万买;7万~8万卖;8万~9万买;8万~9万卖;9万~10万买;9万~10万卖;";
	fcout<<"10万~20万买;10万~20万卖;20万~30万买;20万~30万卖;30万~40万买;30万~40万卖;40万~50万买;40万~50万卖;50万~60万买;50万~60万卖;60万~70万买;60万~70万卖;70万~80万买;70万~80万卖;80万~90万买;80万~90万卖;90万~100万买;90万~100万卖;>100万买;>100万卖"<<endl;
}

void TaskHandle::option1()
{
	std::vector<std::pair<int, int>> monthVec = TimeLib::monthRange(inputParameter);
	for(int i = 0; i < monthVec.size(); i++)
	{
		LOG_INFO<<"year = "<<monthVec.at(i).first<<", month = "<<monthVec.at(i).second;
		this->doTask1ByMonth(hTdb, monthVec.at(i).first, monthVec.at(i).second);
	}
}

void TaskHandle::option2()
{
	this->doTask1ByMonth(hTdb, inputParameter.startYear, inputParameter.startMonth);
}

void TaskHandle::option3()
{
	string resultName = string(".\\task1\\") + inputParameter.stockCode + string("_") + int2str(inputParameter.startYear*100 + inputParameter.startMonth) + string(".txt");
	fstream fcout;
	fcout.open(resultName, fstream::out);
	this->writeFileHeaderForTask1(fcout);

	bool flag = false;
	for (auto iter = this->allStockTikers.begin(); iter != this->allStockTikers.end(); iter++)
	{
		if(iter->stockCode == inputParameter.stockCode) 
		{
			flag = true;
		}
		if(flag) 
		{
			map<int, vector<TDBDefine_Transaction>> allTransMap;

			allTransMap = pNetWorkInstance->GetAllTransactions(hTdb, (char*)(iter->stockCode).c_str(), (char*)(iter->stockType).c_str(), inputParameter.startYear, inputParameter.startMonth);
			anslyseAllTransactions(allTransMap, changeStockCode(iter->stockCode), fcout);
			allTransMap.clear();
		}
	}
	fcout.close();
}

void TaskHandle::option4()
{
	vector<int> dayVec = TimeLib::dayRange(inputParameter);

	for(auto dayIter = dayVec.begin(); dayIter != dayVec.end(); dayIter++)
	{
		doTask1ByDay(hTdb, *dayIter);
	}
}

void TaskHandle::option5()
{
	char filename[100] = {'\0'};
	sprintf(filename, ".\\task2\\kLineData%d%02d%02d-%d%02d%02d(%dminutes).txt", inputParameter.startYear, inputParameter.startMonth, inputParameter.startDay, inputParameter.endYear, inputParameter.endMonth, inputParameter.endDay, inputParameter.cycleNumber);
	fstream fcout;
	fcout.open(string(filename), fstream::out);
	fcout<<"WindCode;Date;Time;Open;Close;High;Low;Volume;Vmap"<<endl;

	int cycleNumber = inputParameter.cycleNumber;
	vector<pair<int, int>> dayVec = TimeLib::dayRange2(inputParameter);
	for(auto dayIter = dayVec.begin(); dayIter != dayVec.end(); dayIter++)
	{
		for (auto iter = allStockTikers.begin(); iter != allStockTikers.end(); iter++)
		{
			vector<TDBDefine_KLine> kDataVec= pNetWorkInstance->GetKData(hTdb, (char*)(iter->stockCode).c_str(), (char*)(iter->stockType).c_str(), dayIter->first, dayIter->second, CYC_MINUTE, cycleNumber, 0, 1);
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

void TaskHandle::option6()
{
	vector<pair<int, int>> monthVec = TimeLib::monthRange(inputParameter);
	for(int i = 0; i < monthVec.size(); i++)
	{
		LOG_INFO<<i<<" "<<monthVec.at(i).first<<" "<<monthVec.at(i).second;
		this->doTask3ByMonth(hTdb, monthVec.at(i).first, monthVec.at(i).second);
	}
}
void TaskHandle::option7()
{
	vector<int> dayVec = TimeLib::dayRange(inputParameter);

	for(auto dayIter = dayVec.begin(); dayIter != dayVec.end(); dayIter++)
	{
		doTask3ByDay(hTdb, *dayIter);
	}
}
void TaskHandle::option8()
{
	vector<int> dayVec = TimeLib::dayRange(inputParameter);

	for(auto dayIter = dayVec.begin(); dayIter != dayVec.end(); dayIter++)
	{
		doTask1ByDay(hTdb, *dayIter);
		doTask2ByDay(hTdb, *dayIter, inputParameter.cycleNumber);
		doTask3ByDay(hTdb, *dayIter);
	}
}
void TaskHandle::doTask3ByDay(THANDLE& hTdb, int date)
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
		vec = pNetWorkInstance->GetTransaction(hTdb, (char*)(iter->stockCode).c_str(), (char*)(iter->stockType).c_str(), date);
		vector<TDBDefine_Transaction> last15;  //日
		vector<TDBDefine_Transaction> last30;  //日

		this->getLast15And30(vec, last15, last30);
		last15transMap[date] = last15;
		last30transMap[date] = last30;

		anslyseAllTransactions(last15transMap, changeStockCode(iter->stockCode), fcoutLast15);
		anslyseAllTransactions(last30transMap, changeStockCode(iter->stockCode), fcoutLast30);

		vec.clear();
		last15transMap.clear();
		last30transMap.clear();
	}
	fcoutLast15.close(); fcoutLast30.close();
	LOG_INFO<<"Finish doTask3ByDay: day = "<<date;
}
void TaskHandle::doTask2ByDay(THANDLE& hTdb, int date, int cycleNumber)
{
	char filename[100] = {'\0'};
	sprintf(filename, ".\\task2\\kLineData%d(%dminutes).txt", date, cycleNumber);
	fstream fcout;
	fcout.open(string(filename), fstream::out);
	fcout<<"WindCode;Date;Time;Open;Close;High;Low;Volume;Vmap"<<endl;

	for (auto iter = allStockTikers.begin(); iter != allStockTikers.end(); iter++)
	{
		vector<TDBDefine_KLine> kDataVec= pNetWorkInstance->GetKData(hTdb, (char*)(iter->stockCode).c_str(), (char*)(iter->stockType).c_str(), date, date, CYC_MINUTE, cycleNumber, 0, 1);
		printf("stock: %s, start: %d, end: %d, size: %d\n", (iter->stockCode).c_str(), date, date, kDataVec.size());
		for (auto iter1 = kDataVec.begin(); iter1 != kDataVec.end(); iter1++) 
		{
			const TDBDefine_KLine& kline = *iter1;
			double vMap = (kline.iVolume == 0) ? 0 : ((double)kline.iTurover/(double)kline.iVolume);
			fcout<<changeStockCode(kline.chWindCode)<<";"<<kline.nDate<<";"<<kline.nTime<<";"<<kline.nOpen<<";"<<kline.nClose<<";"<<kline.nHigh<<";"<<kline.nLow<<";"<<kline.iVolume<<";"<<vMap<<endl;
		}
	}
	fcout.close();
	LOG_INFO<<"Finish doTask2ByDay: day = "<<date;
}

void TaskHandle::doTask1ByDay(THANDLE& hTdb, int date)
{
	string resultName = string(".\\task1\\") + int2str(date) + string(".txt");
	fstream fcout;
	fcout.open(resultName, fstream::out);
	this->writeFileHeaderForTask1(fcout);

	vector<TDBDefine_Transaction> vec;
	map<int, vector<TDBDefine_Transaction>> allTransMap;
	for (auto iter = this->allStockTikers.begin(); iter != this->allStockTikers.end(); iter++) 
	{
		vec = pNetWorkInstance->GetTransaction(hTdb, (char*)(iter->stockCode).c_str(), (char*)(iter->stockType).c_str(), date);

		allTransMap[date] = vec;
		anslyseAllTransactions(allTransMap, changeStockCode(iter->stockCode), fcout);
		allTransMap.clear();
	}
	fcout.close();
	LOG_INFO<<"Finish doTask1ByDay: day = "<<date;
}
void TaskHandle::doTask1ByMonth(THANDLE& hTdb, int year, int month)
{
	fstream fcout;
	string resultName = string(".\\task1\\") + int2str(year*100 + month) + string(".txt");
	fcout.open(resultName, fstream::out);
	this->writeFileHeaderForTask1(fcout);

	map<int, vector<TDBDefine_Transaction>> allTransMap;

	for (auto iter = allStockTikers.begin(); iter != allStockTikers.end(); iter++) 
	{ 
		allTransMap = pNetWorkInstance->GetAllTransactions(hTdb, (char*)(iter->stockCode).c_str(), (char*)(iter->stockType).c_str(), year, month);
		this->anslyseAllTransactions(allTransMap, changeStockCode(iter->stockCode), fcout);
		allTransMap.clear();
	}
	fcout.close();
}

void TaskHandle::anslyseAllTransactions(std::map<int, std::vector<TDBDefine_Transaction>>&allTransMap, const string& stockCode, fstream& fcout)
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
		fcout<<stockCode<<";"<<key;
		for(int i = 0; i < 20; i++)
		{
			this->calBuyAndSellValue(orders.at(i));
			fcout<<";"<<orders.at(i).buyValue<<";"<<orders.at(i).sellValue;
		}
		fcout<<endl;
	}
}


OrderType TaskHandle::checkOrderType(int value)
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

void TaskHandle::calBuyAndSellValue(Order& order)
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

void TaskHandle::doTask3ByMonth(THANDLE& hTdb, int year, int month)
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
		allTransMap = pNetWorkInstance->GetAllTransactions(hTdb, (char*)(iter->stockCode).c_str(), (char*)(iter->stockType).c_str(), year, month);

		//获取last15和last30的数据
		for(auto mapIter = allTransMap.begin(); mapIter != allTransMap.end(); mapIter++)
		{
			vector<TDBDefine_Transaction> last15;  //日
			vector<TDBDefine_Transaction> last30;  //日

			this->getLast15And30(mapIter->second, last15, last30);
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
void TaskHandle::getLast15And30(const vector<TDBDefine_Transaction>& transVec, vector<TDBDefine_Transaction>& last15, vector<TDBDefine_Transaction>& last30)
{
	const int FLAG = 100000;
	for(vector<TDBDefine_Transaction>::const_iterator iter = transVec.begin(); iter != transVec.end(); iter++)
	{
		if(iter->nTime >= 1445 * FLAG)
		{
			last15.push_back(*iter);
		}
		if(iter->nTime >= 1430 * FLAG)
		{
			last30.push_back(*iter);
		}
	}
}