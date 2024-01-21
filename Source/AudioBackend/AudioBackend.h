#pragma once

#include "Brickwall.h"
#include "Filter.h"

// clang-format off
#define WHITE_TO_PINK_ZEROS {0.049922035, -0.095993537, 0.050612699, -0.004408786}
#define WHITE_TO_PINK_POLES {1, -2.494956002, 2.017265875, -0.522189400}
#define DEFAULT_FILTERS_GAIN_DB 34
// clang-format on

/**
 * @brief Class that process the audio signal.
 */
class AudioBackend
{
  public:
    AudioBackend();
    ~AudioBackend();

    /**
     * @brief Set the gain around brickwall.
     * @param dBGain decibel gain to apply before and invert after brickwall.
     */
    void setGainLevel(float dBGain);

    /**
     * @brief Get  the gain around brickwall.
     *
     * @return float decibel gain applied before and inverted after brickwall.
     */
    float getGainLevel() const;

    /**
     * @brief Resets filters feedback and feedforward saved signal, and
     * eventually other things that would require reseting before
     * playing.
     */
    void flushPastSignalResiduals();

    /**
     * @brief Run the sample through the processing chain. pinking -> gain -> brickwall -> gain -> whiting
     * @param input
     * @return CompressionResult
     */
    CompressionResult process(float input);

  private:
    Filter pinkToWhiteFilter; /**< Filter that change pink to white noise */
    Filter whiteToPinkFilter; /**< Filter that change white to pink noise */
    Brickwall limiter;        /**< brickwall that is to be sandwiched between effects */
    float dBGain;    /**< The same gain boost as gainLevel, but kept in decibel for when getGainLevel is called and not
                        used in DSP */
    float gainLevel; /**< Gain boost (NOT in decibels) of the brickwall */
    float
        gainLevelInverse; /**< Save a division for a multiplication at every sample by keep record of inverse of gain */
};