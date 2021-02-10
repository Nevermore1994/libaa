
//
// Created by William.Hua on 2021/2/9.
//

#include <gmock/gmock.h>
using namespace testing;

class AudioFormatReader;
class AudioFormat
{
public:
    virtual bool isCompressed(){
        return false;
    }

    AudioFormatReader* createReaderFor(const std::string& input_file_name){
        return nullptr;
    }
};

class AAudioFormat : public Test
{
public:

};

TEST_F(AAudioFormat, IsNotCompressedByDefault)
{
    AudioFormat format;

    ASSERT_FALSE(format.isCompressed());
}

TEST_F(AAudioFormat, createNullReaderByDefault)
{
    AudioFormat format;
    const std::string input_file_name;
    ASSERT_THAT(format.createReaderFor(input_file_name), IsNull());
}