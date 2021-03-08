/* 
 * auth: Klinger Jonathan 
 * file: main.cpp
 * date: 04.03.21
 */

#include <iostream>
#include <thread>
#include <fstream>


//#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "httplib.h"


using namespace std;

//int main(int argc, char* argv[]) {
int main() {
    httplib::Client cli("http://www.d474base.eu");

    string body;
    ofstream file("test.txt");

    auto res = cli.Get("/", 
      [&](const char *data, size_t data_length) {
          body.append(data, data_length);
          file << body;
          return true;
      });
    
    file.close();

    

    return 0;
}
