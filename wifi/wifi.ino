/****************************************************************
wifi.ino
Manito Security Solutions
Brian Gravelle
Jan 8, 2015
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

// Pins
#define CC3000_INT      2   // Needs to be an interrupt pin (D2/D3)
#define CC3000_EN       7   // Can be any digital pin
#define CC3000_CS       10  // Preferred is pin 10 on Uno
#define IRPin           3 
#define IRDummy         6 

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

void setup() {
  
  // Initialize Serial port
  Serial.begin(115200);
  Serial.print('\n'); 
  Serial.print("---------------------------\n");
  Serial.print("        Manito WiFi        \n");
  Serial.print("---------------------------\n");
  
  pinMode(IRPin, INPUT);
  pinMode(IRDummy, OUTPUT);
  digitalWrite(IRDummy, HIGH);

  initCC3000();  
  getWiFiInfo(); 
  connectToWiFi();
  showConnectionInfo();
  lookupServerIP();
   
  state_change = true;
  armed = true; alarmed = false;

  setArmPost();
  updateServer();
  checkServer();
  
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
  
} // end loop
