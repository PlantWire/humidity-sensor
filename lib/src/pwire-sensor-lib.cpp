#include "./pwire-sensor-lib.h"

namespace {
  static constexpr int VOLTAGE_IN = 5;
  static constexpr int REFERENCE_RESISTOR = 1000;
  static constexpr int MAXVALUE = 1024 * REFERENCE_RESISTOR;

  float calculateResistor(const int input) {
    float voltageOut = 0;
    float unknownResistor = 0;
    float buffer = 0;

    if (input == 0) {
      return MAXVALUE;
    }

    buffer = input * VOLTAGE_IN;
    voltageOut = buffer / 1024.0;
    buffer = (VOLTAGE_IN / voltageOut) - 1;
    unknownResistor = REFERENCE_RESISTOR * buffer;
    return unknownResistor;
  }
}  // namespace

namespace pwireSensorLib {
  int calculateMeasurement(const int input) {
    static constexpr float ONE_PERCENT = MAXVALUE / 100.0;
    return static_cast<int>(100 - (calculateResistor(input) / ONE_PERCENT));
  }

  bool checkPreamble(const int count, uint8_t value) {
    return SPWLPacket::PREAMBLE[count] == value;
  }

  SPWLHeader getHeader(const uint8_t * data) {
    std::array<unsigned char, SPWLPacket::HEADERSIZE> rawHeader{};
    for (size_t i = 0; i < SPWLPacket::HEADERSIZE; i++) {
      rawHeader.at(i) = data[i];
    }
    return SPWLPacket::getHeaderFromRaw(rawHeader);
  }

  SPWLPacket::PacketContainer getPacket(const SPWLHeader header,
      const uint8_t * dataWithChecksum) {
    int currentPacketIndex = 0;
    SPWLPacket::PacketContainer result{};
    SPWLPacket::HeaderContainer rawHeader =
        SPWLPacket::getRawFromHeader(header);
    for (size_t i = 0; i < SPWLPacket::PREAMBLESIZE; i++) {
      result.at(i) = SPWLPacket::PREAMBLE[i];
      currentPacketIndex++;
    }
    std::copy(rawHeader.cbegin(), rawHeader.cend(),
              result.begin() + currentPacketIndex);
    currentPacketIndex += SPWLPacket::HEADERSIZE;
    for (size_t i = 0; i < header.dataSize + SPWLPacket::CHECKSUMSIZE; i++) {
      result.at(currentPacketIndex) = dataWithChecksum[i];
      currentPacketIndex++;
    }
    result.at(currentPacketIndex) = SPWLPacket::TRAILER;
    return result;
  }
}  // namespace pwireSensorLib
