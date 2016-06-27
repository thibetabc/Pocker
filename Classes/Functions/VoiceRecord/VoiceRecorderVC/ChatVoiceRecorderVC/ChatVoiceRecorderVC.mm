//
//  ChatVoiceRecorderVC.m
//  Jeans
//
//  Created by Jeans on 3/23/13.
//  Copyright (c) 2013 Jeans. All rights reserved.
//

#import "ChatVoiceRecorderVC.h"
#import "VoiceConverter.h"
#include "GameDataManager.h"

#define MAX_RECORD_TIME 60
#define MIN_RECORD_TIME 2


@interface ChatVoiceRecorderVC ()<AVAudioRecorderDelegate>{
    CGFloat                 curCount;           //当前计数,初始为0
    CGPoint                 curTouchPoint;      //触摸点
    NSTimer                 *timer;
}

@property (retain, nonatomic)   AVAudioRecorder     *recorder;

@end

@implementation ChatVoiceRecorderVC
@synthesize recorder;

- (void)dealloc{
    [recorder release];
    [super dealloc];
}

#pragma mark - 开始录音
- (void)beginRecordByFileName:(NSString*)_fileName;{
    maxRecordTime = MAX_RECORD_TIME;
    //设置文件名和录音路径
    self.recordFileName = _fileName;
    self.recordFilePath = [VoiceRecorderBaseVC getPathByFileName:recordFileName ofType:@"wav"];

    //初始化录音
    self.recorder = [[[AVAudioRecorder alloc]initWithURL:[NSURL URLWithString:recordFilePath]
                                                settings:[VoiceRecorderBaseVC getAudioRecorderSettingDict]
                                                   error:nil]autorelease];
    recorder.delegate = self;
    recorder.meteringEnabled = YES;

    [recorder prepareToRecord];
    
    //还原计数
    curCount = 0;
    
    //开始录音
    [[AVAudioSession sharedInstance] setCategory: AVAudioSessionCategoryPlayAndRecord error:nil];
    [[AVAudioSession sharedInstance] setActive:YES error:nil];
    UInt32 audioRouteOverride = kAudioSessionOverrideAudioRoute_Speaker;
	AudioSessionSetProperty(kAudioSessionProperty_OverrideAudioRoute, sizeof(audioRouteOverride), &audioRouteOverride);
    [recorder record];
    
    //启动计时器
    [self startTimer];
    
   
}
#pragma mark - 停止录音
- (int) stopRecord
{
    [self stopTimer];
    if (recorder.isRecording)
        [recorder stop];
    if(curCount < MIN_RECORD_TIME) {
        curCount = 0;
        [ChatVoiceRecorderVC deleteFileAtPath:self.recordFilePath];
    }
    else {
//        NSString* amrPath = [VoiceRecorderBaseVC getPathByFileName:recordFileName ofType:@"amr"];
        NSString* amrPath = [NSString stringWithFormat:@"%s%@.amr", GDM->get_user_directory().c_str(), recordFileName];
        [VoiceConverter wavToAmr:self.recordFilePath amrSavePath:amrPath];
//        [ChatVoiceRecorderVC deleteFileAtPath:self.recordFilePath];
    }
    return curCount;
}

#pragma mark - 启动定时器
- (void)startTimer{
    timer = [NSTimer scheduledTimerWithTimeInterval:0.1f target:self selector:@selector(updateMeters) userInfo:nil repeats:YES];
}

#pragma mark - 停止定时器
- (void)stopTimer{
    if (timer && timer.isValid){
        [timer invalidate];
        timer = nil;
    }
}
#pragma mark - 更新音频峰值
- (void)updateMeters{
    if (recorder.isRecording){
    
        //更新峰值
        [recorder updateMeters];
//            double lowPassResults = pow(10, (0.05 * [recorder peakPowerForChannel:0]));
//        NSLog(@"峰值:%f",[recorder averagePowerForChannel:0]);
        
        //倒计时
        if (curCount >= maxRecordTime - 10 && curCount < maxRecordTime) {
            //剩下10秒

        }else if (curCount >= maxRecordTime){
            //时间到
            [self stopRecord];
        }
        curCount += 0.1f;
    }
}

    
#pragma mark - AVAudioRecorder Delegate Methods
- (void)audioRecorderDidFinishRecording:(AVAudioRecorder *)recorder successfully:(BOOL)flag{
    NSLog(@"录音停止");
    
    [self stopTimer];
    curCount = 0;
}
- (void)audioRecorderBeginInterruption:(AVAudioRecorder *)recorder{
    NSLog(@"录音开始");
    [self stopTimer];
    curCount = 0;
}
- (void)audioRecorderEndInterruption:(AVAudioRecorder *)recorder withOptions:(NSUInteger)flags{
    NSLog(@"录音中断");
    [self stopTimer];
    curCount = 0;
}

@end
