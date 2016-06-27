//
//  VoiceRecorderAndroid.h
//  pokerclient
//
//  Created by MirrorMiko on 6/7/15.
//
//

#ifndef __pokerclient__VoiceRecorderAndroid__
#define __pokerclient__VoiceRecorderAndroid__

#include <stdio.h>
#include "VoiceRecorderInterface.h"

class VoiceRecorderAndroid : public VoiceRecorderInterface{
    
public:
    VoiceRecorderAndroid();
    ~VoiceRecorderAndroid();
    virtual void beginRecordByFileName(const std::string& filename);
    virtual int stopRecord();
    
    virtual int amrToWav(const std::string& amrPath, const std::string& savePath);
    
    virtual int wavToAmr(const std::string& wavPath, const std::string& savePath);
    
private:
};

#endif /* defined(__pokerclient__VoiceRecorderAndroid__) */
