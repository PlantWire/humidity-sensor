#include <Arduino.h>
#include <LoRa_E32.h>
#include <pwire-sensor-lib.h>

LoRa_E32 e32ttl100(D2, D3);

void setup() {
  Serial.begin(9600);
  delay(500);
  e32ttl100.begin();
  Serial.println("Hi, I'm going to send message!");
  ResponseStatus rs = e32ttl100.sendMessage("Hello, world?");
  Serial.println(rs.getResponseDescription());
}

void loop() {
  if (e32ttl100.available() > 1) {
    ResponseContainer rc = e32ttl100.receiveMessage();
    if (rc.status.code != 1) {
      rc.status.getResponseDescription();
    } else {
      Serial.println(rc.data);
    }
  }
  if (Serial.available()) {
    String input = Serial.readString();
    e32ttl100.sendMessage(input);
  }
}
