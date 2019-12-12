//
// Created by william on 2019/12/11.
//
#include "aa_audio_file.h"
#include "aa_stft.h"
#include <Eigen/Core>
#include <iostream>
#include <string>
using namespace std;
using namespace libaa;
using namespace Eigen;

int main()
{
    const string filename = "../../res/wav/wav_mono_16bit_44100.wav";
    AudioFile audiofile;
    int ret = audiofile.load(filename);
    if(ret){
        cerr << "open file failed\n";
        return -1;
    }

    auto* left_channel = audiofile.samples[0].data();
    auto num_frames = audiofile.getNumFrames();

    MatrixXcf stft_result;
    STFTOption opts;
    STFT::stft(left_channel, num_frames, opts, stft_result);

    cout << "stft result: " << stft_result.rows() << "x" << stft_result.cols() << endl;

    // istft
    const size_t istft_out_len = (stft_result.cols() - 1) * opts.hop_size + opts.win_size;
    vector<float> istft_out(istft_out_len);

    FFT fft(opts.win_size);
    auto* freq_data = stft_result.data();
    vector<float> time_data(opts.win_size);

    for(int i = 0; i < stft_result.cols(); ++i)
    {
        fft.inverse(freq_data, time_data.data());

        freq_data += stft_result.rows();
    }

    return 0;
}