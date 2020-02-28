//
// Created by william on 2020/2/28.
//

#include "audio_effect/aa_vibrato_effect.h"
#include <gmock/gmock.h>

using namespace std;
using namespace libaa;


TEST(AVibratoEffect, test)
{
    int sample_rate = 44100;
    int block_size = 1024;
    VibratoEffect vibrato;
    vibrato.prepareToPlay(sample_rate, block_size);

    vector<float> left_data(44100*10, 1.0f);
    vector<float> right_data(44100*10, 1.0f);
    float* data_refer_to[2] = {left_data.data(), right_data.data()};
    AudioBuffer<float> test_data(data_refer_to,2,0,left_data.size() );

    vibrato.processBlock(test_data);
}