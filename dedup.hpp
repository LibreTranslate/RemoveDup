#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string_view>
#include <cstring>
#include <tuple>
#include <variant>
#include <string>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#else
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#endif

#include "vendor/xxh32.hpp"

std::tuple<std::string, std::string, size_t> dedup(const std::string &src, const std::string &tgt);