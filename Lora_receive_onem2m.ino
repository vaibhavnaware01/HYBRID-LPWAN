/*********
  Modified from the examples of the Arduino LoRa library
  More resources: https://randomnerdtutorials.com
*********/

#include <SPI.h>
#include <LoRa.h>
#include <HTTPClient.h>
#include <WiFi.h>
#include <time.h>
//define the pins used by the transceiver module
#define ss 5
#define rst 14
#define dio0 2

int keyIndex = 0; // your network key Index number (needed only for WEP)
WiFiClient client;
#define MAIN_SSID "SCRC_LAB_IOT"    // replace MySSID with your WiFi network name
#define MAIN_PASS "Scrciiith@123"  // replace MyPassword with your WiFi password
#define OM2M_Node_ID "WN-L026-01"
//OM2M Components
//#define CSE_IP "onem2m.iiit.ac.in"
#define CSE_IP "dev-onem2m.iiit.ac.in"
#define HTTPS   true
#define CSE_PORT 443
//#define OM2M_ORGIN "WisunMon@20:5T&6OnuL1iZ"
//#define OM2M_ORGIN "wisun:wisunmon"
#define OM2M_ORGIN "Tue_20_12_22:Tue_20_12_22"
#define OM2M_MN "/~/in-cse/in-name/"
#define OM2M_AE "AE-WN"
#define OM2M_DATA_CONT "/Status"
// HTTP Client Object
HTTPClient http;
// Server URL
String server = "http://" + String() + CSE_IP + ":" + String() + CSE_PORT + String() + OM2M_MN;
String path = server + String() + OM2M_AE + "/" + String()+OM2M_Node_ID + String()+OM2M_DATA_CONT ;
void setup() {
  //initialize Serial Monitor
  Serial.begin(115200);
  while (!Serial);
  Serial.println("LoRa Receiver");

  //setup LoRa transceiver module
  LoRa.setPins(ss, rst, dio0);
  
  //replace the LoRa.begin(---E-) argument with your location's frequency 
  //433E6 for Asia
  //866E6 for Europe
  //915E6 for North America
  while (!LoRa.begin(868E6)) {
    Serial.println(".");
    delay(500);
  }
   // Change sync word (0xF3) to match the receiver
  // The sync word assures you don't get LoRa messages from other LoRa transceivers
  // ranges from 0-0xFF
  LoRa.setSyncWord(0xF3);
  Serial.println("LoRa Initializing OK!");
  WiFi.mode(WIFI_STA);
  delay(500);
    WiFi.begin(MAIN_SSID, MAIN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("#");
  }
  Serial.println("connected to WiFI");
}

void loop() {
  // try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // received a packet
    Serial.print("Received packet '");

    // read packet
    while (LoRa.available()) {
      String LoRaData = LoRa.readString();
      Serial.print(LoRaData); 
      if (LoRaData == "1")
      {
        const String oncommand= "L026OFF";
          http.begin(path);
  Serial.println(path);
  // Required Headers
  http.addHeader("X-M2M-Origin", OM2M_ORGIN);
  http.addHeader("Content-Type", "application/json;ty=4");
   String data =  oncommand;
  Serial.println(data);
  Serial.println("Onem2m");
  String req_data = String() + "{\"m2m:cin\": {"
    +
    "\"con\": \"" + data + "\","
    +
    "\"lbl\": \"" + "Node-1" + "\","
    +
    "\"cnf\": \"text\""
    +
    "}}";
  int code = http.POST(req_data);
  http.end();
  if (code == 201) {
    Serial.println("Data Posted Successfully");
  } else {
    Serial.println("Data posting failed with http code-" + String(code));
  }
      }

      else
      {
        const String offcommand= "L026ON";
          http.begin(path);
  Serial.println(path);
  // Required Headers
  http.addHeader("X-M2M-Origin", OM2M_ORGIN);
  http.addHeader("Content-Type", "application/json;ty=4");
   String data =   offcommand;
  Serial.println(data);
  Serial.println("Onem2m");
  String req_data = String() + "{\"m2m:cin\": {"
    +
    "\"con\": \"" + data + "\","
    +
    "\"lbl\": \"" + "Node-1" + "\","
    +
    "\"cnf\": \"text\""
    +
    "}}";
  int code = http.POST(req_data);
  http.end();
  if (code == 201) {
    Serial.println("Data Posted Successfully");
  } else {
    Serial.println("Data posting failed with http code-" + String(code));
  }
      }
    }
    // print RSSI of packet
    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());
  }
}
