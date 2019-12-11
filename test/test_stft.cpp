//
// Created by william on 2019/12/9.
//
#include "aa_fft.h"
#include "aa_test_helper.h"
#include <gmock/gmock.h>
#include <Eigen/Core>
#include <vector>
#include <iostream>

using namespace testing;
using namespace Eigen;
using namespace std;
using namespace libaa;

class STFTOption
{
public:
    size_t hop_size{256};
    size_t win_size{1024};
};

bool isPowerOf2(size_t number)
{
    return (number & number - 1) == 0 && number > 0;
}

class STFT
{
public:
    static void stft(const float* data, size_t data_len, const STFTOption& opts,
              Eigen::MatrixXcf& result)
    {
        if(!isPowerOf2(opts.win_size))
        {
            std::cerr << "stft only support window size of 2^n\n";
            return;
        }

        const int nfft_out = opts.win_size / 2 + 1;
        const int num_frames = data_len / opts.hop_size;

        result.resize(nfft_out, num_frames);
        auto* result_data = result.data();

        int idx = 0;
        vector<float> cur_frame(opts.win_size, 0.0);

        FFT fft(opts.win_size);

        for(;idx < data_len - opts.hop_size;)
        {
            std::copy(data + idx,
                      data + idx + opts.win_size, cur_frame.begin());

            fft.forward(cur_frame.data(), result_data);

            result_data += nfft_out;
            idx += opts.hop_size;
        }
    }
};

class ASTFT : public Test
{
public:
    void SetUp() override
    {
        fake_data.resize(data_length);
        std::fill(fake_data.begin(), fake_data.end(), 1.0);

        opts.hop_size = hop_size;
        opts.win_size = win_size;
    }
    const int hop_size = 6;
    const int win_size = 8;
    const int data_length = 40;

    STFTOption opts;
    vector<float> fake_data;
    Eigen::MatrixXcf stft_result;

};

TEST_F(ASTFT, NumberFramesEqualsSizeDivideHopSize)
{
    STFT::stft(fake_data.data(), fake_data.size(), opts, stft_result);

    ASSERT_THAT(stft_result.rows(), Eq(win_size/2 + 1));
    ASSERT_THAT(stft_result.cols(), Eq(data_length / hop_size));
}

TEST_F(ASTFT, SlideWindowWithHopSize)
{
    STFT::stft(fake_data.data(), fake_data.size(), opts, stft_result);

    FFT fft(win_size);

    int idx = 0;
    int frame_idx = 0;
    for(;idx < fake_data.size() - hop_size;)
    {
        vector<float> cur_frame(fake_data.begin() + idx,
                                fake_data.begin() + idx + win_size);

        vector<complex<float>> cur_fft_out(win_size/2 + 1);
        fft.forward(cur_frame.data(), cur_fft_out.data());

        Eigen::VectorXcf stft_cur = stft_result.col(frame_idx);

        for(int i = 0; i < win_size/2 + 1; ++i)
        {
            ASSERT_THAT(stft_cur(i), ComplexNearEqual(1e-6, cur_fft_out[i]));
        }

        idx += hop_size;
        ++frame_idx;
    }
//    vector<float> first_frame(fake_data.begin(), fake_data.begin() + win_size);
//    vector<float> second_frame(fake_data.begin() + hop_size, fake_data.begin() + 2*win_size);


}