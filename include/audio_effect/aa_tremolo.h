//
// Created by william on 2020/6/4.
//

#pragma once
#include "aa_audio_effect_processor.h"
#include "dsp/aa_lfo.h"
namespace libaa
{
class Tremolo : public AudioEffectProcessor
{
public:
    Tremolo();

    std::string getName() const override;

    void prepareToPlay(double sample_rate, int samplers_per_block) override;

    void reset() override ;

    void releaseResources() override;

    void processBlock(AudioBuffer<float> &buffer) override;

    float frequency_;
    float depth_;
    int waveform_;

private:
    class Impl;
    std::shared_ptr<Impl> impl_;
};
}
