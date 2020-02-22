//
// Created by william on 2019/12/12.
//
#include "dsp/aa_window.h"
#include <gmock/gmock.h>
#include <Eigen/Core>
using namespace testing;
using namespace libaa;

TEST(GetWindow, returnsWindowWithWinSize)
{
    const int win_size = 512;
    Eigen::VectorXf window = Window::getWindow(WindowType::kRectangle, win_size);

    ASSERT_THAT(window.size(), Eq(win_size));
    ASSERT_THAT(window, Eq(Eigen::VectorXf::Ones(win_size)));
}