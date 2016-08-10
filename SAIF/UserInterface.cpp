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
	cout<<"��ѡ������ģʽ:"<<endl;
	cout<<"1). ��ȡһ��ʱ�������й�Ʊ����ʳɽ�����, ����(��2015��6�µ�2016��1��)"<<endl;
	cout<<"2). ��ȡ�ض�һ�������й�Ʊ����ʳɽ�����, ����(2015��5��)"<<endl;
	cout<<"3). ��ȡ�ض�һ���µ�\"�ض�\"��Ʊ��ʼ����ʳɽ�����,����(2015��5��, ��603026.SH��ʼ��ȡ)"<<endl;
	cout<<"4). ��ȡ�����������й�Ʊ����ʳɽ�����, ����(��2016��4��12�յ�2016��4��24��)"<<endl;
	cout<<"5). ��ȡ�����������й�Ʊ��K������, ����(��2015/06/01��2016/01/31)"<<endl;
	cout<<"6). ��ȡһ��ʱ���������й�Ʊ��(last 15 and 30 minutes before close)����ʳɽ�����, ����(��2015��6�µ�2016��1��)"<<endl;
	cout<<"7). ��ȡ�����������й�Ʊ��(last 15 and 30 minutes before close)����ʳɽ�����, ����(��2015/06/01��2015/06/10)"<<endl;
	cout<<"8). ��ȡ�����������й�Ʊ��task1, task2, task3����, ����(��2015/06/01��2015/06/10)"<<endl;
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
		cout<<"�����뿪ʼʱ��(����: 2015/6): ";
		scanf("%d/%d", &input.startYear, &input.startMonth);
		cout<<"���������ʱ��(����: 2015/12): ";
		scanf("%d/%d", &input.endYear, &input.endMonth);
		input.type = 1;
	} 
	else if (type == 2)
	{
		printf("���������?(HINT:2016, 2015, 2014, 2013)\n");
		cin>>year;
		printf("�������·�?(HINT: 1��12)\n");
		cin>>month;
		input.type = 2;
		input.startYear = year;
		input.startMonth = month;
	} 
	else if (type == 3) 
	{
		input.type = 3;
		printf("�������Ʊ����, ����(603028.SH): ");
		cin>>input.stockCode;
		printf("���������?(HINT:2016, 2015, 2014, 2013)\n");
		cin>>year;
		printf("�������·�?(HINT: 1��12)\n");
		cin>>month;
		input.startYear = year;
		input.startMonth = month;
	} 
	else if (type == 4) 
	{
		input.type = 4;
		printf("�����뿪ʼ����, ����(2015/6/18): ");
		scanf("%d/%d/%d", &input.startYear, &input.startMonth, &input.startDay);
		cout<<"�������������, ����(2015/6/31): ";
		scanf("%d/%d/%d", &input.endYear, &input.endMonth, &input.endDay);
	}
	else if (type == 5) 
	{
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
		scanf("%d/%d", &input.startYear, &input.startMonth);
		cout<<"���������ʱ��(����: 2015/12): ";
		scanf("%d/%d", &input.endYear, &input.endMonth);
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