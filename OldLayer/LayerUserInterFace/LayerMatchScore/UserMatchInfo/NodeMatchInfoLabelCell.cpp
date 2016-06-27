//
//  NodeMatchInfoLabelCell.cpp
//  pokerclient
//
//  Created by ll on 15/12/2.
//
//

#include "NodeMatchInfoLabelCell.h"

bool NodeMatchInfoLabelCell::init()
{
    if(!Widget::init()) {
        return false;
    }
    
    root_ = CSLoader::createNode("NodeMatchInfoLabelCell.csb");
    this->addChild(root_);

    text_note_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_note"));
    text_note_->addClickEventListener(std::bind(&NodeMatchInfoLabelCell::click_checkbox, this, std::placeholders::_1));
    text_note_->setFontSize(28);
    image_note_bg_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_note_bg"));
    image_note_bg_->addClickEventListener(std::bind(&NodeMatchInfoLabelCell::click_checkbox, this, std::placeholders::_1));
    
    this->setContentSize(cocos2d::Size(100, 60));
    
    return true;
}

void NodeMatchInfoLabelCell::update_data(const std::string label_word)
{
    textlabel_ = label_word;
    text_note_->setString(textlabel_);
    
    float image_note_bg_width = text_note_->getContentSize().width + 20;
    if (image_note_bg_width < 120)
        image_note_bg_width = 120;
    image_note_bg_->setContentSize(cocos2d::Size(image_note_bg_width, image_note_bg_->getContentSize().height));
    
    text_note_->setPosition(Point((image_note_bg_->getContentSize().width - text_note_->getContentSize().width)/2 + 5, text_note_->getPosition().y));
    
    this->setContentSize(cocos2d::Size(image_note_bg_->getContentSize().width + 20, 60));
}

void NodeMatchInfoLabelCell::click_checkbox(Ref* sender)
{
    if (func_click_label_)
    {
        func_click_label_(textlabel_);
    }
}