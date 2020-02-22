//
// Created by william on 2020/2/21.
//

#include "audio_effect/aa_delay_line.h"
#include "fileio/aa_audio_file.h"
#include <iostream>
using namespace std;
using namespace libaa;

class AudoEffectProcessor
{
public:
    virtual void prepareToPlay(double sample_rate, int samplers_per_block) = 0;
    virtual void reset() = 0;
    virtual void releaseResources() = 0;
    virtual void processBlock() = 0;
};

class DelayEffect
{
public:

};

int main(int argc, char* argv[])
{
    const string input_filename = "../../res/wav/wav_mono_16bit_44100.wav";
    string output_filename = "audio_effect_result.wav";

    AudioFile input_file;
    int failed = input_file.load(input_filename);
    if(failed)
    {
        cerr << "load file failed\n";
        return 0;
    }

    const int num_frames = input_file.getNumFrames();
    const int predefine_block_size = 512;
    int sample_index = 0;
    int acctual_block_size = 0;
    float* left_channel_data = input_file.samples[0].data();
    for(;sample_index < num_frames;)
    {
        acctual_block_size = (sample_index + predefine_block_size >= num_frames) ?
                             (num_frames - sample_index) : (predefine_block_size);


        sample_index += acctual_block_size;
    }

    return 0;
}