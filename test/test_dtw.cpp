//
// Created by william on 2019/12/24.
//
#include "aa_distance.h"
#include <Eigen/Core>
#include <iostream>
#include <gmock/gmock.h>

using namespace testing;
using namespace Eigen;
using namespace std;
using namespace libaa;

class DistanceTest : public Test
{
public:

    void SetUp() override
    {
        X.resize(2, 5);
        X << 1,2,3,4,5,
            1,2,3,4,5;

        Y.resize(2, 3);
        Y << 2,3,4,
             2,3,4;
    }

    ArrayXXf X;
    ArrayXXf Y;
};
TEST_F(DistanceTest, getDistanceMatrix)
{
    ArrayXXf D = Distance::calcDistance(X, Y, DistanceType::kEuclidean);

    EXPECT_THAT(D.rows(), Eq(X.cols()));
    EXPECT_THAT(D.cols(), Eq(Y.cols()));
}

TEST_F(DistanceTest, EucileanDistance)
{
    ArrayXXf D = Distance::calcDistance(X, Y, DistanceType::kEuclidean);

    for(int i = 0; i < D.rows(); ++i)
    {
        for(int j = 0; j < D.cols(); ++j)
        {
            float dist = sqrt((X.col(i) - Y.col(j)).square().sum());

            EXPECT_THAT(D(i, j), FloatNear(dist, 1e-6));
        }
    }
}

TEST_F(DistanceTest, ConsinDistance)
{
    ArrayXXf D = Distance::calcDistance(X, Y, DistanceType::kCosine);

    for(int i = 0; i < D.rows(); ++i)
    {
        for(int j = 0; j < D.cols(); ++j)
        {
            float a = sqrt(X.col(i).square().sum());
            float b = sqrt(Y.col(j).square().sum());
            float c = (X.col(i) * Y.col(j)).sum();

            float dist = 1.0f - (c/(a*b));

            EXPECT_THAT(D(i, j), FloatNear(dist, 1e-6));
        }
    }
}
