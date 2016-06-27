//
//  cmd_def.h
//  pokerclient
//
//  Created by MirrorMiko on 15/5/6.
//
//

#ifndef pokerclient_cmd_def_h
#define pokerclient_cmd_def_h

enum{
    EVENT_DOWLOAD_FILE = 20000,  //下载文件成功。
    EVENT_UPLOAD_FILE_SUCCESS,//上传文件成功
    EVENT_UPLOAD_FILE_FAILED,
    EVENT_CONNECT_SUCCESS,
    EVENT_CONNECT_FAILED,
    EVENT_CONNECT_BREAK,
    EVENT_RECV_ERROR,
    EVENT_ANDROID_BACK,
    IOS_PAY_LIST_REFRESH,
    EVENT_HIT_NOT_ENOUGH_CALL,
};

#endif
