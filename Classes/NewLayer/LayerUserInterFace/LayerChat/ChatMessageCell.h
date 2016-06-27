//
//  ChatMessageCell.h
//  pokerclient
//
//  Created by lvzhangle on 16/4/11.
//
//

#ifndef __pokerclient__ChatMessageCell__
#define __pokerclient__ChatMessageCell__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "cocos-ext.h"
#include "events.h"
#include "msg.pb.h"
#include "ShaderSprite.h"


using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;

#define MESSAGE_CELL_OFFSET 10

#define MESSAGE_BG_WIDTH_MIN 150
#define MESSAGE_BG_WIDTH_MAX 536
#define MESSAGE_BG_HEIGHT_MIN 170

#define MESSAGE_BG_OFFSET_TOP 60
#define MESSAGE_BG_OFFSET_BUTTOM 80
#define MESSAGE_BG_OFFSET_LEFT 60
#define MESSAGE_BG_OFFSET_RIGHT 60

//image_text_width_max 488
#define MESSAGE_TEXT_BG_WIDTH_MAX 575
#define MESSAGE_TEXT_BG_WIDTH_MIN 80
#define MESSAGE_TEXT_BG_HEIGHT_MIN 38

#define MESSAGE_TEXT_BG_OFFSET_TOP 30
#define MESSAGE_TEXT_BG_OFFSET_BUTTOM 30
#define MESSAGE_TEXT_BG_OFFSET_LEFT 30
#define MESSAGE_TEXT_BG_OFFSET_RIGHT 30


#define MESSAGE_TEXT_FONT_SIZE 28
#define MESSAGE_NICK_NAME_FONT_SIZE 26
#define MESSAGE_MESSAGE_TIME_FONT_SIZE 26

#define ZORDER_MESSAGE_BG 0
#define ZORDER_MESSAGE_BG_SPRITE 5
#define ZORDER_MESSAGE_TEXT 10

#define ZORDER_MESSAGE_TIME 30
#define ZORDER_MESSAGE_NICK_NAME 40
#define ZORDER_MESSAGE_HEAD_ICON 50
#define ZORDER_MESSAGE_STATUS 60

class ChatMessageCell : public TableViewCell
{
public:
    CREATE_FUNC(ChatMessageCell);
    virtual bool init();
    
    void update_data(const msg::ChatMessage& chat_message_data, const float listview_width, bool isDisplayMessageTime = false);
    
    void message_cell_ui_clear();
    void init_head_sprite();
    void display_message_time();
    
    void click_head_icon(Ref * sender);
    void click_image_bg(Ref * sender);
    void click_resend_message(Ref * sender);
    
    void handle_download_event(cmd_data_pointer data);

private:
    //std::shared_ptr<msg::ChatMessage> chat_message_data_;
    msg::ChatMessage chat_message_data_;
    
    events::event_dispatch dispatch_;
        
    float listview_width_;
    
    std::string image_head_file_;
    
#pragma mark UI
private:
    //std::vector<Sprite*> emotions_;
    
    std::vector<std::vector<Node*> > vec_emotions_;
    
    Layer *m_pLayer;
    
    RichText * richtext_message_;
    
    ShaderSprite* sprite_head_icon_;
    
    ImageView* image_voice_icon_;
    
    Widget* widget_head_bg_;
    
    ImageView * image_chatmessage_cell_bg_;
    
    Text* text_nick_name_;
    Text* text_message_time_;
    
    Sprite* sprite_sending_;
    Button* btn_resend_;
    
    static std::string select_voice_;
    
    ImageView* image_room_icon_;
    ImageView* image_chatmessage_picture_;
};

#endif /* defined(__pokerclient__ChatMessageCell__) */