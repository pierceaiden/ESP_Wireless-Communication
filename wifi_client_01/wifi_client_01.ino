/*  Connects to the home WiFi network
 *  Asks some network parameters
 *  Sends and receives message from the server in every 2 seconds
 *  Communicates: wifi_server_01.ino
 */ 
#include <dht.h>
#include <SPI.h>
#include <ESP8266WiFi.h>

dht DHT;
byte ledPin = 2;
char ssid[] = "";           // SSID of your home WiFi
char pass[] = "";            // password of your home WiFi
char ssid1[] = "SAM";               // SSID of your home WiFi
char pass1[] = "1234556789";
const int dhtPin = 2;
int timeout=0;
unsigned long askTimer = 0;
int serverconn = 0;
IPAddress server(192,168,1,198);       // the fix IP address of the server
IPAddress server1(192,168,43,80);       // the fix IP address of the server

WiFiClient client;

void setup() {
  Serial.begin(115200);  // only for debug
  pinMode(dhtPin , INPUT);
  WiFi.begin(ssid, pass);             // connects to the WiFi router
  
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
     serverconn = 1;
     WiFi.begin(ssid1, pass1);                 // connects to the SAM
     while (WiFi.status() != WL_CONNECTED) {
       Serial.print(".");
       delay(500);
     }
  }
  Serial.println("\nConnected to wifi");
  Serial.print("Status: "); Serial.println(WiFi.status());    // Network parameters
  Serial.print("IP: ");     Serial.println(WiFi.localIP());
  Serial.print("Subnet: "); Serial.println(WiFi.subnetMask());
  Serial.print("Gateway: "); Serial.println(WiFi.gatewayIP());
  Serial.print("SSID: "); Serial.println(WiFi.SSID());
  Serial.print("Signal: "); Serial.println(WiFi.RSSI());
  pinMode(ledPin, OUTPUT);
}

void loop () {
  if( serverconn == 1){
    client.connect(server1, 80);
  }
  else{
    client.connect(server, 80);
  }
 // client.connect(server, 80);   // Connection to the server
  digitalWrite(ledPin, LOW);    // to show the communication only (inverted logic)
  int value = DHT.read11(dhtPin);
//  Serial.println(".");
//  client.println("Hello server! Client ONE");  // sends the message to the server
//================================================
  Serial.print("Temperature = ");
  Serial.println(DHT.temperature);
  Serial.print("Humidity = ");
  Serial.println(DHT.humidity);
//================================================
  client.print("Temperature = ");
  client.println(DHT.temperature);
  client.print("#");
  client.print("Humidity = ");
  client.print(DHT.humidity);
  client.print("$");
 // String answer = client.readStringUntil('.');   // receives the answer from the sever
 // Serial.println("from server: " + answer);
  DHT.temperature = 0;
  DHT.humidity = 0;
  client.flush();
  digitalWrite(ledPin, HIGH);
  delay(1000);                  // client will trigger the communication after two seconds
}
