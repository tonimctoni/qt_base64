#include "base64encoderdecoder.hpp"
#include <array>
#include <algorithm>

static_assert(sizeof(char)==1);
const char *encoding_chars="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static bool is_encoding_char(const char c) noexcept
{
    return std::find(encoding_chars, encoding_chars+64, c)!=encoding_chars+64;
}

static unsigned char encoding_chars_index(const char c) noexcept
{
    for(unsigned char i=0;i<64;i++)
        if (encoding_chars[i]==c) return i;
    return 0;
}

static std::array<char, 4> encode_chunk_base64(const std::array<unsigned char, 3> chunk) noexcept
{
    std::array<char, 4> ret;
    ret[0]=encoding_chars[(chunk[0]&0b11111100)>>2];
    ret[1]=encoding_chars[((chunk[0]&0b00000011)<<4) | ((chunk[1]&0b11110000)>>4)];
    ret[2]=encoding_chars[((chunk[1]&0b00001111)<<2) | ((chunk[2]&0b11000000)>>6)];
    ret[3]=encoding_chars[(chunk[2]&0b00111111)];
    return ret;
}

std::string encode_base64(const void * data, int data_length) noexcept
{
    if (data_length<0) return std::string();
    std::string ret;
    ret.reserve(data_length*4/3+(data_length%3==0?0:4));
    for(int i=0;i<data_length/3;i++)
    {
        const auto chunk_ptr=reinterpret_cast<const unsigned char *>(data)+i*3;
        const auto encoded_chunk=encode_chunk_base64({chunk_ptr[0], chunk_ptr[1], chunk_ptr[2]});
        ret.append(1, encoded_chunk[0]);
        ret.append(1, encoded_chunk[1]);
        ret.append(1, encoded_chunk[2]);
        ret.append(1, encoded_chunk[3]);
    }

    const auto leftover_chars=data_length%3;
    if(leftover_chars!=0){
        const auto chunk_ptr=reinterpret_cast<const unsigned char *>(data)+data_length-leftover_chars;
        std::array<unsigned char, 3> chunk;
        chunk[0]=chunk_ptr[0];
        chunk[1]=leftover_chars==2?chunk_ptr[1]:0;
        chunk[2]=0;

        const auto encoded_chunk=encode_chunk_base64(chunk);
        ret.append(1, encoded_chunk[0]);
        ret.append(1, encoded_chunk[1]);
        if(leftover_chars==1)
        {
            ret.append(2, '=');
        } else
        {
            ret.append(1, encoded_chunk[2]);
            ret.append(1, '=');
        }
    }

    return ret;
}

static std::array<char, 3> decode_chunk_base64(const std::array<char, 4> chunk) noexcept
{
    std::array<char, 3> ret;
    ret[0]=((encoding_chars_index(chunk[0])&0b111111)<<2)|((encoding_chars_index(chunk[1])&0b110000)>>4);
    ret[1]=((encoding_chars_index(chunk[1])&0b001111)<<4)|((encoding_chars_index(chunk[2])&0b111100)>>2);
    ret[2]=((encoding_chars_index(chunk[2])&0b000011)<<6)|((encoding_chars_index(chunk[3])&0b111111));
    return ret;
}

std::pair<bool, std::string> decode_base64(const char * encoded_data, int encoded_data_length) noexcept
{
    if (encoded_data_length<0) return make_pair(false, std::string());

    if (encoded_data_length==0) 
        return make_pair(true, std::string());
    if (encoded_data_length%4!=0)
        return make_pair(false, std::string());

    const auto num_chunks=encoded_data_length/4;
    if (num_chunks>1 && !std::all_of(encoded_data, encoded_data+encoded_data_length-4, is_encoding_char))
        return make_pair(false, std::string());

    if (!is_encoding_char(encoded_data[encoded_data_length-4]))
        return make_pair(false, std::string());
    if (!is_encoding_char(encoded_data[encoded_data_length-3])) 
        return make_pair(false, std::string());
    if (!is_encoding_char(encoded_data[encoded_data_length-2])
        and !(encoded_data[encoded_data_length-2]=='=' and encoded_data[encoded_data_length-1]=='=')
        )return make_pair(false, std::string());
    if (!is_encoding_char(encoded_data[encoded_data_length-1]) and encoded_data[encoded_data_length-1]!='=')
        return make_pair(false, std::string());

    std::string ret;
    ret.reserve(num_chunks*3);
    for(int i=0;i<num_chunks;i++)
    {
        const auto chunk_ptr=encoded_data+i*4;
        const auto decoded_chunk=decode_chunk_base64({chunk_ptr[0], chunk_ptr[1], chunk_ptr[2], chunk_ptr[3]});
        ret.append(1, decoded_chunk[0]);
        ret.append(1, decoded_chunk[1]);
        ret.append(1, decoded_chunk[2]);
    }

    if(encoded_data[encoded_data_length-1]=='=') ret.pop_back();
    if(encoded_data[encoded_data_length-2]=='=') ret.pop_back();

    return make_pair(true, std::move(ret));
}
