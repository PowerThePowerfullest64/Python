#pragma once

#include <cstdint>
#include <vector>

using byte = uint8_t;

std::vector<byte> rlEncode(const std::vector<byte>& bytes, bool length_first=true) {
    std::vector<byte> result{};

    for (size_t i = 0; i < bytes.size(); ) {
        byte current = bytes[i];
        size_t count = 1;

        for (size_t j = i+1; j < bytes.size() && j < 255; ++j) {
            if (bytes[j] != current)
                break;
            
            count++;
        }

        if (length_first) {
            result.push_back(static_cast<byte>(count));
            result.push_back(current);
        } else {
            result.push_back(current);
            result.push_back(static_cast<byte>(count));
        }

        i += count;
    }

    return result;
}

std::vector<byte> rlDecode(const std::vector<byte>& bytes, bool length_first=true) {
    std::vector<byte> result{};

    for (size_t i = 0; i < bytes.size(); i+=2) {
        uint8_t count;
        byte byte;

        if (length_first) {
            count = bytes[i];
            byte = bytes[i+1];
        } else {
            byte = bytes[i];
            count = bytes[i+1];
        }

        for (int i = 0; i < count; ++i)
            result.push_back(byte);
    }

    return result;
}

// std::string rlEncodeFile(const std::string& input_path, const std::string& output_path, bool length_first=true) {
//     std::ifstream input(input_path, std::ios::binary);
//     std::ostringstream ss;
//     ss << input.rdbuf();
//     input.close();

//     std::string encoded = rlEncode(ss.str(), length_first);

//     std::ofstream output(output_path, std::ios::binary);
//     output.write(encoded.c_str(), encoded.size());
//     output.close();

//     return encoded;
// }

// std::string rlDecodeFile(const std::string& input_path, const std::string& output_path, bool length_first=true) {
//     std::ifstream input(input_path, std::ios::binary);
//     std::ostringstream ss;
//     ss << input.rdbuf();
//     input.close();

//     std::string decoded = rlDecode(ss.str(), length_first);

//     std::ofstream output(output_path, std::ios::binary);
//     output.write(decoded.c_str(), decoded.size());
//     output.close();

//     return decoded;
// }