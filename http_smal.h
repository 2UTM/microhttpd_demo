#pragma once

#include <iostream>
#include "microhttpd.h"
#include <string>
#include <string.h>
#include <stdlib.h>
#include <fstream>
#include <ctime>
#include <iomanip>
//#include "WorkDB.h"
//#include "WorkURLDecode.h"

int runWebServer(); // ������ ��� �������

// ���������� �� ������ ���� �� POST �������, ���� POST �� ��������� ���������, �������� ������, �� ������������ 
int UserIterator(void* cls,
	enum MHD_ValueKind kind,
	const char* key,
	const char* filename,
	const char* content_type,
	const char* transfer_encoding,
	const char* data,
	uint64_t off,
	size_t size);

// ������� ������� �������
int Server(void* cls,
	struct MHD_Connection* connection,
	const char* url,
	const char* method,
	const char* version,
	const char* upload_data,
	size_t* upload_data_size,
	void** ptr);

// ������������
// int notification - ����� �� �����������
// 0 - �� �����
// 1 - ����� success
// 2 - ����� warning
std::string templateWeb(std::string pathToTemplate, int notification);

// �������� ����� �� ������, ��� �����
int getTimeStringAsInt(std::string time);

// �������� ������� �����, ��� �����
int getCurrentTimeAsInt();

int getUpTime(std::string& uptime); // �������� ����� ������ �������