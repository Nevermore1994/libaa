//
// Created by william on 2019/12/6.
//

#include "aa_fft.h"

namespace libaa
{
class FFT::Impl
{
public:
    kiss_fftr_cfg forward_cfg_{nullptr};
    kiss_fftr_cfg inverse_cfg_{nullptr};
};

FFT::FFT(size_t nfft)
    : nfft_(nfft),
      impl_(std::make_shared<Impl>())
{
    impl_->forward_cfg_ = kiss_fftr_alloc(nfft_, 0, 0, 0);
    impl_->inverse_cfg_ = kiss_fftr_alloc(nfft_, 1, 0, 0);
}

FFT::~FFT()
{
    kiss_fftr_free(impl_->forward_cfg_);
    kiss_fftr_free(impl_->inverse_cfg_);
}

void FFT::forward(const float* time_in, std::complex<float>* freq_out)
{
    kiss_fftr(impl_->forward_cfg_, time_in, reinterpret_cast<kiss_fft_cpx*>(freq_out));
}

void FFT::inverse(const std::complex<float>* freq_in, float* time_out)
{
    kiss_fftri(impl_->inverse_cfg_, reinterpret_cast<const kiss_fft_cpx*>(freq_in), time_out);
}
}