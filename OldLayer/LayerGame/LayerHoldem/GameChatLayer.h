#pragma once


#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "cocos-ext.h"
#include "events.h"
#include "msg.pb.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;

class chat_data;

class GameChatLayer : public Layer
{
public:
    CREATE_FUNC(GameChatLayer);
    virtual bool init();
    
    enum REFRESH_MODE{
        REFRESH_OLD_MESSAGE,
        RECEIVE_NEW_MESSAGE
    };
    
    void click_btn_record(Ref* sender);
    void click_btn_emtion(Ref* sender);
    void click_btn_add(Ref* sender);
    
    void click_image_tf_bg(Ref* sender);
    void touch_btn_tf_bg(Ref* sender, Touch* touch, Widget::TouchEventType);
    
    void touch_listview(Ref* sender, Touch* touch, Widget::TouchEventType event);
    
    void reload_data(cmd_data_pointer = nullptr);
    void refresh_chat_message(REFRESH_MODE set_refresh_mode);
    
    void register_event();
    
    void set_not_touch(bool is_not_touch = true){is_not_touch_ = is_not_touch;};
    
    virtual void onExit();
    void set_isShow_chat(bool isShow_chat){isShow_chat_ = isShow_chat;}
    bool get_isShow_chat(){return isShow_chat_;}
private:
    void after_choose_pic(const std::string& filename);
    bool isShow_chat_;
    ListView* listview_;
    Button* btn_record_;
    Button* btn_emtion_;
    Button* btn_add_;
    Button* btn_tf_bg_;
    Button* btn_keyboard_;
    Text* text_unread_num_;

    ImageView* bg_;
    
    
    EditBox* tf_chat_;
    
    events::event_dispatch dispatch_;
    uint32_t groupid;
    std::list<std::shared_ptr<msg::ChatMessage> > chats_;
    
    bool is_not_touch_;
    
    bool can_refresh_list_;
    
    Vec2 touch_record_begin_position_;
    std::string record_filename_;
};