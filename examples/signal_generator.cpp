
//
// Created by William.Hua on 2021/1/15.
//

#include "fileio/aa_audio_file.h"
#include "audio_generator/aa_lfo_generator.h"
using namespace std;
using namespace libaa;

int main(int argc, char* argv[]) {
    string output_filename = "audio_generator.wav";
    AudioFile audio_file;

    LFOGenerator lfo;
    lfo.waveform = GeneratorWaveform::kTriangle;
    lfo.frequency_hz = 200;

    const double sample_rate = 44100;
    lfo.prepare(sample_rate);

    audio_file.setSampleRate(sample_rate);
    audio_file.setNumChannles(2);
    audio_file.setNumBits(16);

    for(int i = 0; i < sample_rate * 10; ++i){
        auto out = lfo.renderAudioOutput();
        audio_file.samples[0].push_back(out.normal_output);
        audio_file.samples[1].push_back(out.quad_phase_output_pos);
    }

    audio_file.saveToWave(output_filename);
}