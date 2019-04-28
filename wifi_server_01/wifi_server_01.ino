#include <SPI.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
byte ledPin = 2;
char ssid[] = "";               // SSID of your home WiFi
char pass[] = "";               // password of your home WiFi
char ssid1[] = "SAM";               // SSID of your home WiFi
char pass1[] = "1234556789";               // password of your home WiFi
WiFiServer server(80);    
ESP8266WebServer htmlserver(80);                //Webserver Object
String mymessage = "";                
int timeout=0;

IPAddress ip(192, 168, 1, 198);            // IP address of the server
IPAddress gateway(192,168,1,199);           // gateway of your network
IPAddress subnet(255,255,255,0);          // subnet mask of your network

const char * indexMsg = "<!DOCTYPE html>"
"<html>"
"<head>"
"<title>AQMS Control HUB</title>"
"<style>"
"* {"
"    box-sizing: border-box;"
"}"
"header, footer {"
"    padding: 0.5em;"
"    color: white;"
"    background-color: black;"
"    clear: right;"
"    text-align: center;"
"}"
".onbutton {"
"    background-color: #4CAF50;"
"    border: none;"
"    color: white;"
"    padding: 15px 32px;"
"    text-align: center;"
"    text-decoration: none;"
"    display: inline-block;"
"    font-size: 20px;"
"    margin: 4px 2px;"
"    cursor: pointer;"
"}"
".offbutton {"
"    background-color: red;"
"    border: none;"
"    color: white;"
"    padding: 15px 32px;"
"    text-align: center;"
"    text-decoration: none;"
"    display: inline-block;"
"    font-size: 20px;"
"    margin: 4px 2px;"
"    cursor: pointer;"
"}"
"</style>"
"</head>"
"<header>"
"   <h1>AQMS Control HUB</h1>"
"</header>"
"<br>"
"<div style=""width: 100%;"">"
"<div style=""float:left; width:50%"">"
"<body>"
"<form method=""post"">"
"<font size=""6""><b>"
"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Host 1<br>"
"<button class=onbutton name=""Connect1"">Connect</button>&nbsp;"
"<button class=offbutton name=""Disconnect1"">Disconnect</button><br>"
"<font size=""6""><b>"
"Status:"
"</font></b>"
"</font>"
"<font size=""4"">"
"<br>Temperature Reading : <br>"
"Humidity level :<br>"
"CO2 levels : <br>"
"</font>"
"</form>"
"</div>"
"</div style=""float:right; width:50%"">"
"<body>"
"<form method=""post"">"
"<font size=""6""><b>"
"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Host 2<br>"
"<button class=onbutton name=""Connect2"">Connect</button>&nbsp;"
"<button class=offbutton name=""Disconnect2"">Disconnect</button><br>"
"<font size=""6""><b>"
"Status:"
"<b>"
"</font>"
"<font size=""4"">"
"<br>Temperature Reading : <br>"
"Humidity level :<br>"
"CO2 levels : <br>"
"</font>"
"</form>"
"</div>"
"<br><br><br><br><br>"
"<footer >TE IT B 11,17,21</footer>"
"</body>"
"</html>"

;


void setup() {
  Serial.begin(115200);                   // only for debug
  WiFi.config(ip, gateway, subnet);       // forces to use the fix IP
  WiFi.begin(ssid, pass);                 // connects to the WiFi router
 
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
    timeout++;
    if(timeout > 15){
      break;
    }
  }
  if( timeout > 15 ){
     Serial.print("\n\nCouldn't connect to ");
     Serial.println(ssid);
     Serial.print("Connecting to ");
     Serial.print(ssid1);
     IPAddress ip1(192, 168, 43, 80);            // IP address of the server
     IPAddress gateway1(192,168,43,1);           // gateway of your network
     IPAddress subnet1(255,255,255,0);          // subnet mask of your network
     WiFi.config(ip1, gateway1, subnet1);       // forces to use the fix IP
     WiFi.begin(ssid1, pass1);                 // connects to the SAM
     
      while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
      }
  }
  server.begin();                         // starts the server
  Serial.println("\nConnected to wifi");
//############# Print the IP address ##################
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP()); //Gets the WiFi shield's IP address and Print the IP address of serial monitor
  Serial.println("/");
//=====================================================
  //Serial.print("Status: "); Serial.println(WiFi.status());  // some parameters from the network
  //Serial.print("IP: ");     Serial.println(WiFi.localIP());
  //Serial.print("Subnet: "); Serial.println(WiFi.subnetMask());
  //Serial.print("Gateway: "); Serial.println(WiFi.gatewayIP());
  //Serial.print("SSID: "); Serial.println(WiFi.SSID());
  //Serial.print("Signal: "); Serial.println(WiFi.RSSI());
  //Serial.print("Networks: "); Serial.println(WiFi.scanNetworks());
  pinMode(ledPin, OUTPUT);

//############## Start the server #####################
  htmlserver.on("/", handleindex);    //Associate the handler function to the path
  htmlserver.begin();
  Serial.println("Server Listening");
//=====================================================
}

void loop () {
  htmlserver.handleClient();                    //Handling of incoming requests
  WiFiClient client = server.available();
  if (client) {
    if (client.connected()) {
      digitalWrite(ledPin, LOW);  // to show the communication only (inverted logic)
      Serial.println("\n...................................\n");
      String request = client.readStringUntil('#');    // receives the message from the client
     // Serial.print("From client: "); 
      Serial.println(request);
      String request1 = client.readStringUntil('$');
      Serial.println(request1);
      client.flush();
    //  client.println("Hi client! No, I am listening.\r"); // sends the answer to the client
      digitalWrite(ledPin, HIGH);
    }
    client.stop();                // tarminates the connection with the client
  }
}

void handleindex() {            //Handler for the root path
  htmlserver.send(200, "text/html", indexMsg);
} 
