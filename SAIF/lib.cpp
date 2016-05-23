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

//»ñÈ¡ËùÓÐÖ¤È¯µÄ´úÂë
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

//ÇëÇó´úÂë±í
void GetCodeTable(THANDLE hTdb, char* szMarket)
{
	TDBDefine_Code* pCodetable = NULL;
	int pCount;
	bool outPutTable = false;
	int ret = TDB_GetCodeTable(hTdb, szMarket, &pCodetable, &pCount);

	vector<TDBDefine_Code> res;

	if (ret == TDB_NO_DATA)
	{
		printf("ÎÞ´úÂë±í£¡");
		return res;
	}
	printf("---------------------------Code Table--------------------\n");
	printf("ÊÕµ½´úÂë±íÏîÊý£º%d\n",pCount);
	
	for (int i=0;i<pCount;i++)
	{
		if(pCodetable[i].nType != 0x10)
			continue;
		/*printf("ÍòµÃ´úÂë chWindCode:%s \n", pCodetable[i].chWindCode);	
		printf("½»Ò×Ëù´úÂë chWindCode:%s \n", pCodetable[i].chCode);			
		printf("ÊÐ³¡´úÂë chWindCode:%s \n", pCodetable[i].chMarket);
		printf("Ö¤È¯ÖÐÎÄÃû³Æ chWindCode:%s \n", pCodetable[i].chCNName);
		printf("Ö¤È¯Ó¢ÎÄÃû³Æ chWindCode:%s \n", pCodetable[i].chENName);
		printf("Ö¤È¯ÀàÐÍ chWindCode:%d \n", pCodetable[i].nType);
		printf("----------------------------------------\n");*/
		Stock stock;
		stock.tdbCode = pCodetable[i];
		allStocks.push_back(stock);
	}
}
//µÇÂ¼
THANDLE logIn(const string& ipAddress, int port, const string& userName, const string& passWord)
{
	//ÉèÖÃ·þÎñÆ÷ÐÅÏ¢
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
		printf("µÇÂ¼³É¹¦!\n");
		return hTdb;
	}
	printf("µÇÂ¼Ê§°Ü!\n");
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
	printf("³É½»Ê±¼ä(Date): %d \n", trans.nDate);
	printf("³É½»Ê±¼ä: %d \n", trans.nTime);
	printf("³É½»´úÂë: %c \n", trans.chFunctionCode);
	printf("Î¯ÍÐÀà±ð: %c \n", trans.chOrderKind);
	printf("BS±êÖ¾: %c \n", trans.chBSFlag);
	printf("³É½»¼Û¸ñ: %d \n", trans.nTradePrice);
	printf("³É½»ÊýÁ¿: %d \n", trans.nTradeVolume);
	printf("½ÐÂôÐòºÅ: %d \n", trans.nAskOrder);
	printf("½ÐÂòÐòºÅ: %d \n", trans.nBidOrder);
	printf("³É½»±àºÅ: %d \n", trans.nIndex);
	printf("------------------------------------------------------\n");
}
//»ñÈ¡KÏß
void GetKData(THANDLE hTdb, char* szCode, char* szMarket, int nBeginDate, int nEndDate, int nCycle, int nUserDef, int nCQFlag, int nAutoComplete)
{
	//ÇëÇóKÏß
	TDBDefine_ReqKLine* req = new TDBDefine_ReqKLine;
	strncpy(req->chCode, szCode, ELEMENT_COUNT(req->chCode));
	strncpy(req->chMarketKey, szMarket, ELEMENT_COUNT(req->chMarketKey));

	req->nCQFlag = (REFILLFLAG)nCQFlag;//³ýÈ¨±êÖ¾£¬ÓÉÓÃ»§¶¨Òå
	req->nBeginDate = nBeginDate;//¿ªÊ¼ÈÕÆÚ
	req->nEndDate = nEndDate;//½áÊøÈÕÆÚ
	req->nBeginTime = 0;//¿ªÊ¼Ê±¼ä
	req->nEndTime = 0;//½áÊøÊ±¼ä

	req->nCycType = (CYCTYPE)nCycle;
	req->nCycDef = 0;

	//·µ»Ø½á¹¹ÌåÖ¸Õë
	TDBDefine_KLine* kLine = NULL;
	//·µ»ØÊý
	int pCount;
	//APIÇëÇóKÏß
	TDB_GetKLine(hTdb,req,&kLine,&pCount);
	delete req;
	req = NULL;

	printf("---------------------------K Data--------------------\n");
	printf("Êý¾ÝÌõÊý£º%d,´òÓ¡ 1/100 Ìõ\n\n",pCount);
	for(int i=0;i<pCount;)
	{
		printf("WindCode:%s\n Code:%s\n Date:%d\n Time:%d\n Open:%d\n High:%d\n Low:%d\n Close:%d\n Volume:%I64d\n Turover:%I64d\n MatchItem:%d\n Interest:%d\n",
			kLine[i].chWindCode,kLine[i].chCode,kLine[i].nDate,kLine[i].nTime,kLine[i].nOpen,kLine[i].nHigh,kLine[i].nLow,kLine[i].nClose,
			kLine[i].iVolume,kLine[i].iTurover,kLine[i].nMatchItems,kLine[i].nInterest);
		i +=100;
	}
	//ÊÍ·Å
	TDB_Free(kLine);
}

//´øÂòÂôÅÌµÄtick
void GetTickData(THANDLE hTdb, char* szCode, char* szMarket, int nDate)
{
	//ÇëÇóÐÅÏ¢
	TDBDefine_ReqTick req = {0};
	strncpy(req.chCode, szCode, sizeof(req.chCode)); //´úÂëÐ´³ÉÄãÏë»ñÈ¡µÄ¹ÉÆ±´úÂë
	strncpy(req.chMarketKey, szMarket, sizeof(req.chMarketKey));
	req.nDate = nDate;
	req.nBeginTime = 0;
	req.nEndTime = 0;

	TDBDefine_Tick *pTick = NULL;
	int pCount;
	int ret = TDB_GetTick(hTdb,&req,&pTick, &pCount);

	printf("---------------------------------------Tick Data------------------------------------------\n");
	printf("¹²ÊÕµ½ %d ÌõTickÊý¾Ý£¬ ´òÓ¡ 1/100 Ìõ£º\n", pCount);

	for(int i=0; i<pCount;)
	{
		TDBDefine_Tick& pTickCopy = pTick[i];
		printf("ÍòµÃ´úÂë chWindCode:%s \n", pTickCopy.chWindCode);
		printf("ÈÕÆÚ nDate:%d \n", pTickCopy.nDate);
		printf("Ê±¼ä nTime:%d \n", pTickCopy.nTime);

		printf("³É½»¼Û nPrice:%d \n", pTickCopy.nPrice);
		printf("³É½»Á¿ iVolume:%I64d \n", pTickCopy.iVolume);
		printf("³É½»¶î(Ôª) iTurover:%I64d \n", pTickCopy.iTurover);
		printf("³É½»±ÊÊý nMatchItems:%d \n", pTickCopy.nMatchItems);
		printf(" nInterest:%d \n", pTickCopy.nInterest);

		printf("³É½»±êÖ¾: chTradeFlag:%c \n", pTickCopy.chTradeFlag);
		printf("BS±êÖ¾: chBSFlag:%c \n", pTickCopy.chBSFlag);
		printf("µ±ÈÕ³É½»Á¿: iAccVolume:%I64d \n", pTickCopy.iAccVolume);
		printf("µ±ÈÕ³É½»¶î: iAccTurover:%I64d \n", pTickCopy.iAccTurover);

		printf("×î¸ß nHigh:%d \n", pTickCopy.nHigh);
		printf("×îµÍ nLow:%d \n", pTickCopy.nLow);
		printf("¿ªÅÌ nOpen:%d \n", pTickCopy.nOpen);
		printf("Ç°ÊÕÅÌ nPreClose:%d \n", pTickCopy.nPreClose);

		//ÂòÂôÅÌ×Ö¶Î
		std::string strOut= array2str(pTickCopy.nAskPrice,ELEMENT_COUNT(pTickCopy.nAskPrice));
		printf("½ÐÂô¼Û nAskPrice:%s \n", strOut.c_str());
		strOut= array2str((const int*)pTickCopy.nAskVolume,ELEMENT_COUNT(pTickCopy.nAskVolume));
		printf("½ÐÂôÁ¿ nAskVolume:%s \n", strOut.c_str());
		strOut= array2str(pTickCopy.nBidPrice,ELEMENT_COUNT(pTickCopy.nBidPrice));
		printf("½ÐÂò¼Û nBidPrice:%s \n", strOut.c_str());
		strOut= array2str((const int*)pTickCopy.nBidVolume,ELEMENT_COUNT(pTickCopy.nBidVolume));
		printf("½ÐÂòÁ¿ nBidVolume:%s \n", strOut.c_str());
		printf("¼ÓÈ¨Æ½¾ù½ÐÂô¼Û nAskAvPrice:%d \n", pTickCopy.nAskAvPrice);
		printf("¼ÓÈ¨Æ½¾ù½ÐÂò¼Û nBidAvPrice:%d \n", pTickCopy.nBidAvPrice);
		printf("½ÐÂô×ÜÁ¿ iTotalAskVolume:%I64d \n", pTickCopy.iTotalAskVolume);
		printf("½ÐÂò×ÜÁ¿ iTotalBidVolume:%I64d \n", pTickCopy.iTotalBidVolume);
#if 0
		//ÆÚ»õ×Ö¶Î
		printf("½áËã¼Û nSettle:%d \n", pTickCopy.nSettle);
		printf("³Ö²ÖÁ¿ nPosition:%d \n", pTickCopy.nPosition);
		printf("ÐéÊµ¶È nCurDelta:%d \n", pTickCopy.nCurDelta);
		printf("×ò½áËã nPreSettle:%d \n", pTickCopy.nPreSettle);
		printf("×ò³Ö²Ö nPrePosition:%d \n", pTickCopy.nPrePosition);

		//Ö¸Êý
		printf("²»¼ÓÈ¨Ö¸Êý nIndex:%d \n", pTickCopy.nIndex);
		printf("Æ·ÖÖ×ÜÊý nStocks:%d \n", pTickCopy.nStocks);
		printf("ÉÏÕÇÆ·ÖÖÊý nUps:%d \n", pTickCopy.nUps);
		printf("ÏÂµøÆ·ÖÖÊý nDowns:%d \n", pTickCopy.nDowns);
		printf("³ÖÆ½Æ·ÖÖÊý nHoldLines:%d \n", pTickCopy.nHoldLines);
#endif
		printf("-------------------------------\n");
		i += 1000;
	}
	//ÊÍ·Å
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

//Öð±Ê³É½»
vector<TDBDefine_Transaction> GetTransaction(THANDLE hTdb, char* szCode, char* szMarketKey, int nDate)
{
	TDBDefine_ReqTransaction req = {0}; //ÇëÇó
	strncpy(req.chCode, szCode, sizeof(req.chCode)); //´úÂëÐ´³ÉÄãÏë»ñÈ¡µÄ¹ÉÆ±´úÂë
	strncpy(req.chMarketKey, szMarketKey, sizeof(req.chMarketKey));
	req.nDate = nDate;
	req.nBeginTime = 0;
	req.nEndTime = 0;

	TDBDefine_Transaction *pTransaction = NULL;
	int pCount;
	int ret = TDB_GetTransaction(hTdb,&req, &pTransaction, &pCount);

	/*if (pTransaction && pCount && pTransaction[pCount-1].nTradeVolume > 0)
	{
		printf("´íÎócode:%s\n", req.chCode);
		strncpy(ErrCode[ErrNum++], req.chCode, sizeof(req.chCode));
	}*/

	//printf("inside: %s, nDate = %d, ÊÕµ½ %d ÌõÖð±Ê³É½»ÏûÏ¢\n", __func__, nDate, pCount);

	vector<TDBDefine_Transaction> vec;
	for (int i=0; i < pCount; i++)
	{
		const TDBDefine_Transaction& trans = pTransaction[i];
		//showTranscation(trans);
		vec.push_back(trans);
	}
	//ÊÍ·Å
	TDB_Free(pTransaction);
	return vec;
}

//»ñÈ¡×ÔÉÏÊÐÈÕÆðËùÓÐµÄÖð±Ê³É½»
map<int, vector<TDBDefine_Transaction>> GetAllTransactions(THANDLE hTdb, char* szCode, char* szMarketKey, int nDate)
{
	map<int, vector<TDBDefine_Transaction>> resMap;
	int flag = 0;
	for(int date = nDate; date > 19900101; date--)
	{
		if(!isValidDay(date))
			continue;
		vector<TDBDefine_Transaction> vec = GetTransaction(hTdb, szCode, szMarketKey, date);
		if(vec.size() > 0) {
			resMap[date] = vec;
			flag = 0;
		} else {
			flag++;
		}
		printf("inside: %s, date: %d, all transactions: %d\n", __func__, date, vec.size());
		if(flag >= 30)
			break;
	}
	return resMap;
}

OrderType checkOrderType(int value)
{
	if(value > BIGGER)
		return Bigger;
	else if (value >= BIG)
		return Big;
	else if (value >= MEDIUM)
		return Medium;
	else
		return Low;
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

// 任务1：
void task1()
{
	for(vector<Stock>::iterator iter = allStocks.begin(); iter != allStocks.end(); ++iter)
	{
		map<int, vector<TDBDefine_Transaction>>& transMap = iter->tdbTransactionMap;
		map<int, vector<TDBDefine_Transaction>>::iterator mapBegin = transMap.begin();
		map<int, vector<TDBDefine_Transaction>>::iterator mapEnd   = transMap.end();

		Order& bigger = (iter->order)[0];
		Order& big    = (iter->order)[1];
		Order& medium = (iter->order)[2];
		Order& low    = (iter->order)[3];

		for (; mapBegin != mapEnd; ++mapBegin)
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
		//计算买入和卖出値
		cout<<(iter->tdbCode).chWindCode<<";"<<transMap->first;
		for (int i = 0; i < 4; ++i)
		{
			Order& order = (iter->order)[i];
			calBuyAndSellValue(Order& order);
			cout<<";"<<order.buyValue<<";"<<order.sellValue;
		} //end for i = 0
		cout<<endl;
	}
}