// SAIF.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "TDBAPI.h"
#include <string>
#include <vector>
#include "lib.h"
#include <map>
#include <set>

#include <Windows.h>
#include <iostream>
using namespace std;


int _tmain(int argc, _TCHAR* argv[])
{
	static plog::ConsoleAppender<plog::TxtFormatter> consoleAppender;
	plog::init(plog::info, &consoleAppender);

	NetWorkUtility* pNetWorkInstane = NetWorkUtility::GetInstance();

	string ipAddress = "114.80.154.34";
	int    port      = 6261;
	string userName  = "TD1056611005";
	string passWord  = "37264673";

	THANDLE hTdb = pNetWorkInstane->logIn(ipAddress, port, userName, passWord);

	InputParameter inputParameter = UserInterface::readUserInput();
	//GetAllStockTikers(allStockTikers, "ticker list.csv");
	TaskHandle taskHandle(hTdb, pNetWorkInstane, inputParameter);

	if(hTdb)
	{
		if(inputParameter.type == 2)
		{
			taskHandle.option2();
		}
		else if (inputParameter.type == 3)
		{
			taskHandle.option3();
		}
		else if (inputParameter.type == 1)
		{
			taskHandle.option1();
		}
		else if (inputParameter.type == 4)
		{	
			taskHandle.option4();
		}
		else if (inputParameter.type == 5)
		{
			taskHandle.option5();
		}
		else if (inputParameter.type == 6)
		{
			taskHandle.option6();
		}
		else if (inputParameter.type == 7)
		{	
			taskHandle.option7();
		}
		else if (inputParameter.type == 8)
		{
			taskHandle.option8();
		}
	}
	cout<<string(50, '*')<<endl;
	cout<<"                  运行完成!          "<<endl;
	cout<<string(50, '*')<<endl;
	TDB_Close(hTdb);
	return 0;
}