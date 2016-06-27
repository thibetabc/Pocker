//
//  CreateGroupLayer.cpp
//  pokerclient
//  创建群 界面
//
//  Created by duanmt on 15/5/4.
//
//

#include "CreateGroupLayer.h"

#include "TipView.h"
#include "utils.h"
#include "GameTools.h"
#include "GameDataManager.h"
#include "cmd_def.h"
#include "LayerChatOld.h"

bool CreateGroupLayer::init()
{
    if(!BaseLayer::init()) {
        return false;
    }
    
    auto root = CSLoader::createNode("LayerRegister.csb");
    this->addChild(root);
    auto image_password = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root, "image_password_confirm"));
    image_password->setVisible(false);
    auto image_referee = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root, "image_referee"));
    image_referee->setVisible(false);
    
    text_referee_note_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_referee"));
    text_referee_note_->setVisible(false);
    
    auto tf_group_name = dynamic_cast<TextField*>(CSLoader::seekNodeByName(root, "tf_username"));
    auto tf_group_descrption = dynamic_cast<TextField*>(CSLoader::seekNodeByName(root, "tf_password"));
    text_license_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_license"));
    auto tf_group_password = dynamic_cast<TextField*>(CSLoader::seekNodeByName(root, "tf_password_confirm"));
    auto tf_referee = dynamic_cast<TextField*>(CSLoader::seekNodeByName(root, "tf_referee"));
    tf_group_password->setVisible(false);
    tf_referee->setVisible(false);
    
    tf_group_name_ = GameTools::createEditBoxFromTextField(this, tf_group_name);
    tf_group_descrption_ = GameTools::createEditBoxFromTextField(this, tf_group_descrption);
    tf_group_password_ = GameTools::createEditBoxFromTextField(this, tf_group_password);

//    tf_group_name_->setTextVerticalAlignment(cocos2d::TextVAlignment::CENTER);
//    tf_group_descrption_->setTextVerticalAlignment(cocos2d::TextVAlignment::CENTER);
//    tf_group_password_->setTextVerticalAlignment(cocos2d::TextVAlignment::CENTER);
    
//    tf_group_name_->setTextColor(Color4B(0, 0, 0, 255));
//    tf_group_descrption_->setTextColor(Color4B(0, 0, 0, 255));
//    tf_group_password_->setTextColor(Color4B(0, 0, 0, 255));
    
    tf_group_name_->setPlaceHolder(tools::local_string("input_group_name", "请填写群名称").c_str());
    tf_group_descrption_->setPlaceHolder(tools::local_string("input_group_descrption", "请填写群备注").c_str());
    tf_group_password_->setPlaceHolder(tools::local_string("input_password", "请填写密码").c_str());
    tf_group_password_->setVisible(false);
    
//    auto text_cancel = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_cancel"));
//    text_cancel->setString(tools::local_string("cancel", "取消"));
//    text_cancel->addClickEventListener(std::bind(&CreateGroupLayer::clickCancel, this, std::placeholders::_1));
    
    auto btn_back = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "btn_back"));
    btn_back->setTitleText("    " + tools::local_string("back", "返回"));
    btn_back->addClickEventListener(std::bind(&CreateGroupLayer::clickCancel, this, std::placeholders::_1));
    
    auto text_register = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_register"));
    text_register->setString(tools::local_string("create_group", "创建群"));
    
    
    auto btn_register = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "btn_register"));
    btn_register->setTitleText(tools::local_string("create_group", "创建群"));
    btn_register->addClickEventListener(std::bind(&CreateGroupLayer::clickRegister, this, std::placeholders::_1));
    
    btn_register->setPosition(image_password->getPosition());
    
    auto text_license_1 = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_license_1"));
    text_license_1->setVisible(false);
    
    auto text_license_2 = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_license_2"));
    text_license_2->setVisible(false);
    
    register_events();
    
    return true;
}

void CreateGroupLayer::clickCancel(Ref* sender)
{
    removeFromParent();
}
void CreateGroupLayer::clickRegister(Ref* sender)
{
    std::string group_name = tf_group_name_->getText();
    std::string group_comment = tf_group_descrption_->getText();
    std::string group_password = tf_group_password_->getText();
    
    if(group_name.empty()) {
        TipView::showAlertView(tools::local_string("group_name_empty", "群名称不能为空！"));
        return;
    }
    
    GDM->send_create_group(group_name, group_comment, group_password, 0, 0);
}

void CreateGroupLayer::register_events()
{
    dispatch_.register_event(EVENT_HANDLE_CREATE_GROUP_FINISHED,
                             BIND_FUNC(this, CreateGroupLayer::handle_create_group_finished));
}


void CreateGroupLayer::handle_create_group_finished(cmd_data_pointer)
{
        LayerChatOld* glc = LayerChatOld::create();
        glc->set_show_dir(LAYER_SHOW_DIRECTION::RIGHT_TO_LEFT);
        BaseLayer* parent = static_cast<BaseLayer*>(this->getParent());
        this->removeFromParent();
        parent->addChild(glc);
}