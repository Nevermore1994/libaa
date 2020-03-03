//
// Created by william on 2020/3/3.
//

#pragma once
#include "audio_effect/aa_audio_effect_processor.h"

namespace libaa
{
class Flanger : public AudioEffectProcessor
{
public:
    Flanger();
    std::string getName() const override {
        return std::string("flanger");
    }
    void prepareToPlay(double sample_rate, int samplers_per_block) override;
    void reset() override;
    void releaseResources() override;
    void processBlock(AudioBuffer<float> &buffer) override;

    float lfo_freq = 6;     // frequency fo the lfo
    float sweep_width = 0.1; // widht of the lfo samples
    float depth = 0.5;       // amount o delayed signal mixed with original (0,1)
    float feedback = 0.8;    // amount of feedback

private:
    class Impl;
    std::shared_ptr<Impl> impl_;
};
}
