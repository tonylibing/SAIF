#pragma once
#include "lib.h"
#include <vector>
class TaskHandle
{
public:
	TaskHandle(THANDLE& hTdb, NetWorkUtility* pNetworkInstance, InputParameter& inputParameter);
	~TaskHandle(void);
	void option1();
	void option2();
	void option3();
	void option4();

	void option5();

	void option6();
	void option7();
	void option8();

	void doTask1ByMonth(THANDLE& hTdb, int year, int month);
	void doTask3ByMonth(THANDLE& hTdb, int year, int month);

	void doTask1ByDay(THANDLE& hTdb, int date);
	void doTask3ByDay(THANDLE& hTdb, int date);

	void doTask2ByDay(THANDLE& hTdb, int date, int cycleNumber);
private:
	std::vector<StockTicker> allStockTikers;
	void writeFileHeaderForTask1(fstream&);
	void anslyseAllTransactions(std::map<int, std::vector<TDBDefine_Transaction>>&, const string&, fstream&);
	OrderType checkOrderType(int value);
	void calBuyAndSellValue(Order& order);
	void getLast15And30(const vector<TDBDefine_Transaction>& transVec, vector<TDBDefine_Transaction>& last15, vector<TDBDefine_Transaction>& last30);

	NetWorkUtility* pNetWorkInstance;
	THANDLE& hTdb;
	InputParameter& inputParameter;
};

