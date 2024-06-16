#include "base64.h"

static const std::string base64_chars =
             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
             "abcdefghijklmnopqrstuvwxyz"
             "0123456789+/";

bool is_base64(unsigned char c) {
    return (isalnum(c) || (c == '+') || (c == '/'));
}

std::string base64::encode(const std::string& input) {
    std::string encoded_string;
    size_t input_length = input.length();
    unsigned char char_array_3[3];
    unsigned char char_array_4[4];
    size_t i = 0;

    while (input_length--) {
        char_array_3[i++] = input[input.length() - input_length - 1];
        if (i == 3) {
            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;

            for(i = 0; (i < 4) ; i++)
                encoded_string += base64_chars[char_array_4[i]];
            i = 0;
        }
    }

    if (i) {
        for(size_t j = i; j < 3; j++)
            char_array_3[j] = '\0';

        char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
        char_array_4[3] = char_array_3[2] & 0x3f;

        for (size_t j = 0; (j < i + 1); j++)
            encoded_string += base64_chars[char_array_4[j]];

        while((i++ < 3))
            encoded_string += '=';
    }

    return encoded_string;
}

std::string base64::decode(std::string const& encoded_string) {
    size_t in_len = encoded_string.size();
    size_t i = 0;
    size_t j = 0;
    size_t in_ = 0;
    unsigned char char_array_4[4], char_array_3[3];
    std::string decoded_string;

    while (in_len-- && ( encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
        char_array_4[i++] = encoded_string[in_]; in_++;
        if (i == 4) {
            for (i = 0; i < 4; i++)
                char_array_4[i] = base64_chars.find(char_array_4[i]);

            char_array_3[0] = ( char_array_4[0] << 2       ) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

            for (i = 0; (i < 3); i++)
                decoded_string += char_array_3[i];
            i = 0;
        }
    }

    if (i) {
        for (j = 0; j < i; j++)
            char_array_4[j] = base64_chars.find(char_array_4[j]);

        char_array_3[0] = ( char_array_4[0] << 2       ) + ((char_array_4[1] & 0x30) >> 4);
        char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
        char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

        for (j = 0; (j < i - 1); j++) decoded_string += char_array_3[j];
    }

    return decoded_string;
}

// int main() {
//     std::string original = "Man! What can I say!";
//     std::string encoded = base64::encode(original);
//     std::string decoded_vec = base64::decode(encoded);
//     std::string decoded(decoded_vec.begin(), decoded_vec.end());
//
//     std::cout << "Original: " << original << std::endl;
//     std::cout << "Encoded: " << encoded << std::endl;
//     std::cout << "Decoded: " << decoded << std::endl;
//
//     return 0;
// }
//
