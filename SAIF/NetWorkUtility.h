#pragma once
#include <string>
#include <vector>
#include <map>

class NetWorkUtility
{
public:
	static NetWorkUtility* GetInstance()
	{
		static NetWorkUtility instance;
		return &instance;
	}
	~NetWorkUtility(void);

	THANDLE logIn(const std::string& ipAddress, int port, const std::string& userName, const std::string& passWord);

	//��ȡһ����Ʊ�ض�һ�������ÿ�ʳɽ�����
	std::vector<TDBDefine_Transaction> GetTransaction(THANDLE hTdb, char* szCode, char* szMarketKey, int nDate);
	//��ȡһ����Ʊ�ض�һ�µ�����ÿ�ʳɽ�����
	std::map<int, std::vector<TDBDefine_Transaction>> GetAllTransactions(THANDLE hTdb, char* szCode, char* szMarketKey, int year, int month);
	//����K��
	std::vector<TDBDefine_KLine> GetKData(THANDLE hTdb, char* szCode, char* szMarket, int nBeginDate, int nEndDate, int nCycleType, int nCycleNumber, int nCQFlag, int nAutoComplete);
private:
	NetWorkUtility(void){};
	NetWorkUtility(const NetWorkUtility&);
	NetWorkUtility& operator= (const NetWorkUtility&) ;
};

