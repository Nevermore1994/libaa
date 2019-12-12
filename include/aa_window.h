//
// Created by william on 2019/12/12.
//
#include <Eigen/Core>

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

class Window
{
public:
    static Eigen::VectorXf getWindow(WindowType win_type, size_t win_size);
};

}
