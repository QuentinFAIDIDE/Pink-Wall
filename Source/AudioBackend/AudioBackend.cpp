#include "AudioBackend.h"
#include "Brickwall.h"
#include <cmath>

#include <iostream>

AudioBackend::AudioBackend()
    : whiteToPinkFilter(WHITE_TO_PINK_POLES, WHITE_TO_PINK_ZEROS),
      pinkToWhiteFilter(WHITE_TO_PINK_ZEROS, WHITE_TO_PINK_POLES), limiter(0.0f)
{
    whiteToPinkFilter.setOutputGainDB(DEFAULT_FILTERS_GAIN_DB);
    pinkToWhiteFilter.setOutputGainDB(-DEFAULT_FILTERS_GAIN_DB);

    gainLevel = 1.0;
    gainLevelInverse = 1.0;
    dBGain = 0;
}

AudioBackend::~AudioBackend()
{
}

void AudioBackend::setGainLevel(float dbGain)
{
    gainLevel = std::pow(10.0f, dbGain / 20.0f);
    gainLevelInverse = 1.0f / gainLevel;
}

CompressionResult AudioBackend::process(float input)
{
    CompressionResult res;
    res.output = pinkToWhiteFilter.filter(input);

    res.output = res.output * gainLevel;
    res = limiter.process(res.output);
    res.output = res.output * gainLevelInverse;

    res.output = whiteToPinkFilter.filter(res.output);

    return res;
}

void AudioBackend::flushPastSignalResiduals()
{
    whiteToPinkFilter.reset();
    pinkToWhiteFilter.reset();
}