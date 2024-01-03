#include <iostream>
#include <fstream>
#include <unordered_map>
#include <cstring>
#include <tuple>
#include <string>

#include "vendor/xxh32.hpp"

std::tuple<std::string, std::string, size_t> dedup(const std::string &src, const std::string &tgt);