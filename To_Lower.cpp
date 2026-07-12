#include <cctype>
#include <algorithm>
#include "To_Lower.h"

std::string to_lower(std::string str) {
    std::transform(str.begin(),str.end(),str.begin(),[] (unsigned char c) {
        return std::tolower(c);
    });

    return str;
}