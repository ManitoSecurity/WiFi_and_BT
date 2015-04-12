// rgb_led.cpp
// implementation of rgb led driver
// Manito Security Solutions
// Brian Gravelle
// March 14 2015

#include "rgb_led.h"


void init_rgb_led() {	
   pinMode(LED_R, OUTPUT);
   pinMode(LED_G, OUTPUT);
   pinMode(LED_B, OUTPUT);
}

void turn_on_red() {
  analogWrite(LED_R, ON);
  analogWrite(LED_G, OFF);
  analogWrite(LED_B, OFF);
}

void turn_on_green() {
  analogWrite(LED_R, OFF);
  analogWrite(LED_G, ON);
  analogWrite(LED_B, OFF);
}

void turn_on_blue() {
  analogWrite(LED_R, OFF);
  analogWrite(LED_G, OFF);
  analogWrite(LED_B, ON);
}

void turn_on_purple() {
  analogWrite(LED_R, ON);
  analogWrite(LED_G, OFF);
  analogWrite(LED_B, ON);
}

void turn_on_GB() {
  analogWrite(LED_R, OFF);
  analogWrite(LED_G, ON);
  analogWrite(LED_B, ON);
}

void turn_on_white() {
  analogWrite(LED_R, ON); 
  analogWrite(LED_G, ON);
  analogWrite(LED_B, ON);
}

void turn_on_RG() {
  analogWrite(LED_R, ON);
  analogWrite(LED_G, ON);
  analogWrite(LED_B, OFF);
}
