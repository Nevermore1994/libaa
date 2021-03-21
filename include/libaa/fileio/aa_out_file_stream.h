
//
// Created by William.Hua on 2021/3/21.
//

#pragma once
#include <string>
#include "aa_output_stream.h"
namespace libaa
{
class OFileStream : public OutputStream
{
public:
    OFileStream() = default;
    explicit OFileStream(const std::string& path);

    ~OFileStream() override;

    bool isOpen() const;

    int open(const std::string& path);

    void close();

    int64_t write(const uint8_t* source_buf, int64_t size) override;

    int64_t tellp() const override;

    int seekp(int64_t pos, int mode) override;

    int64_t length() override;

private:
    FILE* fp_{nullptr};
};

}
