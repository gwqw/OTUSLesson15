#pragma once

#include <string_view>

std::string_view get_common_prefix(std::string_view sv1, std::string_view sv2);
std::wstring_view get_common_prefix_w(std::wstring_view sv1, std::wstring_view sv2);

std::string_view get_suffix(const std::string& str, std::string_view prefix);

