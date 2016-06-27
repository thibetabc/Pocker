#pragma once

#include "ImagePickerBridge.h"
#import <Foundation/Foundation.h>

@class ImagePickerIosDelegate;

class ImagePickerIos : public ImagePickerBridge
{
public:
    ImagePickerIos(const ImagePickerType type);
    
    ImagePickerIosDelegate* delegate_;

    virtual void change_head_pic(const std::string& pic_name);
    
    void setIsChat(bool isChat){m_bIsChat = isChat;}
private:
    bool m_bIsChat = false;
};


@interface ImagePickerIosDelegate : NSObject <UINavigationControllerDelegate, UIImagePickerControllerDelegate>

@property (nonatomic, assign) ImagePickerIos* iosPicker;

@end