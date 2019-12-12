//
// Created by william on 2019/12/6.
//

#pragma once

#include "kiss_fftr.h"
#include <complex>
namespace libaa
{
class FFT
{
public:
    explicit FFT(size_t nfft);

    ~FFT();

    void forward(const float* time_in, std::complex<float>* freq_out);
    void inverse(const std::complex<float>* freq_in, float* time_out);

private:
    size_t nfft_{0};

    class Impl;
    std::shared_ptr<Impl> impl_;
};

}
