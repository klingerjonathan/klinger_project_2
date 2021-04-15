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

asio::io_context ctx;
tcp::resolver resolve(ctx);

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

void send_GET_DELETE(vector<string> input, string n) {
  try {
      spdlog::info(n + ". Request: gestartet...");
      auto results = resolve.resolve(input[1], input[2]);
      tcp::socket sock{ctx};
      asio::connect(sock, results);

      unsigned int file_place = 4;
      //HTTP Request to send
      string req_string = get_basic_header(input[0], input[3], input[1]);

      if (input.size() == 7) {
        spdlog::info(n + ". Request: HTTP Basic Authorization erkannt");
        file_place = 6;
        string auth = input[4] + ":" + input[5];
        string base_auth = base64(auth);
        req_string = req_string + "Authorization: Basic " + base_auth + "\r\n";
      } else {
        req_string = req_string + "\r\n";
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

      spdlog::info(n + ". Request: Status: " + status);
      //Writing in file. . .
      if (input.size() == file_place + 1) {
        ofstream file;
        file.open(input[file_place]);
        file << body;
        file.close();
        spdlog::info(n + ". Request: Datei erfolgreich erstellt");
      } else {
        spdlog::error(n + ". Request: Konnte nicht in datei schreiben...");
      }
      
    } catch (asio::system_error& e) {
      cerr << e.what() << endl;
    } 
      
}

void send_POST_PUT(vector<string> input, string n) {
  try {
      spdlog::info(n + ". Request: gestartet...");
      auto results = resolve.resolve(input[1], input[2]);
      tcp::socket sock{ctx};
      asio::connect(sock, results);

      //HTTP Request to send
      string req_string = get_basic_header(input[0], input[3], input[1]);

      if (input.size() == 7) {
        spdlog::info(n + ". Request: HTTP Basic Authorization erkannt");
        string auth = input[4] + ":" + input[5];
        string base_auth = base64(auth);
        req_string = req_string + "Authorization: Basic " + base_auth + "\r\n";
      } 

      req_string = req_string +
          "Content-Type: " + input[4] + "\r\n"
          "Content-Length: " + to_string(input[5].length()) + "\r\n\r\n" +
          input[5];

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

      spdlog::info(n + ". Request Status: " + status);

      } catch (asio::system_error& e) {
      cerr << e.what() << endl;
      } 
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

  
  if (req1[0] == "GET" || req1[0] == "DELETE") {
    send_GET_DELETE(req1, "1");
  } else if (req1[0] == "POST" || req1[0] == "PUT") {
    send_POST_PUT(req1, "1");
  } else {
    spdlog::error("1. Request: Typ nicht erkannt");
  }

  if (req2.size() > 0) {
    if (req2[0] == "GET" || req2[0] == "DELETE") {
      send_GET_DELETE(req2, "2");
    } else if (req2[0] == "POST" || req2[0] == "PUT") {
      send_POST_PUT(req2, "2");
    } else {
      spdlog::error("2. Request: Typ nicht erkannt");
    }
  }

  if (req3.size() > 0) {
    if (req3[0] == "GET" || req3[0] == "DELETE") {
      send_GET_DELETE(req1, "3");
    } else if (req3[0] == "POST" || req3[0] == "PUT") {
      send_POST_PUT(req3, "3");
    } else {
      spdlog::error("3. Request: Typ nicht erkannt");
    }
  }

  

 

  return 0;
}
