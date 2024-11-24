#include "http_smal.h"
#include "windows.h"
#include "Logger.h"
#include <thread>

///////////////////////////////////////////////////////////////////////////////////////
// Создание дампа в случае аварийного завершения
#include "minidumpapiset.h"
#pragma comment(lib,"Dbghelp.lib")

void CreateDump(_EXCEPTION_POINTERS* ExceptionInfo)
{

    char FileName[32];
    SYSTEMTIME LocalTime;    GetLocalTime(&LocalTime);
    sprintf_s(FileName, _countof(FileName),
        "Memory %04hu.%02hu.%02hu %02hu %02hu %02hu.dmp",
        LocalTime.wYear, LocalTime.wMonth, LocalTime.wDay,
        LocalTime.wHour, LocalTime.wMinute, LocalTime.wSecond);

    HANDLE hFile = CreateFileA(FileName, GENERIC_WRITE, FILE_SHARE_READ, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
    if (INVALID_HANDLE_VALUE != hFile)
    {
        MINIDUMP_EXCEPTION_INFORMATION ExInfo;
        ExInfo.ThreadId = GetCurrentThreadId();
        ExInfo.ExceptionPointers = ExceptionInfo;
        ExInfo.ClientPointers = 0;
        MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hFile, MiniDumpWithFullMemory, &ExInfo, 0, 0);
        CloseHandle(hFile);
    }
}

LONG WINAPI MyUnhandledExceptionFilter(_EXCEPTION_POINTERS* ExceptionInfo)
{
    CreateDump(ExceptionInfo);
    return EXCEPTION_CONTINUE_SEARCH;
}

class INITILIZE
{
public:
    INITILIZE()
    {
        SetUnhandledExceptionFilter(MyUnhandledExceptionFilter);
    }
} Initilize;
///////////////////////////////////////////////////////////////////////////////////////

SERVICE_STATUS servicestatus;
SERVICE_STATUS_HANDLE hstatus;
char PathService[256];

#define NAME_SERVICE "serverUpdaterHostVNC"

void WINAPI CtrlHandler(DWORD request)
{

    switch (request)
    {
    case SERVICE_CONTROL_STOP:

        servicestatus.dwWin32ExitCode = 0;
        servicestatus.dwCurrentState = SERVICE_STOPPED;
        SetServiceStatus(hstatus, &servicestatus);
        printf("%s: ", currentDateTimeMSSQL().c_str());
        printf("Service stopped SERVICE_CONTROL_STOP\n");
        logger("Service stopped SERVICE_CONTROL_STOP\n", "INFO"); // ТЕСТ ЛОГА ЧЕРЕЗ fprintf

        break;

    case SERVICE_CONTROL_SHUTDOWN:

        servicestatus.dwWin32ExitCode = 0;
        servicestatus.dwCurrentState = SERVICE_STOPPED;
        SetServiceStatus(hstatus, &servicestatus);
        printf("%s: ", currentDateTimeMSSQL().c_str());
        printf("Service stopped SERVICE_CONTROL_SHUTDOWN\n");
        logger("Service stopped SERVICE_CONTROL_SHUTDOWN\n", "INFO"); // ТЕСТ ЛОГА ЧЕРЕЗ fprintf

        break;

    default:

        break;

    }

    SetServiceStatus(hstatus, &servicestatus);
}

void WINAPI ServiceMain(int argc, char** argv)
{
    servicestatus.dwServiceType = SERVICE_WIN32;
    servicestatus.dwCurrentState = SERVICE_START_PENDING;
    servicestatus.dwControlsAccepted = SERVICE_ACCEPT_SHUTDOWN | SERVICE_ACCEPT_STOP;//In this example, only two control commands are accepted: system shutdown and service stop
    servicestatus.dwWin32ExitCode = 0;
    servicestatus.dwServiceSpecificExitCode = 0;
    servicestatus.dwCheckPoint = 0;
    servicestatus.dwWaitHint = 0;

    hstatus = RegisterServiceCtrlHandlerA(NAME_SERVICE, CtrlHandler);

    if (hstatus == 0)
    {
        printf("%s: ", currentDateTimeMSSQL().c_str());
        printf("RegisterServiceCtrlHandler failed\n");
        logger("RegisterServiceCtrlHandler failed\n", "ERROR"); // ТЕСТ ЛОГА ЧЕРЕЗ fprintf
        return;
    }
    printf("%s: ", currentDateTimeMSSQL().c_str());
    printf("RegisterServiceCtrlHandler success\n");
    logger("RegisterServiceCtrlHandler success\n", "INFO"); // ТЕСТ ЛОГА ЧЕРЕЗ fprintf

    //Сообщаем о состоянии выполнения в SCM
    servicestatus.dwCurrentState = SERVICE_RUNNING;
    SetServiceStatus(hstatus, &servicestatus);

    // Запуск веб сервера
    runWebServer();
}

int RunService()
{
    SC_HANDLE hSCManager = OpenSCManagerA(NULL, NULL, SC_MANAGER_CREATE_SERVICE);
    SC_HANDLE hService = OpenServiceA(hSCManager, NAME_SERVICE, SERVICE_START);
    if (!StartServiceA(hService, 0, NULL)) {
        CloseServiceHandle(hSCManager);
        printf("%s: ", currentDateTimeMSSQL().c_str());
        printf("Can't start service\n");
        logger("Can't start service\n", "ERROR"); // ТЕСТ ЛОГА ЧЕРЕЗ fprintf
        return -1;
    }

    CloseServiceHandle(hService);
    CloseServiceHandle(hSCManager);
    printf("%s: ", currentDateTimeMSSQL().c_str());
    printf("Start service\n");
    logger("Start service\n", "INFO"); // ТЕСТ ЛОГА ЧЕРЕЗ fprintf
    return 0;
}

int InstallService()
{

    SC_HANDLE hSCManager = OpenSCManagerA(NULL, NULL, SC_MANAGER_CREATE_SERVICE);
    if (!hSCManager)
    {
        printf("%s: ", currentDateTimeMSSQL().c_str());
        printf("Can't open Service Control Manager\n");
        logger("Can't open Service Control Manager\n", "ERROR"); // ТЕСТ ЛОГА ЧЕРЕЗ fprintf
        return -1;
    }

    SC_HANDLE hService = CreateServiceA(
        hSCManager,
        NAME_SERVICE,
        NAME_SERVICE,
        SERVICE_ALL_ACCESS,
        SERVICE_WIN32_OWN_PROCESS,
        SERVICE_AUTO_START,
        SERVICE_ERROR_NORMAL,
        PathService,
        NULL, NULL, NULL, NULL, NULL
    );

    if (!hService) {
        int err = GetLastError();
        switch (err) {
        case ERROR_ACCESS_DENIED:
            printf("%s: ", currentDateTimeMSSQL().c_str());
            printf("InstallService ERROR_ACCESS_DENIED\n");
            logger("InstallService ERROR_ACCESS_DENIED\n", "ERROR"); // ТЕСТ ЛОГА ЧЕРЕЗ fprintf
            break;
        case ERROR_CIRCULAR_DEPENDENCY:
            printf("%s: ", currentDateTimeMSSQL().c_str());
            printf("InstallService ERROR_CIRCULAR_DEPENDENCY\n");
            logger("InstallService ERROR_CIRCULAR_DEPENDENCY\n", "ERROR"); // ТЕСТ ЛОГА ЧЕРЕЗ fprintf
            break;
        case ERROR_DUPLICATE_SERVICE_NAME:
            printf("%s: ", currentDateTimeMSSQL().c_str());
            printf("InstallService ERROR_DUPLICATE_SERVICE_NAME\n");
            logger("InstallService ERROR_DUPLICATE_SERVICE_NAME\n", "ERROR"); // ТЕСТ ЛОГА ЧЕРЕЗ fprintf
            break;
        case ERROR_INVALID_HANDLE:
            printf("%s: ", currentDateTimeMSSQL().c_str());
            printf("InstallService ERROR_INVALID_HANDLE\n");
            logger("InstallService ERROR_INVALID_HANDLE\n", "ERROR"); // ТЕСТ ЛОГА ЧЕРЕЗ fprintf
            break;
        case ERROR_INVALID_NAME:
            printf("%s: ", currentDateTimeMSSQL().c_str());
            printf("InstallService ERROR_INVALID_NAME\n");
            logger("InstallService ERROR_INVALID_NAME\n", "ERROR"); // ТЕСТ ЛОГА ЧЕРЕЗ fprintf
            break;
        case ERROR_INVALID_PARAMETER:
            printf("%s: ", currentDateTimeMSSQL().c_str());
            printf("InstallService ERROR_INVALID_PARAMETER\n");
            logger("InstallService ERROR_INVALID_PARAMETER\n", "ERROR"); // ТЕСТ ЛОГА ЧЕРЕЗ fprintf
            break;
        case ERROR_INVALID_SERVICE_ACCOUNT:
            printf("%s: ", currentDateTimeMSSQL().c_str());
            printf("InstallService ERROR_INVALID_SERVICE_ACCOUNT\n");
            logger("InstallService ERROR_INVALID_SERVICE_ACCOUNT\n", "ERROR"); // ТЕСТ ЛОГА ЧЕРЕЗ fprintf
            break;
        case ERROR_SERVICE_EXISTS:
            printf("%s: ", currentDateTimeMSSQL().c_str());
            printf("InstallService ERROR_SERVICE_EXISTS\n");
            logger("InstallService ERROR_SERVICE_EXISTS\n", "ERROR"); // ТЕСТ ЛОГА ЧЕРЕЗ fprintf
            break;
        default:
            printf("%s: ", currentDateTimeMSSQL().c_str());
            printf("InstallService Undefined\n");
            logger("InstallService Undefined\n", "ERROR"); // ТЕСТ ЛОГА ЧЕРЕЗ fprintf
        }
        CloseServiceHandle(hSCManager);
        return -1;
    }
    CloseServiceHandle(hService);

    CloseServiceHandle(hSCManager);
    printf("%s: ", currentDateTimeMSSQL().c_str());
    printf("Success install service\n");
    logger("Success install service\n", "INFO"); // ТЕСТ ЛОГА ЧЕРЕЗ fprintf
    return 0;
}

int RemoveService()
{
    SC_HANDLE hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (!hSCManager)
    {
        printf("%s: ", currentDateTimeMSSQL().c_str());
        printf("Can't open Service Control Manager\n");
        logger("Can't open Service Control Manager\n", "ERROR"); // ТЕСТ ЛОГА ЧЕРЕЗ fprintf
        return -1;
    }
    SC_HANDLE hService = OpenServiceA(hSCManager, NAME_SERVICE, SERVICE_STOP | DELETE);
    if (!hService)
    {
        printf("%s: ", currentDateTimeMSSQL().c_str());
        printf("Can't remove service\n");
        logger("Can't remove service\n", "ERROR"); // ТЕСТ ЛОГА ЧЕРЕЗ fprintf
        CloseServiceHandle(hSCManager);
        return -1;
    }

    DeleteService(hService);
    CloseServiceHandle(hService);
    CloseServiceHandle(hSCManager);
    printf("%s: ", currentDateTimeMSSQL().c_str());
    printf("Success remove service\n");
    logger("Success remove service\n", "INFO"); // ТЕСТ ЛОГА ЧЕРЕЗ fprintf
    return 0;
}

int main(int argc, CHAR* argv[])
{
    setlocale(LC_ALL, "ru-RU");

    //int err = sqlite3_config(SQLITE_CONFIG_MULTITHREAD);
    //if (err != SQLITE_OK)
    //{
    //    printf("%s: ", currentDateTimeMSSQL().c_str());
    //    printf("Неудалось запустить SQLITE в многопоточном режиме, ошибка: %d!\n", err);
    //    logger("Неудалось запустить SQLITE в многопоточном режиме, ошибка: " + std::to_string(err) + "\n", "ERROR");
    //    return 1;
    //}


    // ТЕСТ ЛОГА ЧЕРЕЗ fprintf
    if (openLogFile())
    {
        printf("%s: ", currentDateTimeMSSQL().c_str());
        printf("Неудалось открыть лог файл!\n");
        return 1;
    }

    //Формируем путь до exe с ключем -service для установки и запуска службы
    GetModuleFileNameA(NULL, PathService, sizeof(PathService));
    std::string strPath;
    strPath.append(PathService);
    strPath.append(" -service");
    memcpy(PathService, strPath.c_str(), strPath.size());

    if (strcmp(argv[argc - 1], ("-service")) == 0)
    {
        SERVICE_TABLE_ENTRYA entrytable[2];

        entrytable[0].lpServiceName = (LPSTR)NAME_SERVICE;
        entrytable[0].lpServiceProc = (LPSERVICE_MAIN_FUNCTIONA)ServiceMain;
        entrytable[1].lpServiceName = NULL;
        entrytable[1].lpServiceProc = NULL;

        StartServiceCtrlDispatcherA(entrytable);
    }
    else if (strcmp(argv[argc - 1], ("-install")) == 0)
    {
        if (InstallService() == 0)
        {
            RunService();
        }
    }
    else if (strcmp(argv[argc - 1], ("-remove")) == 0)
    {
        RemoveService();
    }
    else if (argc - 1 == 0)
    {
        //Запуск веб сервера
        runWebServer();
    }
    else
    {
        printf("Error: Incorrect value key! \n");
        printf("\n");
        printf("serverUpdaterHostVNC.exe -параметр\n");
        printf("\n");
        printf("Значение:       Описение:\n");
        printf("--------------------------------\n");
        printf("без параметра   Запуск как консольное приложение\n");
        printf("-install        Установка и запуск службы\n");
        printf("-remove         Удаление службы\n");
        printf("--------------------------------\n");
        printf("Пример:\n");
        printf("serverUpdaterHostVNC.exe -install\n");
    }

    return 0;
}