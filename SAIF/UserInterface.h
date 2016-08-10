#pragma once
#include "lib.h"

class UserInterface
{
public:
	UserInterface(void);
	~UserInterface(void);

	static InputParameter readUserInput();
private:
	static void printHelp();
};

