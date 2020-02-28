//
// Created by william on 2020/2/28.
//

#include "audio_effect/aa_vibrato_effect.h"

namespace libaa
{
void VibratoEffect::prepareToPlay(double sample_rate, int samplers_per_block)
{
    invert_sample_rate_ = 1.0/sample_rate;

    int num_supported_channel = 2;
    float max_delay_length = 2.0f; // 2 seconds
    dlines_.resize(num_supported_channel);
    for(int i = 0; i < num_supported_channel; ++i)
    {
        dlines_[i].resize(static_cast<size_t>(max_delay_length * sample_rate));
    }

}

void VibratoEffect::processBlock(AudioBuffer<float> &buffer) {
    const int num_channels = buffer.getNumChannels();
    const int num_samples = buffer.getNumSamples();
    float phase = phase_;

    assert(num_channels <= dlines_.size());

    for(int c = 0; c < num_channels; ++c)
    {
        float* channel_data = buffer.getWritePointer(c);
        auto& dline = dlines_[c];

        for(int i = 0; i < num_samples; ++i)
        {
            const float in = channel_data[i];

            // get delay from lfo
            float delay_second = sweep_width*lfo_.lfo(phase, LFO::WaveformType::kWaveformSine);
            int delay_sample = delay_second * getSampleRate();

            // get interpolation delay value
            channel_data[i] = dline.getInterpolation(delay_sample);

            // push input to delay line
            dline.push(in);

            // update phase
            phase += lfo_freq*invert_sample_rate_;
            if(phase >= 1.0f)
            {
                phase -= 1.0f;
            }
        }
    }

    phase_ = phase;
}
}