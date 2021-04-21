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
#include "json.hpp"


using namespace std;
using namespace asio::ip;
using namespace nlohmann;

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

      if (input.size() == 8) {
        spdlog::info(n + ". Request: Cookies erkannt");
        spdlog::info(n + ". Request: HTTP Basic Authorization erkannt");

        file_place = 7;

        string auth = input[5] + ":" + input[6];
        string base_auth = base64(auth);

        req_string = req_string + 
        "Authorization: Basic " + base_auth + "\r\n"
        "Cookie: " + input[4] + "\r\n\r\n";        
      } else if (input.size() == 7) {
        spdlog::info(n + ". Request: HTTP Basic Authorization erkannt");

        file_place = 6;

        string auth = input[4] + ":" + input[5];
        string base_auth = base64(auth);

        req_string = req_string + "Authorization: Basic " + base_auth + "\r\n\r\n";
      } else if (input.size() == 6) {
        spdlog::info(n + ". Request: Cookie erkannt");
        file_place = 5;

        req_string = req_string + "Cookie: " + input[4] + "\r\n\r\n";
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

      if (input.size() == 8) {
        spdlog::info(n + ". Request: Cookies erkannt");
        spdlog::info(n + ". Request: HTTP Basic Authorization erkannt");

        string auth = input[5] + ":" + input[6];
        string base_auth = base64(auth);

        req_string = req_string + 
        "Authorization: Basic " + base_auth + "\r\n"
        "Cookie: " + input[4] + "\r\n";   
      } else if (input.size() == 7) {
        spdlog::info(n + ". Request: HTTP Basic Authorization erkannt");
        string auth = input[4] + ":" + input[5];
        string base_auth = base64(auth);
        req_string = req_string + "Authorization: Basic " + base_auth + "\r\n";
      } else if (input.size() == 6) {
        spdlog::info(n + ". Request: Cookie erkannt");

        req_string = req_string + "Cookie: " + input[4] + "\r\n";
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

  json j;
  ifstream config_file("config.json");
  string line, config;
  while (getline (config_file, line)) {
    config += line;
  }

  config_file.close();

  try { 
    j = json::parse(config);
  } catch (json::parse_error& ex) {
    std::cerr << "parse error at byte " << ex.byte << std::endl;
  }

  
  //Erster Request
  string type1 = "";
  string url1  = "";
  string port1 = "80";
  string path1 = "/";
  string file1 = "test.txt";
  string user1 = "";
  string pw1   = "";

  user1 = j["user1"];
  pw1 = j["pw1"];

  app.add_option("--type1", type1, "Typ von Request 1");
  app.add_option("--url1",  url1,  "URL von Request 1");
  app.add_option("--port1", port1, "Port von Request 1");
  app.add_option("--path1", path1, "Path von Request 1");
  app.add_option("--file1", file1, "Dateiname von Request 1");
  app.add_option("--user1", user1, "User von Request 1");
  app.add_option("--pw1",   pw1,   "Password von Request 1");

  //Zweiter Request
  string type2 = "";
  string url2  = "";
  string port2 = "80";
  string path2 = "/";
  string file2 = "test.txt";
  string user2 = "";
  string pw2   = "";

  user2 = j["user2"];
  pw2 = j["pw2"];

  app.add_option("--type2", type2, "Typ von Request 2");
  app.add_option("--url2",  url2,  "URL von Request 2");
  app.add_option("--port2", port2, "Port von Request 2");
  app.add_option("--path2", path2, "Path von Request 2");
  app.add_option("--file2", file2, "Dateiname von Request 2");
  app.add_option("--user2", user2, "User von Request 2");
  app.add_option("--pw2",   pw2,   "Password von Request 2");

  //Dritter Request
  string type3 = "";
  string url3 = "";
  string port3 = "80";
  string path3 = "/";
  string file3 = "test.txt";
  string user3 = "";
  string pw3   = "";

  user3 = j["user3"];
  pw3 = j["pw3"];

  app.add_option("--type3", type3, "Typ von Request 3");
  app.add_option("--url3",  url3,  "URL von Request 3");
  app.add_option("--port3", port3, "Port von Request 3");
  app.add_option("--path3", path3, "Path von Request 3");
  app.add_option("--file3", file3, "Dateiname von Request 3");
  app.add_option("--user3", user3, "User von Request 3");
  app.add_option("--pw3",   pw3,   "Password von Request 3");


  CLI11_PARSE(app, argc, argv);
  spdlog::info("Userinput erfolgreich aufgenommen!");
  //Reihenfolge
  // -[1/2/3] [GET/POST/PUT/DELETE] [URL] [PORT] [VERZEICHNIS] [DATEI] 
  //                reqx[0]        reqx[1] reqx[2]  reqx[3]    reqx[4]

  vector<string> req1{type1, url1, port1, path1, file1, user1, pw1};
  vector<string> req2{type2, url2, port2, path2, file2, user2, pw2};
  vector<string> req3{type3, url3, port3, path3, file3, user3, pw3};

  
  if (type1 == "GET" || type1 == "DELETE") {
    send_GET_DELETE(req1, "1");
  } else if (type1 == "POST" || type1 == "PUT") {
    send_POST_PUT(req1, "1");
  } else {
    spdlog::error("1. Request: Typ nicht erkannt");
  }

  if (type2 != "") {
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
