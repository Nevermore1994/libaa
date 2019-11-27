//
// Created by hw on 2019/11/27.
//
#pragma once

#include <vector>
#include <string>

namespace libaa
{
class AudioDecoder
{
public:
    AudioDecoder() = default;
    ~AudioDecoder() = default;
    virtual int open(const std::string& filename) = 0;
    virtual int read(float* buffer, size_t size) = 0;
    std::vector<float> read(size_t size)
    {
        std::vector<float> result(size);

        int num_readed = read(result.data(), size);

        if(num_readed != size)
        {
            result.resize(num_readed);
        }

        return result;
    }

    inline size_t getNumChannels() {return num_channels_;}
    inline size_t getSampleRate()  {return sample_rate_;}
    inline size_t getNumFrames()   {return num_frames_;}
    inline size_t getBits()        {return num_bits_;}

protected:
    size_t num_channels_{0};
    size_t sample_rate_ {0};
    size_t num_bits_    {0};
    size_t num_frames_  {0};
};
}
