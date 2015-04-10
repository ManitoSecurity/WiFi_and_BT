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
  * recieves WiFi login info over BT
  * attempts connection, send E (error) or C (connected) BT 
  * must set BT baud to 9600 (SU,96<CR>)
  * In older versions: BT indicates 'X' when IR sensor picks up movement
  *                    phone can send 'A' or 'D' to arm or disarm the device

  **TO DO**
  * make it work!
  * make code pretty

  **Known Problems**
  * trouble with WEP security- might have to send key as hex (see wlan.h in WiFi library's utility)

  **Versions**

  *wifi_v0-0-7*
   * working
   * minor updates for feed changes
    
  *wifi_v0-0-6*
   * working
   * connects to WiFi and phant
   * updates server as intended
   * not fully tested yet
    
  *wifi_v0-0-5*
   * not working
   * phone field added to phant
   * improved communication to phant
    
  *wifi_v0-0-4*
   * untested
   * adds confirmation of wifi connection

  *wifi_v0-0-3*
   * mostly working
   * major latency issues updating feed

  *wifi_v0-0-1*
   * initial code pile
   * does not work

----------------------------------------------------------------------------

**other stuff**
----------------------------------------------------------------------------



