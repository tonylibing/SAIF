#include "StdAfx.h"
#include "TimeLib.h"


TimeLib::TimeLib(void)
{
}


TimeLib::~TimeLib(void)
{
}

int TimeLib::getCurrentDay()
{
	time_t t = time(0); // get time now
	struct tm* now = localtime( &t );
	int res =  (now->tm_year + 1900)*100*100 + (now->tm_mon+1) * 100 + now->tm_mday;
	LOG_INFO <<"currentDay = "<<res;
	return res;
}

std::vector<std::pair<int, int>> TimeLib::monthRange(const InputParameter& inputParameter)
{
	int startY = inputParameter.startYear;
	int startM = inputParameter.startMonth;
	int endY   = inputParameter.endYear;
	int endM   = inputParameter.endMonth;
	vector<pair<int, int>> res;
	while((startY < endY) || (startY == endY && startM <= endM))
	{
		res.push_back(make_pair(startY, startM));
		startM += 1;
		if(startM >= 13) 
		{
			startY += 1;
			startM = 1;
		}
	}
	return res;
}

std::vector<int> TimeLib::dayRange(const InputParameter& inputParameter)
{
	int endY = inputParameter.endYear;
	int endM = inputParameter.endMonth;
	int endD = inputParameter.endDay;

	int startY = inputParameter.startYear;
	int startM = inputParameter.startMonth;
	int startD = inputParameter.startDay;

	std::vector<int> res;
	while((startY < endY) || (startY == endY && startM < endM) || (startY == endY && startM == endM && startD <= endD))
	{
		if(isValid(startY, startM, startD))
		{
			int curDay = startY * 100 * 100 + startM * 100 + startD;
			res.push_back(curDay);
		}
		startD += 1;
		if(startD >= 32)
		{
			startD = 1;
			startM += 1;
		}
		if(startM >= 13) 
		{
			startM = 1;
			startY += 1;
		}
	}
	return res;
}

vector<pair<int, int>> TimeLib::dayRange2(const InputParameter& inputParameter)
{
	int endY  = inputParameter.endYear;
	int endM  = inputParameter.endMonth;
	int endD  = inputParameter.endDay;

	int startY  = inputParameter.startYear;
	int startM  = inputParameter.startMonth;
	int startD  = inputParameter.startDay;

	vector<pair<int, int>> res;
	pair<int, int> element;
	if (startY == endY && startM == endM && startD == endD) {
		element.first = element.second = startY * 100 * 100 + startM * 100 + startD;
		res.push_back(element);
		return res;
	}
	else if (startY == endY && startM == endM) {
		element.first = startY * 100 * 100 + startM * 100 + startD;
		element.second = endY * 100 * 100 + endM * 100 + endD;
		res.push_back(element);
        return res;
	} else if (startY == endY) {
		element.first = startY * 100 * 100 + startM * 100 + startD;
		element.second = startY * 100 * 100 + startM * 100 + 31;
		res.push_back(element);
		for (int m = startM + 1; m < endM; m++) {
			element.first = startY * 100 * 100 + m * 100 + 1;
			element.second = startY * 100 * 100 + m * 100 + 31;
			res.push_back(element);
		}
		element.first = endY * 100 * 100 + endM * 100 + 1;
		element.second = endY * 100 * 100 + endM * 100 + endD;
		res.push_back(element);
	} else {
		element.first = startY * 100 * 100 + startM * 100 + startD;
		element.second = startY * 100 * 100 + startM * 100 + 31;
		res.push_back(element);
		startM += 1;
		if (startM > 12) {
			startM = 1;
			startY += 1;
		}
		while (startY < endY || (startY == endY && startM < endM)) {
			element.first = startY * 100 * 100 + startM * 100 + 1;
			element.second = startY * 100 * 100 + startM * 100 + 31;
			res.push_back(element);
			startM += 1;
			if (startM > 12) {
				startM = 1;
				startY += 1;
			}
		}
		element.first = endY * 100 * 100 + endM * 100 + 1;
		element.second = endY * 100 * 100 + endM * 100 + endD;
		res.push_back(element);
	}
	for(auto i = 0; i < res.size(); i++)
	{
		string endDay = int2str(res.at(i).second);
		int year  = atoi(endDay.substr(0, 4).c_str());
		int month = atoi(endDay.substr(4, 2).c_str());
		int day = atoi(endDay.substr(6, 2).c_str());
		if(month ==1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12)
			res.at(i).second = year * 100 * 100 + month * 100 + 31;
		else
			res.at(i).second = year * 100 * 100 + month * 100 + 30;

		if(year == 2016 && month == 2)
			res.at(i).second = year * 100 * 100 + month * 100 + 29;
		else if(year != 2016 && month == 2)
			res.at(i).second = year * 100 * 100 + month * 100 + 28;
	}
	return res;
}

string TimeLib::formatTime(const SYSTEMTIME& st)
{
	char str[10] = {'\0'};
	sprintf(str, "%02d%02d%02d%03d", st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
	return string(str);
}

int TimeLib::isValid(int yy, int mm, int dd)
{
	if (dd <= 0) return 0;
	switch (mm) {
	case 1: case 3: case 5: case 7:
	case 8: case 10: case 12:
		return dd <= 31;
	case 4: case 6: case 9: case 11:
		return dd <= 30;
	case 2:
		return dd <= 28 + (yy % 400 == 0 || (yy % 4 == 0 && yy % 100 != 0));
	default:
		return 0;  /* invalid month */
	}
}