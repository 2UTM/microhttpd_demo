#include "GetPathExe.h"

// �������� ���� �� exe
std::string getPathExe()
{
	// �������� ���� �� ������������ �����
	CHAR exePathTchar[MAX_PATH];
	GetModuleFileNameA(0, (LPSTR)exePathTchar, MAX_PATH);
	std::string exePath(exePathTchar);
	exePath = exePath.erase(exePath.size() - 24); // �������� serverUpdaterHostVNC.exe

	return exePath;
}

std::string pathExe = getPathExe();