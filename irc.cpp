#include "irc.h"
#include "utils.h"

#define SLEEP  650

static const std::string ping = "PING", pong = "PONG";

IRCConnection::IRCConnection(bool use_ssl, std::string server, uint16_t portno, std::string channel, std::string nick, std::string name, std::string pass)
{
    this->hook_list = new std::list<IRCHook>();
    this->cli       = new TCPClient(use_ssl, server, portno);
    this->nick      = nick;
    this->name      = name;
    this->pass      = pass;
    this->channel   = channel;
}

IRCConnection::~IRCConnection()
{
    log_off();
    delete hook_list;
    delete cli;
}

void IRCConnection::log_off()
{
    send_raw("QUIT");
}

void IRCConnection::log_off(std::string msg)
{
    send_raw("QUIT " + msg);
}

void IRCConnection::add_hook(IRCHook hook)
{
    hook_list->push_back(hook);
}

void IRCConnection::add_hooks(IRCHook *hooks)
{
    for (int i = 0; hooks[i] != nullptr; i++)
        hook_list->push_back(hooks[i]);
}

void IRCConnection::rm_hook(IRCHook hook)
{
    hook_list->remove(hook);
}

void IRCConnection::auth()
{
    send_raw("NICK " + nick);
    sleep_ms(SLEEP);
    send_raw("USER " + nick + " 8 * " + name);
    sleep_ms(SLEEP);
    if (pass != "") {
        send_raw("PASS " + pass);
        sleep_ms(SLEEP);
    }
    join(channel);
}

std::string IRCConnection::get_stream()
{
    rsp = cli->receive();
    return rsp;
}

void IRCConnection::exec_hooks()
{
    for (auto h = hook_list->begin(); h != hook_list->end(); h++)
        (*h)(this, rsp);
}

void IRCConnection::send_raw(std::string request)
{
    cli->send(request + "\r\n");
}

void IRCConnection::join(std::string channel)
{
    send_raw("JOIN " + channel);
}

void IRCConnection::part(std::string channel)
{
    send_raw("PART " + channel);
}

void IRCConnection::send_privmsg(std::string target, std::string msg)
{
    send_raw("PRIVMSG " + target + " :" + msg);
}

void IRCConnection::send_channel(std::string msg)
{
    send_privmsg(channel, msg);
}

void IRCConnection::send_notice(std::string msg)
{
    send_raw("NOTICE " + channel + " :" + msg);
}

void IRCConnection::set_topic(std::string msg)
{
    send_raw("TOPIC " + channel + " :" + msg);
}

void pong_hook(IRCConnection *irc, std::string &rsp)
{
    if (rsp.compare(0, ping.length(), ping, 0, ping.length()) == 0)
        irc->send_raw(pong + rsp.substr(ping.length()));
}
