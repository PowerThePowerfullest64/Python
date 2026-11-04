#pragma once

#include <string>
#include <stdint.h>
#include <fstream>
#include <sstream>

std::string rlEncode(const std::string& s, bool length_first=true) {
    std::string result = "";

    for (size_t i = 0; i < s.size(); ) {
        char current = s[i];
        size_t count = 1;

        for (size_t j = i+1; j < s.size() && j < 255; ++j) {
            if (s[j] != current)
                break;
            
            count++;
        }

        if (length_first) {
            result += static_cast<char>(count);
            result += current;
        } else {
            result += current;
            result += static_cast<char>(count);
        }

        i += count;
    }

    return result;
}

std::string rlDecode(const std::string& s, bool length_first=true) {
    std::string result = "";

    for (size_t i = 0; i < s.size(); i+=2) {
        uint8_t count;
        char ch;

        if (length_first) {
            count = s[i];
            ch = s[i+1];
        } else {
            ch = s[i];
            count = s[i+1];
        }

        result.append(count, ch);
    }

    return result;
}

std::string rlEncodeFile(const std::string& input_path, const std::string& output_path, bool length_first=true) {
    std::ifstream input(input_path, std::ios::binary);
    std::ostringstream ss;
    ss << input.rdbuf();
    input.close();

    std::string encoded = rlEncode(ss.str(), length_first);

    std::ofstream output(output_path, std::ios::binary);
    output.write(encoded.c_str(), encoded.size());
    output.close();

    return encoded;
}

std::string rlDecodeFile(const std::string& input_path, const std::string& output_path, bool length_first=true) {
    std::ifstream input(input_path, std::ios::binary);
    std::ostringstream ss;
    ss << input.rdbuf();
    input.close();

    std::string decoded = rlDecode(ss.str(), length_first);

    std::ofstream output(output_path, std::ios::binary);
    output.write(decoded.c_str(), decoded.size());
    output.close();

    return decoded;
}