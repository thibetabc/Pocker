#pragma once
#include <string>


class VoiceRecorderInterface {
public:
    virtual void beginRecordByFileName(const std::string& filename) = 0;
    virtual int stopRecord() = 0;
    virtual int amrToWav(const std::string& amrPath, const std::string& savePath) = 0;
    virtual int wavToAmr(const std::string& wavPath, const std::string& savePath) = 0;
};
