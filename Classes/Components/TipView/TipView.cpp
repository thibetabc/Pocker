#include "TipView.h"
#include "utils.h"
#include "SceneManager.h"
#include "GameTools.h"

std::list<TipView::EditBoxSet*> TipView::edit_boxes = std::list<TipView::EditBoxSet*>();

//void TipView::get_all_boxes(Node* node, std::list<EditBoxSet*>& boxes)
//{
//    for(auto& iter : node->getChildren()) {
//        auto temp = dynamic_cast<cocos2d::ui::MultiEditBox*>(iter);
//        if(temp) {
//            auto ebs = new EditBoxSet();
//            ebs->box = temp;
//#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
//            ebs->enable = temp->isEditable();
//            if(temp->isEditable()) {
//                cocos2d::log("temp->isEditable()");
//            }
//            else {
//                cocos2d::log("temp->isEditable() ...... no");
//            }
//            temp->setEditable(false);
//#endif
//            boxes.push_back(ebs);
//        }
//        get_all_boxes(iter, boxes);
//    }
//}

TipView* TipView::showAlertView(const std::string& content, const bool cancel)
{
    GameTools::editBoxLoseInputFocus();
    
    TipView* __tip_view__ = dynamic_cast<TipView*>(SceneManager::addTipLayer());
    __tip_view__->update_note(content);
    __tip_view__->show_cancel(cancel);
    __tip_view__->setTag(INT_MAX);
    
    return __tip_view__;
}


bool TipView::init()
{
    if(!Layer::init()) {
        return false;
    }
    
    auto& size = Director::getInstance()->getWinSize();
    auto layout = Layout::create();
    layout->setContentSize(Director::getInstance()->getWinSize());
    layout->setTouchEnabled(true);
    this->addChild(layout);
    
    auto layer = LayerColor::create(Color4B(143, 143, 143, 100));
    this->addChild(layer);
    
    root_ = CSLoader::createNode("LayerTipView.csb");
    this->addChild(root_);
    
    text_tip_title_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_tip_title"));
    btn_confirm_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_confirm"));
    btn_confirm_->addClickEventListener(std::bind(&TipView::click_btn_confirm, this,std::placeholders::_1));
    btn_cancle_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_cancle"));
    btn_cancle_->addClickEventListener(std::bind(&TipView::click_btn_cancle, this,std::placeholders::_1));
    btn_confirm_single_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_confirm_single"));
    btn_confirm_single_->addClickEventListener(std::bind(&TipView::click_btn_confirm, this,std::placeholders::_1));
    btn_confirm_->setVisible(false);
    btn_cancle_->setVisible(false);
    set_language();
    return true;
}

void TipView::set_language()
{
    btn_confirm_->setTitleText(tools::local_string("confirm","确认"));
    btn_confirm_single_->setTitleText(tools::local_string("confirm","确认"));
    btn_cancle_->setTitleText(tools::local_string("cancel","取消"));
}

void TipView::show_cancel(bool isShowCancle)
{
    if (isShowCancle)
    {
        btn_confirm_->setVisible(true);
        btn_cancle_->setVisible(true);
        btn_confirm_single_->setVisible(false);
    }
}

void TipView::click_btn_confirm(Ref* sender)
{
    if(tipview_close_callback_) {
        tipview_close_callback_();
    }
    if(this->getReferenceCount() == 1)
    {
        this->removeFromParent();
    }

}
void TipView::click_btn_cancle(Ref* sender)
{
    if (tipview_cancel_callback_) {
        tipview_cancel_callback_();
    }
    removeFromParent();
}

void TipView::update_note(const std::string& message)
{
    text_tip_title_->setString(message);
}

void TipView::onExit()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    for(auto& iter : edit_boxes) {
        iter->box->setEditable(iter->enable);
    }
#endif
    edit_boxes.clear();
    BaseLayer::onExit();
}