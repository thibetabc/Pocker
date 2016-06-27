#include "MatchScroeNode.h"
#include "utils.h"
#include "MatchScroeCell.h"
#include "cmd_def.h"
#include "game_net_data.h"
#include "PokerDataManager.h"
#include "Room.h"
#include "msg.pb.h"

bool MatchScroeNode::init()
{
    if (!Layer::init()) {
        return false;
    }
    LayerColor *layer = LayerColor::create(Color4B(100,100,100,100));
    this->addChild(layer);
    this->setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    this->setTouchEnabled(true);
    this->setSwallowsTouches(true);

    
    
    return true;
}

bool MatchScroeNode::onTouchBegan(Touch *touch, Event *unused_event)
{
    this->removeFromParent();
    return true;
}

void MatchScroeNode::removeFromParent()
{
    Node::removeFromParent();
    if(dismiss_callback_) {
        dismiss_callback_();
    }
}


void MatchScroeNode::click_btn_close(Ref* sender)
{
    removeFromParent();
}


void MatchScroeNode::handle_get_score(const msg::Processor_420_PokerGetScore_DOWN& processor)
{
    match_objects_.clear();
    std::string subtime = processor.time();
    if (subtime.empty())
    {
        subtime = "游戏时长：无";
    }
    else
    {
        subtime = "游戏时长：" + subtime;
    }
    if (processor.roomtype() == msg::RoomType::OFC3 || processor.roomtype() == msg::RoomType::OFC4)
    {
        root_ = CSLoader::createNode("LayerOfcMatchScorePortrait.csb");
        this->addChild(root_);
        auto text_title = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_title"));
        auto text_nickname = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_name"));
        auto text_score = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_score"));
        auto text_current_time = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_current_time"));
        btn_close = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_ofc_match_close"));
        btn_close->addClickEventListener(std::bind(&MatchScroeNode::click_btn_close, this, std::placeholders::_1));
        
        listview_ = dynamic_cast<ListView*>(CSLoader::seekNodeByName(root_, "listview"));
        
        
        
        text_current_time->setString(subtime);
        
        
    }else
    {
        auto root = CSLoader::createNode("LayerTimeRecord.csb");
        this->addChild(root);
        root_ = root;
        auto text_nickname = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_nick_name"));
        text_buy = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_buy_in"));
        text_win = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_gain"));
        btn_close = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "Button_1"));
        
        //    text_room_name_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_room_name"));
        btn_close->addClickEventListener(std::bind(&MatchScroeNode::click_btn_close, this, std::placeholders::_1));
        //    text_title->setString(tools::local_string("match_result", "实时战绩"));
        text_nickname->setString(tools::local_string("nickname", "昵称"));
        text_buy->setString(tools::local_string("buy_chip", "买入"));
        text_win->setString(tools::local_string("win_chip", "盈利"));
        auto text_play_time = dynamic_cast<Text *>(CSLoader::seekNodeByName(root, "Text_5"));
        text_play_time->setString(subtime);
        listview_ = dynamic_cast<ListView*>(CSLoader::seekNodeByName(root, "ListView_1"));
        listview_->setEnabled(true);
    }

    
    for (auto& it: processor.playerscores().score()) {
        ::std::string nickname = it.userdetailinfo().nickname();
        ::std::string head_pic_name = it.userdetailinfo().picname();
        int32_t buy_amount = it.totalbuyin();
        int32_t win_amount = it.totalwin();
        
        std::shared_ptr<MATCH_OBJECT> obj(new MATCH_OBJECT());
        obj->nickname = nickname;
        obj->head_pic_name = head_pic_name;
        obj->buy_amount = buy_amount;
        obj->win_amount = win_amount;
        obj->room_type = processor.roomtype();
        match_objects_.push_back(obj);
    }
    
    auto sortfuncitonWin = [](std::shared_ptr<MATCH_OBJECT> a, std::shared_ptr<MATCH_OBJECT> b){return a->win_amount  > b->win_amount;};
    std::sort(match_objects_.begin(), match_objects_.end(), sortfuncitonWin);
    
    listview_->removeAllItems();
    for (auto& iter : match_objects_) {
        MatchScroeCell* item = MatchScroeCell::create();
        listview_->pushBackCustomItem(item);
        item->update_data(iter);
    }
    
    //    tableview_->reloadData();
    
    
}