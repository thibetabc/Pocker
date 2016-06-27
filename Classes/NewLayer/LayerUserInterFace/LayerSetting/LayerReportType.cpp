//
//  LayerReportType.cpp
//  pokerclient
//
//  Created by zhongyu on 16/4/22.
//
//

#include "LayerReportType.h"
#include "ProcessorManager.h"
#include "GameTools.h"
#include "utils.h"
#include "cmd_def.h"
#include "NET_CMD.h"
bool LayerReportType::init()
{
    if(!BaseLayer::init())
    {
        return false;
    }
    root_ = CSLoader::createNode("LayerReportType.csb");
    this->addChild(root_);
    btn_back_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_back"));
    btn_back_->addClickEventListener(std::bind(&LayerReportType::click_btn_back, this,std::placeholders::_1));
    text_title_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_title"));
    text_title_->setString(tools::local_string("report_back","反馈"));
    text_confirm_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_confirm"));
    text_confirm_->setTouchEnabled(true);
    text_confirm_->setString(tools::local_string("report","提交"));
    text_confirm_->addClickEventListener(std::bind(&LayerReportType::click_text_confirm, this,std::placeholders::_1));
    
    text_report_dec_=dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_report_dec"));
    text_report_dec_->setString(tools::local_string("describe_qus","请描述您的问题"));
    text_report_dec_->setTextHorizontalAlignment(TextHAlignment::LEFT);
    
    auto tf_input_report = dynamic_cast<TextField*>(CSLoader::seekNodeByName(root_, "tf_input_report"));
    auto contentSize = tf_input_report->getContentSize();
    
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    tf_input_report_ = GameTools::createMultiEditBoxFormTextField(root_, tf_input_report, __FILE__, __LINE__);
    tf_input_report_->setBackgroundColor(Color4B(25, 36, 48, 1));
//    tf_input_report_->setFontColor(Color3B(0, 0, 0));
    tf_input_report_->setPlaceHolder("");
    tf_input_report_->setMultLine(true);
    tf_input_report_->setReturnType(MultiEditBox::KeyboardReturnType::DONE);
    tf_input_report_->setContentSize(Size(tf_input_report_->getContentSize().width, tf_input_report_->getContentSize().height * 1));
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    tf_input_report_ = GameTools::createMultiEditBoxFormTextField(root_, tf_input_report, __FILE__, __LINE__);
    tf_input_report_->setPlaceHolder(tools::local_string("decrip_hoder", "请填写您的举报信息或反馈内容").c_str());
    tf_input_report_->setReturnType(MultiEditBox::KeyboardReturnType::DONE);
#endif
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(LayerReportType::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(LayerReportType::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(LayerReportType::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    auto& size = Director::getInstance()->getWinSize();
    auto layout = Layout::create();
    layout->setContentSize(size);
    layout->setTouchEnabled(true);
    layout->setAnchorPoint(Point::ZERO);
    layout->setSwallowTouches(false);
    layout->addClickEventListener([this](Ref*){
        GameTools::editBoxLoseInputFocus();
    });
    this->addChild(layout, 10);
    
    return true;
}
void LayerReportType::register_events()
{
    dispatch_.register_event(REQ_FEED_BACK,BIND_FUNC(this, LayerReportType::handle_feed_back));
}
void LayerReportType::click_btn_back(cocos2d::Ref *sender)
{
    this->removeFromParent();
}
void LayerReportType::click_text_confirm(cocos2d::Ref *sender)
{
    std::string report=tf_input_report_->getText();
    auto processor=PM->feedback_up(report,"", "");
    send_data(processor, REQ_FEED_BACK);
}
void LayerReportType::handle_feed_back(cmd_data_pointer data)
{
    ClearError();
    std::shared_ptr<net_data_recv_package> rec=dynamic_pointer_cast<net_data_recv_package>(data);
    if (!check_request(rec)) return;
    msg::Response processor;
    processor.ParseFromArray(rec->body, rec->bodysize);
    tf_input_report_->setText("");
    if(processor.status()==msg::Status::SUCCESS)
    {
        this->removeFromParent();
    }
    else
    {
        ShowError(processor.message());
    }
}