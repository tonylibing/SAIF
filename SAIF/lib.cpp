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
//��ȡ����֤ȯ�Ĵ���
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

//��������
vector<TDBDefine_Code> GetCodeTable(THANDLE hTdb, char* szMarket)
{
	TDBDefine_Code* pCodetable = NULL;
	int pCount;
	bool outPutTable = false;
	int ret = TDB_GetCodeTable(hTdb, szMarket, &pCodetable, &pCount);

	vector<TDBDefine_Code> res;

	if (ret == TDB_NO_DATA)
	{
		printf("�޴����");
		return res;
	}
	printf("---------------------------Code Table--------------------\n");
	printf("�յ������������%d\n",pCount);
	
	for (int i=0;i<pCount;i++)
	{
		if(pCodetable[i].nType != 0x10)
			continue;
		/*printf("��ô��� chWindCode:%s \n", pCodetable[i].chWindCode);	
		printf("���������� chWindCode:%s \n", pCodetable[i].chCode);			
		printf("�г����� chWindCode:%s \n", pCodetable[i].chMarket);
		printf("֤ȯ�������� chWindCode:%s \n", pCodetable[i].chCNName);
		printf("֤ȯӢ������ chWindCode:%s \n", pCodetable[i].chENName);
		printf("֤ȯ���� chWindCode:%d \n", pCodetable[i].nType);
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
//��¼
THANDLE logIn(const string& ipAddress, int port, const string& userName, const string& passWord)
{
	//���÷�������Ϣ
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
		printf("��¼�ɹ�!\n");
		return hTdb;
	}
	printf("��¼ʧ��!\n");
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
	printf("�ɽ�ʱ��(Date): %d \n", trans.nDate);
	printf("�ɽ�ʱ��: %d \n", trans.nTime);
	printf("�ɽ�����: %c \n", trans.chFunctionCode);
	printf("ί�����: %c \n", trans.chOrderKind);
	printf("BS��־: %c \n", trans.chBSFlag);
	printf("�ɽ��۸�: %d \n", trans.nTradePrice);
	printf("�ɽ�����: %d \n", trans.nTradeVolume);
	printf("�������: %d \n", trans.nAskOrder);
	printf("�������: %d \n", trans.nBidOrder);
	printf("�ɽ����: %d \n", trans.nIndex);
	printf("------------------------------------------------------\n");
}
//��ȡK��
void GetKData(THANDLE hTdb, char* szCode, char* szMarket, int nBeginDate, int nEndDate, int nCycle, int nUserDef, int nCQFlag, int nAutoComplete)
{
	//����K��
	TDBDefine_ReqKLine* req = new TDBDefine_ReqKLine;
	strncpy(req->chCode, szCode, ELEMENT_COUNT(req->chCode));
	strncpy(req->chMarketKey, szMarket, ELEMENT_COUNT(req->chMarketKey));

	req->nCQFlag = (REFILLFLAG)nCQFlag;//��Ȩ��־�����û�����
	req->nBeginDate = nBeginDate;//��ʼ����
	req->nEndDate = nEndDate;//��������
	req->nBeginTime = 0;//��ʼʱ��
	req->nEndTime = 0;//����ʱ��

	req->nCycType = (CYCTYPE)nCycle;
	req->nCycDef = 0;

	//���ؽṹ��ָ��
	TDBDefine_KLine* kLine = NULL;
	//������
	int pCount;
	//API����K��
	TDB_GetKLine(hTdb,req,&kLine,&pCount);
	delete req;
	req = NULL;

	printf("---------------------------K Data--------------------\n");
	printf("����������%d,��ӡ 1/100 ��\n\n",pCount);
	for(int i=0;i<pCount;)
	{
		printf("WindCode:%s\n Code:%s\n Date:%d\n Time:%d\n Open:%d\n High:%d\n Low:%d\n Close:%d\n Volume:%I64d\n Turover:%I64d\n MatchItem:%d\n Interest:%d\n",
			kLine[i].chWindCode,kLine[i].chCode,kLine[i].nDate,kLine[i].nTime,kLine[i].nOpen,kLine[i].nHigh,kLine[i].nLow,kLine[i].nClose,
			kLine[i].iVolume,kLine[i].iTurover,kLine[i].nMatchItems,kLine[i].nInterest);
		i +=100;
	}
	//�ͷ�
	TDB_Free(kLine);
}

//�������̵�tick
void GetTickData(THANDLE hTdb, char* szCode, char* szMarket, int nDate)
{
	//������Ϣ
	TDBDefine_ReqTick req = {0};
	strncpy(req.chCode, szCode, sizeof(req.chCode)); //����д�������ȡ�Ĺ�Ʊ����
	strncpy(req.chMarketKey, szMarket, sizeof(req.chMarketKey));
	req.nDate = nDate;
	req.nBeginTime = 0;
	req.nEndTime = 0;

	TDBDefine_Tick *pTick = NULL;
	int pCount;
	int ret = TDB_GetTick(hTdb,&req,&pTick, &pCount);

	printf("---------------------------------------Tick Data------------------------------------------\n");
	printf("���յ� %d ��Tick���ݣ� ��ӡ 1/100 ����\n", pCount);

	for(int i=0; i<pCount;)
	{
		TDBDefine_Tick& pTickCopy = pTick[i];
		printf("��ô��� chWindCode:%s \n", pTickCopy.chWindCode);
		printf("���� nDate:%d \n", pTickCopy.nDate);
		printf("ʱ�� nTime:%d \n", pTickCopy.nTime);

		printf("�ɽ��� nPrice:%d \n", pTickCopy.nPrice);
		printf("�ɽ��� iVolume:%I64d \n", pTickCopy.iVolume);
		printf("�ɽ���(Ԫ) iTurover:%I64d \n", pTickCopy.iTurover);
		printf("�ɽ����� nMatchItems:%d \n", pTickCopy.nMatchItems);
		printf(" nInterest:%d \n", pTickCopy.nInterest);

		printf("�ɽ���־: chTradeFlag:%c \n", pTickCopy.chTradeFlag);
		printf("BS��־: chBSFlag:%c \n", pTickCopy.chBSFlag);
		printf("���ճɽ���: iAccVolume:%I64d \n", pTickCopy.iAccVolume);
		printf("���ճɽ���: iAccTurover:%I64d \n", pTickCopy.iAccTurover);

		printf("��� nHigh:%d \n", pTickCopy.nHigh);
		printf("��� nLow:%d \n", pTickCopy.nLow);
		printf("���� nOpen:%d \n", pTickCopy.nOpen);
		printf("ǰ���� nPreClose:%d \n", pTickCopy.nPreClose);

		//�������ֶ�
		std::string strOut= array2str(pTickCopy.nAskPrice,ELEMENT_COUNT(pTickCopy.nAskPrice));
		printf("������ nAskPrice:%s \n", strOut.c_str());
		strOut= array2str((const int*)pTickCopy.nAskVolume,ELEMENT_COUNT(pTickCopy.nAskVolume));
		printf("������ nAskVolume:%s \n", strOut.c_str());
		strOut= array2str(pTickCopy.nBidPrice,ELEMENT_COUNT(pTickCopy.nBidPrice));
		printf("����� nBidPrice:%s \n", strOut.c_str());
		strOut= array2str((const int*)pTickCopy.nBidVolume,ELEMENT_COUNT(pTickCopy.nBidVolume));
		printf("������ nBidVolume:%s \n", strOut.c_str());
		printf("��Ȩƽ�������� nAskAvPrice:%d \n", pTickCopy.nAskAvPrice);
		printf("��Ȩƽ������� nBidAvPrice:%d \n", pTickCopy.nBidAvPrice);
		printf("�������� iTotalAskVolume:%I64d \n", pTickCopy.iTotalAskVolume);
		printf("�������� iTotalBidVolume:%I64d \n", pTickCopy.iTotalBidVolume);
#if 0
		//�ڻ��ֶ�
		printf("����� nSettle:%d \n", pTickCopy.nSettle);
		printf("�ֲ��� nPosition:%d \n", pTickCopy.nPosition);
		printf("��ʵ�� nCurDelta:%d \n", pTickCopy.nCurDelta);
		printf("����� nPreSettle:%d \n", pTickCopy.nPreSettle);
		printf("��ֲ� nPrePosition:%d \n", pTickCopy.nPrePosition);

		//ָ��
		printf("����Ȩָ�� nIndex:%d \n", pTickCopy.nIndex);
		printf("Ʒ������ nStocks:%d \n", pTickCopy.nStocks);
		printf("����Ʒ���� nUps:%d \n", pTickCopy.nUps);
		printf("�µ�Ʒ���� nDowns:%d \n", pTickCopy.nDowns);
		printf("��ƽƷ���� nHoldLines:%d \n", pTickCopy.nHoldLines);
#endif
		printf("-------------------------------\n");
		i += 1000;
	}
	//�ͷ�
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

//��ʳɽ�
vector<TDBDefine_Transaction> GetTransaction(THANDLE hTdb, char* szCode, char* szMarketKey, int nDate)
{
	TDBDefine_ReqTransaction req = {0}; //����
	strncpy(req.chCode, szCode, sizeof(req.chCode)); //����д�������ȡ�Ĺ�Ʊ����
	strncpy(req.chMarketKey, szMarketKey, sizeof(req.chMarketKey));
	req.nDate = nDate;
	req.nBeginTime = 0;
	req.nEndTime = 0;

	TDBDefine_Transaction *pTransaction = NULL;
	int pCount;
	int ret = TDB_GetTransaction(hTdb,&req, &pTransaction, &pCount);

	/*if (pTransaction && pCount && pTransaction[pCount-1].nTradeVolume > 0)
	{
		printf("����code:%s\n", req.chCode);
		strncpy(ErrCode[ErrNum++], req.chCode, sizeof(req.chCode));
	}*/

	//printf("inside: %s, nDate = %d, �յ� %d ����ʳɽ���Ϣ\n", __func__, nDate, pCount);

	vector<TDBDefine_Transaction> vec;
	for (int i=0; i < pCount; i++)
	{
		const TDBDefine_Transaction& trans = pTransaction[i];
		//showTranscation(trans);
		vec.push_back(trans);
	}
	//�ͷ�
	TDB_Free(pTransaction);
	return vec;
}

//��ȡ�������������е���ʳɽ�
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