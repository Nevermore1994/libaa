//
// Created by william on 2019/12/16.
//

#include "aa_fft.h"
#include <gmock/gmock.h>
#include <vector>
#include <Eigen/Core>
using namespace Eigen;
using namespace libaa;

using namespace testing;
using namespace std;

class Yin
{
public:
    Yin(size_t block_size, size_t sample_rate, float threshold)
        : block_size_(block_size),
          sample_rate_(sample_rate),
          threshold_(threshold),
          half_size_(block_size / 2)
    {
        yin_buffer_.resize(half_size_);
    }

    float getPitch(const float* data)
    {
        // diffrence function

        difference(data);
        cumulativeMeanNormaliztion();
        int tau = absoluteThreshold();
        cout << "f0: " << float(sample_rate_)/float(tau) << endl;
        return 0.0f;
    }

    void difference(const float* data)
    {
        Map<const ArrayXf> x(data, block_size_);
        ArrayXf reversed_seq = x.head(half_size_).reverse();

        ArrayXf power_sum = ArrayXf::Zero(half_size_);
        power_sum(0) = reversed_seq.square().sum();

        ArrayXf x_square = x.square();
        for(int i = 1; i < half_size_; ++i)
        {
            power_sum(i) = power_sum(i-1) - x_square(i-1) + x_square(i + half_size_ - 1);
        }

        // autocorelation
        auto conv_result = FFT::fftconvolve(x.data(), x.size(), reversed_seq.data(), reversed_seq.size());
        Map<ArrayXf> auto_cor( conv_result.data() + half_size_ - 1, half_size_ );// only the interesting part

        // calucate d(tau)
        yin_buffer_ = power_sum(0) + power_sum - 2*auto_cor;
    }

    void cumulativeMeanNormaliztion()
    {
        yin_buffer_(0) = 1;

        float cumsum = 0.0;
        for(int tau = 1; tau < yin_buffer_.size(); ++tau)
        {
            cumsum += yin_buffer_(tau);

            yin_buffer_(tau) = (yin_buffer_(tau) * float(tau)) / cumsum;
        }
    }

    int absoluteThreshold()
    {
        int tau = 0;
        for(tau = 1; tau < yin_buffer_.size(); ++tau)
        {
            if(yin_buffer_(tau) < threshold_)
            {
                // find the local minimum
                for(;tau + 1 < yin_buffer_.size() && yin_buffer_(tau + 1) < yin_buffer_(tau); ++tau);

                return tau;
            }

            // TODO: find the global min if cannot find local min
        }

        if(tau == yin_buffer_.size())
        {
            tau = -1;
        }

        return tau;
    }

    size_t block_size_;
    size_t half_size_;
    size_t sample_rate_;
    float threshold_;
    ArrayXf yin_buffer_;
};

ArrayXf genCosWave(size_t size, float f0, size_t sample_rate)
{
    const float PI = std::atan(1.0)*4;

    ArrayXf n = 2*PI*f0/float(sample_rate) * ArrayXf::LinSpaced(size, 0, size - 1);

    return  n.cos();
}

TEST(AYin, calucateAutocorrelation)
{
//    vector<float> fake_data{0,1,2,3,4,5,6,7};

    size_t sample_rate = 44100;
    auto fake_data = genCosWave(512, 413, sample_rate);

    const float threshold = 0.1;
    size_t block_size = fake_data.size();

    Yin yin(block_size, sample_rate, threshold);

//    yin.difference(fake_data.data());
    yin.getPitch(fake_data.data());

    ASSERT_THAT(yin.yin_buffer_.size(),Eq(block_size/2));
//    ASSERT_THAT(yin.yin_buffer_, ElementsAre(0,4,16,36));

//    cout << genCosWave(512, 250, 44100);
}