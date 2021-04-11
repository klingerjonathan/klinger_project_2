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

  //Conversion of Request 1 
  //TYPE
  char *type1;
  string type1_obj(req1[0]);
  type1 = &type1_obj[0];
  //URL
  char *url1;
  string str_obj(req1[1]);
  url1 = &str_obj[0];
  //"/"
  char *sub1;
  string sub1_obj(req1[2]);
  sub1 = &sub1_obj[0];

  //Interpretation of input
  //req1
  if (strcmp(type1, "GET") || strcmp(type1, "get")) { 
    httplib::Client cli(url1);
    if (auto res = cli.Get(sub1)) {
      if (res->status == 200) {
        std::cout << res->body << std::endl;
      }
    } else {
      std::cout << res.error() << std::endl;
    }
  } /*else if (req1.at(0) == "POST" || req1.at(0) == "post") {

  } else if (req1.at(0) == "PUT" || req1.at(0) == "put") {

  } else if (req1.at(0) == "DELETE" || req1.at(0) == "delete") {

  }

*/
  string body;
  ofstream file("test.txt");

    

  file.close();

    

  return 0;
}
