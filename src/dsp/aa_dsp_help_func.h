//
// Created by william on 2019/12/11.
//

#pragma once

namespace libaa
{

bool isPowerOf2(size_t number)
{
    return (number & number - 1) == 0 && number > 0;
}

}
