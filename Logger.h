#pragma once
#include "stdio.h"
#include "GetCurrentDateTime.h"

// ���� ���� ����� fprintf
int openLogFile(); // �������� ����� ����
int closeLogFile(); // �������� ����� ����
void logger(std::string strLog, std::string level); // ������ � ���