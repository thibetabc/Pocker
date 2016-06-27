#pragma mark

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "msg.pb.h"

using namespace cocos2d;
using namespace cocos2d::ui;

class GameNameCell;

class RoomTitle : public Layout
{
public:
    CREATE_FUNC(RoomTitle);
    virtual bool init();
    
    void push_back_item(GameNameCell* item);
    void clear_all_items();
    void set_click_item(const std::function<void(msg::RoomType)>& item) {click_item_ = item;}
    
    void touch_event(Ref*, Touch*, Widget::TouchEventType);
    
    void click_item(msg::RoomType room_type);
    float get_item_width();
    void move_to_left();
    void move_to_right();
    
    void set_view_size(const Size& size);
    
    msg::RoomType get_middle_item();
    
private:
    std::vector<GameNameCell*> items_;
    std::function<void(msg::RoomType)> click_item_;
    
    Node* inner_box_;
    
    bool move_;
    int counter_;
    
    msg::RoomType current_select_idx_;
    
    ClippingRectangleNode* clipper_;

    bool in_game_;
};