#include "Logger.h"
#include "GetPathExe.h"

#define _CRT_SECURE_NO_WARNINGS

extern std::string pathExe;

FILE* file;

// ТЕСТ ЛОГА ЧЕРЕЗ fprintf
int openLogFile()
{
#pragma warning(suppress : 4996) // fopen deprecated
    if ((file = fopen((pathExe + "testlog.log").c_str(), "a")) == NULL)
    {
        return 1;
    }
    setvbuf(file, 0, _IONBF, 0); // отключить буффер и сразу писать в файл
    return 0;
}

int closeLogFile()
{
    if (fclose(file) != 0)
    {
        return 1;
    }
    return 0;
}

void logger(std::string strLog, std::string level)
{
    fprintf(file, "%s [%s]: %s", currentDateTimeMSSQL().c_str(), level.c_str(), strLog.c_str());
}