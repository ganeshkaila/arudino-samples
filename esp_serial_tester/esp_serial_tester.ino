#include <SoftwareSerial.h>

// Use digital I/O pin 8 as Rx, digital I/O pin 9 as Tx.
SoftwareSerial esp8266_dev(8,9);

void setup() {
  Serial.begin(9600);
  esp8266_dev.begin(9600);
  Serial.println("A testing sample for ESP8266 Module serial communication.");
  Serial.println("Enter the following commands and hit enter button.");
  Serial.println("1. AT;                It should return with \"OK\"");
  Serial.println("2. AT+CWLAP;          It should return the available Access Points.");
}

void loop() {
  while(esp8266_dev.available() > 0) 
  {
    char ch = esp8266_dev.read();

    if(ch == '\0')
      continue;

    if(ch != '\r' && ch != '\n' && (ch < 32))
      continue;

    Serial.print(ch);
  }

  while(Serial.available() > 0) 
  {
    char ch = Serial.read();
    Serial.write(ch);
    esp8266_dev.write(ch);
  }
}
