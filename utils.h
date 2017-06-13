#ifndef _UTILS_H_
#define _UTILS_H_

#include <string>
#include <vector>
#include <unistd.h>
#include "irc.h"

void sleep_ms(useconds_t ms);
size_t find_nth_str(std::string str, std::string sub, int n);
std::string get_sender_nick(std::string sender_host);
void send_by_context(IRCConnection *irc, std::string msg, std::string bot_nick, std::vector<std::string> &toks);

#endif
