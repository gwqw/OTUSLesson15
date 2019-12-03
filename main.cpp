#include <iostream>

#include <string>
#include <utility>
#include <string_view>

#include "radix_tree.h"
#include "print_rt.h"

using namespace std;

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
        cout << n << " " << sv.substr(0, pos+1) << '\n';
    }

    cout << getTreeStructure(tr, utf8_formater);

    return 0;
}
