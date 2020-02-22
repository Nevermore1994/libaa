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
    /**
     * Returns a window of a given length and type.
     */
    static Eigen::ArrayXf getWindow(WindowType win_type, size_t win_size);

    /**
     * Returns a overlap window sum.
     *
     * ISTFT uses overlap window sum to recover signal.
     */
    static Eigen::ArrayXf windowSum(WindowType win_type, size_t n_frames, size_t win_size, size_t hop_size);
};

}
