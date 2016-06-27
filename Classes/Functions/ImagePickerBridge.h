#pragma once
#include <string>
#include "cocos2d.h"
enum ImagePickerType{
    CameraWithEdit = 0,
    PhotoLibraryWithEdit = 1,
    PhotoLibraryWithoutEdit = 2,
    CameraWithoutEdit = 3
};

class ImagePickerBridge
{
public:
    virtual void change_head_pic(const std::string& pic_name);
    virtual void do_cancel();
    void SetCallBack(const std::function<void (const std::string&)> callback);
private:
    std::function<void (const std::string&)> finish_callback_;
};
