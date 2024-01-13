#include "Filter.h"
#include "LoadCsv.h"
#include <iostream>
#include <limits>

int main() {

  // first we test the csv loading
  auto testCsv = loadCsv("../Test/test_csv_load.csv");
  if (std::abs(testCsv[0] + 1.0) > std::numeric_limits<float>::epsilon()) {
    std::cerr << "first test csv value is wrong: " << testCsv[0] << std::endl;
    return 1;
  }

  if (std::abs(testCsv[1] - 2.0) > std::numeric_limits<float>::epsilon()) {
    std::cerr << "second test csv value is wrong: " << testCsv[1] << std::endl;
    return 1;
  }

  if (std::abs(testCsv[2] + 3.0) > std::numeric_limits<float>::epsilon()) {
    std::cerr << "third test csv value is wrong: " << testCsv[2] << std::endl;
    return 1;
  }

  // this is our pink filter
  Filter pinkToWhite(
      std::vector<float>{0.049922035, -0.095993537, 0.050612699, -0.004408786},
      std::vector<float>{1, -2.494956002, 2.017265875, -0.522189400});
  pinkToWhite.setOutputGainDB(-34);

  // we load the test data for our pink filter
  auto pinkToWhiteInputData = loadCsv("../Test/test_2_pink_to_white_input.csv");
  auto outputData = pinkToWhiteInputData;
  // we filter the damn signal
  for (int i = 0; i < outputData.size(); i++) {
    outputData[i] = pinkToWhite.filter(pinkToWhiteInputData[i]);
  }

  float epsilonLevel = 1000 * std::numeric_limits<float>::epsilon();
  std::cout << "Testing audio with epsilon level of: " << epsilonLevel
            << std::endl;

  // now we load the result data and compare to what R produced
  auto pinkToWhiteTestResult =
      loadCsv("../Test/test_2_pink_to_white_output.csv");
  for (int i = 0; i < pinkToWhiteTestResult.size(); i++) {
    if (std::abs(pinkToWhiteTestResult[i] - outputData[i]) > epsilonLevel) {
      std::cerr << "Sample difference at index " << i << ": expected "
                << pinkToWhiteTestResult[i] << " got " << outputData[i]
                << std::endl;
      return 1;
    }
  }

  return 0;
}