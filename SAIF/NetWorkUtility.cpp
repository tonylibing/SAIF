#include "StdAfx.h"
#include "NetWorkUtility.h"

NetWorkUtility::~NetWorkUtility(void)
{
}

//登录
THANDLE NetWorkUtility::logIn(const std::string& ipAddress, int port, const std::string& userName, const std::string& passWord)
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
		LOG_INFO << "登录成功!";
		return hTdb;
	}
	LOG_INFO << "登录失败!";
	return NULL;
}
//获取一个股票特定一天的所有每笔成交数据
std::vector<TDBDefine_Transaction> NetWorkUtility::GetTransaction(THANDLE hTdb, char* szCode, char* szMarketKey, int nDate)
{
	TDBDefine_ReqTransaction req = {0}; //请求
	strncpy(req.chCode, szCode, sizeof(req.chCode)); //获取的股票代码
	strncpy(req.chMarketKey, szMarketKey, sizeof(req.chMarketKey));
	req.nDate = nDate;
	req.nBeginTime = 0;
	req.nEndTime = 0;

	TDBDefine_Transaction *pTransaction = NULL;
	int pCount;
	int ret = TDB_GetTransaction(hTdb,&req, &pTransaction, &pCount);

	std::vector<TDBDefine_Transaction> vec;
	for (int i=0; i < pCount; i++)
	{
		vec.push_back(pTransaction[i]);
	}
	//释放
	TDB_Free(pTransaction);
	return vec;
}

//获取一个股票特定一月的所有每笔成交数据
std::map<int, std::vector<TDBDefine_Transaction>> NetWorkUtility::GetAllTransactions(THANDLE hTdb, char* szCode, char* szMarketKey, int year, int month)
{
	std::map<int, std::vector<TDBDefine_Transaction>> resMap;

	for(int i = 1; i <= 31; i++)
	{
		int date = year * 10000 + month * 100 + i;		
		if(date > TimeLib::getCurrentDay())
			break;
		std::vector<TDBDefine_Transaction> vec = this->GetTransaction(hTdb, szCode, szMarketKey, date);
		resMap[date] = vec;
	}
	return resMap;
}

vector<TDBDefine_KLine> NetWorkUtility::GetKData(THANDLE hTdb, char* szCode, char* szMarket, int nBeginDate, int nEndDate, int nCycleType, int nCycleNumber, int nCQFlag, int nAutoComplete)
{
	//请求K线
	TDBDefine_ReqKLine* req = new TDBDefine_ReqKLine;
	strncpy(req->chCode, szCode, ELEMENT_COUNT(req->chCode));
	strncpy(req->chMarketKey, szMarket, ELEMENT_COUNT(req->chMarketKey));

	req->nCQFlag = (REFILLFLAG)nCQFlag;//除权标志，由用户定义
	req->nBeginDate = nBeginDate;//开始日期
	req->nEndDate = nEndDate;//结束日期
	req->nBeginTime = 0;//开始时间

	SYSTEMTIME st;
	GetLocalTime(&st);
	if (nBeginDate == nEndDate && (10000 * st.wYear + 100 * st.wMonth + st.wDay == nBeginDate))
		req->nEndTime = atoi(TimeLib::formatTime(st).c_str());//结束时间
	else
		req->nEndTime = 0;

	req->nCycType = (CYCTYPE)nCycleType;
	req->nCycDef = nCycleNumber;

	//返回结构体指针
	TDBDefine_KLine* kLine = NULL;
	//返回数
	int pCount;
	//API请求K线
	TDB_GetKLine(hTdb, req, &kLine, &pCount);

	delete req;
	req = NULL;

	vector<TDBDefine_KLine> res;
	TDBDefine_KLine kLineObj = {0};

	if(pCount > 0) {
		for(int i=0; i<pCount; i++) {
			res.push_back(kLine[i]);
		}
	} 

	//释放
	TDB_Free(kLine);
	return res;
}