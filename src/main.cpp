/* 
 * auth: Klinger Jonathan 
 * file: main.cpp
 * date: 04.03.21
 */

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <fstream>

#include "spdlog/spdlog.h"
#include "asio.hpp"
#include "CLI11.hpp"


using namespace std;
using namespace asio::ip;

string base64(string str) {
  string command = "printf " + str + " | base64 > base64.txt";
  system(command.c_str());

  ifstream ifs("base64.txt");
  string ret{ std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>() };
  ifs.close();
  return ret;
}

string get_basic_header(string type, string path, string url) {
  string result = {
        type + " " + path + " HTTP/1.1\r\n"
        "Host: " + url + "\r\n"
        //"Authorization: Basic " + base_auth + "\r\n" 
        "Connection: close\r\n"
  };
  return result;
}


int main(int argc, char** argv) {
  spdlog::info("HTTP 1.1 Client gestartet!");
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
  //Reihenfolge
  // -[1/2/3] [GET/POST/PUT/DELETE] [URL] [PORT] [VERZEICHNIS] [DATEI] 
  //                reqx[0]        reqx[1] reqx[2]  reqx[3]    reqx[4]

  
  /* Request Header:
    "GET /t/jonny/post HTTP/1.1
    \r\nHost: ptsv2.com
    \r\nConnection: close\r\n
    \r\n"
  */
    
  
  asio::io_context ctx;
  tcp::resolver resolve(ctx);

  //Input Interpretieren
  if (req1[0] != "") {
    try {
      auto results = resolve.resolve(req1[1], req1[2]);
      tcp::socket sock1{ctx};
      spdlog::info("Request 1: connecting...");
      asio::connect(sock1, results);

      //HTTP Request to send
      string req_string = get_basic_header(req1[0], req1[3], req1[1]);
      int file_pos = 4;

      if (req1[0] == "GET" || req1[0] == "DELETE") {
        spdlog::info("Request 1: " + req1[0] + " erkannt");

        if (req1.size() == 7) {
          spdlog::info("Request 1: Authentifizierung erkannt");

          string auth = req1[4] + ":" + req1[5];
          string base_auth = base64(auth);
          req_string = req_string + "Authorization: Basic " + base_auth + "\r\n";
        } else {
          req_string = req_string + "\r\n";
        }
        
      } else if (req1[0] == "POST" || req1[0] == "PUT") {
        file_pos = 6;
        spdlog::info("Request 1: " + req1[0] + " erkannt");
        req_string = req_string +
          "Content-Type: " + req1[4] + "\r\n"
          "Content-Length: " + to_string(req1[5].length()) + "\r\n\r\n" +
          req1[5];
      } else {
        spdlog::error("Request 1: Falscher Request-TYP!");
      }

      //String to Char[]
      char req[150];
      for (unsigned int i=0; i<req_string.length(); i++) {
        req[i] = req_string[i];
      }

      size_t request_length = strlen(req);
      asio::write(sock1, asio::buffer(req, request_length));
      char reply[1000];
      error_code ec;

      size_t reply_length = asio::read(sock1, asio::buffer(reply), ec);

      //Char[] to String
      string res = "";
      for (unsigned int i=0; i<reply_length; i++) {
        res = res + reply[i];
      }
      //Only get body/status from response
      string body = res.substr(res.find("\r\n\r\n") + 4);
      string status = res.substr(0, res.find("\r\n"));

      spdlog::info("Request 1: Status: " + status);
      //Writing in file. . .
      ofstream file;
      file.open(req1[file_pos]);
      file << body;
      file.close();
      spdlog::info("1. Request: GET response erfolgreich in Datei geschrieben");

    } catch (asio::system_error& e) {
      cerr << e.what() << endl;
    } 
   

  }

  if (req2[0] != "") {
    try {
      auto results = resolve.resolve(req2[1], req2[2]);
      tcp::socket sock{ctx};
      spdlog::info("Request 2: connecting...");
      asio::connect(sock, results);

      //HTTP Request to send
      string req_string = get_basic_header(req2[0], req2[3], req2[1]);
      int file_pos = 4;

      if (req2[0] == "GET" || req2[0] == "DELETE") {
        spdlog::info("Request 2: " + req2[0] + " erkannt");

        if (req2.size() == 7) {
          spdlog::info("Request 2: Authentifizierung erkannt");

          string auth = req2[4] + ":" + req2[5];
          string base_auth = base64(auth);
          req_string = req_string + "Authorization: Basic " + base_auth + "\r\n";
        } else {
          req_string = req_string + "\r\n";
        }
        
      } else if (req2[0] == "POST" || req2[0] == "PUT") {
        file_pos = 6;
        spdlog::info("Request 2: " + req2[0] + " erkannt");
        req_string = req_string +
          "Content-Type: " + req2[4] + "\r\n"
          "Content-Length: " + to_string(req2[5].length()) + "\r\n\r\n" +
          req2[5];
      } else {
        spdlog::error("Request 1: Falscher Request-TYP!");
      }

      //String to Char[]
      char req[150];
      for (unsigned int i=0; i<req_string.length(); i++) {
        req[i] = req_string[i];
      }

      size_t request_length = strlen(req);
      asio::write(sock, asio::buffer(req, request_length));
      char reply[1000];
      error_code ec;

      size_t reply_length = asio::read(sock, asio::buffer(reply), ec);

      //Char[] to String
      string res = "";
      for (unsigned int i=0; i<reply_length; i++) {
        res = res + reply[i];
      }
      //Only get body/status from response
      string body = res.substr(res.find("\r\n\r\n") + 4);
      string status = res.substr(0, res.find("\r\n"));

      spdlog::info("Request 2: Status: " + status);
      //Writing in file. . .
      ofstream file;
      file.open(req2[file_pos]);
      file << body;
      file.close();
      spdlog::info("Request 2: GET response erfolgreich in Datei geschrieben");

    } catch (asio::system_error& e) {
      cerr << e.what() << endl;
    } 
   

  }

  //Request 3
  if (req3[0] != "") {
    try {
      auto results = resolve.resolve(req3[1], req3[2]);
      tcp::socket sock{ctx};
      spdlog::info("Request 3: connecting...");
      asio::connect(sock, results);

      //HTTP Request to send
      string req_string = get_basic_header(req3[0], req3[3], req3[1]);
      int file_pos = 4;

      if (req3[0] == "GET" || req3[0] == "DELETE") {
        spdlog::info("Request 3: " + req1[0] + " erkannt");

        if (req1.size() == 7) {
          spdlog::info("Request 3: Authentifizierung erkannt");

          string auth = req3[4] + ":" + req3[5];
          string base_auth = base64(auth);
          req_string = req_string + "Authorization: Basic " + base_auth + "\r\n";
        } else {
          req_string = req_string + "\r\n";
        }
        
      } else if (req3[0] == "POST" || req3[0] == "PUT") {
        file_pos = 6;
        spdlog::info("Request 3: " + req3[0] + " erkannt");
        req_string = req_string +
          "Content-Type: " + req3[4] + "\r\n"
          "Content-Length: " + to_string(req3[5].length()) + "\r\n\r\n" +
          req3[5];
      } else {
        spdlog::error("Request 3: Falscher Request-TYP!");
      }

      //String to Char[]
      char req[150];
      for (unsigned int i=0; i<req_string.length(); i++) {
        req[i] = req_string[i];
      }

      size_t request_length = strlen(req);
      asio::write(sock, asio::buffer(req, request_length));
      char reply[1000];
      error_code ec;

      size_t reply_length = asio::read(sock, asio::buffer(reply), ec);

      //Char[] to String
      string res = "";
      for (unsigned int i=0; i<reply_length; i++) {
        res = res + reply[i];
      }
      //Only get body/status from response
      string body = res.substr(res.find("\r\n\r\n") + 4);
      string status = res.substr(0, res.find("\r\n"));

      spdlog::info("Request 3: Status: " + status);
      //Writing in file. . .
      ofstream file;
      file.open(req3[file_pos]);
      file << body;
      file.close();
      spdlog::info("Request 2: GET response erfolgreich in Datei geschrieben");

    } catch (asio::system_error& e) {
      cerr << e.what() << endl;
    } 
   

  }

  
  




  return 0;
}
