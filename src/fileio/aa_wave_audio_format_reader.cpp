
//
// Created by William.Hua on 2021/2/10.
//

#include "fileio/aa_wav_audio_format_reader.h"
#include "dr_wav.h"

// drwav_bool32 drwav_init(drwav* pWav,
// drwav_read_proc onRead,
// drwav_seek_proc onSeek,
// void* pUserData,
// const drwav_allocation_callbacks* pAllocationCallbacks);
namespace libaa
{
class WaveAudioFormatReader::Impl
{
public:
    Impl(WaveAudioFormatReader* parent):
        parent_(parent)
    {
        openWaveFromStream();
    }

    ~Impl(){
        drwav_uninit(&wav_);
    }

    void openWaveFromStream(){
        if(drwav_init(&wav_,readCallback,seekCallback,this,nullptr))
        {
            parent_->sample_rate = wav_.sampleRate;
            parent_->num_channels = wav_.channels;
            parent_->length_in_samples = wav_.totalPCMFrameCount;
            parent_->num_bits = wav_.bitsPerSample;
        }
    }

    static size_t readCallback(void* pUserData, void* pBufferOut, size_t bytesToRead)
    {
        auto* self = (WaveAudioFormatReader::Impl*)(pUserData);

        if(!self->parent_->in_stream_.read((char*)pBufferOut, bytesToRead)){
            // clear bits so that seekCallback can seek successfully
            self->parent_->in_stream_.clear();
            return 0;
        }
        return bytesToRead;
    }

    static drwav_bool32 seekCallback(void* pUserData, int offset, drwav_seek_origin origin)
    {
        auto* self = (WaveAudioFormatReader::Impl*)(pUserData);
        if(origin == drwav_seek_origin_current)
        {
            if(!self->parent_->in_stream_.seekg(offset, std::ios::cur)){
                return DRWAV_FALSE;
            }

        }else if(origin == drwav_seek_origin_start)
        {
            int y = self->parent_->in_stream_.tellg();
            (void)y;
            if(!self->parent_->in_stream_.seekg(offset, std::ios::beg)){
                int x = self->parent_->in_stream_.tellg();
                (void)x;
                return DRWAV_FALSE;
            }
        }

        return DRWAV_TRUE;
    }


    WaveAudioFormatReader* parent_;
    drwav wav_;
};

WaveAudioFormatReader::WaveAudioFormatReader(std::istream& in_stream)
    : AudioFormatReader(in_stream),
      impl_(std::make_shared<Impl>(this))
{

}

bool WaveAudioFormatReader::readSamples(float **dest_channels,
                                        int num_dest_channels,
                                        int start_offset_of_dest,
                                        int64_t start_offset_of_file,
                                        int num_samples)
{
    return false;
}
bool WaveAudioFormatReader::isOpenOk() {
    return sample_rate > 0 && num_channels > 0 && length_in_samples > 0;
}

}