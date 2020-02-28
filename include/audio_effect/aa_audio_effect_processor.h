//
// Created by william on 2020/2/24.
//

#pragma once
#include "audio_basics/aa_audio_buffer.h"
#include <string>
namespace libaa
{
class AudioEffectProcessor
{
public:
    /**
     * Returns the processor name
     */
    virtual std::string getName() const  = 0;

    /**
     * This is called by the processor to specify its details before being used.
     */
    virtual void setRateAndBufferSizeDetails(double new_sample_rate, int new_block_size);

    /**
     * Called before processing starts, to let processor prepare itself.
     */
    virtual void prepareToPlay(double sample_rate, int samplers_per_block) = 0;

    /**
     * Resets internal states.
     */
    virtual void reset() = 0;

    /**
     * Called after processing has stopped, to let the object free up any resources it no longer needs.
     */
    virtual void releaseResources() = 0;

    /**
     * Renders the next block.
     */
    virtual void processBlock(AudioBuffer<float>& buffer) = 0;

    /**
     * Returns the current sample rate
     */
    double getSampleRate() const noexcept;

    /**
     * Returns the current block size
     */
    int getBlockSize() const noexcept;

private:
    double sample_rate_{0.0};
    int block_size_{0};
};



}
