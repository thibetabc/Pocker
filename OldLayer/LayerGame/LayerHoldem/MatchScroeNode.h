#pragma once


#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "cocos-ext.h"
#include "events.h"
#include "msg.pb.h"
#include "LayerManager.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;

class MatchScroeNode : public LayerPop
{
public:
    CREATE_FUNC(MatchScroeNode);
    virtual bool init();
    
    void update_data();
    
    void set_dismiss_callback(const std::function<void()>& callback) { dismiss_callback_ = callback;}
    
    virtual void removeFromParent();
    
    void click_btn_close(Ref* sender);
#pragma mark touch delegate
    virtual bool onTouchBegan(Touch *touch, Event *unused_event);
    /** Callback function for touch moved.
     *
     * @param touch Touch infomation.
     * @param unused_event Event information.
     * @js NA
     */
    virtual void onTouchMoved(Touch *touch, Event *unused_event){}
    /** Callback function for touch ended.
     *
     * @param touch Touch infomation.
     * @param unused_event Event information.
     * @js NA
     */
    virtual void onTouchEnded(Touch *touch, Event *unused_event){}


    class MATCH_OBJECT
    {
    public:
        std::string nickname;
        std::string head_pic_name;
        uint32_t buy_amount;
        int32_t win_amount;
        msg::RoomType room_type;
    };
    void register_events();
    
    void handle_get_score(const msg::Processor_420_PokerGetScore_DOWN& processor);
    
    
    Button* btn_close;

private:
    Node* root_;
    Text *text_buy;
    Text *text_win;
//    TableView* tableview_;
    ListView* listview_;
//    Text* text_time_;
//    Text* text_room_name_;
    
    std::function<void()> dismiss_callback_;
   
    
    std::vector<std::shared_ptr<MATCH_OBJECT> > match_objects_;
    
    events::event_dispatch dispatch_;
};