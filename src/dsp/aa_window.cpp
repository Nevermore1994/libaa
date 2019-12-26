//
// Created by william on 2019/12/12.
//

#include "aa_window.h"
#include <functional>

namespace libaa
{

float hammingAtPoint(int point, int numSamples){
    return 0.54f - 0.46f * cosf ( (2.0f * M_PI * point) / ( (float)numSamples - 1.0f) ) ;
}


float triangularAtPoint(int point, int numSamples){
    return ( 2.0f / numSamples ) * ( ( numSamples * 0.5f ) - fabs( point - ( numSamples -1.0f ) * 0.5f ) );
}


float gaussAtPoint(int point, int numSamples){

    float bellW = 0.4f;
    return powf ( M_E, -0.5f * powf ( ( point - ( numSamples - 1 ) * 0.5f ) / ( bellW * ( numSamples - 1 ) * 0.5f ) , 2.0f ) );
}


float blackmanHarrisAtPoint(int point, int numSamples){

    return 0.35875f		- 0.48829f * cosf( 2.0f * M_PI * point / (numSamples-1) )
        + 0.14128f * cosf( 4.0f * M_PI * point / (numSamples-1) )
        - 0.01168f * cosf( 6.0f * M_PI * point / (numSamples-1) );
}

float hanningAtPoint(float point, int numSamples){
    return 0.5f * ( 1.0f - cosf ( (2.0f * M_PI * point) / ( (float)numSamples - 1.0f) ) );
}

Eigen::ArrayXf Window::windowSum(WindowType win_type, size_t n_frames, size_t win_size, size_t hop_size)
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

Eigen::ArrayXf Window::getWindow(WindowType win_type, size_t win_size)
{
    std::function<float(int,int)> win_func = hammingAtPoint;

    if(win_type == WindowType::kRectangle)
    {
        return Eigen::VectorXf::Ones(win_size);
    }
    else if(win_type == WindowType::kHann)
    {
        win_func = hanningAtPoint;
    }
    else if(win_type == WindowType::kHamming)
    {
        win_func = hammingAtPoint;
    }
    else if(win_type == WindowType::kTriangular)
    {
        win_func = triangularAtPoint;
    }
    else if(win_type == WindowType::kGauss)
    {
        win_func = gaussAtPoint;
    }
    else if(win_type == WindowType::kBlackmanHarris)
    {
        win_func = blackmanHarrisAtPoint;
    }

    Eigen::VectorXf window(win_size);
    for(int i = 0; i < win_size; ++i)
    {
        window(i) = win_func(i, win_size);
    }

    return window;
}

}