/* 
 * auth: Klinger Jonathan 
 * file: main.cpp
 * date: 04.03.21
 */

#include <cstring>
#include <iostream>
#include <thread>
#include <fstream>
#include "spdlog/spdlog.h"


//#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "httplib.h"
#include "CLI11.hpp"


using namespace std;


int main(int argc, char** argv) {
  spdlog::info("Einfacher HTTP 1.1 Client gestartet!");
  //Interface
  CLI::App app{"Simple HTTP1.1 Client"};

  //Vectors for up to 3 requests
  vector<string> req1;
  vector<string> req2;
  vector<string> req3;

  

  app.add_option("-1", req1, "First Request");
  app.add_option("-2", req2, "Second Request");
  app.add_option("-3", req3, "Third Request");

  CLI11_PARSE(app, argc, argv);
  spdlog::info("Userinput erfolgreich aufgenommen!");

  //Conversion von req1 
  //URL
  const char *url1 = req1[1].c_str();
  //"/"
  const char *sub1 = req1[2].c_str();


  //Interpretation des inputs
  //req1
  httplib::Client cli(url1);
  if (req1[0] == "GET" || req1[0] == "get") { 
    spdlog::info("1. Request: GET erkannt");
    if (req1.size() > 4) {
      spdlog::info("1. Request: Authentication erkannt!");
      //Konvertiere Username bei Basic auth
      const char *user1 = req1[4].c_str();
      //Konvertiere Passwort bei Basic auth
      const char *pw1 = req1[5].c_str();

      cli.set_basic_auth(user1, pw1);
    }

    if (auto res = cli.Get(sub1)) {
      if (res->status == 200) {
        spdlog::info("1. Request: status: {}", res->status);

        //Writing in file. . .
        ofstream file;
        file.open(req1[3]);
        file << res->body;
        file.close();
        spdlog::info("1. Request: GET response erfolgreich in Datei geschrieben");
      } else {
        spdlog::warn("1. Request: Fehlercode erkannt!");
        spdlog::warn("1. Request: Code: {}", res->status);
        spdlog::warn("1. Request: Message: {}", res->body);
      }
    } else {
        cout << res.error() << endl;
    }
  } else if (req1[0] == "POST" || req1[0] == "post") {
      spdlog::info("1. Request: POST erkannt");
      //Convertiere parameter von string auf char *
      const char *params1 = req1[3].c_str();
      //Convertiere Datentyp von string auf char *
      const char *dat_type1 = req1[4].c_str();

      if (req1.size() > 5) {
        spdlog::info("1. Request: Authentication erkannt!");
        //Konvertiere Username bei Basic auth
        const char *user1 = req1[5].c_str();

        //Konvertiere Passwort bei Basic auth
        const char *pw1 = req1[6].c_str();

        cli.set_basic_auth(user1, pw1);
      }
      
      if (auto res = cli.Post(sub1, params1, dat_type1)) {
        spdlog::warn("1. Request: Status: {}", res->status);
        spdlog::warn("1. Request: Message: {}", res->body);
      } else {
        spdlog::critical("1. Request: POST nicht erfolgreich");
        spdlog::warn("1. Request: Error: {}", res.error());
      }
  } else if (req1[0] == "PUT" || req1[0] == "put") {
      spdlog::info("1. Request: PUT erkannt");
      //Convertiere parameter von string auf char *
      const char *params1 = req1[3].c_str();
      //Convertiere Datentyp von string auf char *
      const char *dat_type1 = req1[4].c_str();

      if (req1.size() > 5) {
        spdlog::info("1. Request: Authentication erkannt!");
        //Konvertiere Username bei Basic auth
        const char *user1 = req1[5].c_str();
  
        //Konvertiere Passwort bei Basic auth
        const char *pw1 = req1[6].c_str();
  
        cli.set_basic_auth(user1, pw1);
      }

      if (auto res = cli.Put(sub1, params1, dat_type1)) {
        spdlog::warn("1. Request: Status: {}", res->status);
        spdlog::warn("1. Request: Message: {}", res->body);
      } else {
        spdlog::critical("1. Request: PUT nicht erfolgreich");
        spdlog::warn("1. Request: Error: {}", res.error());
      }
  } else if (req1[0] == "DELETE" || req1[0] == "delete") {
      spdlog::info("1. Request: DELETE erkannt");
      if (req1.size() > 3) {
        spdlog::info("1. Request: Authentication erkannt!");
        //Konvertiere Username bei Basic auth
        const char *user1 = req1[3].c_str();
  
        //Konvertiere Passwort bei Basic auth
        const char *pw1 = req1[4].c_str();
  
        cli.set_basic_auth(user1, pw1);
      }

      if (auto res = cli.Delete(sub1)) {
        spdlog::warn("1. Request: Status: {}", res->status);
        spdlog::warn("1. Request: Message: {}", res->body);
      } else {
        spdlog::critical("1. Request: DELETE nicht erfolgreich");
        spdlog::warn("1. Request: Error: {}", res.error());
      }
  }

  //REQ2
  //Conversion von req2 
  //URL
  const char *url2 = req2[1].c_str();
  //"/"
  const char *sub2 = req2[2].c_str();


  //Interpretation des inputs
  httplib::Client cli2(url2);
  if (req2[0] == "GET" || req2[0] == "get") { 
    spdlog::info("2. Request: GET erkannt");
    if (req2.size() > 4) {
      spdlog::info("2. Request: Authentication erkannt!");
      //Konvertiere Username bei Basic auth
      const char *user2 = req2[4].c_str();
      //Konvertiere Passwort bei Basic auth
      const char *pw2 = req2[5].c_str();

      cli.set_basic_auth(user2, pw2);
    }

    if (auto res2 = cli2.Get(sub2)) {
      if (res2->status == 200) {
        spdlog::info("2. Request: status: {}", res2->status);

        //Writing in file. . .
        ofstream file;
        file.open(req2[3]);
        file << res2->body;
        file.close();
        spdlog::info("2. Request: GET response erfolgreich in Datei geschrieben");
      } else {
        spdlog::warn("2. Request: Fehlercode erkannt!");
        spdlog::warn("2. Request: Code: {}", res2->status);
        spdlog::warn("2. Request: Message: {}", res2->body);
      }
    } else {
        cout << res2.error() << endl;
    }
  } else if (req2[0] == "POST" || req2[0] == "post") {
      spdlog::info("2. Request: POST erkannt");
      //Convertiere parameter von string auf char *
      const char *params2 = req2[3].c_str();
      //Convertiere Datentyp von string auf char *
      const char *dat_type2 = req2[4].c_str();

      if (req2.size() > 5) {
        spdlog::info("2. Request: Authentication erkannt!");
        //Konvertiere Username bei Basic auth
        const char *user2 = req2[5].c_str();

        //Konvertiere Passwort bei Basic auth
        const char *pw2 = req2[6].c_str();

        cli.set_basic_auth(user2, pw2);
      }
      
      if (auto res2 = cli2.Post(sub2, params2, dat_type2)) {
        spdlog::warn("2. Request: Status: {}", res2->status);
        spdlog::warn("2. Request: Message: {}", res2->body);
      } else {
        spdlog::critical("2. Request: POST nicht erfolgreich");
        spdlog::warn("2. Request: Error: {}", res2.error());
      }
  } else if (req2[0] == "PUT" || req2[0] == "put") {
      spdlog::info("2. Request: PUT erkannt");
      //Convertiere parameter von string auf char *
      const char *params2 = req2[3].c_str();
      //Convertiere Datentyp von string auf char *
      const char *dat_type2 = req2[4].c_str();

      if (req1.size() > 5) {
        spdlog::info("2. Request: Authentication erkannt!");
        //Konvertiere Username bei Basic auth
        const char *user2 = req2[5].c_str();
  
        //Konvertiere Passwort bei Basic auth
        const char *pw2 = req2[6].c_str();
  
        cli.set_basic_auth(user2, pw2);
      }

      if (auto res2 = cli2.Put(sub2, params2, dat_type2)) {
        spdlog::warn("2. Request: Status: {}", res2->status);
        spdlog::warn("2. Request: Message: {}", res2->body);
      } else {
        spdlog::critical("2. Request: PUT nicht erfolgreich");
        spdlog::warn("2. Request: Error: {}", res2.error());
      }
  } else if (req2[0] == "DELETE" || req2[0] == "delete") {
      spdlog::info("2. Request: DELETE erkannt");
      if (req2.size() > 3) {
        spdlog::info("2. Request: Authentication erkannt!");
        //Konvertiere Username bei Basic auth
        const char *user2 = req2[3].c_str();
  
        //Konvertiere Passwort bei Basic auth
        const char *pw2 = req2[4].c_str();
  
        cli.set_basic_auth(user2, pw2);
      }

      if (auto res2 = cli.Delete(sub2)) {
        spdlog::warn("2. Request: Status: {}", res2->status);
        spdlog::warn("2. Request: Message: {}", res2->body);
      } else {
        spdlog::critical("2. Request: DELETE nicht erfolgreich");
        spdlog::warn("2. Request: Error: {}", res2.error());
      }
  }






  return 0;
}
