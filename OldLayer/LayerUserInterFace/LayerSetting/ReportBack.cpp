//
//  ReportBack.cpp
//  pokerclient
//  设置－反馈界面
//
//  Created by admin on 15-9-1.
//
//

#include "ReportBack.h"
#include "utils.h"
#include "GameTools.h"
#include "GameDataManager.h"
#include "ReportType.h"
#include "TipView.h"
#include "cmd_def.h"
#include "ProcessorManager.h"



bool ReportBack::init()
{
    if (!BaseLayer::init()) {
        return false;
    }
    
    auto root = CSLoader::createNode("LayerReportBack.csb");
    this->addChild(root);
    
    text_title = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_title"));
    text_title->setString(tools::local_string("report_back" , "反馈"));
    
    btn_back = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "btn_back"));
    btn_back->setTitleText("    " + tools::local_string("cancel" , "取消"));
    btn_back->addClickEventListener(std::bind(&ReportBack::click_cancel, this, std::placeholders::_1));
    
    text_report = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_confirm"));
    text_report->setString(tools::local_string("report" , "提交"));
    text_report->addClickEventListener(std::bind(&ReportBack::click_report, this, std::placeholders::_1));
    
    auto text_1 = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "Text_4"));
    text_1->setString(tools::local_string("select_pic" , "请举证-图片证据"));
    text_1->setVisible(false);
    
    auto text_2 = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "Text_5"));
    text_2->setString(tools::local_string("好牌"));
    
    auto text_3 = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "Text_6"));
    text_3->setScaleY(text_3->getScaleX());
    text_3->setString(tools::local_string("Copyright© 2015 " , "北京中裕科技有限公司出品"));
    sprite_add_pic = dynamic_cast<Sprite*>(CSLoader::seekNodeByName(root, "Sprite_1"));
    sprite_add_pic->setVisible(false);
    
    auto tf_descrip = dynamic_cast<TextField*>(CSLoader::seekNodeByName(root, "tf_descrip"));

    
    auto contentSize = tf_descrip->getContentSize();

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    tf_descrip_ = GameTools::createMultiEditBoxFormTextField(root, tf_descrip, __FILE__, __LINE__);
    tf_descrip_->setBackgroundColor(Color4B::WHITE);
    tf_descrip_->setPlaceHolder(tools::local_string("decrip_hoder", "请填写您的举报信息或反馈内容").c_str());
    tf_descrip_->setMultLine(true);
    tf_descrip_->setReturnType(MultiEditBox::KeyboardReturnType::DONE);
    tf_descrip_->setContentSize(Size(tf_descrip_->getContentSize().width, tf_descrip_->getContentSize().height * 1));
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    tf_descrip_ = GameTools::createEditBoxFromTextField(root, tf_descrip);
    tf_descrip_->setPlaceHolder(tools::local_string("decrip_hoder", "请填写您的举报信息或反馈内容").c_str());
    tf_descrip_->setReturnType(EditBox::KeyboardReturnType::DONE);
#endif
    
    
    dispatch_.register_event(EVENT_HANDLE_REPORT_BACK_SUCCESS,
                             BIND_FUNC(this, ReportBack::handle_report_back_success));
    
    
    return true;
}

void ReportBack::handle_report_back_success(cmd_data_pointer)
{
    auto tip = TipView::create();
    tip->update_note("举报并反馈成功，我们将尽快处理");
    tip->show_cancel(false);
    this->getParent()->addChild(tip);
    tip->set_tipview_close_callback([this](){
        
        this->removeFromParentAndCleanup(true);
    });

//    auto tip = TipView::showAlertView("举报并反馈成功，我们将尽快处理");
//    tip->set_tipview_close_callback([this](){
//        if(this->getReferenceCount() == 1)
//        {
//            this->removeFromParent();
//        }
//    });
}

void ReportBack::click_report(Ref* sender)
{
    auto processor = PM->feedback_up(tf_descrip_->getText(), get_report_type(), "pic");
    net_manager::shared_instence().send_data(processor, REQ_FEED_BACK);
}
void ReportBack::click_cancel(Ref* sender)
{
    this->removeFromParent();
}
void ReportBack::click_add_pic(Ref* sender)
{

}