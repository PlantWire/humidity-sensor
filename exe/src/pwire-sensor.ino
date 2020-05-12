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
  // Unfortunately due to hardware problems receiving data is not possible
  // readSPWLPacket();
  // This code is a simple replacement with a fixed 5 minute schedule
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

void processPacket(SPWLPacket input) {
  // Valid packet ... JSON deserialization etc.
  measure();
}

int preambleCount = 0;
void readSPWLPacket() {
  while (preambleCount < SPWLPacket::PREAMBLESIZE) {
    if (e32ttl100.available() > 1) {
      ResponseStructContainer rc = e32ttl100.receiveMessage(1);
      if (rc.status.code != 1) {
        rc.status.getResponseDescription();
      } else {
        if (pwireSensorLib::checkPreamble(preambleCount,
            *static_cast<uint8_t *>(rc.data))) {
          preambleCount++;
        } else {
          preambleCount = 0;
        }
      }
    }
  }
  while (e32ttl100.available() < SPWLPacket::HEADERSIZE) {}
  ResponseStructContainer rc = e32ttl100.receiveMessage(SPWLPacket::HEADERSIZE);
  String error{};
  if (rc.status.code != 1) {
    error = rc.status.getResponseDescription();
    Serial.println(error);
  } else {
    SPWLHeader header =
        pwireSensorLib::getHeader(static_cast<uint8_t *>(rc.data));
    if (header.dataSize > 0 && header.dataSize < SPWLPacket::MAXDATASIZE) {
      while (e32ttl100.available() < header.dataSize +
          SPWLPacket::CHECKSUMSIZE) {}
      rc = e32ttl100.receiveMessage(header.dataSize + SPWLPacket::CHECKSUMSIZE);
      if (rc.status.code != 1) {
        error = rc.status.getResponseDescription();
        Serial.println(error);
      } else {
        SPWLPacket::PacketContainer packet =
            pwireSensorLib::getPacket(header, static_cast<uint8_t *>(rc.data));
        std::pair<SPWLPacket, bool> res = SPWLPacket::encapsulatePacket(packet);
        if (res.second) {
          processPacket(res.first);
        } else {
          error = "Invalid packet received";
          Serial.println(error);
        }
      }
    } else {
      error = "Invalid packet received";
      Serial.println(error);
    }
  }
  preambleCount = 0;
}

