#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "irc.h"
#include "utils.h"
#include "lfm.h"
#include "sql.h"

#include "config.def.h"

using namespace std;

extern bool __lfm_error;

static SQLiteDB *sqlite_db;
static string db_lfm_user;
static vector<string> toks, esmaga_quotes;
static bool bot_quit = false;

static int lfm_db_user_get_callback(void *param, int argc, char **argv, char **colname)
{
    if (argc != 0)
        db_lfm_user = {argv[0]};

    return 0;
}

static void parse_request_hook(IRCConnection *irc, string &rsp)
{
    if (rsp.at(0) == ':') {
        int a  = 1,
            b  = rsp.find("\n") - 1, 
            p1 = 0,
            p2 = -1;

        rsp = rsp.substr(a, b - a);

        while ((p2 = rsp.find(" ", p2 + 1)) != string::npos) {
            toks.push_back(rsp.substr(p1, p2 - p1));
            p1 = p2 + 1;
        }

        toks.push_back(rsp.substr(p1));

        if (toks.size() >= 4)
            toks[3] = toks[3].substr(1);
    }
}

static void quit_hook(IRCConnection *irc, string &rsp)
{
    if (toks[1] == "PRIVMSG" && toks[3] == ".quit") {
        if (toks[0] == admin_host || admin_host == "") {
            irc->send_channel("later bois :-)");
            bot_quit = true;
        } else {
            send_by_context(irc, "fuck off", nick, toks);
        }
    }
}

static void core_hooks(IRCConnection *irc, string &rsp)
{
    pong_hook(irc, rsp);
    parse_request_hook(irc, rsp);
    quit_hook(irc, rsp);
}

static void notice_hook(IRCConnection *irc, string &rsp)
{
    if (toks[1] == "PRIVMSG" && toks[3] == ".noticeme")
        send_by_context(irc, "senpai has noticed", nick, toks);
}

static void esmaga_hook(IRCConnection *irc, string &rsp)
{
    if (toks[1] == "PRIVMSG" && toks[3] == ".esmaga") {
        string reply;

        for (int i = 1; i <= 5; i++)
            reply += esmaga_quotes[rand() % esmaga_quotes.size()] + " ";

        send_by_context(irc, reply, nick, toks);
    }
}

static void lfm_hook(IRCConnection *irc, string &rsp)
{
    if (toks[1] == "PRIVMSG" && toks[3] == ".np") {
        if (toks.size() < 5) {
            sqlite_db->exec("SELECT lfm_username FROM"
                            "  (SELECT * FROM irc_users WHERE irc_nick='" + get_sender_nick(toks[0]) + "');",
                            lfm_db_user_get_callback, nullptr);

            if (db_lfm_user != "") {
                send_by_context(irc, lfm_get_np(db_lfm_user), nick, toks);
                db_lfm_user = "";
            } else {
                send_by_context(irc, "usage: .np <lfm_user>", nick, toks);
            }
        } else {
            send_by_context(irc, lfm_get_np(toks[4]), nick, toks);

            if (!__lfm_error) {
                sqlite_db->exec("UPDATE irc_users SET lfm_username='" + toks[4] + "'"
                                "  WHERE irc_nick='" + get_sender_nick(toks[0]) + "';"
                                ""
                                "INSERT INTO irc_users"
                                "  SELECT '" + get_sender_nick(toks[0]) + "', '" + toks[4] + "'"
                                "    WHERE (SELECT Changes()=0);",
                                nullptr, nullptr);
            }
        }
    }
}

int main(const int argc, const char *argv[])
{
    int fd;
    bool use_fifo = false;
    string fifo   = "/tmp/irc_fifo";

    if (argc >= 2) {
        if (strcmp(argv[1], "-p") == 0) {
            use_fifo = true;

            if (mkfifo(fifo.c_str(), 0600) < 0)
                throw std::runtime_error("error creating named pipe");

            if ((fd = open(fifo.c_str(), O_WRONLY)) < 0)
                throw std::runtime_error("error opening named pipe");
        }
    }

    IRCHook hooks[] = {
        core_hooks,
        notice_hook,
        lfm_hook,
        nullptr
    };

    IRCConnection irc = {
        USE_SSL,
        server, portno, channel,
        nick, name, pass
    };

    irc.add_hooks(hooks);

    SQLiteDB db {"lfm_users.db"};
    sqlite_db = &db;

    sqlite_db->exec("CREATE TABLE IF NOT EXISTS"
                    "  irc_users(irc_nick TEXT, lfm_username TEXT);",
                    nullptr, nullptr);

    ifstream file {"strong.txt"};

    if (file.is_open()) {
        string quote;
        struct timespec ts;
        timespec_get(&ts, TIME_UTC);
        srand(ts.tv_nsec);

        while (getline(file, quote))
            esmaga_quotes.push_back(quote);

        file.close();
        irc.add_hook(esmaga_hook);
    } else {
        cerr << "couldn't open file 'strong.txt'" << endl;
    }

    irc.auth();

    while (!bot_quit) {
        try {
            if (use_fifo) {
                string in = irc.get_stream();
                write(fd, in.c_str(), in.length());
            } else {
                cout << irc.get_stream();
            }
        } catch (const runtime_error &e) {
            string err = "couldn't communicate with server";

            if (use_fifo)
                write(fd, err.c_str(), err.length());
            else
                cerr << err << endl;

            break;
        }

        irc.exec_hooks();

        toks.clear();
    }

    if (use_fifo) {
        close(fd);
        unlink(fifo.c_str());
    }

    return 0;
}
