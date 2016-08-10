#include "StdAfx.h"
#include "UserInterface.h"


UserInterface::UserInterface(void)
{
}


UserInterface::~UserInterface(void)
{
}

void UserInterface::printHelp()
{
	cout<<string(50, '-')<<endl;
	cout<<"请选择运行模式:"<<endl;
	cout<<"1). 获取一段时间间隔所有股票的逐笔成交数据, 例如(从2015年6月到2016年1月)"<<endl;
	cout<<"2). 获取特定一个月所有股票的逐笔成交数据, 例如(2015年5月)"<<endl;
	cout<<"3). 获取特定一个月的\"特定\"股票开始的逐笔成交数据,例如(2015年5月, 从603026.SH开始获取)"<<endl;
	cout<<"4). 获取连续几天所有股票的逐笔成交数据, 例如(从2016年4月12日到2016年4月24日)"<<endl;
	cout<<"5). 获取连续几天所有股票的K线数据, 例如(从2015/06/01到2016/01/31)"<<endl;
	cout<<"6). 获取一段时间间隔的所有股票的(last 15 and 30 minutes before close)的逐笔成交数据, 例如(从2015年6月到2016年1月)"<<endl;
	cout<<"7). 获取连续几天所有股票的(last 15 and 30 minutes before close)的逐笔成交数据, 例如(从2015/06/01到2015/06/10)"<<endl;
	cout<<"8). 获取连续几天所有股票的task1, task2, task3数据, 例如(从2015/06/01到2015/06/10)"<<endl;
}

InputParameter UserInterface::readUserInput()
{
	printHelp();
	int type;
	cin>>type;
	if(type < 1 || type > 8)
		readUserInput();

	InputParameter input;

	int year, month;
	if (type == 1) 
	{
		string startTime;
		string endTime;
		cout<<"请输入开始时间(例如: 2015/6): ";
		scanf("%d/%d", &input.startYear, &input.startMonth);
		cout<<"请输入结束时间(例如: 2015/12): ";
		scanf("%d/%d", &input.endYear, &input.endMonth);
		input.type = 1;
	} 
	else if (type == 2)
	{
		printf("请输入年份?(HINT:2016, 2015, 2014, 2013)\n");
		cin>>year;
		printf("请输入月份?(HINT: 1到12)\n");
		cin>>month;
		input.type = 2;
		input.startYear = year;
		input.startMonth = month;
	} 
	else if (type == 3) 
	{
		input.type = 3;
		printf("请输入股票代码, 例如(603028.SH): ");
		cin>>input.stockCode;
		printf("请输入年份?(HINT:2016, 2015, 2014, 2013)\n");
		cin>>year;
		printf("请输入月份?(HINT: 1到12)\n");
		cin>>month;
		input.startYear = year;
		input.startMonth = month;
	} 
	else if (type == 4) 
	{
		input.type = 4;
		printf("请输入开始日期, 例如(2015/6/18): ");
		scanf("%d/%d/%d", &input.startYear, &input.startMonth, &input.startDay);
		cout<<"请输入结束日期, 例如(2015/6/31): ";
		scanf("%d/%d/%d", &input.endYear, &input.endMonth, &input.endDay);
	}
	else if (type == 5) 
	{
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
		scanf("%d/%d", &input.startYear, &input.startMonth);
		cout<<"请输入结束时间(例如: 2015/12): ";
		scanf("%d/%d", &input.endYear, &input.endMonth);
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