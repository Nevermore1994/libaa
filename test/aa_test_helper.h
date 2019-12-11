//
// Created by william on 2019/12/9.
//

#pragma once
#include <gmock/gmock.h>
using namespace testing;

namespace libaa
{
MATCHER_P(NearWithPrecision, ferr, "")
{
    return abs(get<0>(arg) - get<1>(arg)) < ferr;
}

MATCHER_P2(ComplexNearEqual, ferr, rhs, "")
{
    return abs(arg - rhs) < ferr;
}



}
