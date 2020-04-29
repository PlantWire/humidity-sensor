#include "Arduino.h"
#include "LoRa_E32.h"
#include "SPWL.h"
#include <sstream>

// ToDo(ckirchme): D2 or D4
// ToDo(ckirchme): LoRa_E32 e32ttl100(D2, D3, D5, D7, D6);
LoRa_E32 e32ttl100(D2, D3);
constexpr int fiveMinutes = 300000;

int raw = 0;
int Vin = 5;
float Vout = 0;
float R1 = 1000;
float R2 = 0;
float buffer = 0;

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
  raw = analogRead(A0);
  if (raw) {
    buffer = raw * Vin;
    Vout = (buffer)/1024.0;
    buffer = (Vin/Vout) -1;
    R2 = R1 * buffer;

    std::ostringstream ss;
    ss << R2;
    std::string s(ss.str());

    std::pair<SPWLPacket, bool> result = SPWLPacket::
        encapsulateData(std::vector<unsigned char>{s.cbegin(), s.cend()});
    if (result.second) {
      e32ttl100.sendMessage(result.first.rawData().data(),
          result.first.rawDataSize());
    }
  }
}
