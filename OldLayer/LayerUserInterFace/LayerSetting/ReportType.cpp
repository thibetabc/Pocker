//
//  ReportType.cpp
//  pokerclient
//
//  Created by admin on 15-9-1.
//
//

#include "ReportType.h"
#include "utils.h"
#include "SettingLayer.h"
#include "ReportBack.h"

bool ReportType::init()
{
    if (!BaseLayer::init()) {
        return false;
    }
    
    auto root = CSLoader::createNode("LayerReportType.csb");
    this->addChild(root);
    
    text_title = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_title"));
    text_title->setString(tools::local_string("report_type" , "举报类型"));
    
    btn_back = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "btn_back"));
    btn_back->setTitleText("    " + tools::local_string("cancel" , "取消"));
    btn_back->addClickEventListener(std::bind(&ReportType::click_cancel, this, std::placeholders::_1));
    
    text_next = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_confirm"));
    text_next->setString(tools::local_string("next" , "下一步"));
    text_next->addClickEventListener(std::bind(&ReportType::click_next, this, std::placeholders::_1));
    
    
    auto image_type_1 = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root, "image_bg_1"));
    image_type_1->addClickEventListener(std::bind(&ReportType::click_type_1, this, std::placeholders::_1));
    
    auto image_type_2 = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root, "image_bg_2"));
    image_type_2->addClickEventListener(std::bind(&ReportType::click_type_2, this, std::placeholders::_1));
    
    auto image_type_3 = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root, "image_bg_3"));
    image_type_3->addClickEventListener(std::bind(&ReportType::click_type_3, this, std::placeholders::_1));
    
    auto image_type_4 = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root, "image_bg_4"));
    image_type_4->addClickEventListener(std::bind(&ReportType::click_type_4, this, std::placeholders::_1));
    
    auto text_1 = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "Text_5__1"));
    auto text_2 = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "Text_5__2"));
    auto text_3 = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "Text_5__3"));
    auto text_4 = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "Text_5__4"));
    text_1->setScaleY(text_1->getScaleX());
    text_2->setScaleY(text_2->getScaleX());
    text_3->setScaleY(text_3->getScaleX());
    text_4->setScaleY(text_4->getScaleX());
    sprite_select_1 = dynamic_cast<Sprite*>(CSLoader::seekNodeByName(root, "sprite_male_select_1"));
    sprite_select_2 = dynamic_cast<Sprite*>(CSLoader::seekNodeByName(root, "sprite_male_select_2"));
    sprite_select_3 = dynamic_cast<Sprite*>(CSLoader::seekNodeByName(root, "sprite_male_select_3"));
    sprite_select_4 = dynamic_cast<Sprite*>(CSLoader::seekNodeByName(root, "sprite_male_select_4"));
    sprite_select_1->setVisible(false);
    sprite_select_2->setVisible(false);
    sprite_select_3->setVisible(false);
    sprite_select_4->setVisible(false);
    
    

    return true;
}

void ReportType::click_type_1(Ref* sender)
{
    set_report_type_("色情低俗");
    
    sprite_select_1->setVisible(true);
    sprite_select_2->setVisible(false);
    sprite_select_3->setVisible(false);
    sprite_select_4->setVisible(false);
}
void ReportType::click_type_2(Ref* sender)
{
    set_report_type_("赌博");
    
    sprite_select_1->setVisible(false);
    sprite_select_2->setVisible(true);
    sprite_select_3->setVisible(false);
    sprite_select_4->setVisible(false);
}
void ReportType::click_type_3(Ref* sender)
{
    set_report_type_("政治敏感");
    
    sprite_select_1->setVisible(false);
    sprite_select_2->setVisible(false);
    sprite_select_3->setVisible(true);
    sprite_select_4->setVisible(false);
}
void ReportType::click_type_4(Ref* sender)
{
    set_report_type_("违法（暴力恐怖、违禁品等）");
    
    sprite_select_1->setVisible(false);
    sprite_select_2->setVisible(false);
    sprite_select_3->setVisible(false);
    sprite_select_4->setVisible(true);
}

void ReportType::click_cancel(Ref* sender)
{
    /*auto chid_layer = SettingLayer::create();
    chid_layer->update_data();
    this->addChild(chid_layer, -1);*/
    this->removeFromParent();
}
void ReportType::click_next(Ref* sender)
{
    ReportBack* rbl = ReportBack::create();//
    rbl->set_report_type(get_report_type_());
    BaseLayer* parent = static_cast<BaseLayer*>(this->getParent());
    parent->addChild(rbl);
    this->removeFromParent();
}