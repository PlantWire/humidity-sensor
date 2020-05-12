#ifndef LIB_INCLUDE_PWIRE_SENSOR_LIB_H_
#define LIB_INCLUDE_PWIRE_SENSOR_LIB_H_

#include <iosfwd>
#include <cstdint>
#include "../../spwl/lib/include/SPWL.h"

namespace pwireSensorLib {
  int calculateMeasurement(const int input);
  bool checkPreamble(const int count, const uint8_t value);
  SPWLHeader getHeader(const uint8_t * data);
  SPWLPacket::PacketContainer getPacket(const SPWLHeader header,
      const uint8_t * dataWithChecksum);
}
#endif /* LIB_INCLUDE_PWIRE_SENSOR_LIB_H_ */
