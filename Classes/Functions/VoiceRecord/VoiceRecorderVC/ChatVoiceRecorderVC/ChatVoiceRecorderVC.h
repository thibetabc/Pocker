//
//  ChatVoiceRecorderVC.h
//  Jeans
//
//  Created by Jeans on 3/23/13.
//  Copyright (c) 2013 Jeans. All rights reserved.
//

#import "VoiceRecorderBaseVC.h"



@interface ChatVoiceRecorderVC : VoiceRecorderBaseVC

//开始录音
- (void)beginRecordByFileName:(NSString*)_fileName;
- (int) stopRecord;
@end
