#pragma once

#include "Filter.h"

/**
 * @brief Class that process the audio signal.
 */
class AudioBackend
{
  public:
    AudioBackend();
    ~AudioBackend();

  private:
    Filter pinkToWhiteFilter; /**< Filter that change pink to white noise */
    Filter whiteToPinkFilter; /**< Filter that change white to pink noise */
    float gainLevelDB;        /**< Gain level in decibels before and after the filter */
};