#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include "irc.h"
#include "utils.h"

// uncomment line below to include your own config "config.h"
//#define _IRC_CONFIG_H_
#include "config.def.h"

using namespace std;

static vector<string> toks, esmaga_quotes;
static bool bot_quit = false;

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

int main()
{
    IRCHook hooks[] = {
        core_hooks,
        notice_hook,
        nullptr
    };

    IRCConnection irc = {
        server, portno, channel,
        nick, name, pass
    };

    irc.add_hooks(hooks);

    string quote;
    ifstream file {"strong.txt"};

    if (file.is_open()) {
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
        cout << irc.get_stream();
        irc.exec_hooks();

        toks.clear();
    }
}
