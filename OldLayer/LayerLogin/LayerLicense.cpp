
#include "LayerLicense.h"
#include "utils.h"
#include "GameTools.h"
#include "file_utils.h"


bool LayerLicense::init()
{
    if(!BaseLayer::init()) {
        return false;
    }
    

    
    auto root = CSLoader::createNode("LayerLicense.csb");
    this->addChild(root);
    
    text_title_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_title"));
    btn_back_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "btn_back"));
    btn_back_->setTitleText("    " + tools::local_string("back", "返回"));
    auto text_message = dynamic_cast<TextField*>(CSLoader::seekNodeByName(root, "tf_license"));
    
    textView_ = __createTextView(text_message->getContentSize());
    textView_->setAnchorPoint(text_message->getAnchorPoint());
    textView_->setPosition(text_message->getPosition());
    this->addChild(textView_);
    text_message->removeFromParent();
    
    load_text();
    
    btn_back_->addClickEventListener([this](Ref*)
    {
        removeFromParent();
    });
    
    return true;
}

void LayerLicense::load_text()
{
    std::string path = FileUtils::getInstance()->fullPathForFilename("license.txt");
    cocos2d::log("load_text_path:%s", path.c_str());
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    const std::string license = cocos2d::FileUtils::getInstance()->getStringFromFile(path);
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    const std::string license = file_utils::read_file(path);
#endif
    textView_->setString(license);
}