//
// Created by william on 2019/11/28.
//

#pragma once
#include <string>
#include <vector>

namespace libaa
{
class AudioFile
{
public:
    AudioFile();
    /**
     * open file and decode it. All decoded samples store in the member variable 'samples'
     * @param filename
     * @return 0 successful, others failed.
     */
    int load(const std::string& filename);

    int getSampleRate() const;
    int getNumChannels() const;
    int getNumBits() const;
    int getNumFrames() const;

    int saveToWave(const std::string& save_path) const;

    std::vector<std::vector<float>> samples;

    class Impl;
    std::shared_ptr<Impl> impl_;
};

}
