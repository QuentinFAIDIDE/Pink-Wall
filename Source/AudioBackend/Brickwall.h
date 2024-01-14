#pragma once

/**
 * @brief The result of the limiting/compressing
 * of a single sample. There is both the output
 * sample value and the gain reduction level in decibels.
 */
struct CompressionResult
{
    float output;
    float gainReductionDB;
};

/**
 * @brief Class that implements a very simple brickwall effect.
 */
class Brickwall
{
  public:
    Brickwall(float decibelsLimit);
    ~Brickwall();

    /**
     * @brief Process the input sample, and apply limiting.
     *
     * @param input input sample between -1 and 1.
     * @return CompressionResult Both the output sample value and the gain
     * reduction in decibels.
     */
    CompressionResult process(float input);

  private:
    float gainLimit; /**< Maximum value of the output signal */
};