#ifndef _LFM_H_
#define _LFM_H_

#include <iostream>
#include <cstring>
#include <pugixml.hpp>
#include "tcp.h"
#include "config.def.h"

static const std::string lfm_url = "ws.audioscrobbler.com";
static const std::string lfm_req = "/2.0/?method=user.getrecenttracks&user=%s&api_key=%s";

std::string lfm_get_np(std::string lfm_user);

#endif
