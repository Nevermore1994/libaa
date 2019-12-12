//
// Created by william on 2019/12/11.
//

#include "aa_stft.h"
#include "aa_dsp_help_func.h"

namespace libaa
{


void STFT::stft(const float* data, size_t data_len, const STFTOption& opts,
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
    std::vector<float> cur_frame(opts.win_size, 0.0);

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
}
