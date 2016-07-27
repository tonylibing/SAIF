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
	cout<<"请选择运行模式:(输入1或者2或者3或者4或者5)"<<endl;
	cout<<"1). 获取一段时间间隔的逐笔成交数据, 例如(从2015年6月到2016年1月)"<<endl;
	cout<<"2). 获取特定一个月的逐笔成交数据, 例如(2015年5月)"<<endl;
	cout<<"3). 获取特定一个月份的\"特定\"股票开始的逐笔成交数据,例如(2015年5月, 从603026.SH开始获取)"<<endl;
	cout<<"4). 获取特定一天的逐笔成交数据, 例如(2016年4月12日)"<<endl;
	cout<<"5). 获取特定一段时间间隔所有股票的K线数据, 例如(从2015/06/01到2016/01/31)"<<endl;
	cout<<"6). 获取主动成交额(first 15 minutes and 30 minutes after open, last 15 minutes and 30 minutes before close)"<<endl;
}
InputParameter readInput() {
	usage();
	int type;
	cin>>type;
	if(type != 1 && type != 2 && type != 3 && type != 4 && type != 5 && type != 6)
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
	} else if (type == 4) {
		input.type = 4;
		printf("请输入指定的日期, 例如(2015/6/18): ");
		scanf("%d/%d/%d", &input.startYear, &input.startMonth, &input.startDay);
		//string str;
		//cin>>str;
		//input.startYear  = atoi(str.substr(0, 4).c_str());
		//input.startMonth = atoi(str.substr(5, 7).c_str());
		//input.startDay = atoi(str.substr(8, 10).c_str());
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
		//printf("请输入年份?(HINT:2016, 2015, 2014, 2013)\n");
		//cin>>year;
		//printf("请输入月份?(HINT: 1到12)\n");
		//cin>>month;
		//input.startYear = year;
		//input.startMonth = month;
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

//逐笔成交
vector<TDBDefine_Transaction> GetTransaction(THANDLE hTdb, char* szCode, char* szMarketKey, int nDate)
{
	TDBDefine_ReqTransaction req = {0}; //请求
	strncpy(req.chCode, szCode, sizeof(req.chCode)); //代码写成你想获取的股票代码
	strncpy(req.chMarketKey, szMarketKey, sizeof(req.chMarketKey));
	req.nDate = nDate;
	req.nBeginTime = 0;
	req.nEndTime = 0;

	TDBDefine_Transaction *pTransaction = NULL;
	int pCount;
	int ret = TDB_GetTransaction(hTdb,&req, &pTransaction, &pCount);

	/*if (pTransaction && pCount && pTransaction[pCount-1].nTradeVolume > 0)
	{
	printf("错误code:%s\n", req.chCode);
	strncpy(ErrCode[ErrNum++], req.chCode, sizeof(req.chCode));
	}*/

	//printf("inside: %s, nDate = %d, 收到 %d 条逐笔成交消息\n", __func__, nDate, pCount);

	vector<TDBDefine_Transaction> vec;
	for (int i=0; i < pCount; i++)
	{
		const TDBDefine_Transaction& trans = pTransaction[i];
		//showTranscation(trans);
		vec.push_back(trans);
	}
	//释放
	TDB_Free(pTransaction);
	return vec;
}

//获取自上市日起所有的逐笔成交
map<int, vector<TDBDefine_Transaction>> GetAllTransactions(THANDLE hTdb, char* szCode, char* szMarketKey, int year, int month)
{
	map<int, vector<TDBDefine_Transaction>> resMap;

	for(int i = 1; i <= 31; i++)
	{
		int date = year * 10000 + month * 100 + i;		
		if(date > getCurrentDay())
			break;
		//printf("GetAllTransactions for %s, date: %d\n", szCode, date);
		vector<TDBDefine_Transaction> vec = GetTransaction(hTdb, szCode, szMarketKey, date);
		resMap[date] = vec;
	}
	return resMap;
}

vector<pair<int, int>> timeRange(int startY, int startM, int endY, int endM)
{
	vector<pair<int, int>> res;
	while((startY < endY) || (startY == endY && startM <= endM))
	{
		res.push_back(make_pair(startY, startM));
		startM += 1;
		if(startM >= 13) 
		{
			startY += 1;
			startM = 1;
		}
	}
	return res;
}
vector<int> timeRange(int startY, int startM, int startD, int endY, int endM, int endD)
{
	vector<int> res;
	while((startY < endY) || (startY == endY && startM < endM) || (startY == endY && startM == endM && startD <= endD))
	{
		if(isValid(startY, startM, startD))
		{
			int curDay = startY * 100 * 100 + startM * 100 + startD;
			res.push_back(curDay);
		}
		startD += 1;
		if(startD >= 32)
		{
			startD = 1;
			startM += 1;
		}
		if(startM >= 13) 
		{
			startM = 1;
			startY += 1;
		}
	}
	return res;
}
vector<pair<int, int>> timeRange2(int startY, int startM, int startD, int endY, int endM, int endD)
{
    vector<pair<int, int>> res;
    pair<int, int> element;
    if (startY == endY && startM == endM) {
        element.first = startY * 100 * 100 + startM * 100 + startD;
        element.second = endY * 100 * 100 + endY * 100 + endD;
        res.push_back(element);
    } else if (startY == endY) {
        element.first = startY * 100 * 100 + startM * 100 + startD;
        element.second = startY * 100 * 100 + startM * 100 + 31;
        res.push_back(element);
        for (int m = startM + 1; m < endM; m++) {
            element.first = startY * 100 * 100 + m * 100 + 1;
            element.second = startY * 100 * 100 + m * 100 + 31;
            res.push_back(element);
        }
        element.first = endY * 100 * 100 + endM * 100 + 1;
        element.second = endY * 100 * 100 + endM * 100 + endD;
        res.push_back(element);
    } else {
        element.first = startY * 100 * 100 + startM * 100 + startD;
        element.second = startY * 100 * 100 + startM * 100 + 31;
        res.push_back(element);
        startM += 1;
        if (startM > 12) {
            startM = 1;
            startY += 1;
        }
        while (startY < endY || (startY == endY && startM < endM)) {
            element.first = startY * 100 * 100 + startM * 100 + 1;
            element.second = startY * 100 * 100 + startM * 100 + 31;
            res.push_back(element);
            startM += 1;
            if (startM > 12) {
                startM = 1;
                startY += 1;
            }
        }
        element.first = endY * 100 * 100 + endM * 100 + 1;
        element.second = endY * 100 * 100 + endM * 100 + endD;
        res.push_back(element);
    }
	for(auto i = 0; i < res.size(); i++)
	{
		string endDay = int2str(res.at(i).second);
		int year  = atoi(endDay.substr(0, 4).c_str());
		int month = atoi(endDay.substr(4, 2).c_str());
		int day = atoi(endDay.substr(6, 2).c_str());
		if(month ==1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12)
			res.at(i).second = year * 100 * 100 + month * 100 + 31;
		else
			res.at(i).second = year * 100 * 100 + month * 100 + 30;

		if(year == 2016 && month == 2)
			res.at(i).second = year * 100 * 100 + month * 100 + 29;
		else if(year != 2016 && month == 2)
			res.at(i).second = year * 100 * 100 + month * 100 + 28;
	}
    return res;
}

int isValid(int yy, int mm, int dd) {
	if (dd <= 0) return 0;
	switch (mm) {
	case 1: case 3: case 5: case 7:
	case 8: case 10: case 12:
		return dd <= 31;
	case 4: case 6: case 9: case 11:
		return dd <= 30;
	case 2:
		return dd <= 28 + (yy % 400 == 0 || (yy % 4 == 0 && yy % 100 != 0));
	default:
		return 0;  /* invalid month */
	}
}


vector<TDBDefine_KLine> GetKData(THANDLE hTdb, char* szCode, char* szMarket, int nBeginDate, int nEndDate, int nCycleType, int nCycleNumber, int nCQFlag, int nAutoComplete)
{
	//请求K线
	TDBDefine_ReqKLine* req = new TDBDefine_ReqKLine;
	strncpy(req->chCode, szCode, ELEMENT_COUNT(req->chCode));
	strncpy(req->chMarketKey, szMarket, ELEMENT_COUNT(req->chMarketKey));

	req->nCQFlag = (REFILLFLAG)nCQFlag;//除权标志，由用户定义
	req->nBeginDate = nBeginDate;//开始日期
	req->nEndDate = nEndDate;//结束日期
	req->nBeginTime = 0;//开始时间
	req->nEndTime = 0;//结束时间

	req->nCycType = (CYCTYPE)nCycleType;
	req->nCycDef = nCycleNumber;

	//返回结构体指针
	TDBDefine_KLine* kLine = NULL;
	//返回数
	int pCount;
	//API请求K线
	TDB_GetKLine(hTdb, req, &kLine, &pCount);
	//printf("GetKData: result number: %d, szCode: %s\n", pCount, szCode);
	delete req;
	req = NULL;

	vector<TDBDefine_KLine> res;
	TDBDefine_KLine kLineObj = {0};

	if(pCount > 0) {
		for(int i=0; i<pCount; i++) {
			res.push_back(kLine[i]);
		}
	} else {
		//int hour   = 9;
		//int minute = 30;
		//while (hour < 15 || (hour == 15 && minute == 0)) 
		//{
		//	if(hour < 11 || (hour == 11 && minute <= 30) || (hour == 15 && minute == 0) || (hour < 15 && hour >= 13))
		//	{
		//		for(int i = 0; i <= 8; i++)
		//			kLineObj.chWindCode[i] = szCode[i];
		//		kLineObj.chWindCode[9] = '\0';
		//		kLineObj.nDate = nBeginDate;
		//		kLineObj.nTime = (hour * 100 + minute) * 100000;
		//		res.push_back(kLineObj);
		//	}

		//	minute += nCycleNumber;
		//	if (minute >= 60) 
		//	{
		//		hour += 1;
		//		minute = 0;
		//	}

		//}
	}

	//释放
	TDB_Free(kLine);
	return res;
}

void writeFileHeaderForTask1(fstream& fcout)
{
	fcout<<"股票名字;日期;<1万买;<1万卖;1万~2万买;1万~2万卖;2万~3万买;2万~3万卖;3万~4万买;3万~4万卖;4万~5万买;4万~5万卖;5万~6万买;5万~6万卖;6万~7万买;6万~7万卖;7万~8万买;7万~8万卖;8万~9万买;8万~9万卖;9万~10万买;9万~10万卖;";
	fcout<<"10万~20万买;10万~20万卖;20万~30万买;20万~30万卖;30万~40万买;30万~40万卖;40万~50万买;40万~50万卖;50万~60万买;50万~60万卖;60万~70万买;60万~70万卖;70万~80万买;70万~80万卖;80万~90万买;80万~90万卖;90万~100万买;90万~100万卖;>100万买;>100万卖"<<endl;
}
