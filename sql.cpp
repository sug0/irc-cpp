#include "sql.h"

SQLiteDB::SQLiteDB(std::string path)
{
    err_msg = nullptr;
    rc      = sqlite3_open(path.c_str(), &db);

    if (rc != SQLITE_OK) {
        std::string err {sqlite3_errmsg(db)};
        sqlite3_close(db);
        throw std::runtime_error(err);
    }
}

SQLiteDB::~SQLiteDB()
{
    sqlite3_free(err_msg);        
    sqlite3_close(db);
}

void SQLiteDB::exec(std::string sql_request, SQLiteCallback func, void *param)
{
    rc = sqlite3_exec(db, sql_request.c_str(), func, param, &err_msg);

    if (rc != SQLITE_OK) {
        std::string err {err_msg};
        sqlite3_free(err_msg);        
        sqlite3_close(db);
        throw std::runtime_error(err);
    }
}
