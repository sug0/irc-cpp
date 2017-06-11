#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include "irc.h"

// uncomment line below to include your own config "config.h"
//#define _IRC_CONFIG_H_
#include "config.def.h"

using namespace std;

static bool quit_irc   = false;
static bool has_esmaga = true;

static void quit_hook(IRCConnection *irc, string rsp)
{
    if (rsp.find(".quit") != string::npos) {
        irc->send_channel("later :^)");
        quit_irc = true;
    }
}

static void notice_hook(IRCConnection *irc, string rsp)
{
    if (rsp.find(".noticeme") != string::npos)
        irc->send_notice("senpai has noticed");
}

static void esmaga_hook(IRCConnection *irc, string rsp)
{
    if (rsp.find(".esmaga") != string::npos) {
        size_t size;
        string reply;
        vector<string> vec;
        ifstream file {"strong.txt"};

        if (file.is_open()) {
            while (getline(file, reply))
                vec.push_back(reply);

            size  = vec.size();
            reply = "";

            for (int i = 1; i <= 5; i++)
                reply += vec[rand() % size] + " ";

            file.close();
            irc->send_channel(reply);
        } else {
            cerr << "couldn't open file 'strong.txt'" << endl;
        }
    }
}

static void toggle_esmaga_hook(IRCConnection *irc, string rsp)
{
    if (rsp.find(".toggle_esmaga") != string::npos) {
        if (has_esmaga) {
            irc->rm_hook(esmaga_hook);
            irc->send_channel("removed '.esmaga' hook");
            has_esmaga = false;
        } else {
            irc->add_hook(esmaga_hook);
            irc->send_channel("added '.esmaga' hook");
            has_esmaga = true;
        }
    }
}

int main()
{
    IRCHook hooks[]   = {pong_hook, quit_hook, notice_hook, esmaga_hook, toggle_esmaga_hook, nullptr};
    IRCConnection irc = {
        server, portno, channel,
        nick, name, pass
    };

    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    srand(ts.tv_nsec);

    irc.add_hooks(hooks);
    irc.auth();

    while (!quit_irc) {
        cout << irc.get_stream();
        irc.exec_hooks();
    }

    return 0;
}
