#include "InputLayer.h"
#include "utils.h"
#include "GameTools.h"


bool InputLayer::init()
{
    if(!Layer::init()) {
        return false;
    }
    
    auto layout = Layout::create();
    layout->setTouchEnabled(true);
    layout->setContentSize(Director::getInstance()->getWinSize());
    this->addChild(layout, -1);
    
    auto root = CSLoader::createNode("LayerInput.csb");
    this->addChild(root);
    
    auto btn_cancel = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "btn_cancel"));
    auto btn_confirm = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "btn_confirm"));
    
    btn_cancel->addClickEventListener(std::bind(&InputLayer::click_btn_cancel, this, std::placeholders::_1));
    btn_confirm->addClickEventListener(std::bind(&InputLayer::click_btn_confirm, this, std::placeholders::_1));
    
    auto tf_input = dynamic_cast<TextField*>(CSLoader::seekNodeByName(root, "tf_input"));
    tf_input_ = GameTools::createEditBoxFromTextField(root, tf_input);
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    tf_input_->setContentSize(Size(tf_input_->getContentSize().width, tf_input_->getContentSize().height * 2));
#endif
    
    return true;
}

void InputLayer::click_btn_confirm(Ref* sender)
{
    std::string msg = tf_input_->getText();
    if(callback_) {
        callback_(msg);
    }
    if(close_callback_) {
        close_callback_();
    }
    this->removeFromParent();
}
void InputLayer::click_btn_cancel(Ref* sender)
{
    this->removeFromParent();
    if(close_callback_) {
        close_callback_();
    }
}