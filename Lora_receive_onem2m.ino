#include <SPI.h>
#include <LoRa.h>
#include <HTTPClient.h>
#include <WiFi.h>
#include <time.h>

// Define the pins used by the transceiver module
#define ss 5
#define rst 14
#define dio0 2

#define MAIN_SSID "SCRC_LAB_IOT"    // Replace with your WiFi network name
#define MAIN_PASS "Scrciiith@123"   // Replace with your WiFi password
#define OM2M_Node_ID "WN-L026-01"
#define CSE_IP "dev-onem2m.iiit.ac.in"
#define HTTPS true
#define CSE_PORT 443
#define OM2M_ORGIN "Tue_20_12_22:Tue_20_12_22"
#define OM2M_MN "/~/in-cse/in-name/"
#define OM2M_AE "AE-WN"
#define OM2M_DATA_CONT "/Status"

HTTPClient http;
WiFiClient client;

const String server = "http://" + String(CSE_IP) + ":" + String(CSE_PORT) + OM2M_MN;
const String path = server + OM2M_AE + "/" + OM2M_Node_ID + OM2M_DATA_CONT;

void setup() {
  Serial.begin(115200);
  while (!Serial);
  Serial.println("LoRa Receiver");

  LoRa.setPins(ss, rst, dio0);
  
  while (!LoRa.begin(868E6)) {
    Serial.println(".");
    delay(500);
  }
  LoRa.setSyncWord(0xF3);
  Serial.println("LoRa Initializing OK!");

  WiFi.mode(WIFI_STA);
  delay(500);
  WiFi.begin(MAIN_SSID, MAIN_PASS);
  unsigned long startAttemptTime = millis();

  while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 10000) {
    delay(500);
    Serial.print("#");
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Connected to WiFi");
  } else {
    Serial.println("Failed to connect to WiFi");
  }
}

void sendToOneM2M(const String& data) {
  http.begin(path);
  http.addHeader("X-M2M-Origin", OM2M_ORGIN);
  http.addHeader("Content-Type", "application/json;ty=4");

  String req_data = "{\"m2m:cin\": {\"con\": \"" + data + "\",\"lbl\": \"Node-1\",\"cnf\": \"text\"}}";
  int code = http.POST(req_data);
  http.end();

  if (code == 201) {
    Serial.println("Data Posted Successfully");
  } else {
    Serial.println("Data posting failed with HTTP code: " + String(code));
  }
}

void loop() {
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    Serial.print("Received packet '");

    String LoRaData;
    while (LoRa.available()) {
      LoRaData = LoRa.readString();
      Serial.print(LoRaData);
    }

    if (LoRaData == "1") {
      sendToOneM2M("L026OFF");
    } else {
      sendToOneM2M("L026ON");
    }

    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());
  }
}
