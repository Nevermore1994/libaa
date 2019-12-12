//
// Created by william on 2019/12/11.
//
#include "aa_audio_file.h"
#include "aa_stft.h"
#include "aa_window.h"
#include <Eigen/Core>
#include <iostream>
#include <fstream>
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

    // stft
    ArrayXXcf stft_result;
    STFTOption opts;
    STFT::stft(left_channel, num_frames, opts, stft_result);

    cout << "stft result: " << stft_result.rows() << "x" << stft_result.cols() << endl;

    // istft
    Eigen::ArrayXf istft_out;
    STFT::istft(stft_result, opts, istft_out);

    // copy synthetised data to audio file
    audiofile.setChannelData(0, istft_out.data(), istft_out.size());
    audiofile.saveToWave("istft_result.wav");

    return 0;
}