#ifndef __pokerclient__ChatMessageCell__
#define __pokerclient__ChatMessageCell__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "BaseLayer.h"
#include "cocos-ext.h"
#include "events.h"
#include "msg.pb.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;

class chat_data;

class ChatMessageCell : public Widget {
public:
    CREATE_FUNC(ChatMessageCell);
    
    virtual bool init();
    
    void update_data(const std::shared_ptr<msg::ChatMessage>& data, const float listview_width, const bool in_game = false);
    
    void click_image_bg(Ref* sender);
    void click_sprite_icon(Ref* sender);
    void click_button_resend(Ref* sender);
    const std::shared_ptr<msg::ChatMessage>& get_message(){return data_;}
    void update_header_image(cmd_data_pointer);
    
    void handle_upload_succeed(cmd_data_pointer);
    
    void set_not_touch(bool is_not_touch = true){is_not_touch_ = is_not_touch;};
    
//    void reset();
    ListView* listview;
private:
//    Text* text_message_;
//    Sprite* sprite_voice_;
//    ImageView* image_bg_;
//    Text* text_time_;
//    ImageView* image_head_;
//    ImageView* image_system_time_;
//    Text* text_system_time_;
    
    
//    Point image_head_origin_position_;
    
    Node* root_;
    
    Size size_;
    
    std::vector<Sprite*> emotions_;
    
    std::shared_ptr<msg::ChatMessage> data_;
    bool in_game_;
//    std::map<Node*, std::pair<Point, Size> > element_positions_;
    
    std::string download_image_;
    
    events::event_dispatch dispatch_;
    
    
    Layer *m_pLayer;
    
    float listview_width_;
    
    int sound_id_;
    
    static ChatMessageCell* select_cell;
    
    bool is_not_touch_;
};

#endif /* defined(__pokerclient__ChatMessageCell__) */
