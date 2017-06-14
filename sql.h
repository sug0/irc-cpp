#ifndef _LFM_DB_H_
#define _LFM_DB_H_

#include <string>
#include <stdexcept>
#include <sqlite3.h>

// callback
extern "C" typedef int (*SQLiteCallback)(void *param, int argc, char **argv, char **colname);

class SQLiteDB {
    int rc;
    sqlite3 *db;
    char *err_msg;

public:
    SQLiteDB(std::string path);
    ~SQLiteDB();
    void exec(std::string sql_request, SQLiteCallback func, void *param);
};

#endif
