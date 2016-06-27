#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "SceneManager.h"
#include "events.h"

#include "ImagePickerBridge.h"

using namespace cocos2d;
using namespace cocos2d::ui;

class ImagePickerBridge;

class LayerChoosePhoto : public BaseLayer {
public:
    CREATE_FUNC(LayerChoosePhoto);
    virtual bool init();
    
    void click_image_camera(Ref* sender);
    void click_image_album(Ref* sender);
    void click_image_cancel(Ref* sender);
    
    virtual void onEnterTransitionDidFinish();
    void removeFromParentAndCleanup(bool cleanup);
    ImagePickerBridge* picker_;
    
    void set_dismiss_callback(const std::function<void()>& callback) {
        dis_miss_callback_ = callback;
    }
    
    void set_choose_photo_callback(const std::function<void(std::string)> & callback){
        choose_photo_callback_ = callback;
    }
    void setIsChat(bool isChat){m_bIsChat = isChat;}
    void close();
private:
    bool m_bIsChat = false;
    cocos2d::Point origin_;
    Node* root_;
    
    std::function<void()> dis_miss_callback_;
    std::function<void(std::string)> choose_photo_callback_;
};

extern ImagePickerBridge* __createImagePicker(const ImagePickerType type);