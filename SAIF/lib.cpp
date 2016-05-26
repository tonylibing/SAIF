#include "stdafx.h"
#include "lib.h"

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
		vec.push_back(stockTiker);
	}
	fcin.close();
	fstream fcout("ticker_list.txt", fstream::out);
	for_each(vec.begin(), vec.end(), [&fcout](const StockTicker& stockTicker){
		fcout<<stockTicker.stockCode.substr(0, 6)<<endl;
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

	settings.nRetryCount = 30;
	settings.nRetryGap = 30;
	settings.nTimeOutVal = 30;

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

bool isValidDay(int date)
{
	int day   = date%100;
	int month = (date/100)%100;

	return (day>=1 && day <= 31) && (month >= 1 && month <= 12);
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
//获取K线
void GetKData(THANDLE hTdb, char* szCode, char* szMarket, int nBeginDate, int nEndDate, int nCycle, int nUserDef, int nCQFlag, int nAutoComplete)
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

	req->nCycType = (CYCTYPE)nCycle;
	req->nCycDef = 0;

	//返回结构体指针
	TDBDefine_KLine* kLine = NULL;
	//返回数
	int pCount;
	//API请求K线
	TDB_GetKLine(hTdb,req,&kLine,&pCount);
	delete req;
	req = NULL;

	printf("---------------------------K Data--------------------\n");
	printf("数据条数：%d,打印 1/100 条\n\n",pCount);
	for(int i=0;i<pCount;)
	{
		printf("WindCode:%s\n Code:%s\n Date:%d\n Time:%d\n Open:%d\n High:%d\n Low:%d\n Close:%d\n Volume:%I64d\n Turover:%I64d\n MatchItem:%d\n Interest:%d\n",
			kLine[i].chWindCode,kLine[i].chCode,kLine[i].nDate,kLine[i].nTime,kLine[i].nOpen,kLine[i].nHigh,kLine[i].nLow,kLine[i].nClose,
			kLine[i].iVolume,kLine[i].iTurover,kLine[i].nMatchItems,kLine[i].nInterest);
		i +=100;
	}
	//释放
	TDB_Free(kLine);
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
map<int, vector<TDBDefine_Transaction>> GetAllTransactions(THANDLE hTdb, char* szCode, char* szMarketKey, int nDate)
{
	//char filename[50];
	//sprintf(filename, ".//transaction//%s.txt", szCode);
	//fstream fcout(filename, fstream::out);

	map<int, vector<TDBDefine_Transaction>> resMap;
	int flag = 0;
	for(int date = nDate; date > 19900101; date--)
	{
		if(!isValidDay(date))
			continue;
		vector<TDBDefine_Transaction> vec = GetTransaction(hTdb, szCode, szMarketKey, date);
		if(vec.size() > 0) 
		{
			resMap[date] = vec;
			flag = 0;
		} else {
			flag++;
		}
		//printf("inside: %s, date: %d, all transactions: %d\n", __func__, date, vec.size());
		if(flag >= 10)
			break;
	}
	//fcout.close();
	return resMap;
}