/**
 * myPhant.h
 *
 * Author: Brian Gravelle
 * Based on Phant.h by Todd Treece <todd@sparkfun.com>
 *
 */

#ifndef Phant_h
#define Phant_h

//#include "Arduino.h"
//#include <SPI.h>
#include <SFE_CC3000.h>
#include <SFE_CC3000_Client.h>

class Phant {

  public:
    Phant(char* host, char* publicKey, char* privateKey, SFE_CC3000 &cc3000);
    bool connect();
    bool isConnected();
    bool close();
    char* add(char* field, char* data);
    char* add(char* param, int length);
    char* queryString();
    //char* url();
    void get();
    char recieve();
    void post();
    void post(char* params);
    void makeEmpty();

  private:
    char* _pub;
    char* _prv;
    char* _host;
    char* _params;
    int _param_length;
    SFE_CC3000_Client _client;

};

#endif
