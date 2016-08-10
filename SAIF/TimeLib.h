#pragma once
#include "lib.h"
#include <vector>
class TimeLib
{
public:
	TimeLib(void);
	static int getCurrentDay();
	static std::vector<std::pair<int, int>> monthRange(const InputParameter&);
	static std::vector<int> dayRange(const InputParameter&);
	static vector<pair<int, int>> dayRange2(const InputParameter&);
	static string formatTime(const SYSTEMTIME& st);
	static int isValid(int yy, int mm, int dd);
	~TimeLib(void);
};

