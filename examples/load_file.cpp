//
// Created by william on 2019/12/1.
//
#include "libaa/fileio/aa_audio_file.h"
#include <string>
#include <iostream>
using namespace std;
using namespace libaa;

int main(int argc, char* argv[])
{
    if(argc < 2)
    {
        cerr << "Usage: load_file {audio-filename} [{output-filename}]" << endl;
        return -1;
    }

    string input_filename = argv[1];
    string output_filename = "load_file_result.wav";
    if(argc == 3)
    {
        output_filename = argv[2];
    }

    AudioFile audio_file;
    int failed = audio_file.load(input_filename);
    if(failed)
    {
        cerr << "load file failed\n";
        return 0;
    }

    audio_file.saveToWave(output_filename);

    return 0;
}