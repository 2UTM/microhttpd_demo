#define _CRT_SECURE_NO_WARNINGS

//#include "server_ssl.h"
#include "NLTemplate.h"
#include "http_smal.h"
//#include "WorkConfig.h"
//#include "boost/date_time/posix_time/posix_time.hpp"

#pragma comment(lib,"libmicrohttpd-dll.lib")

//#include "WorkDBMSSQL.h"
#include "Logger.h"

//boost::posix_time::ptime startTime; // время запуска сервера
bool status = true; // статус обработки данных

std::string sPathСatalogWeb; // Путь до рабочего каталога с закрывающем слешом /../../

// Шаблонизатор
std::string templateWeb(std::string pathToTemplate, int notification)
{
    //открываем соединение
    /*SQLHANDLE conn = NULL;
    SQLHANDLE stmt = NULL;
    SQLHANDLE env = NULL;
    if (connectDBMSSQL(conn, stmt, env))
    {
        printf("%s: ", currentDateTimeMSSQL().c_str());
        printf("Ошибка соединения с базой!\n");
        logger("Ошибка соединения с базой!\n", "ERROR");
        return "Ошибка соединения с базой!";
    }*/
    //std::string pathDB = "clients.db";
    //sqlite3* connectDB = 0; // хендлер для соединения к базе
    //if (sqlite3_open(pathDB.c_str(), &connectDB))
    //{
    //    printf("%s: ", currentDateTimeMSSQL().c_str());
    //    printf("Ошибка sqlite3_open\n");
    //    logger("Ошибка sqlite3_open\n!\n", "ERROR");
    //    sqlite3_close(connectDB);
    //    return "Ошибка sqlite3_open!";
    //}
    //
    std::vector<std::vector<std::string>> resultAllDB; // для результата всех клиентов из базы

    // для демонтсрации заполнил тестами
    std::vector<std::string> v = { "test", "test", "test", "test", "test"};
    resultAllDB.push_back(v);
    resultAllDB.push_back(v);
    resultAllDB.push_back(v);
    resultAllDB.push_back(v);
    resultAllDB.push_back(v);

    /*if (selectAllClientsMSSQL(stmt, resultAllDB))
    {
        printf("%s: ", currentDateTimeMSSQL().c_str());
        printf("Ошибка выполнения запроса selectAllClientsMSSQL в базе!\n");
        logger("Ошибка выполнения запроса selectAllClientsMSSQL в базе!\n", "ERROR");
        return "Ошибка выполнения запроса в базе!";
    }
    closeConnectDBMSSQL(conn, stmt, env);*/

    //if (resultAllDB.size() == 0) // если вектор пустой, то базы нет
    //{
    //    printf("%s: ", currentDateTimeMSSQL().c_str());
    //    printf("БАЗА НЕ НАЙДЕНА!\n");
    //    logger("БАЗА НЕ НАЙДЕНА!\n", "ERROR");
    //    return "БАЗА НЕ НАЙДЕНА!";
    //}

    // Удаляем первый элемент - first id
    //resultAllDB.erase(resultAllDB.begin());

    int count = resultAllDB.size();

    NL::Template::LoaderFile loader; // загрузчик
    NL::Template::Template t(loader); // шаблонизатор

    if (pathToTemplate.find("table") != std::string::npos) // если в pathToTemplate есть table, таблицу надо генерить
    {
        t.load(sPathСatalogWeb + "template/table.html"); // загружаем шаблон
        t.block("trTable").repeat(count); // сколько строк в таблице

        // время работы сервера
        std::string upTime;
        getUpTime(upTime);
        t.set("uptime", upTime);

        // Заполняем таблицу
        for (int i = 0; i < count; ++i)
        {
            // Заполняем таблицу
            t.block("trTable")[i].set("id", resultAllDB[i][0]);

            //Переводим имя в ютф 8
            std::string resultNameClient;
            /*if (stringToUtf8MSSQL(resultAllDB[i][1], resultNameClient))
            {
                printf("%s: ", currentDateTimeMSSQL().c_str());
                printf("stringToUtf8 error web-interface table\n");
                logger("Ошибка stringToUtf8 в веб интерфейсе, в таблице\n", "ERROR");
            }*/
            //t.block("trTable")[i].set("name", resultNameClient);
            t.block("trTable")[i].set("time", resultAllDB[i][2]);
            t.block("trTable")[i].set("time", resultAllDB[i][3]);
            t.block("trTable")[i].set("version", resultAllDB[i][4]);
        }

        // Уведомление
        int portWeb = 0;
        //readConfigWebInterface(portWeb);
        std::string strPortWeb = std::to_string(portWeb);
        if (notification == 0)
        {
            t.set("notification", "");
        }
        if (notification == 1)
        {
            std::string notificationCode =
                "<meta http-equiv=\"refresh\" content=\"5; URL = http://localhost:" + strPortWeb + "\">"
                "<div class=\"alert alert-success alert-dismissible fade show\" role=\"alert\">"
                "<strong>Успешно!</strong>"
                "<br>"
                "Операция успешно завершена."
                "<button type=\"button\" class=\"close\" data-dismiss=\"alert\" aria-label=\"Close\">"
                "<span aria-hidden=\"true\">&times;</span>"
                "</button>"
                "</div>";

            // Переводим в utf-8
            std::string notificationCodeUtf8;
            //stringToUtf8(notificationCode, notificationCodeUtf8);

            t.set("notification", notificationCodeUtf8);
        }
        if (notification == 2)
        {
            std::string notificationCode =
                "<meta http-equiv=\"refresh\" content=\"5; URL = http://localhost:" + strPortWeb + "\">"
                "<div class=\"alert alert-warning alert-dismissible fade show\" role=\"alert\">"
                "<strong>Ошибка!</strong>"
                "<br>"
                "Операция завершилась ошибкой, подробности в журнале событий."
                "<button type=\"button\" class=\"close\" data-dismiss=\"alert\" aria-label=\"Close\">"
                "<span aria-hidden=\"true\">&times;</span>"
                "</button>"
                "</div>";

            // Переводим в utf-8
            std::string notificationCodeUtf8;
            //stringToUtf8(notificationCode, notificationCodeUtf8);

            t.set("notification", notificationCodeUtf8);
        }

        // Пишем в переменную таблицу
        std::stringstream myStreamStringTable;
        t.render(myStreamStringTable); // рендеринг шаблона
        std::string myStringTable = myStreamStringTable.str();
        
        t.load(sPathСatalogWeb + "template/index.html"); // загружаем главный шаблон
        t.set("content", myStringTable);

        // Пишем в переменную
        std::stringstream myStreamString;
        t.render(myStreamString); // рендеринг шаблона
        std::string myString = myStreamString.str();
        return myString;
    }

    // если в pathToTemplate есть options, опции надо генерить
    if (pathToTemplate.find("options") != std::string::npos)
    {
        t.load(sPathСatalogWeb + "template/options.html"); // загружаем шаблон
        
        // Читаем конфиг
        std::string portWebInteface, portServer, pathUpdate, address, port, base, user, pass;
        //readConfig(portWebInteface, portServer, pathUpdate, address, port, base, user, pass);

        // Заполняем
        t.set("value_port_web_interface", portWebInteface);
        t.set("value_port_server", portServer);
        t.set("value_path_update", "\"" + pathUpdate + "\"");
        t.set("value_address_db", address);
        t.set("value_port_db", port);
        t.set("value_base_db", base);
        t.set("value_user_db", user);
        t.set("value_pass_db", pass);

        // Пишем в переменную таблицу
        std::stringstream myStreamStringSendMessageClient;
        t.render(myStreamStringSendMessageClient); // рендеринг шаблона
        std::string myStringTableSendMessageClient = myStreamStringSendMessageClient.str();

        t.load(sPathСatalogWeb + "template/index.html"); // загружаем главный шаблон
        t.set("content", myStringTableSendMessageClient);

        // Пишем в переменную
        std::stringstream myStreamString;
        t.render(myStreamString); // рендеринг шаблона
        std::string myString = myStreamString.str();
        return myString;
    }

    // если в pathToTemplate есть event_log, лог надо генерить
    if (pathToTemplate.find("event_log") != std::string::npos)
    {
        t.load(sPathСatalogWeb + "template/event_log.html"); // загружаем шаблон
        t.set("log", "TODO LOGGING"); // TODO сделать передачу лога

        // Пишем в переменную таблицу
        std::stringstream myStreamStringEventLog;
        t.render(myStreamStringEventLog); // рендеринг шаблона
        std::string myStringTableEventLog = myStreamStringEventLog.str();

        t.load(sPathСatalogWeb + "template/index.html"); // загружаем главный шаблон
        t.set("content", myStringTableEventLog);

        // Пишем в переменную
        std::stringstream myStreamString;
        t.render(myStreamString); // рендеринг шаблона
        std::string myString = myStreamString.str();
        return myString;
    }

    t.load(sPathСatalogWeb + "template/index.html"); // загружаем шаблон
    
     // окрываем файл для чтения
    std::string resultFile;
    std::string line;
    std::ifstream file(pathToTemplate);
    if (file.is_open())
    {
        while (std::getline(file, line))
        {
            resultFile += line;
        }
    }
    else
    {
        resultFile = "Недоступный шаблон";
        return resultFile;
    }
    file.close();     // закрываем файл
    t.set("content", resultFile);

    // Пишем в переменную
    std::stringstream myStreamString;
    t.render(myStreamString); // рендеринг шаблона
    std::string myString = myStreamString.str();

    //sqlite3_close(connectDB);
    //closeConnectDBMSSQL(conn, stmt, env);

    return myString;
}

// Получить время из строки, как число
int getTimeStringAsInt(std::string time)
{
    int hour, minute, second, day, month, year;
    if (sscanf(time.c_str(), "%d.%d.%d", &day, &month, &year) == 3)
    {
        int date = year * 10000 + month * 100 + day;
        return date;
    }
    if (sscanf(time.c_str(), "%d:%d:%d %d.%d.%d", &hour, &minute, &second, &day, &month, &year) == 6)
    {
        int date = year * 10000 + month * 100 + day;
        return date;
    }

    return 0;
}

// Получить текущее время, как число
int getCurrentTimeAsInt()
{
    // Получаем текущее время как строку
    time_t t = std::time(nullptr);
    tm tm = *std::localtime(&t);

    std::ostringstream oss;
    oss << std::put_time(&tm, "%H:%M %d-%m-%Y");
    std::string strTime = oss.str();

    // Переводим в число
    int hour, minute, day, month, year;
    if (sscanf(strTime.c_str(), "%d:%d %d-%d-%d", &hour, &minute, &day, &month, &year) == 5)
    {
        int date = year * 10000 + month * 100 + day;
        return date;
    }

    return 0;
}

// получаем время работы сервера
int getUpTime(std::string& uptime)
{
    // Получаем дельту между датой запуска и текущей датой
    //boost::posix_time::ptime currentTime(boost::posix_time::second_clock::local_time());
    //boost::posix_time::time_duration timeDuration = currentTime - startTime;
    //std::string tempUptime = boost::posix_time::to_simple_string(timeDuration);

    //int hours = atoi(tempUptime.substr(0, tempUptime.find(":")).c_str()); // получаем часы
    //tempUptime.erase(0, tempUptime.find(":")); // отрезаем часы
    //int day = hours / 24; // считаем дни
    //hours = hours - day * 24; // отнимаем часы по кол-ву дней
    //std::string stringHours = std::to_string(hours);
    //if (stringHours.size() < 2) // приводим часы к виду 00
    //{
    //    stringHours.insert(0, "0");
    //}
    //std::string stringDay = std::to_string(day);
    //if (stringDay.size() < 2) // приводим дни к виду 00
    //{
    //    stringDay.insert(0, "0");
    //}
    //uptime = stringDay + ":" + stringHours + tempUptime;

    return 0;
}

//Вызывается на каждое поле из POST запроса, пока POST не обработан полностью отвечать нельзя, из документации 
int UserIterator(void* cls, enum MHD_ValueKind kind, const char* key, const char* filename, const char* content_type, const char* transfer_encoding, const char* data, uint64_t off, size_t size)
{
    return MHD_YES;
}

// Главная функция сервера
int Server(void* cls, struct MHD_Connection* connection, const char* url, const char* method, const char* version, const char* upload_data, size_t* upload_data_size, void** ptr) {
    static int dummy;
    struct MHD_Response* response;
    int ret;

    std::cout << "url: " << url << std::endl;
    
    // Обработка POST
    if (0 == strcmp(method, "POST"))
    {
        struct MHD_PostProcessor* pp = (MHD_PostProcessor*)*ptr;

        // В первый раз действительны только заголовки, не отвечайте на первой итерации
        // И создаем пост процессор
        if (pp == NULL)
        {
            pp = MHD_create_post_processor(connection, 1024, (MHD_PostDataIterator)UserIterator, 0);
            *ptr = pp;
            return MHD_YES;
        }

        // Обработка данных
        if (*upload_data_size != 0)
        {
            MHD_post_process(pp, upload_data, *upload_data_size);
            std::string data(upload_data);

            // Сохранение настроек
            if (data.find("port_web_interface=") != std::string::npos)
            {
                //// парсим запрос
                //// Разбиваем по &
                //std::vector<std::string> v;
                //boost::split(v, data, boost::is_any_of("&"));
                //// и парсим
                //std::string portWebInterface, portServer, pathUpdate, address, port, base, user, pass;
                //for (std::string elem : v)
                //{
                //    if (elem.find("port_web_interface=") != std::string::npos)
                //    {
                //        portWebInterface = elem.substr(elem.find("port_web_interface=") + 19);
                //        continue;
                //    }
                //    if (elem.find("port_update=") != std::string::npos)
                //    {
                //        portServer = elem.substr(elem.find("port_update=") + 12);
                //        continue;
                //    }
                //    if (elem.find("path_update=") != std::string::npos)
                //    {
                //        pathUpdate = elem.substr(elem.find("path_update=") + 12);
                //        continue;
                //    }
                //    if (elem.find("address_db=") != std::string::npos)
                //    {
                //        address = elem.substr(elem.find("address_db=") + 11);
                //        continue;
                //    }
                //    if (elem.find("port_db=") != std::string::npos)
                //    {
                //        port = elem.substr(elem.find("port_db=") + 8);
                //        continue;
                //    }
                //    if (elem.find("base_db=") != std::string::npos)
                //    {
                //        base = elem.substr(elem.find("base_db=") + 8);
                //        continue;
                //    }
                //    if (elem.find("user_db=") != std::string::npos)
                //    {
                //        user = elem.substr(elem.find("user_db=") + 8);
                //        continue;
                //    }
                //    if (elem.find("password=") != std::string::npos)
                //    {
                //        pass = elem.substr(elem.find("password=") + 9);
                //        continue;
                //    }
                //}

                //// Проверяем порты
                //if (atoi(portServer.c_str()) < 0 || atoi(portServer.c_str()) > 65535)
                //{
                //    printf("%s: ", currentDateTimeMSSQL().c_str());
                //    printf("portServer invalid port");
                //    logger("Неверный порт сервера!\n", "ERROR");
                //    status = false;
                //    *upload_data_size = 0;
                //    return MHD_YES;
                //}
                //if (atoi(portWebInterface.c_str()) < 0 || atoi(portWebInterface.c_str()) > 65535)
                //{
                //    printf("%s: ", currentDateTimeMSSQL().c_str());
                //    printf("portWebInterface invalid port");
                //    logger("Неверный порт веб интерфейса!\n", "ERROR");
                //    status = false;
                //    *upload_data_size = 0;
                //    return MHD_YES;
                //}
                //if (atoi(port.c_str()) < 0 || atoi(port.c_str()) > 65535)
                //{
                //    printf("%s: ", currentDateTimeMSSQL().c_str());
                //    printf("MSSQL invalid port");
                //    logger("Неверный порт для соединения с базой данных!\n", "ERROR");
                //    status = false;
                //    *upload_data_size = 0;
                //    return MHD_YES;
                //}

                //// Декодируем пути
                //std::string resultPathUpdate;
                //if (URLDecode(pathUpdate, resultPathUpdate))
                //{
                //    printf("%s: ", currentDateTimeMSSQL().c_str());
                //    printf("URLDecode error web-interface options");
                //    logger("Ошибка URLDecode в веб интерфейсе!\n", "ERROR");
                //    status = false;
                //    *upload_data_size = 0;
                //    return MHD_YES;
                //}
                //// Декодируем имя базы
                //std::string resultBase;
                //if (URLDecode(pathUpdate, resultPathUpdate))
                //{
                //    printf("%s: ", currentDateTimeMSSQL().c_str());
                //    printf("URLDecode error web-interface options");
                //    logger("Ошибка URLDecode в веб интерфейсе!\n", "ERROR");
                //    status = false;
                //    *upload_data_size = 0;
                //    return MHD_YES;
                //}
                //// Декодируем пароль
                //std::string resultPass;
                //if (URLDecode(pathUpdate, resultPathUpdate))
                //{
                //    printf("%s: ", currentDateTimeMSSQL().c_str());
                //    printf("URLDecode error web-interface options");
                //    logger("Ошибка URLDecode в веб интерфейсе!\n", "ERROR");
                //    status = false;
                //    *upload_data_size = 0;
                //    return MHD_YES;
                //}

                // Сохраняем опции
                //saveOption(portWebInterface, portServer, resultPathUpdate, address, port, resultBase, user, resultPass);
            }

            *upload_data_size = 0;
            return MHD_YES;
        }
        else
        {
            // Открываем главную страницу
            if (status == false)
            {
                std::string str = templateWeb(sPathСatalogWeb + "template/table.html", 2);
                int len = str.length() + 1;
                char* page = new char[len];
                strcpy(page, str.c_str());

                response = MHD_create_response_from_buffer(strlen(page), (void*)page, MHD_RESPMEM_MUST_FREE); // передаём буфер с контентом страницы

                MHD_add_response_header(response, "Content-Type", "text/html");

                ret = MHD_queue_response(connection, MHD_HTTP_NOT_FOUND, response);
                MHD_destroy_response(response);
                MHD_destroy_post_processor(pp);
                return ret;
            }
            else
            {
                std::string str = templateWeb(sPathСatalogWeb + "template/table.html", 1);
                int len = str.length() + 1;
                char* page = new char[len];
                strcpy(page, str.c_str());

                response = MHD_create_response_from_buffer(strlen(page), (void*)page, MHD_RESPMEM_MUST_FREE); // передаём буфер с контентом страницы

                MHD_add_response_header(response, "Content-Type", "text/html");

                ret = MHD_queue_response(connection, MHD_HTTP_NOT_FOUND, response);
                MHD_destroy_response(response);
                MHD_destroy_post_processor(pp);
                return ret;
            }
        }
    }

    // Обработка GET
    if (0 == strcmp(method, "GET"))
    {
        // В первый раз действительны только заголовки, не отвечайте на первой итерации
        if (&dummy != *ptr) 
        {
            *ptr = &dummy;
            return MHD_YES;
        }

        // Обработка данных
        if (0 != *upload_data_size)
            return MHD_NO;

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Страница настройки
        if (strcmp(url, "/options") == 0) // если url /options
        {
            std::string str = templateWeb(sPathСatalogWeb + "template/options.html", 0);
            int len = str.length() + 1;
            char* page = new char[len];
            strcpy(page, str.c_str());

            response = MHD_create_response_from_buffer(strlen(page), (void*)page, MHD_RESPMEM_MUST_FREE); // передаём буфер с контентом страницы

            *ptr = NULL; // очищаем указатель контекста

            // Добавляем заголовки
            MHD_add_response_header(response, "Content-Type", "text/html");

            ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
            MHD_destroy_response(response);
            return ret;
        }
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Страница о программе
        if (strcmp(url, "/about_programm") == 0) // если url /about_programm
        {
            std::string str = templateWeb(sPathСatalogWeb + "template/about_programm.html", 0);
            int len = str.length() + 1;
            char* page = new char[len];
            strcpy(page, str.c_str());

            response = MHD_create_response_from_buffer(strlen(page), (void*)page, MHD_RESPMEM_MUST_FREE); // передаём буфер с контентом страницы

            *ptr = NULL; // очищаем указатель контекста

            // Добавляем заголовки
            MHD_add_response_header(response, "Content-Type", "text/html");

            ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
            MHD_destroy_response(response);
            return ret;
        }
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Страница журнал событий
        if (strcmp(url, "/event_log") == 0) // если url /event_log
        {
            std::string str = templateWeb(sPathСatalogWeb + "template/event_log.html", 0);
            int len = str.length() + 1;
            char* page = new char[len];
            strcpy(page, str.c_str());

            response = MHD_create_response_from_buffer(strlen(page), (void*)page, MHD_RESPMEM_MUST_FREE); // передаём буфер с контентом страницы

            *ptr = NULL; // очищаем указатель контекста

            // Добавляем заголовки
            MHD_add_response_header(response, "Content-Type", "text/html");

            ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
            MHD_destroy_response(response);
            return ret;
        }
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Главная страница
        std::string str = templateWeb(sPathСatalogWeb + "template/table.html", 0);
        int len = str.length() + 1;
        char* page = new char[len];
        strcpy(page, str.c_str());

        response = MHD_create_response_from_buffer(strlen(page), (void*)page, MHD_RESPMEM_MUST_FREE); // передаём буфер с контентом страницы

        *ptr = NULL; // очищаем указатель контекста

        // Добавляем заголовки
        MHD_add_response_header(response, "Content-Type", "text/html");

        ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
        MHD_destroy_response(response);
        return ret;
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    }

    return MHD_NO;
}

// Запуск веб сервера
int runWebServer()
{
    //// Если время запуска not_a_date_time, запрашиваем
    //if (startTime == boost::posix_time::not_a_date_time)
    //{
    //    startTime = boost::posix_time::second_clock::local_time();
    //}

    printf("%s: ", currentDateTimeMSSQL().c_str());
    printf("Веб сервер запущен...\n\n");
    logger("Веб сервер запущен\n", "INFO");

    // Старт веб сервера
    int port = 44444;
    //readConfigWebInterface(port);
    struct MHD_Daemon* dem = MHD_start_daemon(MHD_USE_THREAD_PER_CONNECTION, port, NULL, NULL, (MHD_AccessHandlerCallback)&Server, NULL, MHD_OPTION_END);

    if (dem == NULL)
    {
        printf("%s: ", currentDateTimeMSSQL().c_str());
        printf("Ошибка запуска веб сервера\n");
        logger("Ошибка запуска веб сервера\n", "ERROR");
    }

    (void)getc(stdin);
    MHD_stop_daemon(dem);

    return 0;
}