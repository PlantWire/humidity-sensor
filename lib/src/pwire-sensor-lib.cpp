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
}  // namespace pwireSensorLib
