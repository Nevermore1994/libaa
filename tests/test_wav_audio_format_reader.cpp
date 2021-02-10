
//
// Created by William.Hua on 2021/2/9.
//
#include "fileio/aa_wav_audio_format_reader.h"

#include "aa_test_helper.h"
#include <gmock/gmock.h>
#include <fstream>
#include <sstream>
using namespace testing;
using namespace libaa;
using namespace std;


class AWaveAudioFormatReader : public Test
{
public:
    void SetUp() override
    {
        test_file = std::make_unique<ScopeWaveFile>(test_file_name,sample_rate,
                                                    num_channels,
                                                    num_frames,
                                                    num_bits,
                                                    fill_val);
    }

    void TearDown() override
    {
        test_file = nullptr;
    }

    const size_t sample_rate = 44100;
    const size_t num_channels = 2;
    const size_t num_frames = sample_rate * 1;
    const size_t num_bits = 16;
    const float fill_val = 0.1f;

    const std::string test_file_name="wave_audio_format_reader_test.wav";
    std::unique_ptr<ScopeWaveFile> test_file;
};


TEST_F(AWaveAudioFormatReader, InitWithInputStream)
{
    ifstream in_stream(test_file_name);
    WaveAudioFormatReader reader(in_stream);
}

TEST_F(AWaveAudioFormatReader, ReturnsFalseIfOpenFailed)
{
    stringstream fake_stream;
    fake_stream << "";

    WaveAudioFormatReader reader(fake_stream);

    ASSERT_FALSE(reader.isOpenOk());
}

TEST_F(AWaveAudioFormatReader, ReturnTrueIfOpenSuccessfully)
{
    ifstream in_stream(test_file_name);

    WaveAudioFormatReader reader(in_stream);

    ASSERT_TRUE(reader.isOpenOk());
}

TEST_F(AWaveAudioFormatReader, DISABLED_CanReadSamples)
{
    ifstream in_stream(test_file_name);
    WaveAudioFormatReader reader(in_stream);

    int n_read_samples = 64;
    vector<float> left_buffer(n_read_samples);
    vector<float> right_buffer(n_read_samples);

    float* dest[2]{left_buffer.data(), right_buffer.data()};
    reader.readSamples(dest, num_channels, 0, 0, n_read_samples);

    vector<float> ref(n_read_samples, fill_val);
    ASSERT_THAT(left_buffer, ContainerEq(ref));
}