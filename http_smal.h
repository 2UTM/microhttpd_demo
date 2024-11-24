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

int runWebServer(); // «апуск веб сервера

// ¬ызываетс€ на каждое поле из POST запроса, пока POST не обработан полностью, отвечать нельз€, из документации 
int UserIterator(void* cls,
	enum MHD_ValueKind kind,
	const char* key,
	const char* filename,
	const char* content_type,
	const char* transfer_encoding,
	const char* data,
	uint64_t off,
	size_t size);

// √лавна€ функци€ сервера
int Server(void* cls,
	struct MHD_Connection* connection,
	const char* url,
	const char* method,
	const char* version,
	const char* upload_data,
	size_t* upload_data_size,
	void** ptr);

// шаблонизатор
// int notification - нужно ли уведомление
// 0 - не нужно
// 1 - нужно success
// 2 - нужно warning
std::string templateWeb(std::string pathToTemplate, int notification);

// ѕолучить врем€ из строки, как число
int getTimeStringAsInt(std::string time);

// ѕолучить текущее врем€, как число
int getCurrentTimeAsInt();

int getUpTime(std::string& uptime); // получаем врем€ работы сервера