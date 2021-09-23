#include <chrono>
#include <cstdio>
#include <signal.h>

#include "alStarocksServer.h"

#define SERVER_CERT_FILE "./cert.pem"
#define SERVER_PRIVATE_KEY_FILE "./key.pem"

#define LOG_TAG      "alStarocksServer.cc"

using namespace httplib;

stardb_handle_t *starocksdb = NULL;
// SAPIInfo StarApiInfo[] = {
//     {"/starocks/info", {"GET"}},
//     {"/starocks/create/db", {"POST"}},
//     {"/starocks/put", {"POST"}},
//     {"/starocks/get", {"GET"}},
//     {"/starocks/person", {"POST"}},
//     {"/starocks/persons/info", {"GET"}},
//     {"/starocks/persons/count", {"GET"}},
//     {"/starocks/face/snaps", {"GET", "POST"}},
//     {"/starocks/temp/data", {"GET","POST", "PUT"}},
// };

std::string dump_headers(const Headers &headers) {
  std::string s;
  char buf[BUFSIZ];

  for (auto it = headers.begin(); it != headers.end(); ++it) {
    const auto &x = *it;
    snprintf(buf, sizeof(buf), "%s: %s\n", x.first.c_str(), x.second.c_str());
    s += buf;
  }

  return s;
}

std::string log(const Request &req, const Response &res) {
  std::string s;
  char buf[BUFSIZ];

  s += "================================\n";

  snprintf(buf, sizeof(buf), "%s %s %s", req.method.c_str(),
           req.version.c_str(), req.path.c_str());
  s += buf;

  std::string query;
  for (auto it = req.params.begin(); it != req.params.end(); ++it) {
    const auto &x = *it;
    snprintf(buf, sizeof(buf), "%c%s=%s",
             (it == req.params.begin()) ? '?' : '&', x.first.c_str(),
             x.second.c_str());
    query += buf;
  }
  snprintf(buf, sizeof(buf), "%s\n", query.c_str());
  s += buf;

  s += dump_headers(req.headers);

  s += "--------------------------------\n";

  snprintf(buf, sizeof(buf), "%d %s\n", res.status, res.version.c_str());
  s += buf;
  s += dump_headers(res.headers);
  s += "\n";

  if (!res.body.empty()) { s += res.body; }

  s += "\n";

  return s;
}

void exit_hander(int sig)
{
  if(sig == SIGINT){
    log_i("Received Ctrl^C signal, exit...");
    if(starocksdb != NULL){
      stardb_close_database(starocksdb);
    }
    exit(0);
  }
}

int main(void) {
  elogInit();
  signal(SIGINT, exit_hander);

#ifdef CPPHTTPLIB_OPENSSL_SUPPORT
  SSLServer svr(SERVER_CERT_FILE, SERVER_PRIVATE_KEY_FILE);
#else
  Server svr;
#endif  
  if (!svr.is_valid()) {
    log_e("server has an error...");
    return -1;
  }

  char *dberr = NULL;
  starocksdb =  stardb_open_database(STAROCKS_DB_PATH, &dberr);
  if( !starocksdb || dberr != NULL){
      log_e("Open database failed, path: %s, reason: %s", STAROCKS_DB_PATH, dberr);
      if(dberr != NULL) free(dberr);
      svr.stop();
      return -1;
  }

  svr.Get("/", [=](const Request & /*req*/, Response &res) {
    res.set_redirect("/hi");
  });

  svr.Get("/hi", [](const Request & /*req*/, Response &res) {
    log_i("test hello");
    int starocks_api_person(const char *body);
    res.set_content("{\"code\":1, \"msg\":\"success\"}", "application/json");
  });

  svr.Post("/starocks/person", [&](const auto& req, auto& res) {
    starocks_api_person(starocksdb, req, res);
  });

  svr.Get("/starocks/persons/info", [](const Request &req, Response &res) {
    starocks_api_persons_info(starocksdb, req, res);
  });

  svr.Post("/starocks/face/snaps", [&](const auto& req, auto& res) {
    starocks_api_face_snaps(starocksdb, req, res);
  });

  svr.Get("/starocks/face/snaps", [](const Request &req, Response &res) {
    starocks_api_face_snaps(starocksdb, req, res);
  });

  svr.Post("/starocks/temp/data", [&](const auto& req, auto& res) {
    starocks_api_temp_data(starocksdb, req, res);
  });

  svr.Get("/starocks/temp/data", [](const Request &req, Response &res) {
    starocks_api_temp_data(starocksdb, req, res);
  });

  svr.Put("/starocks/temp/data", [](const Request &req, Response &res) {
    starocks_api_temp_data(starocksdb, req, res);
  });

  svr.Get("/dump", [](const Request &req, Response &res) {
    res.set_content(dump_headers(req.headers), "text/plain");
  });

  svr.Get("/stop",
          [&](const Request & /*req*/, Response & /*res*/) { svr.stop(); });

  svr.set_error_handler([](const Request & /*req*/, Response &res) {
    const char *fmt = "{\"code\":0, \"msg\":\"%s\"}";
    char buf[BUFSIZ];
    snprintf(buf, sizeof(buf), fmt, detail::status_message(res.status));
    res.set_content(buf, "application/json");
  });

  svr.set_logger([](const Request &req, const Response &res) {
    log_d("\n%s", log(req, res).c_str());
  });

  svr.listen("localhost", 8777);

  log_i("Http server stoped, exit...");
  stardb_close_database(starocksdb);

  return 0;
}
