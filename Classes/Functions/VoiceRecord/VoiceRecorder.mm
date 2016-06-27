//
//  VoiceRecorder.cpp
//  AndroidProject
//
//  Created by duanmt on 15/4/23.
//
//

#include "VoiceRecorder.h"
#import "ChatVoiceRecorderVC.h"
#include "VoiceRecorder.h"
#include "VoiceConverter.h"

std::shared_ptr<VoiceRecorderInterface> __createRecorder()
{
    return std::shared_ptr<VoiceRecorderInterface>(new VoiceRecorder());
}


VoiceRecorder::VoiceRecorder()
{
    recoder_ = [[ChatVoiceRecorderVC alloc] init];
}
VoiceRecorder::~VoiceRecorder()
{
    [recoder_ release];
}

void VoiceRecorder::beginRecordByFileName(const std::string& filename)
{
    NSString* file = [NSString stringWithFormat:@"%s", filename.c_str()];
    [recoder_ beginRecordByFileName:file];
}

int VoiceRecorder::stopRecord()
{
    return [recoder_ stopRecord];
}

int VoiceRecorder::amrToWav(const std::string& amrPath, const std::string& savePath)
{
    NSString* arm_path = [NSString stringWithUTF8String:amrPath.c_str()];
    NSString* save_path = [NSString stringWithUTF8String:savePath.c_str()];
    return [VoiceConverter amrToWav:arm_path wavSavePath:save_path];
}

int VoiceRecorder::wavToAmr(const std::string& wavPath, const std::string& savePath)
{
    NSString* wave_path = [NSString stringWithUTF8String:wavPath.c_str()];
    NSString* save_path = [NSString stringWithUTF8String:savePath.c_str()];
    return [VoiceConverter wavToAmr:wave_path amrSavePath:save_path];
}