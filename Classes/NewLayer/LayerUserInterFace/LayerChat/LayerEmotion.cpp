#include "LayerEmotion.h"
#include "file_utils.h"
#include "GameTools.h"
#include "GameDataManager.h"
#include "group_data.h"
#include "msg.pb.h"
#include "user_data.h"

bool LayerEmotion::init()
{
    if(!Layer::init()) {
        return false;
    }
    
    auto layout = Layout::create();
    layout->setContentSize(this->getContentSize());
    layout->setTouchEnabled(true);
    layout->addClickEventListener([this](Ref*) {
        if(dismiss_callback_) {
            dismiss_callback_();
        }
        this->removeFromParent();
    });
    this->addChild(layout);
    
    auto root = CSLoader::createNode("LayerEmotion.csb");
    this->addChild(root);
    
    btn_voice_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "btn_voice"));
    btn_keyboard_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "btn_keyboard"));
    btn_del_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "btn_delete"));
    
    btn_voice_->addClickEventListener(std::bind(&LayerEmotion::click_btn_voice, this, std::placeholders::_1));
    btn_keyboard_->addClickEventListener(std::bind(&LayerEmotion::click_btn_keyboard, this, std::placeholders::_1));
    btn_del_->addClickEventListener(std::bind(&LayerEmotion::click_btn_del, this, std::placeholders::_1));
    
    auto btn_send = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "btn_send"));
    btn_send->addClickEventListener(std::bind(&LayerEmotion::click_btn_send, this, std::placeholders::_1));
    
    pageview_ = dynamic_cast<PageView*>(CSLoader::seekNodeByName(root, "pageview"));
    //    void addEventListener(const ccPageViewCallback& callback);
    pageview_->addEventListener(std::bind(&LayerEmotion::pageview_turn, this, std::placeholders::_1, std::placeholders::_2));
    tf_chat_ = dynamic_cast<TextField*>(CSLoader::seekNodeByName(root, "tf_input"));
    tf_chat_->setVisible(false);
    
    image_container_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root, "image_input_bg"));
    
    //    tf_chat_ = GameTools::createEditBoxFromTextField(root, tf_chat);
    //    tf_chat_->setDelegate(this);
    
    init_emotions();
    
    return true;
}


void LayerEmotion::init_emotions()
{
    std::vector<std::string> files;
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    std::string dir = "emotion";
    dir = FileUtils::getInstance()->fullPathForFilename(dir);
    dir += "/";
    file_utils::get_files_in_dir(dir, &files);
    
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    
    files.push_back("emotion/bluefaces_01.png");
    files.push_back("emotion/bluefaces_02.png");
    files.push_back("emotion/bluefaces_03.png");
    files.push_back("emotion/bluefaces_04.png");
    files.push_back("emotion/bluefaces_05.png");
    files.push_back("emotion/bluefaces_06.png");
    files.push_back("emotion/bluefaces_07.png");
    files.push_back("emotion/bluefaces_08.png");
    files.push_back("emotion/bluefaces_09.png");
    files.push_back("emotion/bluefaces_10.png");
    files.push_back("emotion/bluefaces_11.png");
    files.push_back("emotion/bluefaces_12.png");
    files.push_back("emotion/bluefaces_13.png");
    files.push_back("emotion/bluefaces_14.png");
    files.push_back("emotion/bluefaces_16.png");
    files.push_back("emotion/bluefaces_17.png");
    files.push_back("emotion/bluefaces_18.png");
    files.push_back("emotion/bluefaces_19.png");
    files.push_back("emotion/bluefaces_20.png");
    files.push_back("emotion/bluefaces_22.png");
    files.push_back("emotion/bluefaces_23.png");
    files.push_back("emotion/bluefaces_24.png");
    files.push_back("emotion/bluefaces_25.png");
    files.push_back("emotion/bluefaces_26.png");
    files.push_back("emotion/bluefaces_27.png");
    files.push_back("emotion/bluefaces_28.png");
    files.push_back("emotion/bluefaces_29.png");
    files.push_back("emotion/bluefaces_30.png");
    files.push_back("emotion/bluefaces_34.png");
    files.push_back("emotion/bluefaces_35.png");
    files.push_back("emotion/bluefaces_36.png");
    files.push_back("emotion/bluefaces_38.png");
    files.push_back("emotion/bluefaces_39.png");
    files.push_back("emotion/bluefaces_40.png");
    files.push_back("emotion/blueqface_001.png");
    files.push_back("emotion/blueqface_002.png");
    files.push_back("emotion/blueqface_003.png");
    files.push_back("emotion/blueqface_004.png");
    files.push_back("emotion/blueqface_007.png");
    files.push_back("emotion/blueqface_008.png");
    files.push_back("emotion/blueqface_009.png");
    files.push_back("emotion/blueqface_010.png");
    files.push_back("emotion/blueqface_011.png");
    files.push_back("emotion/blueqface_014.png");
    files.push_back("emotion/blueqface_015.png");
    files.push_back("emotion/blueqface_016.png");
    files.push_back("emotion/blueqface_017.png");
    files.push_back("emotion/blueqface_018.png");
    files.push_back("emotion/blueqface_021.png");
    files.push_back("emotion/blueqface_022.png");
    files.push_back("emotion/blueqface_024.png");
    files.push_back("emotion/blueqface_025.png");
    files.push_back("emotion/blueqface_027.png");
    files.push_back("emotion/blueqface_028.png");
    files.push_back("emotion/blueqface_029.png");
    files.push_back("emotion/blueqface_030.png");
    files.push_back("emotion/blueqface_031.png");
    files.push_back("emotion/blueqface_032.png");
    files.push_back("emotion/blueqface_034.png");
    files.push_back("emotion/blueqface_035.png");
    files.push_back("emotion/blueqface_036.png");
    files.push_back("emotion/blueqface_037.png");
    files.push_back("emotion/blueqface_038.png");
    files.push_back("emotion/blueqface_039.png");
    files.push_back("emotion/blueqface_040.png");
    
#endif
    Layout* layout;
    for (int idx = 0; idx < files.size(); idx++)
    {
        int intLine = (idx / 6) % 4;   //行
        int intColumn = idx % 6; //列
        
        if (idx % (6*4) == 0)
        {
            layout = Layout::create();
            pageview_->addPage(layout);
        }
        
        std::string filename = files.at(idx);
        log("filename:%s", filename.c_str());
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
        filename = dir + filename;
#endif
        ImageView* iv = ImageView::create(filename);
        iv->setTouchEnabled(true);
        iv->setPosition(Point(intColumn * 85 + 80, (4 - intLine) * 80 - 40));
        iv->setScale(0.5);
        iv->addClickEventListener(std::bind(&LayerEmotion::click_emotion, this, std::placeholders::_1));
        layout->addChild(iv);
        
    }
    
    float width = 0;
    float center = pageview_->getPosition().x + pageview_->getContentSize().width / 2;
    
    
    for (size_t i = 0; i < pageview_->getPages().size(); i++) {
        Sprite* icon = Sprite::create("image/page_view_point_normal.png");
        width = icon->getContentSize().width;
        
        float left_origin = center - width * pageview_->getPages().size() / 2;
        icon->setPosition(i * (icon->getContentSize().width + 10) + left_origin, 50);
        
        this->addChild(icon);
        page_icons_.push_back(icon);
    }
    
    this->pageview_turn(pageview_, PageView::EventType::TURNING);
}

#define  MAX_COUNT 7

void LayerEmotion::click_emotion(Ref* sender)
{
    ImageView* iv = dynamic_cast<ImageView*>(sender);
    if(iv) {
        auto& imagename = iv->getImageName();
        log("imagename:%s", imagename.c_str());
        auto iv = Sprite::create(imagename);
        iv->setAnchorPoint(Point::ZERO);
        iv->setScale(0.5);
        image_container_->addChild(iv);
        input_emotions_.push_back(std::make_pair(imagename, iv));
        
        auto emotions = input_emotions_;
        while (emotions.size() > MAX_COUNT) {
            auto& front = emotions.front();
            front.second->setVisible(false);
            emotions.erase(emotions.begin());
        }
        int idx = 0;
        for (auto& iter : emotions) {
            iter.second->setVisible(true);
            iter.second->setPosition(Point(idx * iter.second->getContentSize().width/2 + 10, 0));
            idx++;
        }
    }
}

void LayerEmotion::click_btn_send(Ref* sender)
{
    if(input_emotions_.empty()) {
        return;
    }
    std::string message;
    
    for (auto& iter : input_emotions_) {
        std::string filename = file_utils::file_name(iter.first);
        if (message.empty()) {
            message = filename;
        }
        else {
            message = message + "#" + filename;
        }
    }
    
    for (auto& iter : input_emotions_) {
        iter.second->removeFromParent();
    }
    input_emotions_.clear();
    
    if (emotion_callback_)
    {
        emotion_callback_(message);
    }
    
//    // by lvzhangle
//    msg::ChatMessage * ChatMessage = new msg::ChatMessage();
    
//    ChatMessage->mutable_userdetailinfo()->set_userid(GDM->get_user_id());
//    ChatMessage->mutable_userdetailinfo()->set_nickname(GDM->get_user_data()->get_nickname());
//    ChatMessage->mutable_userdetailinfo()->set_picname(GDM->get_user_data()->get_head_pic_name());
//    ChatMessage->mutable_userdetailinfo()->set_sex(GDM->get_user_data()->get_sex());
//    
//    ChatMessage->mutable_messageinfo()->mutable_picinfo()->set_picname(message);
//    ChatMessage->mutable_messageinfo()->set_type(msg::MessageType::Picture);
//    if (GDM->get_chat_status() == 1){
//        ChatMessage->set_groupid(GDM->get_select_group_id());
//    }else if (GDM->get_chat_status() == 2){
//        ChatMessage->set_receiveuserid(GDM->get_select_friend_id());
//    }
//    GDM->send_chat_message(*ChatMessage);
}
void LayerEmotion::click_btn_keyboard(Ref* sender)
{
    if (chat_callback_) {
        chat_callback_();
    }
    this->removeFromParent();
}
void LayerEmotion::click_btn_del(Ref* sender)
{
    if(!input_emotions_.empty()) {
        input_emotions_.back().second->removeFromParent();
        input_emotions_.pop_back();
        
        auto emotions = input_emotions_;
        while (emotions.size() > MAX_COUNT) {
            auto& front = emotions.front();
            front.second->setVisible(false);
            emotions.erase(emotions.begin());
        }
        int idx = 0;
        for (auto& iter : emotions) {
            iter.second->setVisible(true);
            iter.second->setPosition(Point(idx * iter.second->getContentSize().width/2 + 10, 0));
            idx++;
        }
    }
}
void LayerEmotion::click_btn_voice(Ref* sender)
{
    if (voice_callback_) {
        voice_callback_();
    }
    this->removeFromParent();
}

//void LayerEmotion::editBoxEditingDidBegin(EditBox* editBox)
//{
//    if(chat_callback_) {
//        chat_callback_();
//    }
//    this->removeFromParent();
//}

void LayerEmotion::removeFromParent()
{
    Layer::removeFromParent();
    
}

void LayerEmotion::pageview_turn(Ref*, PageView::EventType event)
{
    if(event == PageView::EventType::TURNING) {
        auto idx = pageview_->getCurPageIndex();
        for (auto& iter : page_icons_) {
            iter->setTexture("image/page_view_point_normal.png");
        }
        auto icon = page_icons_.at(idx);
        icon->setTexture("image/page_view_point_select.png");
    }
}