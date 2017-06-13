#include "utils.h"

void sleep_ms(useconds_t ms)
{
    usleep(ms * 1000);
}

size_t find_nth_str(std::string str, std::string sub, int n)
{
    size_t pos = -1;

    for (int i = 1; i <= n; i++) {
        if ((pos = str.find(sub, pos + 1)) == std::string::npos)
            return std::string::npos;
    }

    return pos;
}

std::string get_sender_nick(std::string sender_host)
{
    return sender_host.substr(0, sender_host.find("!"));
}

void send_by_context(IRCConnection *irc, std::string msg, std::string bot_nick, std::vector<std::string> &toks)
{
    if (toks[2] == bot_nick)
        irc->send_privmsg(get_sender_nick(toks[0]), msg);
    else
        irc->send_channel(get_sender_nick(toks[0]) + ": " + msg);
}
