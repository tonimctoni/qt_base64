#ifndef BASE64ENCODERDECODER_HEADER
#define BASE64ENCODERDECODER_HEADER
#include <tuple>
#include <string>

std::string encode_base64(const void * data, int data_length) noexcept;
std::pair<bool, std::string> decode_base64(const char * encoded_data, int encoded_data_length) noexcept; //since there is no std::optional yet

#endif//BASE64ENCODERDECODER_HEADER