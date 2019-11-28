#pragma once

#include <string_view>

bool starts_with(std::string_view sv, std::string_view prefix);

std::string_view get_common_prefix(std::string_view sv1, std::string_view sv2);

std::string get_suffix(const std::string& str, std::string_view prefix);
