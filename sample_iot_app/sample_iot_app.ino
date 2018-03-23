#include <SoftwareSerial.h>

#define DEBUG true

// Use digital I/O pin 8 as Rx, digital I/O pin 9 as Tx.
SoftwareSerial esp8266_dev(8,9);

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  Serial.begin(9600);
  esp8266_dev.begin(9600);

  Serial.println("Resetting the WiFi module.");
  writeToEsp8266("AT+RST\r\n", 2000, DEBUG);

  Serial.println("Setting the WiFi module in AP mode.");
  writeToEsp8266("AT+CWMODE=2\r\n", 1000, DEBUG);

  Serial.println(">>> Now, you will be able to see new WiFi Access point. We need to connect to this AP to control the LED on Arduino Nano.");

  Serial.println("Getting the IP address of the WiFi module.");
  String ip_addr = writeToEsp8266("AT+CIFSR\r\n", 1000, DEBUG);

  Serial.println("Configuring the WiFi module to accept multiple connections.");
  writeToEsp8266("AT+CIPMUX=1\r\n", 1000, DEBUG);

  Serial.println("Starting HTTP Server.");
  writeToEsp8266("AT+CIPSERVER=1,80\r\n", 1000, DEBUG);

  Serial.println("Update this ip address \"" + ip_addr + "\" in the index.html file.");
}

void loop() {
  if(esp8266_dev.available() > 0) 
  {
    if(esp8266_dev.find("+IPD,"))
    {
      delay(1000);

      int conn_id = esp8266_dev.read() - 48;
      
      if (esp8266_dev.find("led=on")) {
        digitalWrite(LED_BUILTIN, HIGH);
        Serial.println("LED ON");
      } else if(esp8266_dev.find("led=off")) {
        digitalWrite(LED_BUILTIN, LOW);
        Serial.println("LED OFF");
      } else {
        Serial.println("Invalid HTTP request received."); 
      }

      String closeCmd = "AT+CIPCLOSE=";
      closeCmd += conn_id;
      closeCmd += "\r\n";

      writeToEsp8266(closeCmd, 1000, DEBUG);
    }
  }
}

String writeToEsp8266(String cmd, const int timeout, bool debug) {
  String resp = "";

  esp8266_dev.print(cmd);

  long int time = millis();

  while ( (time + timeout) > millis()) {
    while(esp8266_dev.available() > 0) {
      char ch = esp8266_dev.read();
      resp += ch;
    }
  }

  if(debug) {
    Serial.print(resp);
  }

  return resp;
}
