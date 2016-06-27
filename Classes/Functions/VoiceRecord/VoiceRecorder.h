//
//  VoiceRecorder.h
//  AndroidProject
//
//  Created by duanmt on 15/4/23.
//
//

#ifndef __AndroidProject__VoiceRecorder__
#define __AndroidProject__VoiceRecorder__

#include <string>
#include "VoiceRecorderInterface.h"

@class ChatVoiceRecorderVC;

class VoiceRecorder : public VoiceRecorderInterface{
    
public:
    VoiceRecorder();
    ~VoiceRecorder();
    virtual void beginRecordByFileName(const std::string& filename);
    virtual int stopRecord();
    
    virtual int amrToWav(const std::string& amrPath, const std::string& savePath);
    
    virtual int wavToAmr(const std::string& wavPath, const std::string& savePath);
    
private:
    ChatVoiceRecorderVC* recoder_;
};

#endif /* defined(__AndroidProject__VoiceRecorder__) */
