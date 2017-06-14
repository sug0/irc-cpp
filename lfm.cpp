#include "lfm.h"

bool __lfm_error = false;

std::string get_recent_tracks(std::string user)
{
    char req[lfm_req.length() + user.length() + lfm_key.length() + 1];
    sprintf(req, lfm_req.c_str(), user.c_str(), lfm_key.c_str());

    std::string response, r;
    TCPClient lfm {false, std::string {lfm_url}, 80};

    lfm.send("GET " + std::string {req} + " HTTP/1.0\n\n");

    while ((r = lfm.receive()) != "")
        response += r;

    return response;
}

std::string http_parse_body(std::string raw_http)
{
    return raw_http.substr(raw_http.find("\n\r") + 3UL);
}

std::string play_status(pugi::xml_node n)
{
    return (!n) ? "is now playing" : "last played";
}

std::string lfm_get_np(std::string lfm_user)
{
    std::string http_resp = http_parse_body(get_recent_tracks(lfm_user));

    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer(http_resp.c_str(), http_resp.length());

    if (!result)
        throw std::runtime_error("failed to parse xml");

    pugi::xml_node track = doc.child("lfm").child("recenttracks").child("track");

    if (!track) {
        __lfm_error = true;
        return "user '" + lfm_user + "' isn't available";
    }

    __lfm_error = false;

    return lfm_user + " " + play_status(track.child("date"))
         + " '" + track.child_value("artist") + " - " + track.child_value("name") + "' "
         + "from the album '" + track.child_value("album") + "'";
}
