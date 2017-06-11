#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include "irc.h"

// uncomment line below to include your own config "config.h"
//#define _IRC_CONFIG_H_
#include "config.def.h"

using namespace std;

static string msg;
static bool quit_irc   = false;
static bool has_esmaga = true;

static size_t find_nth_str(string str, string sub, int n)
{
    size_t pos = -1;

    for (int i = 1; i <= n; i++) {
        if ((pos = str.find(sub, pos + 1)) == string::npos)
            return string::npos;
    }

    return pos;
}

static void quit_hook(IRCConnection *irc, string &rsp)
{
    if (msg == ".quit") {
        irc->send_channel("later :^)");
        quit_irc = true;
    }
}

static void notice_hook(IRCConnection *irc, string &rsp)
{
    if (msg == ".noticeme")
        irc->send_notice("senpai has noticed");
}

static void esmaga_hook(IRCConnection *irc, string &rsp)
{
    if (msg == ".esmaga") {
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

static void toggle_esmaga_hook(IRCConnection *irc, string &rsp)
{
    if (msg == ".toggle_esmaga") {
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

void topic_hook(IRCConnection *irc, string &rsp)
{
    if (msg == ".topic")
        irc->set_topic("shieeett");
}

void parse_msg_hook(IRCConnection *irc, string &rsp)
{
    if (rsp.find("PRIVMSG") != string::npos) {
        size_t a = find_nth_str(rsp, ":", 2) + 1UL,
               b = rsp.find("\n") - 1UL;

        msg = rsp.substr(a, b - a);
    }
}

int main()
{
    IRCHook hooks[] = {
        pong_hook,
        quit_hook,
        notice_hook,
        esmaga_hook,
        toggle_esmaga_hook,
        topic_hook,
        parse_msg_hook,
        nullptr
    };

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
        msg = "";
    }

    return 0;
}
