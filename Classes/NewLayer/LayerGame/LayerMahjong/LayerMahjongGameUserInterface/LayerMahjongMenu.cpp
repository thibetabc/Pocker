//
//  LayerMahjongMenu.cpp
//  pokerclient
//
//  Created by zhongyu on 16/6/7.
//
//

#include "LayerMahjongMenu.h"
#include "GameDataManager.h"
#include "Room.h"
#include "PokerDataManager.h"
#include "ChatMessageManager.h"
// 引入相关的头文件
#include "LayerShare.h"
bool LayerMahjongMenu::init()
{
    if (!MahjongChildLayer::init())
    {
        return false;
    }
    root_ = CSLoader::createNode("LayerMahjongMenu.csb");
    this->addChild(root_);
    
    image_red_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_,"image_red"));
    image_red_->setVisible(false);
    
    image_mahjong_menu_bg_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_,"image_mahjong_menu_bg"));
    
    btn_chat_0_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_,"btn_chat_0"));
    btn_chat_0_->addClickEventListener(std::bind(&LayerMahjongMenu::click_btn_chat, this,std::placeholders::_1));
    
    btn_match_0_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_,"btn_match_0"));
    btn_match_0_->addClickEventListener(std::bind(&LayerMahjongMenu::click_btn_match, this,std::placeholders::_1));
    
    btn_share_0_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_,"btn_share_0"));
    btn_share_0_->addClickEventListener(std::bind(&LayerMahjongMenu::click_btn_share, this,std::placeholders::_1));
    
    btn_record_last_hand_0_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_,"btn_record_last_hand_0"));
    btn_record_last_hand_0_->addClickEventListener(std::bind(&LayerMahjongMenu::click_btn_record_last_hand, this,std::placeholders::_1));
    btn_record_last_hand_0_->setVisible(false);
    
    text_unread_message_count_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_,"text_unread_message_count"));
    
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(LayerMahjongMenu::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(LayerMahjongMenu::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(LayerMahjongMenu::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    

    
    return true;
}
void LayerMahjongMenu::refresh_chat_btn()
{
    if (this->GetRoom()->get_group_id() == 0)
    {
        btn_chat_0_->setVisible(false);
    }
    else
    {
        btn_chat_0_->setVisible(true);
    }
}
#pragma mark click_event
void LayerMahjongMenu::click_btn_chat(cocos2d::Ref *sender)
{
    ChatMessageManager::getInstance()->clear_unreal_message_by(GetRoom()->get_group_id(), ChatMessageManager::MESSAGETYPE::GROUPCHAT);
    handle_receive_chat(nullptr);//接收信息
    
    gcl = LayerGameChat::createByGroup(GetRoom()->get_group_id());
    this->addChild(gcl, 0,"LayerGameChat");
}
void LayerMahjongMenu::click_btn_match(cocos2d::Ref *sender)
{
    auto &ofc_room_data = this->GetRoom();
    if (ofc_room_data == nullptr)
    {
        cocos2d::log("OfcBackGroundLayer::click_btn_standup ofc_room_data == nullptr");
        return;
    }
    uint32_t roomid = ofc_room_data->get_id();
    PDM->send_get_score(roomid);
}
void LayerMahjongMenu::click_btn_share(cocos2d::Ref *sender)
{
    uint32_t uintUserID = GDM->get_user_id();
    uint32_t uintGroupID = this->GetRoom()->get_group_id();
    uint32_t uintRoomID = this->GetRoom()->get_id();
    
    auto layerShare = LayerShare::create();
    layerShare->setShareGameOfcRoomInfo(uintUserID, uintGroupID, uintRoomID);
    AddGlobalLayer(layerShare);
}
void LayerMahjongMenu::click_btn_record_last_hand(cocos2d::Ref *sender)
{
    
}

#pragma mark touch_event
void LayerMahjongMenu::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event)
{
    Point touchLocation = this->convertTouchToNodeSpace(touch);
    if ( !image_mahjong_menu_bg_->getBoundingBox().containsPoint(touchLocation) )
    {
        this->removeFromParent();
    }
}

#pragma mark func
void LayerMahjongMenu::handle_receive_chat(cmd_data_pointer)
{
    uint32_t unread_message_count = ChatMessageManager::getInstance()->get_unread_message_count(GetRoom()->get_group_id(),ChatMessageManager::MESSAGETYPE::GROUPCHAT);
    if((unread_message_count != 0) && (gcl == NULL)) {
        if (btn_chat_0_->isVisible())
        {
            text_unread_message_count_->setString(tools::to_string(unread_message_count));
            image_red_->setVisible(true);
            text_unread_message_count_->setVisible(true);
        }
    }
    else if(unread_message_count == 0){
        if (btn_chat_0_->isVisible())
        {
            image_red_->setVisible(false);
            text_unread_message_count_->setVisible(false);
        }
    }
}