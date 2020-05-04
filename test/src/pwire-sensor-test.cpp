#include <sstream>

#include "../cute/cute.h"
#include "../cute/cute_runner.h"
#include "../cute/ide_listener.h"
#include "../cute/xml_listener.h"

#include "../../lib/include/pwire-sensor-lib.h"

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

bool runAllTests(int argc, char const *argv[]) {
  cute::suite s;
  s.push_back(CUTE(maxMeasurementTest));
  s.push_back(CUTE(minMeasurementTest));
  s.push_back(CUTE(almostMinMeasurementTest));
  s.push_back(CUTE(middleMeasurementTest));
  cute::xml_file_opener xmlfile(argc, argv);
  cute::xml_listener<cute::ide_listener<>> lis(xmlfile.out);
  bool success = cute::makeRunner(lis, argc, argv)(s, "AllTests");
  return success;
}

int main(int argc, char const *argv[]) {
  return runAllTests(argc, argv) ? EXIT_SUCCESS : EXIT_FAILURE;
}
