//
// Created by william on 2019/12/12.
//

#pragma once
#include <cstdlib>

namespace libaa
{

enum class WindowType
{
    kRectangle = 0,
    kHann,
    kHamming,
    kTriangular,
    kGauss,
    kBlackmanHarris
};

class STFTOption
{
public:
    size_t hop_size{256};
    size_t win_size{1024};
    WindowType win_type{WindowType::kHann};
};

}
