//
// Created by william on 2020/2/28.
//

#include "audio_effect/aa_audio_effect_processor.h"

namespace libaa
{
void AudioEffectProcessor::setRateAndBufferSizeDetails(double new_sample_rate, int new_block_size)
{
    sample_rate_ = new_sample_rate;
    block_size_ = new_block_size;
}
double AudioEffectProcessor::getSampleRate() const noexcept
{
    return sample_rate_;
}
int AudioEffectProcessor::getBlockSize() const noexcept
{
    return block_size_;
}
}