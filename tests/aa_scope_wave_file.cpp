
//
// Created by William.Hua on 2021/2/9.
//
#include "aa_test_helper.h"
#include "fileio/aa_audio_file.h"
namespace libaa
{
ScopeWaveFile::ScopeWaveFile(std::string output_path,
              size_t sample_rate,
              size_t num_channel,
              size_t num_frames,
              size_t num_bits,
              float fill_val):
    output_path_(std::move(output_path))
{
    AudioFile audio_file;
    audio_file.setNumChannles(num_channel);
    audio_file.setSampleRate(sample_rate);
    audio_file.setNumFrames(num_frames);
    audio_file.setNumBits(num_bits);

    for(auto c = 0u; c < num_channel; ++c){
        std::fill(audio_file.samples[c].begin(),
                  audio_file.samples[c].end(),
                  fill_val);
    }

    audio_file.saveToWave(output_path_);
}

ScopeWaveFile::~ScopeWaveFile()
{
    std::remove(output_path_.c_str());
}
}
