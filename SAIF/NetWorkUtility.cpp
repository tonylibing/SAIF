#include "StdAfx.h"
#include "NetWorkUtility.h"

NetWorkUtility::~NetWorkUtility(void)
{
}

//��¼
THANDLE NetWorkUtility::logIn(const std::string& ipAddress, int port, const std::string& userName, const std::string& passWord)
{
	//���÷�������Ϣ
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
		LOG_INFO << "��¼�ɹ�!";
		return hTdb;
	}
	LOG_INFO << "��¼ʧ��!";
	return NULL;
}
//��ȡһ����Ʊ�ض�һ�������ÿ�ʳɽ�����
std::vector<TDBDefine_Transaction> NetWorkUtility::GetTransaction(THANDLE hTdb, char* szCode, char* szMarketKey, int nDate)
{
	TDBDefine_ReqTransaction req = {0}; //����
	strncpy(req.chCode, szCode, sizeof(req.chCode)); //��ȡ�Ĺ�Ʊ����
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
	//�ͷ�
	TDB_Free(pTransaction);
	return vec;
}

//��ȡһ����Ʊ�ض�һ�µ�����ÿ�ʳɽ�����
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
	//����K��
	TDBDefine_ReqKLine* req = new TDBDefine_ReqKLine;
	strncpy(req->chCode, szCode, ELEMENT_COUNT(req->chCode));
	strncpy(req->chMarketKey, szMarket, ELEMENT_COUNT(req->chMarketKey));

	req->nCQFlag = (REFILLFLAG)nCQFlag;//��Ȩ��־�����û�����
	req->nBeginDate = nBeginDate;//��ʼ����
	req->nEndDate = nEndDate;//��������
	req->nBeginTime = 0;//��ʼʱ��

	SYSTEMTIME st;
	GetLocalTime(&st);
	if (nBeginDate == nEndDate && (10000 * st.wYear + 100 * st.wMonth + st.wDay == nBeginDate))
		req->nEndTime = atoi(TimeLib::formatTime(st).c_str());//����ʱ��
	else
		req->nEndTime = 0;

	req->nCycType = (CYCTYPE)nCycleType;
	req->nCycDef = nCycleNumber;

	//���ؽṹ��ָ��
	TDBDefine_KLine* kLine = NULL;
	//������
	int pCount;
	//API����K��
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

	//�ͷ�
	TDB_Free(kLine);
	return res;
}