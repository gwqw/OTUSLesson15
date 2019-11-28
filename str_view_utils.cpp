#include "str_view_utils.h"

using namespace std;

bool starts_with(std::string_view sv, std::string_view prefix) {
    return sv.substr(0, prefix.size()) == prefix;
}

std::string_view get_common_prefix(std::string_view sv1, std::string_view sv2) {
    size_t cnt = 0;
    while (cnt < sv1.size() && cnt < sv2.size() && sv1[cnt] == sv2[cnt]) {
        ++cnt;
    }
    return sv1.substr(0, cnt);
}
