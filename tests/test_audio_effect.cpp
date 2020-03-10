//
// Created by william on 2020/2/28.
//

#include "audio_effect/aa_vibrato_effect.h"
#include "audio_effect/aa_delay_effect.h"
#include "audio_basics/aa_delay_line_array.h"
#include "audio_effect/aa_flanger.h"
#include <gmock/gmock.h>

using namespace std;
using namespace libaa;
using namespace testing;

class AudioEffectTest : public Test
{
public:
    void SetUp() override
    {
        left_data.resize(sample_rate*2, 1.0f);
        right_data.resize(sample_rate*2, 1.0f);

        data_refer_to[0] = left_data.data();
        data_refer_to[1] = right_data.data();

        block = AudioBuffer<float>(data_refer_to,2,0,left_data.size() );
    }
    int sample_rate = 44100;
    int block_size = 1024;

    vector<float> left_data;
    vector<float> right_data;
    float* data_refer_to[2];
    AudioBuffer<float> block;

};


TEST_F(AudioEffectTest, DelayTest)
{
    DelayEffect processor;
    processor.setRateAndBufferSizeDetails(sample_rate, block_size);
    processor.prepareToPlay(sample_rate, block_size);

    processor.processBlock(block);
    processor.reset();
    processor.releaseResources();
}

TEST_F(AudioEffectTest, VibratoTest)
{
    VibratoEffect processor;
    processor.setRateAndBufferSizeDetails(sample_rate, block_size);
    processor.prepareToPlay(sample_rate, block_size);

    processor.processBlock(block);
    processor.reset();
    processor.releaseResources();
}

TEST_F(AudioEffectTest, FlangerTest)
{
    Flanger processor;
    processor.setRateAndBufferSizeDetails(sample_rate, block_size);
    processor.prepareToPlay(sample_rate, block_size);

    processor.processBlock(block);
    processor.reset();
    processor.releaseResources();
}

class Chorus : public AudioEffectProcessor
{
public:
    Chorus():
        impl_(std::make_shared<Impl>())
    {
    }

    virtual ~Chorus() = default;

    string getName() const override
    {
        return std::string("chorus");
    }
    void prepareToPlay(double sample_rate, int samplers_per_block) override
    {
        impl_->invert_sample_rate_ = 1.0/sample_rate;

        const int num_supported_channel = 2;
        const float max_delay_length = 1.0f; // 1 seconds
        const auto max_delay_length_sample = static_cast<size_t>(max_delay_length * sample_rate);

        impl_->dlines_.allocateDelayLines(num_supported_channel, max_delay_length_sample);
    }
    void reset() override
    {

    }
    void releaseResources() override
    {

    }
    void processBlock(AudioBuffer<float> &buffer) override
    {
        const auto num_channels = buffer.getNumChannels();
        const auto num_samples = buffer.getNumSamples();

        float ph = 0.0f;

        for(size_t c = 0; c < num_channels; ++c)
        {
            auto* channel_data = buffer.getWritePointer(c);
            auto* dline = impl_->dlines_.getDelayLine(c);

            ph = impl_->phase_;

            for(size_t i = 0; i < num_samples; ++i)
            {
                const float in = channel_data[i];

                if(num_voices == 2)
                {
                    auto weight = 1.0f;

                    // get delay from lfo
                    float delay_second = min_delay + sweep_width*impl_->lfo_.lfo(ph, LFO::WaveformType::kWaveformSine);
                    float delay_sample = delay_second * static_cast<float>(getSampleRate());

                    float interpolation_val = dline->getInterpolation(delay_sample);
                    channel_data[i] = in + weight * depth * interpolation_val;
                }

                // update phase
                ph += lfo_freq*impl_->invert_sample_rate_;
                if(ph >= 1.0f)
                {
                    ph -= 1.0f;
                }
            }
        }

        impl_->phase_ = ph;
    }

    float min_delay   = 0.03f;      // minimum lenght of delay line
    float sweep_width = 0.02f;      // amount of change from min to max
    float depth       = 1.0f;       // mix level of delayed signal [0, 1]
    float lfo_freq    = 0.2f;       // lfo frequency(hz)

    int num_voices    = 2;          // how many voices to use in the chorus [2,5]
    int stereo        = 0;          // whether to use stereo chorus


private:
    class Impl
    {
    public:
        float phase_={0.0f};
        float invert_sample_rate_{0.0f};
        LFO lfo_;
        DelayLineArray<float> dlines_;
    };
    std::shared_ptr<Impl> impl_;
};

TEST_F(AudioEffectTest, Chorus)
{
    Chorus processor;
    processor.setRateAndBufferSizeDetails(sample_rate, block_size);
    processor.prepareToPlay(sample_rate, block_size);

    processor.processBlock(block);
    processor.reset();
    processor.releaseResources();
}