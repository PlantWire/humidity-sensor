#include "Arduino.h"
#include "LoRa_E32.h"
#include "SPWL.h"
#include <pwire-sensor-lib.h>
#include <sstream>

// ToDo(ckirchme): D2 or D4
// ToDo(ckirchme): LoRa_E32 e32ttl100(D2, D3, D5, D7, D6);
LoRa_E32 e32ttl100(D2, D3);
constexpr int fiveMinutes = 300000;

void setup() {
  Serial.begin(9600);
  delay(500);
  pinMode(D7, OUTPUT);
  pinMode(D6, OUTPUT);
  pinMode(A0, INPUT);
  digitalWrite(D7, 0);
  digitalWrite(D6, 0);
  e32ttl100.begin();
  measure();
}

void loop() {
  delay(fiveMinutes);
  measure();
}

void measure() {
  int measurement = pwireSensorLib::calculateMeasurement(analogRead(A0));

  std::ostringstream ss;
  ss << measurement;
  std::string s(ss.str());

  std::pair<SPWLPacket, bool> result = SPWLPacket::
      encapsulateData(std::vector<unsigned char>{s.cbegin(), s.cend()});
  if (result.second) {
    e32ttl100.sendMessage(result.first.rawData().data(),
        result.first.rawDataSize());
  }
}
