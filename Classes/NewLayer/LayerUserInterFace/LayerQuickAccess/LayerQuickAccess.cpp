//
//  LayerQuickAccess.cpp
//  pokerclient
//
//  Created by zhongyu on 16/3/30.
//
//

#include "LayerQuickAccess.h"
#include "utils.h"
#include "SceneManager.h"
#include "GameTools.h"
#include "PokerDataManager.h"
#include "GameDataManager.h"

#include "LayerCreateGameRoom.h"

bool LayerQuickAccess::init()
{
    if (!BaseLayer::init()) {
        return false;
    }
    this->set_show_dir(DIRECTION_NONE);
 
    root_=CSLoader::createNode("LayerQuickAccess.csb");
    this->addChild(root_);
    
    text_title_=dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_title"));
    text_title_->setString(tools::local_string("quick_access_title","好牌"));
    
    text_roomtip_=dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_roomtip"));
    text_roomtip_->setString(tools::local_string("quick_access_room_tip","请输入房间号"));
    
    btn_back_=dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_back"));
    btn_back_->addClickEventListener(std::bind(&LayerQuickAccess::click_btn_back, this,std::placeholders::_1));
    
    auto tf_room_input=dynamic_cast<TextField*>(CSLoader::seekNodeByName(root_, "tf_room_input"));
    tf_room_input_ = GameTools::createEditBoxFromTextField(this,tf_room_input);
    tf_room_input_->setFontColor(Color3B(255, 255, 255));
    tf_room_input_->setPlaceholderFontColor(Color3B(151, 151, 151));
    tf_room_input_->setInputMode(cocos2d::ui::EditBox::InputMode::NUMERIC);
    
    btn_enter_room_=dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_enter_room"));
    btn_enter_room_->addClickEventListener(std::bind(&LayerQuickAccess::click_btn_enter_room, this,std::placeholders::_1));
    btn_enter_room_->setTitleText(tools::local_string("enter_room","进入房间"));
    
    btn_create_room_=dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_create_room"));
    btn_create_room_->addClickEventListener(std::bind(&LayerQuickAccess::click_btn_create_room, this,std::placeholders::_1));
    btn_create_room_->setTitleText(tools::local_string("quick_access_create_room","创建房间"));
    
    return true;
}

void LayerQuickAccess::click_btn_back(cocos2d::Ref *sender)
{
    log("返回上一层");
    this->removeFromParent();
}

void LayerQuickAccess::click_btn_create_room(cocos2d::Ref *sender)
{
    log("创建房间");
    GameTools::editBoxLoseInputFocus();
    LayerCreateGameRoom* lcgr = LayerCreateGameRoom::create(GDM->get_last_game_room_type(), false);
    lcgr->set_dismiss_callback([&](){this->removeFromParent();});
    this->addChild(lcgr);
}

void LayerQuickAccess::click_btn_enter_room(cocos2d::Ref *sender)
{
    ClearError();
    GameTools::editBoxLoseInputFocus();
    std::string roomId=tf_room_input_->getText();
    for (int i = 0; i < strlen(roomId.c_str()); i++) {
        cocos2d::log("%d",roomId.c_str()[i]);
        if (48 <= roomId.c_str()[i] && (roomId.c_str()[i] <= 57)) {
            cocos2d::log("数字");
        }else
        {
            std::string errormsg=tools::local_string("whole_number","房间号为整数");
            ShowError(errormsg);
            return;
        }
    }
    if (roomId.empty())
    {
        std::string errormsg=tools::local_string("room_empty","房间号不能为空");
        ShowError(errormsg);
        return;
    }
    log("玩家输入的房间号：%s",roomId.c_str());
    PDM->send_enter_room(atoi(roomId.c_str()));
}
