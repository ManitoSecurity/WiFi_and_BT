/****************************************************************
wifi_and_bt.ino
Manito Security Solutions
Brian Gravelle
Prototype for WiFi communication from Arduino
many thanks to Shawn Hymel @ SparkFun Electronics for creating
  the CC3000 library
  https://github.com/sparkfun/SFE_CC3000_Library
The security mode is defined by one of the following:
WLAN_SEC_UNSEC, WLAN_SEC_WEP, WLAN_SEC_WPA, WLAN_SEC_WPA2
Hardware Connections:
 
 Uno Pin    CC3000 Board    Function
 
 +5V        VCC or +5V      5V
 GND        GND             GND
 2          INT             Interrupt
 7          EN              WiFi Enable
 10         CS              SPI Chip Select
 11         MOSI            SPI MOSI
 12         MISO            SPI MISO
 13         SCK             SPI Clock
Resources:
Include SPI.h, SFE_CC3000.h, and SFE_CC3000_Client.h
(SFE_CC3000_Library_master)
myPhant.h

Development environment specifics:
Written in Arduino 1.0.5
Tested with Arduino UNO R3

pub  5JZO9K83dRU0KlA39EGZ
pri  7BMDzNyXeAf0Kl25JoW1
url  https://data.sparkfun.com/streams/5JZO9K83dRU0KlA39EGZ
****************************************************************/

#include <SPI.h>
#include <SFE_CC3000.h>
#include <SFE_CC3000_Client.h>
#include <myPhant.h>

#include <SoftwareSerial.h> 

// Pins
#define CC3000_INT      2   // Needs to be an interrupt pin (D2/D3)
#define CC3000_EN       7   // Can be any digital pin
#define CC3000_CS       10  // Preferred is pin 10 on Uno
#define IRPin           3 
#define IRDummy         6
#define bluetoothTx     4  // TX-O pin of bluetooth mate, Arduino D2
#define bluetoothRx     5  // RX-I pin of bluetooth mate, Arduino D3
//#define LED_PIN1        3
//#define LED_PIN2        9

// Connection info data lengths
#define IP_ADDR_LEN     4   // Length of IP address in bytes

// Constants
unsigned int ap_security = WLAN_SEC_WEP; // Security of network
unsigned int timeout = 60000;             // Milliseconds

char server[] = "data.sparkfun.com";      // sparkfun data
char pub_key[] = "5JZO9K83dRU0KlA39EGZ";  // public key
char pri_key[] = "7BMDzNyXeAf0Kl25JoW1";  // private key
int waitTime= 30000;                      // limit update interval

// Global Variables
char ap_ssid[33];     // SSID of network
char ap_password[33]; // Password of network
IPAddress remote_ip;
ConnectionInfo connection_info;
int digiIRout;        // reading from IR
boolean armed, alarmed, state_change;
char postString[33];
char phantReply[64];

SFE_CC3000 wifi(CC3000_INT, CC3000_EN, CC3000_CS);
Phant phant(server, pub_key, pri_key, wifi);


boolean connection_bt;
char btReply[256];
int loopCount;
SoftwareSerial bluetooth(bluetoothTx, bluetoothRx);

/***********************************************\
              Things for WiFi
\***********************************************/
void initCC3000(){

  if ( wifi.init() ) {
    Serial.print("CC3000 initialization complete");
    Serial.print('\n'); 
  } else {
    Serial.print("Something went wrong during CC3000 init!");
    Serial.print('\n'); 
  }
  
}

void getWiFiInfo(){
 
  Serial.setTimeout(10000);
  Serial.print("Enter SSID: ");
  Serial.readBytesUntil('\n',ap_ssid,33);
  delay(1000);
  Serial.print("\nEnter Password: ");
  Serial.readBytesUntil('\n',ap_password,33);
  delay(1000);
  Serial.print('\n');
  Serial.print('\n');
  
}

void connectToWiFi(){
  Serial.print("Connecting to SSID: ");
  Serial.print(ap_ssid);
  Serial.print('\n'); 
  if(!wifi.connect(ap_ssid, ap_security, ap_password, timeout)) {
    Serial.print("Error: Could not connect to AP");
    Serial.print('\n'); 
  }
}

void showConnectionInfo(){
  int i;
  
  if ( !wifi.getConnectionInfo(connection_info) ) {
    Serial.print("Error: Could not obtain connection details");
    Serial.print('\n'); 
  } else {
    Serial.print("IP Address: ");
    for (i = 0; i < IP_ADDR_LEN; i++) {
      Serial.print(connection_info.ip_address[i]);
      if ( i < IP_ADDR_LEN - 1 ) {
        Serial.print(".");
      }
    }
    Serial.print('\n'); 
  }  
}

void lookupServerIP(){
  int i;
 
  Serial.print("Looking up IP address of: ");
  Serial.print(server);
  Serial.print('\n'); 
  if ( !wifi.dnsLookup(server, &remote_ip) ) {
    Serial.print("Error: Could not lookup host by name");
    Serial.print('\n'); 
  } else {
    Serial.print("IP address found: ");
    for (i = 0; i < IP_ADDR_LEN; i++) {
      Serial.print(remote_ip[i], DEC);
      if ( i < IP_ADDR_LEN - 1 ) {
        Serial.print(".");
      }
    }
    Serial.print('\n');
  }
}

void setDisarmPost(){
  char string[] = "armed=F&alert=F";
  int j = 0;
  while(string[j] != '\0') {
     postString[j] = string[j];
     j++;
  }

  postString[j] = '\0';
}

void setArmPost(){
  char string[] = "armed=T&alert=F"; 
    int j = 0;
  while(string[j] != '\0') {
     postString[j] = string[j];
     j++;
  }

  postString[j] = '\0';
}

void setAlertPost(){
  char string[] = "armed=T&alert=T";
    int j = 0;
  while(string[j] != '\0') {
     postString[j] = string[j];
     j++;
  }

  postString[j] = '\0';
}

void updateServer(){  

  boolean connection = phant.connect();
  //delay(100);
  //if(connection) {
  //  Serial.print("Clearing data on ");
  //  Serial.print(server);
  //  Serial.print('\n');  
  //  phant.makeEmpty();
  //} else {
  //  Serial.print('\n');
  //  Serial.print("Failed to connect to ");
  //  Serial.print(server); 
  //  Serial.print('\n');   
  //}
  //delay(100);
  //
  //connection = phant.connect();
  delay(100);
  if(connection) { 
    Serial.print("Posting to ");
    Serial.print(server);
    Serial.print('\n'); 
    phant.post(postString);
  } else {
    Serial.print('\n');
    Serial.print("Failed to connect to ");
    Serial.print(server); 
    Serial.print('\n');   
  }
  
  delay(waitTime);
  
} //end updateServer

void checkServer(){ 
  char c;
  int i = 0;
  int nl_cnt = 0;
  
  if(phant.connect()) {
    phant.get();
    Serial.print("Getting data from ");
    Serial.print(server);
    Serial.print('\n');
    Serial.print('\n'); 
    c = phant.recieve();
    delay(100);
    
    while (c != '\0') {
	
      if ( (nl_cnt == 15) && (i < 62) ) {
         phantReply[i] = c; 
         i++;
      } else {
		if (c == '\n') 
         nl_cnt++;
      }  
      
	  //Serial.print(c); useful for debug
      c = phant.recieve();
    }
	    
    Serial.print("Phant data: ");
    Serial.print(phantReply); 
    Serial.print("\n"); 
   
  } else {
    Serial.print('\n');
    Serial.print("Failed to connect to ");
    Serial.print(server); 
    Serial.print('\n');   
  }
  
}

void syncArmToServer(){
    if( phantReply[2] == 'F') 
		armed = false;
	else 
		armed = true;
}

void syncAlertToServer(){
	if( phantReply[0] == 'T') 
		alarmed = true;
	else  
		alarmed = false;
}

/***********************************************\
                Things for BT
\***********************************************/

void sendCmd(char* cmd){
  delay(1100); 
  checkArmed_bt(); //grab message sent in that second (must b/c these delay are most of the time)
  bluetooth.print("$");  // Print three times individually
  bluetooth.print("$");
  bluetooth.print("$"); 
  delay(1100);  // Short delay, wait for the Mate to send back CMD */
  purgeReply();
  
  bluetooth.println(cmd);
  getReply();  
  bluetooth.println("---"); 
  purgeReply(); 
}

void sendMsg(char* msg){
  delay(50);
  //bluetooth.print("P,");
  bluetooth.println(msg);
  delay(50);
}

void getReply(){
  int i = 0;
  delay(100);
  while(bluetooth.available()>0)  // If the bluetooth sent any characters
  {
    btReply[i] = (char)bluetooth.read();  
    i++;
  }
  btReply[i] = '\0';
  Serial.println(btReply);
  delay(100);
}

void purgeReply(){
  int i = 0;
  delay(100);
  while(bluetooth.available() > 0)  // If the bluetooth sent any characters
  {
    bluetooth.read();  
  }
}

void getSSID_bt(){
  int j = 0;
  int data = 0;
  
  bluetooth.print("enter SSID");
  
  while( ((char)data != '#') && (j < 33) ){
    if(bluetooth.available() > 0){
      data = bluetooth.read(); 
      delay(200);
    
    
      if((char)data != '#'){
        ap_ssid[j] = (char)data; 
        j++;
      }
    }    
  }
  
  ap_ssid[j] = '\0';
  
  Serial.print("SSID: ");
  Serial.println(ap_ssid);
}

void getPASS_bt(){
  int j = 0;
  int data = 0;
  
  bluetooth.print("enter password.");
  
  while( ((char)data != '#') && (j < 33) ){
    if(bluetooth.available() > 0){
      data = bluetooth.read(); 
      delay(200);
    
    
      if((char)data != '#'){
        ap_password[j] = (char)data; 
        j++;
      }
    }    
  }
  
  ap_password[j] = '\0';
  
  Serial.print("password: ");
  Serial.println(ap_password);
}

void checkArmed_bt(){
  delay(500);
  getReply();
  if(btReply[0] == 'A'){
    armed = true;
    sendMsg("A");
  }
  if(btReply[0] == 'D'){
    armed = false;
    sendMsg("D");
  }
}

void checkBTConnection(){
  delay(100);
  sendCmd("GK");  
  if(btReply[0] == '1') connection_bt = true;
  else connection_bt = false;
}

void checkBTStatus(){
  //checkArmed_bt();
  
  //avoid delays that make arming and disarming difficult
  if(loopCount == 20){
    checkBTConnection();
    loopCount = 0;
  }
  loopCount++;
}

/***********************************************\
                General Things
\***********************************************/

void setup() {
  
  // Initialize Serial port
  Serial.begin(115200);
  Serial.print('\n'); 
  Serial.print("---------------------------\n");
  Serial.print("     Manito WiFi and BT    \n");
  Serial.print("---------------------------\n");
  
  pinMode(IRPin, INPUT);
  pinMode(IRDummy, OUTPUT);
  digitalWrite(IRDummy, HIGH);

  //WiFi setup
  initCC3000();  
  getWiFiInfo(); //WiFi command through serial
  connectToWiFi();
  showConnectionInfo();
  lookupServerIP();
   
  state_change = true;
  armed = true; alarmed = false;

  setArmPost();
  updateServer();
  checkServer();

  //BT setup
  bluetooth.begin(9600);  
  // 115200 can be too fast at times for NewSoftSerial to relay the data reliably
  // The Bluetooth Mate defaults to 115200bps
  // so be sure to fix you default SU,96
  delay(500);
  getReply(); 
  sendCmd("D");
  
  Serial.print("       Setup Complete      ");
  Serial.print('\n'); 
  Serial.print("---------------------------");
  Serial.print('\n'); 
  
} //end setup


void loop() {
    
	//delay(30000);
    digiIRout = digitalRead(IRPin);
    
	if ( armed ) {
		if(digiIRout == LOW) { 
		  setAlertPost();
		  state_change = !alarmed; //if not alarming you changed!
		  alarmed = true;
		} else {
		  setArmPost();
		  state_change = alarmed;
		  alarmed = false;
		}
	
	}

	if( state_change ) {
		
	   checkServer();
	   syncArmToServer();
	   delay(100);
       if( armed ) {
	      updateServer();
	   } else {
		   syncAlertToServer();
		   delay(100);
		   Serial.println("disarmed");
		   state_change = false;
	   }
	   
	    state_change = false;
	}
  
	checkBTStatus();
	if( connection_bt ) {
		setDisarmPost();
		updateServer();
		armed = false;
		alarmed = false;
	}

} // end loop