#ifndef _IRC_CONN_H_
#define _IRC_CONN_H_

#include <list>
#include <string>
#include "tcp.h"

typedef void (*IRCHook)(class IRCConnection *, std::string);

class IRCConnection {
    std::string rsp, nick, name, pass, channel;
    std::list<IRCHook> *hook_list;
    TCPClient *cli;

public:
    IRCConnection(std::string server, uint16_t portno, std::string channel, std::string nick, std::string name, std::string pass);
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
    void send_channel(std::string msg);
    void send_notice(std::string msg);
    void set_topic(std::string msg);
};

void pong_hook(IRCConnection *irc, std::string rsp);

#endif
