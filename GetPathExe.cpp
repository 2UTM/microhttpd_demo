#include "GetPathExe.h"

// получаем путь до exe
std::string getPathExe()
{
	// Получаем путь до исполняемого файла
	CHAR exePathTchar[MAX_PATH];
	GetModuleFileNameA(0, (LPSTR)exePathTchar, MAX_PATH);
	std::string exePath(exePathTchar);
	exePath = exePath.erase(exePath.size() - 24); // отрезаем serverUpdaterHostVNC.exe

	return exePath;
}

std::string pathExe = getPathExe();