#include "stdafx.h"
#include "lib.h"

string changeStockCode(string stockCode)
{
	if(stockCode == "T00018.SH")
		return "1000018";
	else
		return stockCode.substr(0, 6);
}

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}

std::vector<std::string> split(const std::string &s, char delim) {
	std::vector<std::string> elems;
	split(s, delim, elems);
	return elems;
}

void usage() {
	cout<<string(50, '-')<<endl;
	cout<<"请选择运行模式:(输入1或者2或者3或者4或者5或者6或者7或者8)"<<endl;
	cout<<"1). 获取一段时间间隔所有股票的逐笔成交数据, 例如(从2015年6月到2016年1月)"<<endl;
	cout<<"2). 获取特定一个月所有股票的逐笔成交数据, 例如(2015年5月)"<<endl;
	cout<<"3). 获取特定一个月份的\"特定\"股票开始的逐笔成交数据,例如(2015年5月, 从603026.SH开始获取)"<<endl;
	cout<<"4). 获取连续几天所有股票的逐笔成交数据, 例如(从2016年4月12日到2016年4月24日)"<<endl;
	cout<<"5). 获取连续几天所有股票的K线数据, 例如(从2015/06/01到2016/01/31)"<<endl;
	cout<<"6). 获取一段时间间隔的所有股票的(last 15 and 30 minutes before close)的逐笔成交数据, 例如(从2015年6月到2016年1月)"<<endl;
	cout<<"7). 获取连续几天所有股票的(last 15 and 30 minutes before close)的逐笔成交数据, 例如(从2015/06/01到2015/06/10)"<<endl;
	cout<<"8). 获取连续几天所有股票的task1, task2, task3数据, 例如(从2015/06/01到2015/06/10)"<<endl;
}
InputParameter readInput() {
	usage();
	int type;
	cin>>type;
	if(type != 1 && type != 2 && type != 3 && type != 4 && type != 5 && type != 6 && type != 7 && type != 8)
		readInput();

	InputParameter input;

	int year, month;
	if (type == 1) {
		string startTime;
		string endTime;
		cout<<"请输入开始时间(例如: 2015/6): ";
		cin>>startTime;
		cout<<"请输入结束时间(例如: 2015/12): ";
		cin>>endTime;
		input.type = 1;
		input.startYear  = atoi(startTime.substr(0, 4).c_str());
		input.startMonth = atoi(startTime.substr(5, 7).c_str());
		input.endYear    = atoi(endTime.substr(0, 4).c_str());
		input.endMonth   = atoi(endTime.substr(5, 7).c_str());
	} else if (type == 2) {
		printf("请输入年份?(HINT:2016, 2015, 2014, 2013)\n");
		cin>>year;
		printf("请输入月份?(HINT: 1到12)\n");
		cin>>month;
		input.type = 2;
		input.startYear = year;
		input.startMonth = month;
	} else if (type == 3) {
		input.type = 3;
		printf("请输入股票代码, 例如(603028.SH): ");
		cin>>input.stockCode;
		printf("请输入年份?(HINT:2016, 2015, 2014, 2013)\n");
		cin>>year;
		printf("请输入月份?(HINT: 1到12)\n");
		cin>>month;
		input.startYear = year;
		input.startMonth = month;
	} else if (type == 4) 
	{
		input.type = 4;
		printf("请输入开始日期, 例如(2015/6/18): ");
		scanf("%d/%d/%d", &input.startYear, &input.startMonth, &input.startDay);
		cout<<"请输入结束日期, 例如(2015/6/31): ";
		scanf("%d/%d/%d", &input.endYear, &input.endMonth, &input.endDay);
	}
	else if (type == 5) {
		input.type = 5;
		string startTime;
		string endTime;
		cout<<"请输入开始时间(例如: 2015/06/01): ";
		scanf("%d/%d/%d", &input.startYear, &input.startMonth, &input.startDay);
		cout<<"请输入结束时间(例如: 2015/12/31): ";
		scanf("%d/%d/%d", &input.endYear, &input.endMonth, &input.endDay);
		cout<<"请输数据间隔周期(HINT:取值范围0到60): ";
		cin>>input.cycleNumber;
	} 
	else if (type == 6)
	{
		input.type = 6;
		string startTime;
		string endTime;
		cout<<"请输入开始时间(例如: 2015/6): ";
		cin>>startTime;
		cout<<"请输入结束时间(例如: 2015/12): ";
		cin>>endTime;
		input.startYear  = atoi(startTime.substr(0, 4).c_str());
		input.startMonth = atoi(startTime.substr(5, 7).c_str());
		input.endYear    = atoi(endTime.substr(0, 4).c_str());
		input.endMonth   = atoi(endTime.substr(5, 7).c_str());
	}
	else if (type == 7)
	{
		input.type = 7;
		string startTime;
		string endTime;
		cout<<"请输入开始时间(例如: 2015/06/01): ";
		scanf("%d/%d/%d", &input.startYear, &input.startMonth, &input.startDay);
		cout<<"请输入结束时间(例如: 2015/12/31): ";
		scanf("%d/%d/%d", &input.endYear, &input.endMonth, &input.endDay);
	}
	else if (type == 8)
	{
		input.type = 8;
		string startTime;
		string endTime;
		cout<<"请输入开始时间(例如: 2015/06/01): ";
		scanf("%d/%d/%d", &input.startYear, &input.startMonth, &input.startDay);
		cout<<"请输入结束时间(例如: 2015/06/11): ";
		scanf("%d/%d/%d", &input.endYear, &input.endMonth, &input.endDay);
		cout<<"请输数据间隔周期(HINT:取值范围0到60): ";
		cin>>input.cycleNumber;
	}
	return input;
}
int getCurrentDay()
{
	time_t t = time(0); // get time now
	struct tm* now = localtime( &t );
	return (now->tm_year + 1900)*100*100 + (now->tm_mon+1) * 100 + now->tm_mday;
}
//获取所有证券的代码
void GetAllStockTikers(vector<StockTicker>& vec, const string& fileName)
{
	fstream fcin(fileName, fstream::in);
	if (!fcin.is_open())
	{
		return;
	}
	string line;
	getline(fcin, line);
	while (getline(fcin, line))
	{
		vector<string> tokens = split(line, ';');
		StockTicker stockTiker;
		stockTiker.stockCode = tokens[0];
		stockTiker.stockName = tokens[1];
		stockTiker.boardDay  = atoi((tokens[2].substr(0, 4) + tokens[2].substr(5, 7) + tokens[2].substr(9, 11)).c_str());
		if(stockTiker.stockCode.find("SH") != string::npos)
			stockTiker.stockType = "SH-2-0";
		else
			stockTiker.stockType = "SZ-2-0";
		vec.push_back(stockTiker);
	}
	fcin.close();
	fstream fcout("ticker_list.txt", fstream::out);
	for_each(vec.begin(), vec.end(), [&fcout](const StockTicker& stockTicker){
		fcout<<stockTicker.stockCode<<endl;
	});
	fcout.close();
}

//请求代码表
vector<TDBDefine_Code> GetCodeTable(THANDLE hTdb, char* szMarket)
{
	TDBDefine_Code* pCodetable = NULL;
	int pCount;
	bool outPutTable = false;
	int ret = TDB_GetCodeTable(hTdb, szMarket, &pCodetable, &pCount);

	vector<TDBDefine_Code> res;

	if (ret == TDB_NO_DATA)
	{
		printf("无代码表！");
		return res;
	}
	printf("---------------------------Code Table--------------------\n");
	printf("收到代码表项数：%d\n",pCount);

	for (int i=0;i<pCount;i++)
	{
		if(pCodetable[i].nType != 0x10)
			continue;
		/*printf("万得代码 chWindCode:%s \n", pCodetable[i].chWindCode);	
		printf("交易所代码 chWindCode:%s \n", pCodetable[i].chCode);			
		printf("市场代码 chWindCode:%s \n", pCodetable[i].chMarket);
		printf("证券中文名称 chWindCode:%s \n", pCodetable[i].chCNName);
		printf("证券英文名称 chWindCode:%s \n", pCodetable[i].chENName);
		printf("证券类型 chWindCode:%d \n", pCodetable[i].nType);
		printf("----------------------------------------\n");*/
		res.push_back(pCodetable[i]);
	}
	cout<<res.size()<<endl;
	return res;
}
void StoreCodeTable(const vector<TDBDefine_Code> stockCodes, const string& fileName)
{
	fstream fcout(fileName, fstream::out);
	for (auto iter = stockCodes.begin(); iter!=stockCodes.end(); ++iter)
	{
		fcout<<iter->chWindCode<<";"<<iter->chCode<<";"<<iter->chMarket<<";"<<iter->chCNName<<";"<<iter->chENName<<";"<<iter->nType<<endl;
	}
	fcout.close();
}
//登录
THANDLE logIn(const string& ipAddress, int port, const string& userName, const string& passWord)
{
	//设置服务器信息
	OPEN_SETTINGS settings = {0};
	strcpy(settings.szIP, ipAddress.c_str());
	sprintf(settings.szPort, "%d", port);
	strcpy(settings.szUser, userName.c_str());
	strcpy(settings.szPassword,  passWord.c_str());

	settings.nRetryCount = 100;
	settings.nRetryGap = 1;
	settings.nTimeOutVal = 300;

	TDBDefine_ResLogin LoginRes = {0};
	THANDLE hTdb = TDB_Open(&settings, &LoginRes);

	if (hTdb)
	{
		printf("登录成功!\n");
		return hTdb;
	}
	printf("登录失败!\n");
	return NULL;
}

void showTranscation(const TDBDefine_Transaction& trans)
{
	printf("---------------------------------------Transaction Data------------------------------------------\n");
	printf("成交时间(Date): %d \n", trans.nDate);
	printf("成交时间: %d \n", trans.nTime);
	printf("成交代码: %c \n", trans.chFunctionCode);
	printf("委托类别: %c \n", trans.chOrderKind);
	printf("BS标志: %c \n", trans.chBSFlag);
	printf("成交价格: %d \n", trans.nTradePrice);
	printf("成交数量: %d \n", trans.nTradeVolume);
	printf("叫卖序号: %d \n", trans.nAskOrder);
	printf("叫买序号: %d \n", trans.nBidOrder);
	printf("成交编号: %d \n", trans.nIndex);
	printf("------------------------------------------------------\n");
}

//带买卖盘的tick
void GetTickData(THANDLE hTdb, char* szCode, char* szMarket, int nDate)
{
	//请求信息
	TDBDefine_ReqTick req = {0};
	strncpy(req.chCode, szCode, sizeof(req.chCode)); //代码写成你想获取的股票代码
	strncpy(req.chMarketKey, szMarket, sizeof(req.chMarketKey));
	req.nDate = nDate;
	req.nBeginTime = 0;
	req.nEndTime = 0;

	TDBDefine_Tick *pTick = NULL;
	int pCount;
	int ret = TDB_GetTick(hTdb,&req,&pTick, &pCount);

	printf("---------------------------------------Tick Data------------------------------------------\n");
	printf("共收到 %d 条Tick数据， 打印 1/100 条：\n", pCount);

	for(int i=0; i<pCount;)
	{
		TDBDefine_Tick& pTickCopy = pTick[i];
		printf("万得代码 chWindCode:%s \n", pTickCopy.chWindCode);
		printf("日期 nDate:%d \n", pTickCopy.nDate);
		printf("时间 nTime:%d \n", pTickCopy.nTime);

		printf("成交价 nPrice:%d \n", pTickCopy.nPrice);
		printf("成交量 iVolume:%I64d \n", pTickCopy.iVolume);
		printf("成交额(元) iTurover:%I64d \n", pTickCopy.iTurover);
		printf("成交笔数 nMatchItems:%d \n", pTickCopy.nMatchItems);
		printf(" nInterest:%d \n", pTickCopy.nInterest);

		printf("成交标志: chTradeFlag:%c \n", pTickCopy.chTradeFlag);
		printf("BS标志: chBSFlag:%c \n", pTickCopy.chBSFlag);
		printf("当日成交量: iAccVolume:%I64d \n", pTickCopy.iAccVolume);
		printf("当日成交额: iAccTurover:%I64d \n", pTickCopy.iAccTurover);

		printf("最高 nHigh:%d \n", pTickCopy.nHigh);
		printf("最低 nLow:%d \n", pTickCopy.nLow);
		printf("开盘 nOpen:%d \n", pTickCopy.nOpen);
		printf("前收盘 nPreClose:%d \n", pTickCopy.nPreClose);

		//买卖盘字段
		std::string strOut= array2str(pTickCopy.nAskPrice,ELEMENT_COUNT(pTickCopy.nAskPrice));
		printf("叫卖价 nAskPrice:%s \n", strOut.c_str());
		strOut= array2str((const int*)pTickCopy.nAskVolume,ELEMENT_COUNT(pTickCopy.nAskVolume));
		printf("叫卖量 nAskVolume:%s \n", strOut.c_str());
		strOut= array2str(pTickCopy.nBidPrice,ELEMENT_COUNT(pTickCopy.nBidPrice));
		printf("叫买价 nBidPrice:%s \n", strOut.c_str());
		strOut= array2str((const int*)pTickCopy.nBidVolume,ELEMENT_COUNT(pTickCopy.nBidVolume));
		printf("叫买量 nBidVolume:%s \n", strOut.c_str());
		printf("加权平均叫卖价 nAskAvPrice:%d \n", pTickCopy.nAskAvPrice);
		printf("加权平均叫买价 nBidAvPrice:%d \n", pTickCopy.nBidAvPrice);
		printf("叫卖总量 iTotalAskVolume:%I64d \n", pTickCopy.iTotalAskVolume);
		printf("叫买总量 iTotalBidVolume:%I64d \n", pTickCopy.iTotalBidVolume);
#if 0
		//期货字段
		printf("结算价 nSettle:%d \n", pTickCopy.nSettle);
		printf("持仓量 nPosition:%d \n", pTickCopy.nPosition);
		printf("虚实度 nCurDelta:%d \n", pTickCopy.nCurDelta);
		printf("昨结算 nPreSettle:%d \n", pTickCopy.nPreSettle);
		printf("昨持仓 nPrePosition:%d \n", pTickCopy.nPrePosition);

		//指数
		printf("不加权指数 nIndex:%d \n", pTickCopy.nIndex);
		printf("品种总数 nStocks:%d \n", pTickCopy.nStocks);
		printf("上涨品种数 nUps:%d \n", pTickCopy.nUps);
		printf("下跌品种数 nDowns:%d \n", pTickCopy.nDowns);
		printf("持平品种数 nHoldLines:%d \n", pTickCopy.nHoldLines);
#endif
		printf("-------------------------------\n");
		i += 1000;
	}
	//释放
	TDB_Free(pTick);
}

string int2str(int n)
{
	char szBuf[32];
	_snprintf(szBuf, sizeof(szBuf)/sizeof(szBuf[0]), "%d", n);
	return std::string(szBuf);
}

string array2str(const int* arr, int len)
{
	string str;
	for (int i=0; i<len; i++)
	{
		if (i == len-1)
		{
			str += int2str(arr[i]) + " ";
		}
		else 
		{
			str += int2str(arr[i]) + ",";
		}
	}
	return str;
}







