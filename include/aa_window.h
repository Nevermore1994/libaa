//
// Created by william on 2019/12/12.
//
#include "aa_options.h"
#include <Eigen/Core>

namespace libaa
{

class Window
{
public:
    static Eigen::ArrayXf getWindow(WindowType win_type, size_t win_size);

    static Eigen::ArrayXf windowSum(WindowType win_type, size_t n_frames, size_t win_size, size_t hop_size)
    {
        size_t n = (n_frames - 1) * hop_size + win_size;
        Eigen::ArrayXf x(n);

        Eigen::ArrayXf window = getWindow(win_type, win_size).array();

        for(int i = 0;i < n_frames; ++i)
        {
            const int overlap_add_i = i * hop_size;
            x.segment(overlap_add_i, win_size) += window;
        }

        return x;
    }
};

}
