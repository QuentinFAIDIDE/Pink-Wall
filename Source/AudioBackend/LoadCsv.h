#pragma once

#include <string>
#include <vector>

/**
 * @brief Load the second column of a csv as a float.
 * Note that it abort on newline, so better have no newlines except at the end.
 *
 * @param path path to the file.
 * @return std::vector<float> vector of float values
 */
std::vector<float> loadCsv(std::string path);
