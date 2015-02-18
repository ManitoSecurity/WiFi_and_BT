/**
 * myPhant.h
 *
 * Author: Brian Gravelle
 * Based on Phant.h by Todd Treece <todd@sparkfun.com>
 *
 */
 
//#include "Arduino.h"
//#include <SPI.h>
#include <SFE_CC3000.h>
#include <SFE_CC3000_Client.h>

#include "myPhant.h"

Phant::Phant(char* host, char* publicKey, char* privateKey, SFE_CC3000 &cc3000)
{
  _host = host;
  _pub = publicKey;
  _prv = privateKey;
  _params[0] = '\0';
  _param_length = 0;
  _client = SFE_CC3000_Client(cc3000);
}

bool Phant::connect()
{
  return _client.connect(_host, 80);
}

bool Phant::isConnected()
{
  return _client.connected();
}

bool Phant::close()
{
  return _client.close();
}

char* Phant::add(char* param, int length)
{
  int j = 0;
  
  //while(field[j] != '\0') {
  while(j < 17) {
     _params[j] = param[j];
     j++;
  }

  _params[j] = '\0';

  return _params;

}

char* Phant::add(char* field, char* data) 
{

  _params = "&alert=T&armed=F";
  _param_length = 15;
  return "Hi there";

/*
  int j = 0;

  _params[_param_length] = '&';
  _param_length++;
  while(field[j] != '\0') {
     _params[_param_length] = field[j];
     j++; _param_length++;
  }
  _params[_param_length] = '=';
  _param_length++;

  j = 0;
  while(data[j] != '\0') {
     _params[_param_length] = data[j];
     j++; _param_length++;
  }
  _params[_param_length] = '\0';
  _param_length++;

  return "hi";
*/
}

char* Phant::queryString() 
{
  return _params;
}
/*
char* Phant::url() {

  String result = "http://" + _host + "/input/" + _pub + ".txt";
  result += "?private_key=" + _prv + _params;

  _params = "";

  return "this doesn't do anytihng yet";

}
*/
void Phant::get() 
{

  _client.print("GET /output/"); _client.print(_pub); 
     _client.println(".csv HTTP/1.1");
  _client.print("Host: "); _client.println(_host);
  _client.println("Connection: close");
  _client.print("\n");
}

char Phant::recieve(){

  if ( _client.available() )
    return _client.read();
  else
    return '\0';

}

void Phant::post(char* params) 
{

  //skip first &
  //char* params = &_params[1];
  
  _client.print("POST /input/"); _client.print(_pub); 
      _client.println(".txt HTTP/1.1");
  _client.print("Host: "); _client.println(_host);
  _client.print("Phant-Private-Key: "); _client.println(_prv);
  _client.println("Connection: close");
  _client.println("Content-Type: application/x-www-form-urlencoded");
  //_client.print("Content-Length: "); _client.print(_param_length);
  _client.print("Content-Length: 15");
  _client.print("\n\n");
  _client.print(params);

  _params[0] = '\0';
  _param_length = 0;

}

void Phant::makeEmpty() 
{

  int _param_length;
  _client.print("DELETE /input/"); _client.print(_pub); 
     _client.println(".txt HTTP/1.1");
  _client.print("Host: "); _client.println(_host);
  _client.print("Phant-Private-Key: "); _client.println(_prv);
  _client.println("Connection: close");
  _client.print("\n");

}
