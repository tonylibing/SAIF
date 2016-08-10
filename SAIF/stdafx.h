// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <algorithm>
#include <windows.h>
#include <string>

#include "TDBAPI.h"

#include "plog\Log.h"
#include "plog\Appenders\ConsoleAppender.h"

#include "NetWorkUtility.h"
#include "TimeLib.h"
#include "UserInterface.h"
#include "TaskHandle.h"

#pragma warning(push) 
#pragma warning(disable:4996) 

#if defined ( WIN32 )
#define __func__ __FUNCTION__
#endif

// TODO: reference additional headers your program requires here
