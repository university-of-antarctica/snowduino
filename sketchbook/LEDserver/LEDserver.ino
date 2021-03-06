/*--------------------------------------------------------------
  Program:      eth_websrv_LED

  Description:  Arduino web server that serves up a web page
                allowing the user to control an LED
  
  Hardware:     - Arduino Uno and official Arduino Ethernet
                  shield. Should work with other Arduinos and
                  compatible Ethernet shields.
                - LED and resistor in series connected between
                  Arduino pin 2 and GND
                
  Software:     Developed using Arduino 1.0.3 software
                Should be compatible with Arduino 1.0 +
  
  References:   - WebServer example by David A. Mellis and 
                  modified by Tom Igoe
                - Ethernet library documentation:
                  http://arduino.cc/en/Reference/Ethernet

  Date:         11 January 2013
 
  Author:       W.A. Smith, http://startingelectronics.com
--------------------------------------------------------------*/

#include <SPI.h>
#include <Ethernet.h>
//#include <string>

const int redled = 2;
const int buttonPin = 5;


// MAC address from Ethernet shield sticker under board
byte mac[] = { 0x00, 0x1D, 0xFE, 0xEA, 0x39, 0x17 };
IPAddress ip=(192, 168, 1, 26); // IP address, may need to change depending on network
//byte ip[] =  {192,168,0,1}; //attempt at different specification
EthernetServer server(80);  // create a server at port 80

String HTTP_req;// (char*)malloc(sizeof(char)*500);          // stores the HTTP request
boolean LED_status = 1;   // state of LED, off by default
int BTN_status = 0;   // state of button
int i = 0;


void setup()
{
  Serial.begin(9600);       // for diagnostics
  
  // start the Ethernet connection:
  Serial.println("Trying to get an IP address using DHCP");
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // initialize the ethernet device not using DHCP:
    Ethernet.begin(mac, ip);
  }
  // print your local IP address:
  Serial.print("My IP address: ");
  ip = Ethernet.localIP();
  for (byte thisByte = 0; thisByte < 4; thisByte++) {
    // print the value of each byte of the IP address:
    Serial.print(ip[thisByte], DEC);
    Serial.print("."); 
  }
  
  
   // Ethernet.begin(mac, ip);  // initialize Ethernet device
    server.begin();           // start to listen for clients
    
    pinMode(redled, OUTPUT);       // LED on pin 2
    pinMode(buttonPin, INPUT);
}

void loop()
{
    /*
    delay(1000);
    BTN_status = digitalRead(buttonPin);
    if (BTN_status == HIGH){
      //client.println("<h1>BUTTON IS BEING PRESSED</h1>");
       digitalWrite(redled,HIGH);
    }
    else{
//      client.println("<h3> button not pressed </h3>");
      digitalWrite(redled,LOW);
    } */
    
    //delay(1000);
    
    
    if (LED_status) {    // switch LED on
        digitalWrite(redled, HIGH);
    }
    else {              // switch LED off
        digitalWrite(redled, LOW);
    }
    HTTP_req="nothing";
    //i = 0;
    
    EthernetClient client = server.available();  // try to get client

    if (client) {  // got client?
        boolean currentLineIsBlank = true;
        while (client.connected()) {
            if (client.available()) {   // client data available to read
                char c = client.read(); // read 1 byte (character) from client
                HTTP_req+= c;  // save the HTTP request 1 char at a time
               // ++i;
                //Serial.print(HTTP_req);
                // last line of client request is blank and ends with \n
                // respond to client only after last line received
                if (c == '\n' && currentLineIsBlank) {
                    // send a standard http response header
                    Serial.println("parsing request...");
                    Serial.println("<req>");
                    Serial.println(HTTP_req);
                    Serial.println("</req>");
                    //HTTP_req[i]=0; //null terminate the string
                    //respondToRequest(client, HTTP_req);
                    respondToRequest(client,HTTP_req);
                    break;
                }
                // every line of text received from the client ends with \r\n
                if (c == '\n') {
                    // last character on line of received text
                    // starting new line with next character read
                    currentLineIsBlank = true;
                } 
                else if (c != '\r') {
                    // a text character was received from client
                    currentLineIsBlank = false;
                }
            } // end if (client.available())
            else{
             client.stop(); 
            }
        } // end while (client.connected())
        delay(1);      // give the web browser time to receive the data
        client.stop(); // close the connection
    } // end if (client)
}

void respondToRequest(EthernetClient client, String & request){
    //String request = String(req);
    Serial.print("request: ");
    Serial.println(request);
    Serial.println("</request>");
    
    
    String findLED = String("blinkLED");
    String findAJAXJSON = "myajax";
    String findAJAXHTML = "ajaxtest";  
  
    int loc = request.indexOf(findLED);
    if( loc > -1){
      Serial.print("turning on or off b/c of ");
      Serial.print(findLED);
      Serial.print(" found at ");
      Serial.print(loc);
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/html");
      client.println("Connection: close");
      //client.println("Refresh: 1");
      client.println();
      client.println("<!DOCTYPE html>");
      client.println("<html>");
      client.println("<head>");
      client.println("<title>Arduino LED Control</title>");
      client.println("</head>");
      client.println("<body>");
      client.println("<h1>LED</h1>");
     /* if(request.indexOf("on=1") > -1){
         LED_status = 1;
          client.println("<h2>ON</h2>");
      }
      if(request.indexOf("on=0") > -1){
         LED_status = 0;
         client.println("<h2>OFF</h2>");
      }*/
      LED_status = -( LED_status-1);
    }
    else if(request.indexOf(findAJAXJSON) > -1){
      client.println("<data>hi</data>");
      client.println("<status>good</status>");
    }
    else if(request.indexOf(findAJAXHTML)>-1){
      char myString[] = "<!DOCTYPE html>"
                  "<html> "
                  "<head> "
                  "*<script src=\"http://ajax.googleapis.com/ajax/libs/jquery/1.11.2/jquery.min.js\"></script>"
                  "<script>"
                  "$(document).ready(function(){"
                  "    $(\"button\").click(function(){"
                  "        $.get(\"192.168.1.10/?myajax=1\", function(data, status){"
                  "            alert(\"Data: \" + data + \"\nStatus: \" + status);"
                  "        });"
                  "    });"
                  "});"
                  "</script>"
                  "</head>"
                  "<body>"
                  ""
                  "<button>Send an HTTP GET request to a page and get the result back</button>"
                  ""
                  "</body>"
                  "</html>";
        client.println(myString);
    }
    else{
           char myString[] = ""
                  "HTTP/1.1 200 OK\n"
                  "Content-Type: text/html\n"
                  "Connection: close\n"
                  "\n"
                   "<!DOCTYPE html>\n"
                  "<html> \n"
                  "<body>\n"
                  "\n"
                  "<h1> default result! </h1>\n"
                  "\n"
                  "</body>\n"
                  "</html>\n";
           client.print(myString);
           Serial.print(myString);
    }
   
}
/*
void processButton(EthernetClient cl){
      
    BTN_status = digitalRead(buttonPin);
    if (BTN_status == HIGH){
      cl.println("<h1>BUTTON IS BEING PRESSED</h1>");
    }
    else{
      cl.println("<h3> button not pressed </h3>");
    } 
    
}
void printSite(EthernetClient client){
  client.println("HTTP/1.1 200 OK");
client.println("Content-Type: text/html");
client.println("Connection: close");
client.println();
// send web page
client.println("<!DOCTYPE html>");
client.println("<html>");
client.println("<head>");
client.println("<title>Arduino LED Control</title>");
client.println("</head>");
client.println("<body>");
client.println("<h1>LED</h1>");
client.println("<p>Click to switch LED on and off.</p>");
client.println("<form method=\"get\">");
ProcessCheckbox(client);
client.println("</form>");

processButton(client);

 
client.println("</body>");
client.println("</html>");
Serial.print(HTTP_req);
HTTP_req = "";    // finished with request, empty string
}

void analyzeGETparams(EthernetClient cl){

  
}


// switch LED and send back HTML for LED checkbox
void ProcessCheckbox(EthernetClient cl)
{
    if (HTTP_req.indexOf("LED2=2") > -1) {  // see if checkbox was clicked
        // the checkbox was clicked, toggle the LED
        if (LED_status) {
            LED_status = 0;
        }
        else {
            LED_status = 1;
        }
    }
    
    
    
    
    if (LED_status) {    // switch LED on
        digitalWrite(redled, HIGH);
        // checkbox is checked
        cl.println("<input type=\"checkbox\" name=\"LED2\" value=\"2\" \
        onclick=\"submit();\" checked>LED2");
    }
    else {              // switch LED off
        digitalWrite(redled, LOW);
        // checkbox is unchecked
        cl.println("<input type=\"checkbox\" name=\"LED2\" value=\"2\" \
        onclick=\"submit();\">LED2");
    }
 
    
}
*/

