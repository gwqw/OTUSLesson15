#include "str_view_utils.h"

#include <string>
#include <algorithm>

using namespace std;

template<typename CharT>
std::basic_string_view<CharT> get_common_prefix_int(std::basic_string_view<CharT> sv1,
                                                std::basic_string_view<CharT> sv2) {
    auto [it1, _] = mismatch(sv1.begin(), sv1.end(), sv2.begin(), sv2.end());
    return {sv1.begin(), static_cast<size_t>(it1 - sv1.begin())};
}

std::string_view get_common_prefix(std::string_view sv1, std::string_view sv2) {
    return get_common_prefix_int(sv1, sv2);
}

std::wstring_view get_common_prefix_w(std::wstring_view sv1, std::wstring_view sv2) {
    return get_common_prefix_int(sv1, sv2);
}

std::string_view get_suffix(const std::string& str, std::string_view prefix) {
    auto suffix = string_view(str);
    suffix.remove_prefix(prefix.size());
    return suffix;
}
