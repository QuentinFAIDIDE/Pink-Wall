#pragma once

#include <vector>

/**
 * @brief Class that describes and implements a mono IIR filter
 * behaviour. For optimization purpose, it only support order 4 filters.
 */
class Filter {
public:
  /**
   * @brief Construct a new Filter object with the given coefficients.
   *
   * @param ai feedback coefficients
   * @param bi feedforward coefficients
   */
  Filter(std::vector<float> ai, std::vector<float> bi);

  /**
   * @brief Run the filter on a sample.
   *
   * @param sample the sample to sample.
   * @return float the filtered sample.
   */
  float filter(float sample);

  /**
   * @brief Reset all the input and output signal saved.
   *
   */
  void reset();

  /**
   * @brief Sets the output gain of this filter in decibels.
   *
   * @param dbOutputGain decibel gain applied.
   */
  void setOutputGainDB(float dbOutputGain);

private:
  std::vector<float> a; /**< Feedback coefficients */
  std::vector<float> b; /**< Feedforward coefficients */
  std::vector<float> x; /**< Past and current input signal */
  std::vector<float> y; /**< Past and current output signal */
  size_t order;         /**< Order of the filter, should always be 4 */
  size_t iter;      /**< helper value between 0 and order-1 incremented at every
                       sample to pick x and ys */
  float outputGain; /**< Gain applied on output signal (output is multiplied by
                       this value) */
};