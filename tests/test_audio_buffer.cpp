//
// Created by william on 2020/2/22.
//
#include "audio_basics/aa_audio_buffer.h"

#include <gmock/gmock.h>

using namespace libaa;
using namespace testing;

class AAudioBuffer : public Test
{
public:
    void SetUp() override
    {
        for(int i = 0; i < num_samples; ++i)
        {
            left_data[i] = static_cast<float>(i);
            right_data[i] = left_data[i];
        }
    }
    constexpr static int num_samples = 512;
    constexpr static int num_channels = 2;
    float left_data[num_samples];
    float right_data[num_samples];

};

TEST_F(AAudioBuffer, CreatAnEmptyBuffer)
{
    AudioBuffer<float> empty_buffer;

    int samples = empty_buffer.getNumSamples();
    int channels = empty_buffer.getNumChannels();
    EXPECT_THAT(samples, Eq(0));
    EXPECT_THAT(channels, Eq(0));
}

TEST_F(AAudioBuffer, CreatAnEmptyBufferWithSizeAndNumChannels)
{
    AudioBuffer<float> buffer(num_channels, num_samples);

    int samples = buffer.getNumSamples();
    int channels = buffer.getNumChannels();

    EXPECT_THAT(samples, Eq(num_samples));
    EXPECT_THAT(channels, Eq(num_channels));
}

TEST_F(AAudioBuffer, AllocMemoryWhenCreateWithSize)
{
    AudioBuffer<float> buffer(num_channels, num_samples);

    float* wp = buffer.getWritePointer(0);
    const float* rp = buffer.getReadPointer(0);
    ASSERT_THAT(wp, Not(nullptr));
    ASSERT_THAT(rp, Not(nullptr));
}

TEST_F(AAudioBuffer, CreateWithDataReferTo)
{
    vector<float*> data_refet_to = {left_data, right_data};

    int num_channel_to_use = 2;
    int start_sample = 0;
    AudioBuffer<float> buffer(data_refet_to.data(), num_channel_to_use, start_sample, num_samples);

    EXPECT_THAT(buffer.getNumSamples(), num_samples);
    EXPECT_THAT(buffer.getNumChannels(), num_channel_to_use);
    EXPECT_THAT(buffer.getWritePointer(0), Not(nullptr));
    EXPECT_THAT(buffer.getWritePointer(1), Not(nullptr));
    EXPECT_THAT(buffer.getReadPointer(0)[233], FloatEq(233));
    EXPECT_THAT(buffer.getReadPointer(1)[233], FloatEq(233));
}

TEST_F(AAudioBuffer, CreateWithDataReferToWillShareTheSameMemory)
{
    vector<float*> data_refet_to = {left_data, right_data};

    int num_channel_to_use = 2;
    int start_sample = 0;
    AudioBuffer<float> buffer(data_refet_to.data(), num_channel_to_use, start_sample, num_samples);

    left_data[233] = 1;
    right_data[233] = 1;
    EXPECT_THAT(buffer.getReadPointer(0)[233], FloatEq(1));
    EXPECT_THAT(buffer.getReadPointer(1)[233], FloatEq(1));

    buffer.getWritePointer(0)[233] = 0;
    buffer.getWritePointer(1)[233] = 0;
    EXPECT_THAT(buffer.getReadPointer(0)[233], FloatEq(0));
    EXPECT_THAT(buffer.getReadPointer(1)[233], FloatEq(0));
}

TEST_F(AAudioBuffer, AllSamplesInAllChannelSetToZeroAfterClear)
{
    vector<float*> data_refet_to = {left_data, right_data};
    int num_channel_to_use = 2;
    int start_sample = 0;
    AudioBuffer<float> buffer(data_refet_to.data(), num_channel_to_use, start_sample, num_samples);

    buffer.clear();

    int arbitrary_index = 100;
    EXPECT_THAT(buffer.getReadPointer(0)[arbitrary_index], FloatEq(0));
    EXPECT_THAT(buffer.getReadPointer(1)[arbitrary_index], FloatEq(0));
}