#include "LanguageCell.h"


bool LanguageCell::init()
{
    if(!TableViewCell::init()) {
        return false;
    }
    
    auto root = CSLoader::createNode("NodeLanguage.csb");
    this->addChild(root);
    
    auto image_language = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root, "image_language"));
    this->setContentSize(image_language->getContentSize());
    image_language->addClickEventListener(std::bind(&LanguageCell::click_image_language, this, std::placeholders::_1));
    
    text_language_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_language"));
    
    return true;
}

void LanguageCell::click_image_language(Ref* sender)
{
    const std::string& text = text_language_->getString();
    if(callback_) {
        callback_(text);
    }
}

void LanguageCell::update_ui(const std::pair<std::string, std::pair<std::string, std::string> >& data)
{
    data_ = data;
    text_language_->setString(data.second.first);
}