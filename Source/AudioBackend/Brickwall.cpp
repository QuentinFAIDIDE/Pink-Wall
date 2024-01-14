#include "Brickwall.h"
#include <cmath>

Brickwall::Brickwall(float decibelsLimit)
{
    gainLimit = std::pow(10.0f, decibelsLimit / 20.0f);
}

Brickwall::~Brickwall()
{
}

CompressionResult Brickwall::process(float input)
{
    if (std::abs(input) > gainLimit)
    {
        float dbReduction = 20.0f * std::log10(gainLimit / std::abs(input));
        return (struct CompressionResult){std::copysignf(gainLimit, input), dbReduction};
    }
    return (struct CompressionResult){input, 0.0f};
}