#include <SPI.h>
#include <LoRa.h>

int counter = 0;
int ldr=16;//Set A0(Analog Input) for LDR.
int value=0;
int flag = 0;
#define ss 5
#define rst 14
#define dio0 2
void setup() {
  Serial.begin(115200);
//  while (!Serial);

  Serial.println("LoRa Sender");
  LoRa.setPins(ss, rst, dio0);
  while (!LoRa.begin(868E6)) {
    Serial.println(".");
    delay(500);
  }
  LoRa.setSyncWord(0xF3);
  Serial.println("LoRa Initializing OK!");
    pinMode(ldr,INPUT);
}

void loop() {
 // Serial.print("Sending packet : ");
  //Serial.println(counter);
value=digitalRead(ldr);//Reads the Value of LDR(light).
Serial.println("LDR value is :");//Prints the value of LDR to Serial Monitor.
Serial.println(value);
   LoRa.beginPacket(); // send packet
   /////////////////
if (value >=1 && flag == 0) {

Serial.println("LDR is DARK");
LoRa.print("0");
flag = 1;
}
else if (value <=0 && flag == 1)
{

Serial.println("LDR is BRIGHT");
LoRa.print("1");
flag = 0;
}
//////////////
 //LoRa.print("LDR Value is: ");
 //LoRa.print(value);
  //LoRa.print(counter);
 LoRa.endPacket();
   }
//   }
