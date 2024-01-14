#include "Filter.h"
#include <cmath>
#include <limits>
#include <stdexcept>

Filter::Filter(std::vector<float> ai, std::vector<float> bi)
{

    if (ai.size() != bi.size())
    {
        throw std::runtime_error("This filtering class does not support different "
                                 "feedback and feedforward order");
    }

    // this is for optimization purpose.
    if (ai.size() != 4)
    {
        throw std::runtime_error("Only filters of order 4 are supported by this class.");
    }

    a = ai;
    b = bi;
    order = ai.size();
    outputGain = 1.0;
    iter = 0;

    reset();
}

float Filter::filter(float sample)
{

    switch (iter)
    {
    case 0:
        iter = 1;
        x[0] = sample;
        y[0] = (b[0] * x[0]) + (b[1] * x[3]) + (b[2] * x[2]) + (b[3] * x[1]) - (a[1] * y[3]) - (a[2] * y[2]) -
               (a[3] * y[1]);
        y[0] = y[0] / a[0];
        return outputGain * y[0];

    case 1:
        iter = 2;
        x[1] = sample;
        y[1] = (b[0] * x[1]) + (b[1] * x[0]) + (b[2] * x[3]) + (b[3] * x[2]) - (a[1] * y[0]) - (a[2] * y[3]) -
               (a[3] * y[2]);
        y[1] = y[1] / a[0];
        return outputGain * y[1];

    case 2:
        iter = 3;
        x[2] = sample;
        y[2] = (b[0] * x[2]) + (b[1] * x[1]) + (b[2] * x[0]) + (b[3] * x[3]) - (a[1] * y[1]) - (a[2] * y[0]) -
               (a[3] * y[3]);
        y[2] = y[2] / a[0];
        return outputGain * y[2];

    case 3:
        iter = 0;
        x[3] = sample;
        y[3] = (b[0] * x[3]) + (b[1] * x[2]) + (b[2] * x[1]) + (b[3] * x[0]) - (a[1] * y[2]) - (a[2] * y[1]) -
               (a[3] * y[0]);
        y[3] = y[3] / a[0];
        return outputGain * y[3];

    default:
        throw std::runtime_error("iter went over 3");
    }
}

void Filter::reset()
{
    x.resize(order);
    y.resize(order);
    std::fill(x.begin(), x.end(), 0.0f);
    std::fill(y.begin(), y.end(), 0.0f);
}

void Filter::setOutputGainDB(float dbOutputGain)
{
    outputGain = std::pow(10.0f, dbOutputGain / 20.0f);
}