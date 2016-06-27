#include "PlayerActionNode.h"
#include "PokerDataManager.h"
#include "game_net_data.h"
#include "UserPropertyLayer.h"
#include "cmd_def.h"
#include "Room.h"

bool PlayerActionNode::init()
{
    if(!Layer::init()) {
        return false;
    }
    
    /*auto layout = Layout::create();
    layout->setContentSize(Director::getInstance()->getWinSize());
    layout->setTouchEnabled(true);
    layout->addClickEventListener([this](Ref*) {
        this->removeFromParent();
    });
    this->addChild(layout, -1);*/
    
    auto root = CSLoader::createNode("LayerPlayerActionII.csb");
    root->setPositionY(root->getPositionY() - 45);
    root->setPositionX(root->getPositionX() - 10);
    this->addChild(root);

    image_standup_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "Button_1"));
    image_standup_->addClickEventListener(std::bind(&PlayerActionNode::click_standup, this, std::placeholders::_1));
    point_image_1 = image_standup_->getPosition();
    image_quit_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "Button_2"));
    image_quit_->addClickEventListener(std::bind(&PlayerActionNode::click_quit, this, std::placeholders::_1));
    point_image_2 = image_quit_->getPosition();
    image_setting_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "Button_3"));
    image_setting_->addClickEventListener(std::bind(&PlayerActionNode::click_setting, this, std::placeholders::_1));
    point_image_3 = image_setting_->getPosition();
    auto btn_remove = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "Button_4"));
    btn_remove->addClickEventListener(std::bind(&PlayerActionNode::click_remove, this));
    events::event_dispatch dispatch_;
    dispatch_.register_event(EVENT_QUIT_ROOM,BIND_FUNC(this, PlayerActionNode::quit));
    this->setTouchEnabled(true);
    this->setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    this->setSwallowsTouches(true);
    return true;
}

void PlayerActionNode::click_remove()
{
    removeFromParent();
}

void PlayerActionNode::set_is_tournament(bool is_tournament)
{
    if (is_tournament)
    {
        image_standup_->setVisible(false);
        image_quit_->setPosition(point_image_1);
    }else
    {
        image_standup_->setVisible(true);
        image_standup_->setPosition(point_image_1);
        image_quit_->setPosition(point_image_2);
        image_setting_->setPosition(point_image_3);
    }
}


void PlayerActionNode::quit(cmd_data_pointer data)
{
    click_quit(NULL);
}

void PlayerActionNode::click_standup(Ref* sender)
{
    auto &poker_room_data = this->GetRoom();
    if (poker_room_data == nullptr)
    {
        cocos2d::log("NodeOfcPlayer::update_dealer ofc_room_data == nullptr");
        return;
    }
    uint32_t roomid = poker_room_data->get_room_id();
    
    //uint32_t seat_id = PDM->get_seat_num();
    
    PDM->send_seat_info(roomid, msg::SittingStatus::OBSERVER, 0);
    this->removeFromParent();
}

void PlayerActionNode::click_quit(Ref* sender)
{
    auto &poker_room_data = this->GetRoom();
    if (poker_room_data == nullptr)
    {
        cocos2d::log("NodeOfcPlayer::update_dealer ofc_room_data == nullptr");
        return;
    }
    uint32_t roomid = poker_room_data->get_room_id();

    //uint32_t seat_id = PDM->get_seat_num();
    
    PDM->send_seat_info(roomid, msg::SittingStatus::LEAVE, 0);
    
    removeFromParent();
}

void PlayerActionNode::click_setting(Ref* sender)
{
    
    UserPropertyLayer* upl = UserPropertyLayer::create(false);
//    upl->set_btn_close_visible(true);
    
    auto &poker_room_data = this->GetRoom();
    if (poker_room_data == nullptr)
    {
        cocos2d::log("PlayerActionNode::click_setting poker_room_data == nullptr");
    }else{
        upl->set_click_item(poker_room_data->get_room_type());
    }

    
    this->addChild(upl);
    
}

void PlayerActionNode::onExit()
{
    if(dismiss_callback_) {
        dismiss_callback_();
    }
    LayerPop::onExit();
}