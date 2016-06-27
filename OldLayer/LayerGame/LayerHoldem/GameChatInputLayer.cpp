#include "GameChatInputLayer.h"
#include "GameTools.h"
#include "GameDataManager.h"
#include "PokerDataManager.h"
#include "user_data.h"

bool GameChatInputLayer::init()
{
    if(!Layer::init()) {
        return false;
    }
    
    auto& size = Director::getInstance()->getWinSize();
    auto layout = ui::Layout::create();
    layout->setContentSize(size);
    layout->setTouchEnabled(true);
    layout->setAnchorPoint(Point::ZERO);
    this->addChild(layout, -10);
    layout->addClickEventListener([=](Ref*) {
        this->removeFromParent();
    });
    
    
    root_ = CSLoader::createNode("GameChatInputLayerPortrait.csb");
    this->addChild(root_);
    
    image_container_ = CSLoader::seekNodeByName(root_, "image_container");
    auto image_back = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_back"));
    auto image_send = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_send"));
    auto image_clear = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_clear"));
    auto tf_inputbox = dynamic_cast<TextField*>(CSLoader::seekNodeByName(root_, "tf_inputbox"));
    
    tf_inputbox_ = GameTools::createEditBoxFromTextField(image_container_, tf_inputbox);
    tf_inputbox_->setInputMode(cocos2d::ui::EditBox::InputMode::SINGLE_LINE);
    tf_inputbox_->setReturnType(EditBox::KeyboardReturnType::SEND);
    tf_inputbox_->setDelegate(this);
    
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
/*    auto glview = cocos2d::Director::getInstance()->getOpenGLView();
    float scalex = glview->getScaleX();
    float scaley = glview->getScaleY();
    tf_inputbox_->setContentSize(Size(tf_inputbox_->getContentSize().width * scalex, tf_inputbox_->getContentSize().height * scaley));*/
#endif
    tf_inputbox_->setPlaceHolder("");
    
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
//    tf_inputbox_->setKeyBoardWillShowCallback(std::bind(&GameChatInputLayer::keyBoardWillShow, this, std::placeholders::_1, std::placeholders::_2));
//    tf_inputbox_->setKeyBoardWillHideCallback(std::bind(&GameChatInputLayer::keyBoardWillHide, this, std::placeholders::_1, std::placeholders::_2));
#endif
    
    image_back->addClickEventListener(std::bind(&GameChatInputLayer::click_image_back, this, std::placeholders::_1));
    image_send->addClickEventListener(std::bind(&GameChatInputLayer::click_image_send, this, std::placeholders::_1));
    image_clear->addClickEventListener(std::bind(&GameChatInputLayer::click_image_clear, this, std::placeholders::_1));
    
    tf_inputbox_->touchDownAction(tf_inputbox_, cocos2d::ui::Widget::TouchEventType::ENDED);
    
    return true;
}

void GameChatInputLayer::click_image_back(Ref* sender)
{
    GameTools::editBoxLoseInputFocus();
    this->removeFromParent();
}
void GameChatInputLayer::click_image_clear(Ref* sender)
{
    tf_inputbox_->setText("");
}
void GameChatInputLayer::click_image_send(Ref* sender)
{
    std::string message = tf_inputbox_->getText();
    //GDM->send_chat_message(msg::MessageType::Text, GDM->get_select_group_id(), message);
    
    // by lvzhangle
    msg::ChatMessage * ChatMessage = new msg::ChatMessage();
    
    ChatMessage->mutable_userdetailinfo()->set_userid(GDM->get_user_id());
    ChatMessage->mutable_messageinfo()->mutable_textinfo()->set_text(message);
    ChatMessage->mutable_messageinfo()->set_type(msg::MessageType::Text);
    ChatMessage->set_groupid(groupid);
    GDM->send_chat_message(*ChatMessage);
    
    tf_inputbox_->setText("");
}

#pragma mark EditBox delegate
void GameChatInputLayer::editBoxReturn(EditBox* editBox)
{
    
/*    std::string content = tf_inputbox_->getText();
    
    //GDM->send_chat_message(msg::MessageType::Text, GDM->get_select_group_id(), content);
    
    //取消掉点击别处发送消息
    // by lvzhangle
    msg::ChatMessage * ChatMessage = new msg::ChatMessage();
    
    ChatMessage->mutable_userdetailinfo()->set_userid(GDM->get_user_id());
    ChatMessage->mutable_userdetailinfo()->set_nickname(GDM->get_user_data()->get_nickname());
    ChatMessage->mutable_userdetailinfo()->set_picname(GDM->get_user_data()->get_head_pic_name());
    ChatMessage->mutable_userdetailinfo()->set_sex(GDM->get_user_data()->get_sex());
    
    ChatMessage->mutable_messageinfo()->mutable_textinfo()->set_text(content);
    ChatMessage->mutable_messageinfo()->set_type(msg::MessageType::Text);
    ChatMessage->set_groupid(GDM->get_select_group_id());
    GDM->send_chat_message(*ChatMessage);
    
    tf_inputbox_->setText("");
    
    scheduleOnce([this](float){
        this->removeFromParent();
    }, 0.1f, "GameChatInputLayer::editBoxReturn");*/
}

void GameChatInputLayer::keyBoardWillShow(const float duration, const float distance)
{
    float dis = distance;
    //    if (dis < 0.0f) dis = 0.0f;
    auto glview = cocos2d::Director::getInstance()->getOpenGLView();
//    dis *= glview->getScaleY();
    
//    dis *= 2;
//    dis += 20;
    float scale = glview->getDesignResolutionSize().height/(glview->getScreenSize().height / glview->getScaleY());
    dis *= scale;
    
    log("GameChatInputLayer::keyBoardWillShow:%f", dis);
    
    Point p = image_container_->getPosition() + Point(0, dis);
    image_container_->stopAllActions();
    image_container_->runAction(MoveTo::create(duration, p));
}
void GameChatInputLayer::keyBoardWillHide(const float duration, const float distance)
{
    return;
    float dis = distance;
    if (dis < 0.0f) dis = 0.0f;
//    auto glview = cocos2d::Director::getInstance()->getOpenGLView();
//    dis *= glview->getScaleY();
    
    image_container_->stopAllActions();
    image_container_->runAction(MoveTo::create(duration, Point::ZERO));
}