#include <string>
#include <stdint.h>

#ifndef _IRC_CONFIG_H_

static const std::string server  = "irc.freenode.net";
static const uint16_t portno     = 6666;
static const std::string channel = "##vaitefoder";
static const std::string nick    = "sugobot";
static const std::string name    = "sugo";
static const std::string pass    = "";

#else

#include "config.h"

#endif
