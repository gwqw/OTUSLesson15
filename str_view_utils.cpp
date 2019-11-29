#include "str_view_utils.h"

#include <string>

using namespace std;

template<typename CharT>
std::basic_string_view<CharT> get_common_prefix_int(std::basic_string_view<CharT> sv1,
                                                std::basic_string_view<CharT> sv2) {
    size_t cnt = 0;
    while (cnt < sv1.size() && cnt < sv2.size() && sv1[cnt] == sv2[cnt]) {
        ++cnt;
    }
    return sv1.substr(0, cnt);
}

std::string_view get_common_prefix(std::string_view sv1, std::string_view sv2) {
    return get_common_prefix_int(sv1, sv2);
}

std::wstring_view get_common_prefix_w(std::wstring_view sv1, std::wstring_view sv2) {
    return get_common_prefix_int(sv1, sv2);
}


std::string get_suffix(const std::string& str, std::string_view prefix) {
    auto suffix = string_view(str);
    suffix.remove_prefix(prefix.size());
    return string(suffix);
}
