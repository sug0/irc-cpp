#ifndef _IRC_CONN_H_
#define _IRC_CONN_H_

#include <list>
#include <string>
#include "tcp.h"

#define IRC_WHITE        "0"
#define IRC_BLACK        "1"
#define IRC_BLUE_NAVY    "2"
#define IRC_GREEN        "3"
#define IRC_RED          "4"
#define IRC_BROWN        "5"
#define IRC_PURPLE       "6"
#define IRC_ORANGE       "7"
#define IRC_YELLOW       "8"
#define IRC_LIGHT_GREEN  "9"
#define IRC_TEAL         "10"
#define IRC_LIGHT_CYAN   "11"
#define IRC_LIGHT_BLUE   "12"
#define IRC_PINK         "13"
#define IRC_GRAY         "14"
#define IRC_LIGHT_GRAY   "15"

#define IRC_BOLD         "\x02"
#define IRC_COLORED      "\x03"
#define IRC_ITALIC       "\x1D"
#define IRC_UNDERLINED   "\x1F"
#define IRC_VIDEO        "\x16"
#define IRC_RESET        "\x0F"

#define IRC_COLOR_FMT(STR, FG)         IRC_COLORED FG        STR IRC_COLORED
#define IRC_COLOR_FMT_BG(STR, FG, BG)  IRC_COLORED FG "," BG STR IRC_COLORED

#define IRC_BOLD_FMT(STR)        IRC_BOLD       STR IRC_RESET
#define IRC_ITALIC_FMT(STR)      IRC_ITALIC     STR IRC_RESET
#define IRC_UNDERLINED_FMT(STR)  IRC_UNDERLINED STR IRC_RESET
#define IRC_VIDEO_FMT(STR)       IRC_VIDEO      STR IRC_RESET

typedef void (*IRCHook)(class IRCConnection *, std::string &);

class IRCConnection {
    std::string rsp, nick, name, pass, channel;
    std::list<IRCHook> *hook_list;
    TCPClient *cli;

public:
    IRCConnection(bool use_ssl, std::string server, uint16_t portno, std::string channel, std::string nick, std::string name, std::string pass);
    ~IRCConnection();
    void log_off();
    void log_off(std::string msg);
    void add_hook(IRCHook hook);
    void add_hooks(IRCHook *hooks);
    void rm_hook(IRCHook hook);
    void auth();
    std::string get_stream();
    void exec_hooks();
    void send_raw(std::string request);
    void join(std::string channel);
    void part(std::string channel);
    void send_privmsg(std::string target, std::string msg);
    void send_channel(std::string msg);
    void send_notice(std::string msg);
    void set_topic(std::string msg);
};

void pong_hook(IRCConnection *irc, std::string &rsp);

#endif
