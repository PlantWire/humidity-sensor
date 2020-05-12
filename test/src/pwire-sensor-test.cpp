#include <sstream>

#include "../cute/cute.h"
#include "../cute/cute_runner.h"
#include "../cute/ide_listener.h"
#include "../cute/xml_listener.h"

#include "../../lib/include/pwire-sensor-lib.h"
#include "../../spwl/lib/include/SPWL.h"

void maxMeasurementTest() {
  int result = pwireSensorLib::calculateMeasurement(1023);
  ASSERT_EQUALM("Max measurement not correct!", 99, result);
}

void minMeasurementTest() {
  int result = pwireSensorLib::calculateMeasurement(0);
  ASSERT_EQUALM("Min measurement not correct!", 0, result);
}

void almostMinMeasurementTest() {
  int result = pwireSensorLib::calculateMeasurement(1);
  ASSERT_EQUALM("Almost min measurement not correct!", 0, result);
}

void middleMeasurementTest() {
  int result = pwireSensorLib::calculateMeasurement(5);
  ASSERT_EQUALM("Middle measurement not correct!", 80, result);
}

void preambleCheckTest() {
  for (int i = 0; i < SPWLPacket::PREAMBLESIZE; i++) {
    ASSERT_EQUALM("Preamble check not correct!",
        pwireSensorLib::checkPreamble(i, SPWLPacket::PREAMBLE[i]), true);
  }
}

void getHeaderTest() {
  SPWLHeader input{};
  input.dataSize = 17;
  input.senderAddress = 1;
  input.channel = 23;
  input.last = true;

  SPWLPacket::HeaderContainer rawHeader =
      SPWLPacket::getRawFromHeader(input);
  SPWLHeader result =
      pwireSensorLib::getHeader(static_cast<uint8_t *>(rawHeader.data()));

  ASSERT_EQUALM("DataSize not correct!", 17, result.dataSize);
  ASSERT_EQUALM("SenderAddress not correct!", 1, result.senderAddress);
  ASSERT_EQUALM("Channel not correct!", 23, result.channel);
  ASSERT_EQUALM("Last value not correct!", true, result.last);
}

void getPacketTest() {
  SPWLPacket::DataContainer input {'H', 'e', 'l', 'l', 'o'};
  std::pair<SPWLPacket, bool> res = SPWLPacket::encapsulateData(input);
  ASSERTM("Result is missing.", res.second);
  SPWLPacket::PacketContainer raw = res.first.rawData();
  SPWLPacket::HeaderContainer rawHeader{};
  std::copy(raw.cbegin() + SPWLPacket::PREAMBLESIZE,
      raw.cbegin() + SPWLPacket::PREAMBLESIZE + SPWLPacket::HEADERSIZE,
      rawHeader.begin());
  SPWLHeader header = SPWLPacket::getHeaderFromRaw(rawHeader);
  ASSERT_EQUALM("DataSize not correct!", 5, header.dataSize);
  SPWLPacket::DataContainer data{};
  std::copy(raw.cbegin() + SPWLPacket::PREAMBLESIZE + SPWLPacket::HEADERSIZE,
      raw.cbegin() + SPWLPacket::PREAMBLESIZE + SPWLPacket::HEADERSIZE
      + header.dataSize + SPWLPacket::CHECKSUMSIZE,
      std::back_inserter(data));

  // The test itself
  SPWLPacket::PacketContainer result =
      pwireSensorLib::getPacket(header, data.data());
  res = SPWLPacket::encapsulatePacket(result);
  ASSERTM("Result is missing.", res.second);
  ASSERT_EQUALM("Data not correct!", input, res.first.getData());
}

bool runAllTests(int argc, char const *argv[]) {
  cute::suite s;
  s.push_back(CUTE(maxMeasurementTest));
  s.push_back(CUTE(minMeasurementTest));
  s.push_back(CUTE(almostMinMeasurementTest));
  s.push_back(CUTE(middleMeasurementTest));
  s.push_back(CUTE(preambleCheckTest));
  s.push_back(CUTE(getHeaderTest));
  s.push_back(CUTE(getPacketTest));
  cute::xml_file_opener xmlfile(argc, argv);
  cute::xml_listener<cute::ide_listener<>> lis(xmlfile.out);
  bool success = cute::makeRunner(lis, argc, argv)(s, "AllTests");
  return success;
}

int main(int argc, char const *argv[]) {
  return runAllTests(argc, argv) ? EXIT_SUCCESS : EXIT_FAILURE;
}
