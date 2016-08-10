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
	cout<<"��ѡ������ģʽ:(����1����2����3����4����5����6����7����8)"<<endl;
	cout<<"1). ��ȡһ��ʱ�������й�Ʊ����ʳɽ�����, ����(��2015��6�µ�2016��1��)"<<endl;
	cout<<"2). ��ȡ�ض�һ�������й�Ʊ����ʳɽ�����, ����(2015��5��)"<<endl;
	cout<<"3). ��ȡ�ض�һ���·ݵ�\"�ض�\"��Ʊ��ʼ����ʳɽ�����,����(2015��5��, ��603026.SH��ʼ��ȡ)"<<endl;
	cout<<"4). ��ȡ�����������й�Ʊ����ʳɽ�����, ����(��2016��4��12�յ�2016��4��24��)"<<endl;
	cout<<"5). ��ȡ�����������й�Ʊ��K������, ����(��2015/06/01��2016/01/31)"<<endl;
	cout<<"6). ��ȡһ��ʱ���������й�Ʊ��(last 15 and 30 minutes before close)����ʳɽ�����, ����(��2015��6�µ�2016��1��)"<<endl;
	cout<<"7). ��ȡ�����������й�Ʊ��(last 15 and 30 minutes before close)����ʳɽ�����, ����(��2015/06/01��2015/06/10)"<<endl;
	cout<<"8). ��ȡ�����������й�Ʊ��task1, task2, task3����, ����(��2015/06/01��2015/06/10)"<<endl;
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
		cout<<"�����뿪ʼʱ��(����: 2015/6): ";
		cin>>startTime;
		cout<<"���������ʱ��(����: 2015/12): ";
		cin>>endTime;
		input.type = 1;
		input.startYear  = atoi(startTime.substr(0, 4).c_str());
		input.startMonth = atoi(startTime.substr(5, 7).c_str());
		input.endYear    = atoi(endTime.substr(0, 4).c_str());
		input.endMonth   = atoi(endTime.substr(5, 7).c_str());
	} else if (type == 2) {
		printf("���������?(HINT:2016, 2015, 2014, 2013)\n");
		cin>>year;
		printf("�������·�?(HINT: 1��12)\n");
		cin>>month;
		input.type = 2;
		input.startYear = year;
		input.startMonth = month;
	} else if (type == 3) {
		input.type = 3;
		printf("�������Ʊ����, ����(603028.SH): ");
		cin>>input.stockCode;
		printf("���������?(HINT:2016, 2015, 2014, 2013)\n");
		cin>>year;
		printf("�������·�?(HINT: 1��12)\n");
		cin>>month;
		input.startYear = year;
		input.startMonth = month;
	} else if (type == 4) 
	{
		input.type = 4;
		printf("�����뿪ʼ����, ����(2015/6/18): ");
		scanf("%d/%d/%d", &input.startYear, &input.startMonth, &input.startDay);
		cout<<"�������������, ����(2015/6/31): ";
		scanf("%d/%d/%d", &input.endYear, &input.endMonth, &input.endDay);
	}
	else if (type == 5) {
		input.type = 5;
		string startTime;
		string endTime;
		cout<<"�����뿪ʼʱ��(����: 2015/06/01): ";
		scanf("%d/%d/%d", &input.startYear, &input.startMonth, &input.startDay);
		cout<<"���������ʱ��(����: 2015/12/31): ";
		scanf("%d/%d/%d", &input.endYear, &input.endMonth, &input.endDay);
		cout<<"�������ݼ������(HINT:ȡֵ��Χ0��60): ";
		cin>>input.cycleNumber;
	} 
	else if (type == 6)
	{
		input.type = 6;
		string startTime;
		string endTime;
		cout<<"�����뿪ʼʱ��(����: 2015/6): ";
		cin>>startTime;
		cout<<"���������ʱ��(����: 2015/12): ";
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
		cout<<"�����뿪ʼʱ��(����: 2015/06/01): ";
		scanf("%d/%d/%d", &input.startYear, &input.startMonth, &input.startDay);
		cout<<"���������ʱ��(����: 2015/12/31): ";
		scanf("%d/%d/%d", &input.endYear, &input.endMonth, &input.endDay);
	}
	else if (type == 8)
	{
		input.type = 8;
		string startTime;
		string endTime;
		cout<<"�����뿪ʼʱ��(����: 2015/06/01): ";
		scanf("%d/%d/%d", &input.startYear, &input.startMonth, &input.startDay);
		cout<<"���������ʱ��(����: 2015/06/11): ";
		scanf("%d/%d/%d", &input.endYear, &input.endMonth, &input.endDay);
		cout<<"�������ݼ������(HINT:ȡֵ��Χ0��60): ";
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

	settings.nRetryCount = 100;
	settings.nRetryGap = 1;
	settings.nTimeOutVal = 300;

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







