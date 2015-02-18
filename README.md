================================================================
Manito Security WiFi and Bluetooth
================================================================

Arduino sketches to prototype WiFi communication
CC3000 info here: https://www.sparkfun.com/products/12072

----------------------------------------------------------------------------

**wifi** 
----------------------------------------------------------------------------
  * combining the bluetooth and WiFi systems for Project MAnito
  * WiFi uses CC3000
  * will post to https://data.sparkfun.com/streams/5JZO9K83dRU0KlA39EGZ
  * uses myPhant class
  * bluetooth communication with rn41
  * for BT indicates 'X' when IR sensor picks up movement
  * master can send 'A' or 'D' to arm or disarm the device

  **TO DO**
  * make it work!
  * make code pretty

  **Known Problems**
  * trouble with WEP security- might have to send key as hex (see wlan.h in WiFi library's utility)
  * wierd error on CC3000 objects

  **Versions**
    
  *wifi_v0-0-1*
   * initial code pile
   * does not work

----------------------------------------------------------------------------

**other stuff**
----------------------------------------------------------------------------



