#include "EmotionLayer.h"
#include "file_utils.h"
#include "GameTools.h"
#include "GameDataManager.h"
#include "msg.pb.h"

bool EmotionLayer::init()
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
    btn_emotion_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "btn_emotion"));
    btn_del_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "btn_del"));
    
    btn_voice_->addClickEventListener(std::bind(&EmotionLayer::click_btn_voice, this, std::placeholders::_1));
    btn_emotion_->addClickEventListener(std::bind(&EmotionLayer::click_btn_emotion, this, std::placeholders::_1));
    btn_del_->addClickEventListener(std::bind(&EmotionLayer::click_btn_del, this, std::placeholders::_1));
    
    auto btn_send = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "btn_send"));
    btn_send->addClickEventListener(std::bind(&EmotionLayer::click_btn_send, this, std::placeholders::_1));
    
    pageview_ = dynamic_cast<PageView*>(CSLoader::seekNodeByName(root, "pageview"));
//    void addEventListener(const ccPageViewCallback& callback);
    pageview_->addEventListener(std::bind(&EmotionLayer::pageview_turn, this, std::placeholders::_1, std::placeholders::_2));
    auto tf_chat = dynamic_cast<TextField*>(CSLoader::seekNodeByName(root, "tf_chat"));
    tf_chat_ = tf_chat;
    tf_chat_->setVisible(false);
    
    image_container_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root, "image_container"));
    
//    tf_chat_ = GameTools::createEditBoxFromTextField(root, tf_chat);
//    tf_chat_->setDelegate(this);
    
    init_emotions();
    
    return true;
}


void EmotionLayer::init_emotions()
{
    std::vector<std::string> files;
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    std::string dir = "emotion";
    dir = FileUtils::getInstance()->fullPathForFilename(dir);
    dir += "/";
    file_utils::get_files_in_dir(dir, &files);

#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    
    files.push_back("emotion/ep_blush.png");
    files.push_back("emotion/ep_deng.png");
    files.push_back("emotion/ep_dizzy.png");
    files.push_back("emotion/ep_evilish.png");
    files.push_back("emotion/ep_grin.png");
    files.push_back("emotion/ep_he.png");
    files.push_back("emotion/ep_huh.png");
    files.push_back("emotion/ep_kiss.png");
    files.push_back("emotion/ep_laugh.png");
    files.push_back("emotion/ep_omg.png");
    files.push_back("emotion/ep_on_fire.png");
    files.push_back("emotion/ep_paranoid.png");
    files.push_back("emotion/ep_pissedoff.png");
    files.push_back("emotion/ep_sad.png");
    files.push_back("emotion/ep_slow.png");
    files.push_back("emotion/ep_smile.png");
    files.push_back("emotion/ep_snooty.png");
    files.push_back("emotion/ep_sweating.png");
    files.push_back("emotion/ep_want.png");
    files.push_back("emotion/ep_x_x.png");
    files.push_back("emotion/ep_xd.png");
    files.push_back("emotion/ep_zzgif.png");
    files.push_back("emotion/expression.png");
    
#endif
    auto layout = Layout::create();
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 4; j++) {
            int idx = j * 6 + i;
            if(idx >= files.size()) break;
            std::string filename = files.at(idx);
            log("filename:%s", filename.c_str());
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
            filename = dir + filename;
#endif
            ImageView* iv = ImageView::create(filename);
            iv->setTouchEnabled(true);
            iv->setPosition(Point(i * 85 + 80, (4 - j) * 80 - 40));
            iv->setScale(0.5);
            iv->addClickEventListener(std::bind(&EmotionLayer::click_emotion, this, std::placeholders::_1));
            layout->addChild(iv);
        }
    }
    btn_del_->setVisible(false);
    Button* btn = dynamic_cast<Button*>(btn_del_->clone());
    btn->setPosition(Point(5 * 85 + 80, (4 - 3) * 80 - 40));
    btn->setVisible(true);
    btn->addClickEventListener(std::bind(&EmotionLayer::click_btn_del, this, std::placeholders::_1));
    layout->addChild(btn);
    
    pageview_->addPage(layout);
    
//    Layout* layout1 =  dynamic_cast<Layout*>(layout->clone());
//    pageview_->addPage(layout1);
    
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

void EmotionLayer::click_emotion(Ref* sender)
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
            iter.second->setPosition(Point(idx * iter.second->getContentSize().width/2 + 10, 10));
            idx++;
        }
    }
}

void EmotionLayer::click_btn_send(Ref* sender)
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
    
    
    // by lvzhangle
    msg::ChatMessage ChatMessage;
    
    ChatMessage.mutable_userdetailinfo()->set_userid(GDM->get_user_id());
    
    ChatMessage.mutable_messageinfo()->mutable_picinfo()->set_picname(message);
    ChatMessage.mutable_messageinfo()->set_type(msg::MessageType::Picture);
    if (chat_status == 1){
        ChatMessage.set_groupid(group_id_);
    }else if (chat_status == 2){
        ChatMessage.set_receiveuserid(group_id_);
    }
    GDM->send_chat_message(ChatMessage);
}
void EmotionLayer::click_btn_emotion(Ref* sender)
{
    if (chat_callback_) {
        chat_callback_();
    }
    this->removeFromParent();
}
void EmotionLayer::click_btn_del(Ref* sender)
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
            iter.second->setPosition(Point(idx * iter.second->getContentSize().width/2 + 10, 10));
            idx++;
        }
    }
}
void EmotionLayer::click_btn_voice(Ref* sender)
{
    if (voice_callback_) {
        voice_callback_();
    }
    this->removeFromParent();
}

void EmotionLayer::editBoxEditingDidBegin(EditBox* editBox)
{
    if(chat_callback_) {
        chat_callback_();
    }
    this->removeFromParent();
}

void EmotionLayer::removeFromParent()
{
    Layer::removeFromParent();
  
}

void EmotionLayer::pageview_turn(Ref*, PageView::EventType event)
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