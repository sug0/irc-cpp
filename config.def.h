#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <string>
#include <stdint.h>

#ifndef _IRC_CONFIG_H_

#ifdef _USE_SSL_
    #define USE_SSL  true
#else
    #define USE_SSL  false
#endif

static const std::string server     = "irc.freenode.net";
#if USE_SSL == true
    static const uint16_t portno    = 6697;
#else
    static const uint16_t portno    = 6667;
#endif
static const std::string channel    = "##vaitefoder";
static const std::string nick       = "sugobot";
static const std::string name       = "sugo";
static const std::string pass       = "";
static const std::string admin_host = "";
static const std::string lfm_key    = "";

#else

#include "config.h"

#endif

#endif
