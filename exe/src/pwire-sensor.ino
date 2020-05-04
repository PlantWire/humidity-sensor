#include "Arduino.h"
#include "LoRa_E32.h"
#include "SPWL.h"
#include <pwire-sensor-lib.h>  // ToDO(ckirchme): linter error with "
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
  std::string value(ss.str());

  std::string message {
      "{"
        "\"Type\": \"HumidityMeasurementResponse\","
        "\"Target\": \"fe2c15fc-85d2-4691-be70-f4adb326a334\","
        "\"Sender\": \"575ee151-23ee-4c8b-8c09-1a5c010d516f\","
        "\"Content\": {"
          "\"Value\":" + value +
        "}"
      "}"};

  std::pair<SPWLPacket, bool> result = SPWLPacket::
      encapsulateData(std::vector<unsigned char>{message.cbegin(),
                                                 message.cend()});
  if (result.second) {
    e32ttl100.sendMessage(result.first.rawData().data(),
        result.first.rawDataSize());
  }
}
