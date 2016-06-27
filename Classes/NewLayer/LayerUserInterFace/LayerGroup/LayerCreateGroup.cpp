//
//  LayerCreateGroup.cpp
//  pokerclient
//  创建俱乐部 界面
//
//  Created by duanmt on 15/5/4.
//
//

#include "LayerCreateGroup.h"
#include "TipView.h"
#include "utils.h"
#include "GameTools.h"
#include "ProcessorManager.h"
#include "cmd_def.h"
#include "LayerChat.h"
#include "NET_CMD.h"
#include <regex>
#include "LayerGroupList.h"

bool LayerCreateGroup::init()
{
    if(!BaseLayer::init()) {
        return false;
    }
    
    auto root = CSLoader::createNode("LayerGroupCreate.csb");
    this->addChild(root);
    
    text_title_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_title"));
    
    auto group_name_root = CSLoader::seekNodeByName(root, "img_group_name_bg");
    auto group_name = dynamic_cast<TextField*>(CSLoader::seekNodeByName(root, "textfield_name"));
    group_name->setPlaceHolder(tools::local_string("input_club_account", "请填写俱乐部名称").c_str());
    
    auto group_remark_root = CSLoader::seekNodeByName(root, "img_remark_bg");
    auto group_remark = dynamic_cast<TextField*>(CSLoader::seekNodeByName(root, "textfield_remark"));
    group_remark->setPlaceHolder(tools::local_string("input_club_remarks","请填写俱乐部备注").c_str());
    
    auto btn_back = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "btn_back"));
    btn_back->addClickEventListener(std::bind(&LayerCreateGroup::clickCancel, this, std::placeholders::_1));
    
    btn_register = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "btn_create"));
    btn_register->addClickEventListener(std::bind(&LayerCreateGroup::clickRegister, this, std::placeholders::_1));
    
    tf_group_name_ = GameTools::createEditBoxFromTextField(group_name_root, group_name);
    tf_group_name_->setFontColor(Color3B::WHITE);
    tf_group_name_->setPlaceholderFontColor(Color3B(151, 151, 151));
//    tf_group_name_->setInputFlag(EditBox::InputFlag::PASSWORD);
    tf_group_name_->setReturnType(EditBox::KeyboardReturnType::GO);
    tf_group_name_->setDelegate(this);
    tf_group_name_->setMaxLength(TITLE_MAX_LENGHT);
    
    tf_group_remark_ = GameTools::createEditBoxFromTextField(group_remark_root, group_remark);
    tf_group_remark_->setFontColor(Color3B::WHITE);
    tf_group_remark_->setPlaceholderFontColor(Color3B(151, 151, 151));
    tf_group_remark_->setReturnType(EditBox::KeyboardReturnType::GO);
    tf_group_remark_->setDelegate(this);
    tf_group_remark_->setMaxLength(TITLE_MAX_LENGHT);
    
    register_events();
    set_language();
    
    return true;
}

void LayerCreateGroup::set_language()
{
    text_title_->setText(tools::local_string("create_club","创建俱乐部"));
    btn_register->setTitleText(tools::local_string("create_club","创建俱乐部"));
}

void LayerCreateGroup::clickCancel(Ref* sender)
{
    auto layer = dynamic_cast<LayerGroupList*>(this->getParent());
    layer->requestGroupList();
    removeFromParent();
}
void LayerCreateGroup::clickRegister(Ref* sender)
{
    std::string group_name = tf_group_name_->getText();
    std::string group_remark = tf_group_remark_->getText();
    
    if(!std::regex_match(group_name, std::regex("^[\u4E00-\u9FA5A-Za-z0-9_]+$"))) {
        TipView::showAlertView(tools::local_string("group_name_empty", "只支持中文、英文、数字和下划线"));
        return;
    }
    send_msg(group_name, group_remark);
}

void LayerCreateGroup::editBoxEditingDidEnd(EditBox* editBox)
{
//    clickRegister(nullptr);
}
void LayerCreateGroup::register_events()
{
    dispatch_.register_event(REQ_CREATE_GROUP,
                             BIND_FUNC(this, LayerCreateGroup::handle_create_group_finished));
}


void LayerCreateGroup::handle_create_group_finished(cmd_data_pointer data)
{
    std::shared_ptr<net_data_recv_package> rec = dynamic_pointer_cast<net_data_recv_package>(data);
    if(!check_request(rec)) return;
    auto processor = PM->CreateGroup_Down(rec);
    auto reponse = processor.response();
    auto status = reponse.status();
    switch (status) {
        case msg::Status::SUCCESS:
        {
            LayerChat* glc = LayerChat::createByGroup(processor.groupinfo());
            glc->set_show_dir(LAYER_SHOW_DIRECTION::RIGHT_TO_LEFT);
            AddGlobalLayer(glc);
            removeFromParent();
        }
            break;
            
        case msg::Status::FAILED:
            TipView::showAlertView(reponse.message());
            break;
    }
    
}

void LayerCreateGroup::send_msg(string name, string remark)
{
    
    msg::GroupInfo groupinfo;
    groupinfo.set_groupname(name);
    groupinfo.set_groupcomment(remark);
    auto processor = PM->CreateGroup_UP(groupinfo);
    send_data(processor, REQ_CREATE_GROUP);
}


