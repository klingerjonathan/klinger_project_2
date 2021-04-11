/* 
 * auth: Klinger Jonathan 
 * file: main.cpp
 * date: 04.03.21
 */

#include <cstring>
#include <iostream>
#include <thread>
#include <fstream>


//#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "httplib.h"
#include "CLI11.hpp"


using namespace std;


int main(int argc, char** argv) {
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

  //Conversion von req1 
  //URL
  const char *url1 = req1[1].c_str();
  //"/"
  const char *sub1 = req1[2].c_str();


  //Interpretation des inputs
  //req1
  httplib::Client cli(url1);
  if (req1[0] == "GET" || req1[0] == "get") { 
    if (req1.size() > 4) {
      //Konvertiere Username bei Basic auth
      const char *user1 = req1[4].c_str();
      //Konvertiere Passwort bei Basic auth
      const char *pw1 = req1[5].c_str();

      cli.set_basic_auth(user1, pw1);
    }

    if (auto res = cli.Get(sub1)) {
      if (res->status == 200) {
        cout << res->status << endl;

        //Writing in file. . .
        ofstream file;
        file.open(req1[3]);
        file << res->body;
        file.close();
      } else {
        cout << res->status << endl;
        cout << res->body << endl;
      }
    } else {
        cout << res->body << endl;
    }
  } else if (req1[0] == "POST" || req1[0] == "post") {
      //Convertiere parameter von string auf char *
      const char *params1 = req1[3].c_str();
      //Convertiere Datentyp von string auf char *
      const char *dat_type1 = req1[4].c_str();

      if (req1.size() > 5) {
        //Konvertiere Username bei Basic auth
        const char *user1 = req1[5].c_str();

        //Konvertiere Passwort bei Basic auth
        const char *pw1 = req1[6].c_str();

        cli.set_basic_auth(user1, pw1);
      }
      
      if (auto res = cli.Post(sub1, params1, dat_type1)) {
        cout << res->status << endl;
        cout << res->body << endl;
      } else {
        cout << res.error() << endl;
      }
  } else if (req1[0] == "PUT" || req1[0] == "put") {
      //Convertiere parameter von string auf char *
      const char *params1 = req1[3].c_str();
      //Convertiere Datentyp von string auf char *
      const char *dat_type1 = req1[4].c_str();

      if (req1.size() > 5) {
        //Konvertiere Username bei Basic auth
        const char *user1 = req1[5].c_str();
  
        //Konvertiere Passwort bei Basic auth
        const char *pw1 = req1[6].c_str();
  
        cli.set_basic_auth(user1, pw1);
      }

      if (auto res = cli.Put(sub1, params1, dat_type1)) {
        cout << res->status << endl;
        cout << res->body << endl;
      } else {
        cout << res.error() << endl;
      }
  } else if (req1[0] == "DELETE" || req1[0] == "delete") {
      if (req1.size() > 3) {
        //Konvertiere Username bei Basic auth
        const char *user1 = req1[3].c_str();
  
        //Konvertiere Passwort bei Basic auth
        const char *pw1 = req1[4].c_str();
  
        cli.set_basic_auth(user1, pw1);
      }

      if (auto res = cli.Delete(sub1)) {
        std::cout << res->body << std::endl;
      } else {
        std::cout << res.error() << std::endl;
      }
  }






  return 0;
}
