//  群列表？

#include "EnterGroupLayer.h"
#include "GameTools.h"
#include "utils.h"
#include "GameDataManager.h"

bool EnterGroupLayer::init()
{
    if(!Layer::init()) {
        return false;
    }
    auto layout = Layout::create();
    layout->setTouchEnabled(true);
    layout->setContentSize(Director::getInstance()->getWinSize());
    this->addChild(layout, -1);
    
    auto root = CSLoader::createNode("LayerEnterGroup.csb");
    this->addChild(root);
    
    auto text_group_name = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_group_name"));
    auto text_group_password = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_group_password"));
    text_group_name->setString(tools::local_string("group_name", "群名称"));
    text_group_password->setString(tools::local_string("group_password", "群密码"));
    
    text_group_name_edit_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_group_name_edit"));
    
    auto btn_cancel = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "btn_cancel"));
    auto btn_confirm = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "btn_confirm"));
    btn_cancel->addClickEventListener(std::bind(&EnterGroupLayer::click_btn_cancel, this, std::placeholders::_1));
    btn_confirm->addClickEventListener(std::bind(&EnterGroupLayer::click_btn_confirm, this, std::placeholders::_1));
    
    btn_cancel->setTitleText(tools::local_string("cancel", "取消"));
    btn_confirm->setTitleText(tools::local_string("confirm", "确定"));
    
    auto tf_password = dynamic_cast<TextField*>(CSLoader::seekNodeByName(root, "tf_password"));
    
    tf_password_ = GameTools::createEditBoxFromTextField(root, tf_password);
    tf_password_->setInputFlag(cocos2d::ui::EditBox::InputFlag::PASSWORD);
    
    tf_password_->setFontColor(Color4B::WHITE);
    
    text_group_name_edit_->setString(group_data_->groupname());
    
    return true;
}

void EnterGroupLayer::click_btn_confirm(Ref* sender)
{
    std::string password = tf_password_->getText();
    GDM->send_enter_group(group_data_->groupid(), password);
    if(callback_) {
        callback_();
    }
    this->removeFromParent();
}
void EnterGroupLayer::click_btn_cancel(Ref* sender)
{
    removeFromParent();
}

void EnterGroupLayer::onExit()
{
    if(callback_) {
        callback_();
    }
    LayerPop::onExit();
}