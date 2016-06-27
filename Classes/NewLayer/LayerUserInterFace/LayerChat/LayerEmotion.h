#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "SceneManager.h"
#include "cocos-ext.h"
#include "events.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;

class LayerEmotion : public Layer
{
public:
    
    CREATE_FUNC(LayerEmotion);
    virtual bool init();
    
    void init_emotions();
    
    void click_emotion(Ref* sender);
    void click_btn_send(Ref* sender);
    virtual void click_btn_keyboard(Ref* sender);
    void click_btn_del(Ref* sender);
    void click_btn_voice(Ref* sender);
    
    
    void pageview_turn(Ref*, PageView::EventType);
    
    virtual void removeFromParent();
    void set_dismiss_callback(const std::function<void()>& callback) { dismiss_callback_ = callback;}
    void set_voice_callback(const std::function<void()>& callback) { voice_callback_ = callback;}
    void set_chat_callback(const std::function<void()>& callback) { chat_callback_ = callback;}
    void set_emoition_callback(const std::function<void(std::string)>& callback) {emotion_callback_ = callback;}
    
    
    std::vector<std::pair<std::string, Sprite*> > get_input_emotions() {return input_emotions_;}
    
//#pragma mark EditBoxDelegate
//    virtual void editBoxEditingDidBegin(EditBox* editBox);
//    virtual void editBoxEditingDidEnd(EditBox* editBox) {};
//    virtual void editBoxTextChanged(EditBox* editBox, const std::string& text) {};
//    virtual void editBoxReturn(EditBox* editBox){}
//    
    
private:
    Button* btn_voice_;
    Button* btn_keyboard_;
    Button* btn_del_;
    TextField* tf_chat_;
    ImageView* image_container_;
    
    //    EditBox* tf_chat_;
    
    PageView* pageview_;
    
    std::vector<Sprite*> page_icons_;
    
    std::vector<std::pair<std::string, Sprite*> > input_emotions_;
    
    std::function<void()> dismiss_callback_;
    std::function<void()> voice_callback_;
    std::function<void()> chat_callback_;
    std::function<void(std::string)> emotion_callback_;
};