#pragma once
#include "stdio.h"
#include "GetCurrentDateTime.h"

// ТЕСТ ЛОГА ЧЕРЕЗ fprintf
int openLogFile(); // открытие файла лога
int closeLogFile(); // закрытие файла лога
void logger(std::string strLog, std::string level); // запись в лог