#include <stdio.h>
#include <stdlib.h>
#include "ext/sqlite3/sqlite3.h"
#include <Windows.h>
#include <string>
#include <iostream>
#include <map>

class SQLite
{
private:
    std::string m_db_name;
    sqlite3 *m_db;
    bool m_isOpen = false;
    std::map<std::string, int> mapping;

    static int callback(void *data, int argc, char **argv, char **azColName){
        int i;
        fprintf(stderr, "%s: ", (const char*)data);

        for(i = 0; i<argc; i++){
            printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
        }

        printf("\n");
        return 0;
    }

    void Exec(const std::string& sql)
    {
        char *zErrMsg = 0;
        char* data = "Callback function called";
        /* Execute SQL statement */
        int rc = sqlite3_exec(m_db, sql.c_str(), callback, (void *)data, &zErrMsg);

        if( rc != SQLITE_OK ) {
            fprintf(stderr, "SQL error: %s\n", zErrMsg);
            sqlite3_free(zErrMsg);
        } else {
            fprintf(stdout, "Operation done successfully\n");
        }
    }

    std::string Create_A_Table()
    {
        std::string sql = "CREATE TABLE";
        std::string buffer, name;

        std::cout << "Enter table name: ";
        std::cin >> name;
        sql += (name + "(");

        std::cout << "Enter column parameters (CAPS ONLY!!!) (type 'end' to finish):" << std::endl;

        while (true)
        {
            std::getline(std::cin, buffer);
            if (buffer != "end")
            {
                sql += (buffer + ",");
            }
            else
            {
                sql.pop_back();
                sql += " );";
                break;
            }
        }

        std::cout << "Table created" << std::endl;
        return sql;
    }

    std::string insert()
    {
        std::string sql = "INSERT INTO ";
        std::string name, headers, values;

        std::cout << "Enter the table name: ";
        std::getline(std::cin, name);

        std::cout << "Enter headers names:" << std::endl;
        std::getline(std::cin, headers);

        std::cout << "Enter values:" << std::endl;
        std::getline(std::cin, values);

        sql += (name + " (" + headers + ") VALUES (" + values + ");");

        std::cout << "Data has been added" << std::endl;
        return sql;
    }

    std::string select()
    {
        std::string sql = "SELECT ";
        std::string headers, name;

        std::cout << "Enter table name: ";
        std::getline(std::cin, name);

        std::cout << "Enter the headers names to select or type 'all' to select all headers:" << std::endl;
        std::getline(std::cin, headers);
        if (headers == "all")
            headers = "*";

        sql += (headers + " FROM " + name);

        std::cout << "Operation cleared successfully" << std::endl;
        return sql;
    }

    std::string update()
    {
        std::string sql = "UPDATE ";
        std::string name, changes, condition;

        std::cout << "Enter table name: ";
        std::getline(std::cin, name);

        std::cout << "Enter what to change:" << std::endl;
        std::getline(std::cin, changes);

        std::cout << "Enter condition for changes:" << std::endl;
        std::getline(std::cin, condition);

        sql += (name + " set " + changes + " where " + condition);

        return sql;
    }

    std::string deletion()
    {
        std::string sql = "DELETE from ";
        std::string name, condition;

        std::cout << "Enter table name: ";
        std::getline(std::cin, name);

        std::cout << "Enter condition for deletion:" << std::endl;
        std::getline(std::cin, condition);

        sql += (name + " where " + condition);

        return sql;
    }
public:

    SQLite(const std::string& db_name) : m_db_name(db_name)
    {
        mapping["add a table"] = 1;
        mapping["insert data"] = 2;
        mapping["select"] = 3;
        mapping["update"] = 4;
        mapping["delete"] = 5;
    }

    ~SQLite()
    {
        if(!m_isOpen)
            sqlite3_close(m_db);
    }

    void Open()
    {
        try
        {
            int rc = sqlite3_open("dt.db", &m_db);

            if( rc ) {
                fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(m_db));
            } else {
                m_isOpen = true;
                fprintf(stderr, "Opened database successfully\n");
            }
        }
        catch (const std::exception& e)
        {
            std::cerr << e.what() << std::endl;
        }
    }



    void Work()
    {
        std::string sql;
        std::string command;
        std::cin.ignore(1000, '\n');
        std::cout << "Enter the command: ";
        std::getline(std::cin, command);

        switch (mapping[command]) {
            case 1:
                Exec(Create_A_Table());
                break;
            case 2:
                Exec(insert());
                break;
            case 3:
                Exec(select());
                break;
            case 4:
                Exec(update());
                break;
            case 5:
                Exec(deletion());
                break;
            default:
                break;
        }
    }
};

int main(int argc, char* argv[]) {

    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);

    std::string db_name;

    std::cout << "Enter the database to open: ";
    std::cin >> db_name;

    SQLite s(db_name);

    s.Open();
    s.Work();

    return 0;
}