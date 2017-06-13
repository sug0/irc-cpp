#include <string>
#include <stdint.h>

#ifndef _IRC_CONFIG_H_

static const std::string server     = "irc.freenode.net";
#ifdef _USE_SSL_
static const uint16_t portno        = 6697;
#else
static const uint16_t portno        = 6667;
#endif
static const std::string channel    = "##vaitefoder";
static const std::string nick       = "sugobot";
static const std::string name       = "sugo";
static const std::string pass       = "";
static const std::string admin_host = "";

#else

#include "config.h"

#endif
