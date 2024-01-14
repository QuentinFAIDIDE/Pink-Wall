#include "Brickwall.h"
#include "Filter.h"
#include "LoadCsv.h"
#include <iostream>
#include <limits>

int testCsvLoading()
{
    // first we test the csv loading
    auto testCsv = loadCsv("../Test/test_csv_load.csv");
    if (std::abs(testCsv[0] + 1.0) > std::numeric_limits<float>::epsilon())
    {
        std::cerr << "first test csv value is wrong: " << testCsv[0] << std::endl;
        return 1;
    }

    if (std::abs(testCsv[1] - 2.0) > std::numeric_limits<float>::epsilon())
    {
        std::cerr << "second test csv value is wrong: " << testCsv[1] << std::endl;
        return 1;
    }

    if (std::abs(testCsv[2] + 3.0) > std::numeric_limits<float>::epsilon())
    {
        std::cerr << "third test csv value is wrong: " << testCsv[2] << std::endl;
        return 1;
    }

    return 0;
}

int testFilter()
{
    // this is our pink filter
    Filter pinkToWhite(std::vector<float>{0.049922035, -0.095993537, 0.050612699, -0.004408786},
                       std::vector<float>{1, -2.494956002, 2.017265875, -0.522189400});
    pinkToWhite.setOutputGainDB(-34);

    // we load the test data for our pink filter
    auto pinkToWhiteInputData = loadCsv("../Test/test_2_pink_to_white_input.csv");
    auto outputData = pinkToWhiteInputData;
    // we filter the damn signal
    for (int i = 0; i < outputData.size(); i++)
    {
        outputData[i] = pinkToWhite.filter(pinkToWhiteInputData[i]);
    }

    float epsilonLevel = 1000 * std::numeric_limits<float>::epsilon();
    std::cout << "Testing audio with epsilon level of: " << epsilonLevel << std::endl;

    // now we load the result data and compare to what R produced
    auto pinkToWhiteTestResult = loadCsv("../Test/test_2_pink_to_white_output.csv");
    for (int i = 0; i < pinkToWhiteTestResult.size(); i++)
    {
        if (std::abs(pinkToWhiteTestResult[i] - outputData[i]) > epsilonLevel)
        {
            std::cerr << "Sample difference at index " << i << ": expected " << pinkToWhiteTestResult[i] << " got "
                      << outputData[i] << std::endl;
            return 1;
        }
    }

    return 0;
}

int testBrickwall()
{

    Brickwall brickwall(-6.0f);

    auto testInput = loadCsv("../Test/Test/test_4_brickwall_6dB_input.csv");
    auto outputData = testInput;
    auto outputGainReduc = testInput;

    for (int i = 0; i < outputData.size(); i++)
    {
        auto res = brickwall.process(testInput[i]);
        outputData[i] = res.output;
        outputGainReduc[i] = res.gainReductionDB;
    }

    float epsilonLevel = 1000 * std::numeric_limits<float>::epsilon();
    std::cout << "Testing audio with epsilon level of: " << epsilonLevel << std::endl;

    auto referenceOutputData = loadCsv("../Test/test_4_brickwall_6dB_output.csv");
    auto referenceOutputGainReduc = loadCsv("../Test/test_5_brickwall_reduc_6dB_output.csv");

    for (int i = 0; i < outputData.size(); i++)
    {

        if (std::abs(outputData[i] - referenceOutputData[i]) > epsilonLevel)
        {
            std::cerr << "Sample difference at index " << i << ": expected " << referenceOutputData[i] << " got "
                      << outputData[i] << std::endl;
            return 1;
        }

        if (std::abs(outputGainReduc[i] - referenceOutputGainReduc[i]) > epsilonLevel)
        {
            std::cerr << "Gain reduc difference at index " << i << ": expected " << referenceOutputData[i] << " got "
                      << outputData[i] << std::endl;
            return 1;
        }
    }

    return 0;
}

int main()
{

    int ret = testCsvLoading();
    if (ret != 0)
    {
        return 1;
    }

    ret = testFilter();
    if (ret != 0)
    {
        return 1;
    }

    ret = testBrickwall();
    if (ret != 0)
    {
        return 1;
    }

    return 0;
}