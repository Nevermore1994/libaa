//
// Created by william on 2019/11/27.
//

#include "dr_wav.h"
#include "aa_audio_file.h"
#include "aa_wav_decoder.h"
#include "aa_audio_decoder_factory.h"



#include <iostream>

namespace libaa
{

class AudioFile::Impl
{
public:
    int load(const std::string& filename)
    {
        std::string file_ext = filename.substr(filename.find(".") + 1);

        if(file_ext == ".wav")
        {
            loadWaveFile(filename);
        }

        return 0;
    }

    int loadWaveFile(const std::string& filename)
    {
        return 0;
    }

    std::unique_ptr<AudioDecoder> createDecoder(const std::string& filename)
    {
        std::string::size_type idx = filename.rfind('.');
        std::string extension;
        if(idx != std::string::npos)
        {
            std::string extension = filename.substr(idx+1);
        }
        else
        {
            extension = "Unsupported";
        }

        if(extension == "wav")
        {
            return std::make_unique<WavDecoder>();
        }

        std::cerr << "Unsupport " << extension << " audio format\n";
        return nullptr;
    }

    std::unique_ptr<AudioDecoder> decoder;
};

AudioFile::AudioFile()
    : impl_(std::make_shared<Impl>())
{
    
}

int AudioFile::load(const std::string &filename)
{
    impl_->decoder = AudioDecoderFactory::createDecoder(filename);
    if(impl_->decoder == nullptr)
    {
        return -1;
    }

    int failed = impl_->decoder->open(filename);
    if(failed)
    {
        return -1;
    }

    std::vector<float> fbuffer(getNumFrames() * getNumChannels());
    auto num_decoded = impl_->decoder->decodeFile(fbuffer.data(), fbuffer.size());

    // interleave to planar
    const int num_channels = getNumChannels();
    samples.resize(num_channels);
    for(auto& s : samples) {s.resize(getNumFrames());}

    for(int c = 0; c < num_channels; ++c)
    {
        for(int i = 0; i < num_decoded; ++i)
        {
            samples[c][i] = fbuffer[i*num_channels + c];
        }
    }

    return 0;
}
int AudioFile::getNumChannels() const
{
    return impl_->decoder->getNumChannels();
}
int AudioFile::getSampleRate() const
{
    return impl_->decoder->getSampleRate();
}
int AudioFile::getNumBits() const
{
    return impl_->decoder->getBits();
}
int AudioFile::getNumFrames() const
{
    return impl_->decoder->getNumFrames();
}
int AudioFile::saveToWave(const std::string& save_path) const
{
    drwav_data_format format;
    drwav wav;
    format.container = drwav_container_riff;     // <-- drwav_container_riff = normal WAV files, drwav_container_w64 = Sony Wave64.
    format.format = DR_WAVE_FORMAT_PCM;          // <-- Any of the DR_WAVE_FORMAT_* codes.
    format.channels = getNumChannels();
    format.sampleRate = getSampleRate();
    format.bitsPerSample = getNumBits();
    drwav_init_file_write(&wav, save_path.c_str(), &format, NULL);

    std::vector<int16_t> data_s16_interleave(getNumChannels()*getNumFrames());

    int r = 0;
    for(int c = 0; c < getNumChannels(); ++c)
    {
        for(int i = 0; i < getNumFrames(); ++i)
        {
            float x = samples[c][i];
            float xx = 0.0;
            xx = ((x < -1) ? -1 : ((x > 1) ? 1 : x));
            xx = xx + 1;
            r = (int)(xx * 32767.5f);
            r = r - 32768;
            data_s16_interleave[i*getNumChannels() + c] = (short)r;
        }
    }

    drwav_uint64 framesWritten = drwav_write_pcm_frames(&wav, getNumFrames(), data_s16_interleave.data());

    drwav_uninit(&wav);
    return 0;
}

}