/* 
 * auth: Klinger Jonathan 
 * file: main.cpp
 * date: 04.03.21
 */

#include <iostream>
#include <thread>


//#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "httplib.h"


using namespace std;

//int main(int argc, char* argv[]) {
int main() {
    httplib::Client cli("http://ziwax.de:3002");


    //Teste GET mit Ziwax API
    if (auto res = cli.Get("/firmen")) {
        if (res->status == 200) {
            std::cout << res->body << std::endl;
        }   
    } else {
      //auto err = res.error();
      std::cout << "Oh nein! " << res.error() << std::endl;
    }

    return 0;
}
