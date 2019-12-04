#include <iostream>

#include <string>
#include <utility>
#include <string_view>

#include "radix_tree.h"
#include "print_rt.h"

using namespace std;

bool is_2octet_utf8_high_byte(unsigned char c) {
    return c == 0xd0 || c == 0xd1;
}

bool is_2octet_utf8_low_byte(unsigned char c) {
    return (c >>= 6) == 0b10;
}

int main(int argc, char* argv[]) {
    vector<string> names;
    string name;
    RadixTree tr;
    while (cin >> name) {
        tr.insert(name);
        names.push_back(move(name));
    }

    for (const auto& n : names) {
        string_view sv = n;
        auto pos = tr.find(n);
        // utf-8 fix
        if (is_2octet_utf8_high_byte(sv[pos])
            && pos+1 < sv.size() && is_2octet_utf8_low_byte(sv[pos+1])
        ) {
            ++pos;
        }
        cout << n << " " << sv.substr(0, pos+1) << '\n';
    }

    cout << getTreeStructure(tr, utf8_formater);

    return 0;
}
