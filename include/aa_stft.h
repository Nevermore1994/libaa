//
// Created by william on 2019/12/11.
//

#pragma once
#include "aa_fft.h"
#include <Eigen/Core>
#include <vector>
#include <iostream>

namespace libaa
{

class STFTOption
{
public:
    size_t hop_size{256};
    size_t win_size{1024};
};

void stft(const float* data, size_t data_len, const STFTOption& opts, Eigen::MatrixXcf& result);

}
