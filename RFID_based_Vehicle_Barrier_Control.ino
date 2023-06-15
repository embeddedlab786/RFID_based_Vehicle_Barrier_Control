#include "MFRC522.h"

#define RST_PIN 9
#define SS_PIN 10

MFRC522 mfrc522(SS_PIN, RST_PIN);

byte readCard[4];
String tagID = "";

int check=0;
const int Set=3;
String Tag[Set] = {"ID1","ID2","ID3"};

#define g_led 2 // Green LED Pin
#define y_led 3 // Yellow LED Pin
#define r_led 4 // Red LED Pin

#define servo 7

void setup() {
Serial.begin(9600);// Initiating
SPI.begin(); // SPI bus
mfrc522.PCD_Init(); // MFRC522

pinMode(g_led, OUTPUT);
pinMode(y_led, OUTPUT);
pinMode(r_led, OUTPUT);

pinMode(servo, OUTPUT);
servoPulse(servo, 30); 

digitalWrite(g_led, 0);
digitalWrite(y_led, 0);
digitalWrite(r_led, 1);

delay(500);
}

void loop() {//Wait until new tag is available
  while(getID()){    
    Serial.println(tagID);
    check = 0;  
    for (int i=0; i<Set; i++){
    if(tagID == Tag[i]){check=1;
      digitalWrite(g_led, 1);
      digitalWrite(y_led, 0);
      digitalWrite(r_led, 0);
      for(int angle=30; angle<=140; angle++){servoPulse(servo, angle);}
      delay(5000);
      digitalWrite(g_led, 0);
      digitalWrite(y_led, 1);
      digitalWrite(r_led, 0);
      delay(1000);
      for(int angle=140; angle>=30; angle--){servoPulse(servo, angle);}
      i=Set;
     }
    }
  
if(check==0){
  for(int x=0; x<10; x++){
digitalWrite(g_led, 0);
digitalWrite(y_led, 0);
digitalWrite(r_led, 0);
delay(500);  
digitalWrite(y_led, 1);
delay(500); 
  }  
}
      
digitalWrite(g_led, 0);
digitalWrite(y_led, 0);
digitalWrite(r_led, 1);
}
delay(10);
}

//Read new tag if available
boolean getID(){// Getting ready for Reading PICCs
  if ( ! mfrc522.PICC_IsNewCardPresent()) { //If a new PICC placed to RFID reader continue
  return false;
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) { //Since a PICC placed get Serial and continue
  return false;
  }
  tagID = "";
  for ( uint8_t i = 0; i < 4; i++) { // The MIFARE PICCs that we use have 4 byte UID
  //readCard[i] = mfrc522.uid.uidByte[i];
  tagID.concat(String(mfrc522.uid.uidByte[i], HEX)); // Adds the 4 bytes in a single String variable
  }
  tagID.toUpperCase();
  mfrc522.PICC_HaltA(); // Stop reading
  return true;
}

void servoPulse (int pin, int angle){
int pwm = (angle*11) + 500;      // Convert angle to microseconds
 digitalWrite(pin, HIGH);
 delayMicroseconds(pwm);
 digitalWrite(pin, LOW);
 delay(30);                   // Refresh cycle of servo
}
