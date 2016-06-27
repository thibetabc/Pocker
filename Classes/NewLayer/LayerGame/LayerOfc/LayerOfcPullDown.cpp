//
//  LayerOfcPullDown.cpp
//  pokerclient
//
//  Created by zhongyu on 16/5/19.
//
//

#include "LayerOfcPullDown.h"
#include "utils.h"
#include "PokerDataManager.h"
#include "Room.h"
#include "GameDataManager.h"
#include "TipView.h"
#include "LayerGameSetting.h"
bool LayerOfcPullDown::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    root_ = CSLoader::createNode("LayerOfcMenu.csb");
    this->addChild(root_);
    
    image_leave_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_leave"));
    image_leave_->setTouchEnabled(true);
    image_leave_->addClickEventListener(std::bind(&LayerOfcPullDown::click_leave, this,std::placeholders::_1));
    
    image_setting_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_setting"));
    image_setting_->setTouchEnabled(true);
    image_setting_->addClickEventListener(std::bind(&LayerOfcPullDown::click_setting, this,std::placeholders::_1));
    
    image_stand_up_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_stand_up"));
    image_stand_up_->setTouchEnabled(true);
    image_stand_up_->addClickEventListener(std::bind(&LayerOfcPullDown::click_stand_up, this,std::placeholders::_1));
    
    text_leave_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_leave"));
    text_leave_->setTouchEnabled(true);
    text_leave_->addClickEventListener(std::bind(&LayerOfcPullDown::click_leave, this,std::placeholders::_1));
    
    text_stand_up_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_stand_up"));
    text_stand_up_->setTouchEnabled(true);
    text_stand_up_->addClickEventListener(std::bind(&LayerOfcPullDown::click_stand_up, this,std::placeholders::_1));
    
    text_setting_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_setting"));
    text_setting_->setTouchEnabled(true);
    text_setting_->addClickEventListener(std::bind(&LayerOfcPullDown::click_setting, this,std::placeholders::_1));
    
    text_stand_up_->setString(tools::local_string("standing_up","站起"));
    text_leave_->setString(tools::local_string("leave_room","离开"));
    text_setting_->setString(tools::local_string("setting","设置"));
    
    image_ofc_pull_bg_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_ofc_pull_bg"));
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(LayerOfcPullDown::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(LayerOfcPullDown::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(LayerOfcPullDown::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    
    return true;
}

void LayerOfcPullDown::click_leave(cocos2d::Ref *sender)
{
    auto &ofc_room_data = this->GetRoom();
    if (ofc_room_data == nullptr)
    {
        cocos2d::log("OfcBackGroundLayer::click_btn_quit ofc_room_data == nullptr");
        return;
    }
    uint32_t roomid = ofc_room_data->get_id();
    auto users = ofc_room_data->get_users();
    
    auto self_user = users.find(GDM->get_user_id());
    if(self_user != users.end() && self_user->second->get_status() == 0)
    {
        TipView* tip = TipView::showAlertView(tools::local_string("will_quit_room", "本局游戏之后将生效"));
        tip->show_cancel(true);
        tip->set_tipview_close_callback([=](){
            //uint32_t seat_id = self_user->second->get_seat_num();//
            uint32_t seat_id = 0;
            PDM->send_seat_info(roomid, msg::SittingStatus::LEAVE, seat_id);
            
        });
    }else
    {
        //by lvzhangle --20151223
        //uint32_t seat_id = self_user->get_seat_num();
        uint32_t seat_id = 0;
        PDM->send_seat_info(roomid, msg::SittingStatus::LEAVE, seat_id);
    }
//    this->removeFromParent();
}

void LayerOfcPullDown::click_setting(cocos2d::Ref *sender)
{
    LayerGameSetting* lgs = LayerGameSetting::create(false);
    lgs->set_change_setting_callback([&](){
        newOfcPokerRoom* opr = dynamic_cast<newOfcPokerRoom*>(SceneManager::GetRoomLayer());
        if (opr)
        {
            opr->refresh_user_poker_cards();
        }
    });
    this->addChild(lgs);
}

void LayerOfcPullDown::click_stand_up(cocos2d::Ref *sender)
{
    auto &ofc_room_data = this->GetRoom();
    if (ofc_room_data == nullptr)
    {
        cocos2d::log("OfcBackGroundLayer::click_btn_standup ofc_room_data == nullptr");
        return;
    }
    uint32_t roomid = ofc_room_data->get_id();
    auto users = ofc_room_data->get_users();
    
    auto self_user = users.find(GDM->get_user_id());
    if(self_user != users.end() && ((self_user->second->get_status() == 0) || (self_user->second->get_status() == 5) ))//status: inGame || willLeave
    {
        TipView* tip = TipView::showAlertView(tools::local_string("will_quit_room", "本局游戏之后将生效"));
        tip->show_cancel(true);
        tip->set_tipview_close_callback([=](){
            // by lvzhangle --20151226
            //            int32_t seat_id = self_user->second->get_seat_num();//
            //            if(seat_id != 0) {
            //                PDM->send_seat_info(roomid, msg::SittingStatus::OBSERVER, seat_id);
            //            }
            int32_t seat_id = 0;
            PDM->send_seat_info(roomid, msg::SittingStatus::OBSERVER, seat_id);
        });
    }else{
        
        //by lvzhangle --20151223
        if (self_user != users.end())
        {
            uint32_t seat_id = self_user->second->get_seat_num();
            if(seat_id != 0) {
                PDM->send_seat_info(roomid, msg::SittingStatus::OBSERVER, seat_id);
            }
        }
    }
    this->removeFromParent();
}
void LayerOfcPullDown::onTouchEnded(Touch* touch, Event* event)
{
    //将openGL坐标转化为node坐标
    Point touchLocation = this->convertTouchToNodeSpace(touch);
    if (image_ofc_pull_bg_->getBoundingBox().containsPoint(touchLocation))
    {
        
    }else
    {
        this->removeFromParent();
    }
}