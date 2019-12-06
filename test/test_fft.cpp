//
// Created by william on 2019/12/6.
//

#include "aa_fft.h"
#include <gmock/gmock.h>
#include <vector>
#include <complex>
using namespace testing;
using namespace std;
using namespace libaa;

MATCHER_P(NearWithPrecision, ferr, "")
{
    return abs(get<0>(arg) - get<1>(arg)) < ferr;
}

class AFFT : public Test
{
public:
    AFFT()
        :fft(nfft)
    {
    }

    const int nfft = 8;
    FFT fft;
    vector<float> time_in = {0, 1, 2, 3, 4, 5, 6, 7};
    vector<complex<float>> freq_in
        {
            {28, 0},
            {-4, 9.65685425},
            {-4, 4},
            {-4, 1.65685425},
            {-4, 0}
        };

    vector<float> time_out_gt = {0, 8, 16, 24, 32, 40, 48, 56};
    vector<complex<float>> freq_data_gt
        {
            {28, 0},
            {-4, 9.65685425},
            {-4, 4},
            {-4, 1.65685425},
            {-4, 0}
        };
};

TEST_F(AFFT, forward)
{
    vector<complex<float>> freq_out(nfft/2 + 1);

    fft.forward(time_in.data(), freq_out.data());

    ASSERT_THAT(freq_out, Pointwise(NearWithPrecision(1e-6), freq_data_gt));
}

TEST_F(AFFT, inverse)
{
    vector<float> time_out(nfft);

    fft.inverse(freq_in.data(), time_out.data());

    ASSERT_THAT(time_out, ContainerEq(time_out_gt));
}