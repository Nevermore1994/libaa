//
// Created by hw on 2019/11/27.
//
#pragma once

#include "aa_audio_decoder.h"

namespace libaa
{
class WavDecoder : public AudioDecoder
{
public:
    WavDecoder();
    ~WavDecoder() = default;
    int open(const std::string& filename) override;
    int read(float *buffer, size_t size) override;

private:
    class Impl;
    std::shared_ptr<Impl> impl_;
};
}
