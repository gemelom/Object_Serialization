#ifndef BASE_64_H
#define BASE_64_H

#include <iostream>
#include <string>
#include <vector>

namespace base64 {
    std::string encode(const std::string &input);
    std::string decode(std::string const& encoded_string);
}

#endif // !BASE_64_H
