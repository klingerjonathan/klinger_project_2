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

char* convert(string str) {
  char *c_str;
  string obj(str);
  c_str = &obj[0];
  return c_str;
}

//int main(int argc, char* argv[]) {
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

  //Conversion von Request 1 
  //TYPE
  char *type1 = convert(req1[0]);
  //URL
  char *url1 = convert(req1[1]);
  //"/"
  char *sub1 = convert(req1[2]);

  cout << type1 << endl;
  cout << url1 << endl;
  cout << sub1 << endl;
  //Interpretation des inputs
  //req1
  httplib::Client cli(url1);
  if (req1[0] == "GET" || req1[0] == "get") { 
    if (req1.size() > 4) {
      //Konvertiere Username bei Basic auth
      char *user1 = convert(req1[4]);
      //Konvertiere Passwort bei Basic auth
      char *pw1 = convert(req1[5]);

      cli.set_basic_auth(user1, pw1);
    }

    if (auto res = cli.Get(sub1)) {
      if (res->status == 200) {
        cout << res->status << endl;

        //Writing in file. . .
        ofstream file(req1[3]);
        file << res->body;
        file.close();
      } else {
        cout << res->status << endl;
        cout << res->body << endl;
      }
    } else {
        std::cout << res.error() << std::endl;
    }
  } else if (req1[0] == "POST" || req1[0] == "post") {
      //Convertiere parameter von string auf char *
      char *params1 = convert(req1[3]);
      //Convertiere Datentyp von string auf char *
      char *dat_type1 = convert(req1[4]);

      if (req1.size() > 5) {
        //Konvertiere Username bei Basic auth
        char *user1 = convert(req1[5]);

        //Konvertiere Passwort bei Basic auth
        char *pw1 = convert(req1[6]);

        cli.set_basic_auth(user1, pw1);
      }
      
      if (auto res = cli.Post(sub1, params1, dat_type1)) {
        if (res->status == 200) {
          std::cout << res->body << std::endl;
        }
      } else {
          std::cout << res.error() << std::endl;
      }
  } else if (req1[0] == "PUT" || req1[0] == "put") {
      //Convertiere parameter von string auf char *
      char *params1 = convert(req1[3]);
      //Convertiere Datentyp von string auf char *
      char *dat_type1 = convert(req1[4]);

      if (req1.size() > 5) {
        //Konvertiere Username bei Basic auth
        char *user1;
        string user1_obj(req1[5]);
        user1 = &user1_obj[0];
  
        //Konvertiere Passwort bei Basic auth
        char *pw1;
        string pw1_obj(req1[6]);
        pw1 = &pw1_obj[0];
  
        cli.set_basic_auth(user1, pw1);
      }

      if (auto res = cli.Put(sub1, params1, dat_type1)) {
        if (res->status == 200) {
          cout << res->body << endl;
        } else {
          cout << res->status << endl;
          cout << res->body << endl;
        }
      } else {
          cout << res.error() << endl;
      }
  } else if (req1[0] == "DELETE" || req1[0] == "delete") {
      if (req1.size() > 3) {
        //Konvertiere Username bei Basic auth
        char *user1 = convert(req1[3])
  
        //Konvertiere Passwort bei Basic auth
        char *pw1 = convert(req1[4]);
  
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
