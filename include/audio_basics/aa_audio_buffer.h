//
// Created by william on 2020/2/22.
//

#pragma once
#include <vector>

using std::vector;

namespace libaa
{
template <typename T>
class AudioBuffer
{
public:
    AudioBuffer()
        : size_(0),
          num_channels_(0),
          channels_(32, nullptr)
    {

    }

    explicit AudioBuffer(int num_channels, int num_samples)
        : num_channels_(num_channels),
          size_(num_samples)
    {
        assert(num_channels >= 0);
        assert(num_samples >= 0);

        allocateData();
    }

    AudioBuffer(T* const* data_refer_to,
                int num_channel_to_use,
                int start_sample,
                int num_samples)
        : num_channels_(num_channel_to_use),
          size_(num_samples)
    {
        assert(data_refer_to != nullptr);
        assert(num_channel_to_use >= 0);
        assert(start_sample >= 0);
        assert(num_samples >= 0);

        allocateChannels(data_refer_to, start_sample);
    }

    void allocateChannels(T* const* data_refer_to,
                          int offset)
    {
        channels_.resize(num_channels_ + 1);
        channels_[num_channels_] = nullptr;

        for(int i = 0; i < num_channels_; ++i)
        {
            channels_[i] = data_refer_to[i] + offset;
        }
    }


    int getNumSamples() const {return size_;}
    int getNumChannels() const {return num_channels_;}

    T* getWritePointer(int channel_number) const noexcept
    {
        assert(channel_number < num_channels_);

        return channels_[channel_number];
    }

    const T* getReadPointer(int channel_number) const noexcept
    {
        assert(channel_number < num_channels_);

        return channels_[channel_number];
    }

    void clear() const noexcept
    {
        for(int i = 0; i < num_channels_; ++i)
        {
            std::fill(channels_[i], channels_[i]+size_, 0);
        }
    }

private:
    void allocateData()
    {
        auto num_space_to_allocate = size_ * num_channels_;

        allocated_data_.resize(num_space_to_allocate);
        channels_.resize(num_channels_ + 1);
        for(int i = 0; i < num_channels_; ++i)
        {
            channels_[i] = (allocated_data_.data() + i*size_);
        }
        channels_[num_channels_] = nullptr;
    }

    size_t size_;
    size_t num_channels_;
    std::vector<T*> channels_;
    std::vector<T> allocated_data_;
};
}
