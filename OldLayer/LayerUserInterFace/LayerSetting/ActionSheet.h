#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "BaseLayer.h"
#include "events.h"
#include "LayerManager.h"
#include "ImagePickerBridge.h"

using namespace cocos2d;
using namespace cocos2d::ui;

class ActionSheet : public LayerPop {
public:
    CREATE_FUNC(ActionSheet);
    virtual bool init();
    
    void click_image_camera(Ref* sender);
    void click_image_album(Ref* sender);
    void click_image_cancel(Ref* sender);
    
    virtual void onEnterTransitionDidFinish();
    
    ImagePickerBridge* picker_;
    
    void set_dismiss_callback(const std::function<void()>& callback) {
        dis_miss_callback_ = callback;
    }
    void set_after_choose_pic_callback(const std::function<void(const std::string&)>& callback) { after_choose_pic_callback_ = callback;}
    void after_choose_pic(const std::string& filename);
    void setIsChat(bool isChat){m_bIsChat = isChat;}
    void close();
private:
    bool m_bIsChat = false;
    Point origin_;
    Node* root_;
    
    std::function<void (const std::string&)> after_choose_pic_callback_;
    std::function<void()> dis_miss_callback_;
};

extern ImagePickerBridge* __createImagePicker(const ImagePickerType type);