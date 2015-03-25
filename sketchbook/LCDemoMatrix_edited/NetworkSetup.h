#ifndef SNOWDUINO_NETWORKSETUP_H_
#define SNOWDUINO_NETWORKSETUP_H_

#define SS_SD_CARD   4
#define SS_ETHERNET 10

#include <SPI.h>
#include <Ethernet.h>

class NetworkSetup{
  private:
    byte mac[6] = { 0x00, 0x1D, 0xFE, 0xEA, 0x39, 0x17 };
    IPAddress ip=(192, 168, 1, 19); // IP address, may need to change depending on network

    
    void EnableEthernet(){
      pinMode(SS_SD_CARD, OUTPUT);
      pinMode(SS_ETHERNET, OUTPUT);
      digitalWrite(SS_SD_CARD, HIGH); // disable SD card
      digitalWrite(SS_ETHERNET, LOW); // enable ethernet
      Serial.println("disabled sd card, enabled ethernet");
    }
    
    void JoinNetwork(){
      Serial.println("Trying to get an IP address using DHCP");
      if (Ethernet.begin(mac) == 0) {
        Serial.write("Failed to configure Ethernet using DHCP");
        // initialize the ethernet device not using DHCP:
        Ethernet.begin(mac, ip);
      } 
    }
    void PrintIp(){
      // print your local IP address:
      Serial.print("My IP address: ");
      ip = Ethernet.localIP();
      for (byte thisByte = 0; thisByte < 4; thisByte++) {
        // print the value of each byte of the IP address:
        Serial.print(ip[thisByte], DEC);
        Serial.print("."); 
      } 
      Serial.println("");
    }
  public:

    void Setup(){
      
      EnableEthernet();
      JoinNetwork();
      PrintIp();
}

    
};
#endif
