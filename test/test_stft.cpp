//
// Created by william on 2019/12/9.
//
#include "aa_fft.h"
#include "aa_stft.h"
#include "aa_test_helper.h"
#include <gmock/gmock.h>
#include <Eigen/Core>
#include <vector>
#include <iostream>

using namespace testing;
using namespace Eigen;
using namespace std;
using namespace libaa;

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

}