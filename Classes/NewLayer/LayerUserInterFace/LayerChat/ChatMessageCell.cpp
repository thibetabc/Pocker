//
//  ChatMessageCell.cpp
//  pokerclient
//
//  Created by lvzhangle on 16/4/11.
//
//

#include "ChatMessageCell.h"
#include "SceneManager.h"
#include "GameDataManager.h"
#include "PokerDataManager.h"
#include "FileManager.h"
#include "ChatMessageManager.h"
#include "AudioManager.h"

#include "utils.h"
#include "TipView.h"
#include "VoiceRecorderInterface.h"
#include "cmd_def.h"
#include "LayerUserInfo.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "SoundPlayerInterface.h"
#endif

std::string ChatMessageCell::select_voice_ = "";

bool ChatMessageCell::init()
{
    if (!TableViewCell::init()) {
        return false;
    }
    
    //std::shared_ptr<msg::ChatMessage> chat_message_data_ = nullptr;
    
    listview_width_ = 0.0;
    
    sprite_head_icon_ = nullptr;
    
    image_chatmessage_cell_bg_ = nullptr;
    
    image_voice_icon_ = nullptr;
    
    sprite_sending_ = nullptr;
    
    btn_resend_ = nullptr;
    
    image_room_icon_ = nullptr;
    
    //image_chatmessage_picture_ = nullptr;
    
    image_head_file_ = "";
    
    widget_head_bg_ = Widget::create();
    
    widget_head_bg_->setContentSize(cocos2d::Size(80, 80));
    
    //widget_head_bg_->setEnabled(true);
    widget_head_bg_->setTouchEnabled(true);
    widget_head_bg_->setSwallowTouches(false);
    
    widget_head_bg_->addClickEventListener(std::bind(&ChatMessageCell::click_head_icon, this, std::placeholders::_1));
    
    this->addChild(widget_head_bg_);
    
    text_nick_name_ = Text::create("", "", MESSAGE_NICK_NAME_FONT_SIZE);
    text_nick_name_->setColor(cocos2d::Color3B(255, 255, 255));
    this->addChild(text_nick_name_);
    
    text_message_time_ = Text::create("", "", MESSAGE_MESSAGE_TIME_FONT_SIZE);
    text_message_time_->setColor(cocos2d::Color3B(255, 255, 255));
    this->addChild(text_message_time_);
    
    this->setAnchorPoint(cocos2d::Vec2::ZERO);
    
    dispatch_.register_event(EVENT_DOWLOAD_FILE,
                             BIND_FUNC(this, ChatMessageCell::handle_download_event));
    
    return true;
}

void ChatMessageCell::message_cell_ui_clear()
{
    //clear
    if (!vec_emotions_.empty())
    {
        for (auto vecIter : vec_emotions_)
        {
            for (auto emotionIter : vecIter)
            {
                if (emotionIter)
                {
                    emotionIter->removeFromParent();
                }
            }
        }
        
        vec_emotions_.clear();
    }
    
    if (richtext_message_)
    {
        richtext_message_->removeFromParent();
        richtext_message_ = nullptr;
    }

    if (image_voice_icon_)
    {
        image_voice_icon_->removeFromParent();
        image_voice_icon_ = nullptr;
    }
    
    if (text_nick_name_)
    {
        text_nick_name_->setVisible(false);
    }
    
    if (text_message_time_)
    {
        text_message_time_->setVisible(false);
    }
    
    if (sprite_head_icon_)
    {
        sprite_head_icon_->setVisible(false);
    }
    
    if (widget_head_bg_)
    {
        widget_head_bg_->setVisible(false);
    }
    
    if (image_chatmessage_cell_bg_)
    {
        image_chatmessage_cell_bg_->addClickEventListener(nullptr);
    }
    
    if (sprite_sending_)
    {
        sprite_sending_->stopAllActions();
        sprite_sending_->removeFromParent();
        sprite_sending_ = nullptr;
    }
    
    if (btn_resend_)
    {
        btn_resend_->removeFromParent();
        btn_resend_ = nullptr;
    }
    
    if (image_room_icon_)
    {
        image_room_icon_->removeFromParent();
        image_room_icon_ = nullptr;
    }
    
    
//    if (image_chatmessage_picture_)
//    {
//        image_chatmessage_picture_->removeFromParent();
//        image_chatmessage_picture_ = nullptr;
//    }
    
    if (image_chatmessage_cell_bg_)
    {
        image_chatmessage_cell_bg_->removeFromParent();
        image_chatmessage_cell_bg_ = nullptr;
    }
}

void ChatMessageCell::update_data(const msg::ChatMessage& chat_message_data, const float listview_width, bool isDisplayMessageTime)
{
    chat_message_data_ = chat_message_data;
    
    listview_width_ = listview_width;
    
    bool is_mine = (chat_message_data_.has_userdetailinfo()) && (GDM->get_user_id() == chat_message_data_.userdetailinfo().userid());
    
    switch (chat_message_data_.messageinfo().type()) {
#pragma mark MessageText
        case msg::MessageType::Text:
        {
            message_cell_ui_clear();
            
            richtext_message_ = RichText::create();
            
            richtext_message_->ignoreAnchorPointForPosition(true);
            
            std::string strMessage = chat_message_data_.messageinfo().textinfo().text();
            
            cocos2d::Color3B colorText;
            if (is_mine)
            {
                colorText = cocos2d::Color3B(255, 255, 255);
            }else
            {
                colorText = cocos2d::Color3B(151, 151, 151);
            }
            
            if (!strMessage.empty())
            {
                std::u16string utf16String;
                StringUtils::UTF8ToUTF16(strMessage, utf16String);
                
                int intStrCur = 0;
                
                while (utf16String.length() - intStrCur > 125)
                {
                    std::u16string u16StrText;
                    
                    u16StrText = utf16String.substr(intStrCur, 125);

                    std::string strText;
                    StringUtils::UTF16ToUTF8(u16StrText, strText);
                    
                    RichElementText * richEleText = RichElementText::create(0, colorText, 255, strText, "", MESSAGE_TEXT_FONT_SIZE);
                    
                    richtext_message_->pushBackElement(richEleText);
                    
                    intStrCur += 125;
                }
                std::u16string u16StrText;
                
                u16StrText = utf16String.substr(intStrCur, utf16String.length());
                
                std::string strText;
                StringUtils::UTF16ToUTF8(u16StrText, strText);
                
                RichElementText * richEleText = RichElementText::create(0, colorText, 255, strText, "", MESSAGE_TEXT_FONT_SIZE);
                
                richtext_message_->pushBackElement(richEleText);
            }
            this->addChild(richtext_message_);
            
            if (image_chatmessage_cell_bg_)
            {
                
            }else
            {
                image_chatmessage_cell_bg_ = ImageView::create("login_res/chat_res/image_chatmessage_text_bg.png");
                
                this->addChild(image_chatmessage_cell_bg_);
            }
            
            
            //cocos2d::Rect(60, 80, 415, 30)
            //cocos2d::Rect(35, 30, 418, 30)
            image_chatmessage_cell_bg_->setScale9Enabled(true);
            image_chatmessage_cell_bg_->setCapInsets(cocos2d::Rect(35, 30, 418, 30));
            
            richtext_message_->ignoreContentAdaptWithSize(true);
            richtext_message_->formatText();
            
            if (richtext_message_->getContentSize().width < MESSAGE_TEXT_BG_WIDTH_MAX - MESSAGE_TEXT_BG_OFFSET_LEFT - MESSAGE_TEXT_BG_OFFSET_RIGHT)
            {
                
            }else
            {
                richtext_message_->ignoreContentAdaptWithSize(false);
                richtext_message_->setContentSize(cocos2d::Size(MESSAGE_TEXT_BG_WIDTH_MAX - MESSAGE_TEXT_BG_OFFSET_LEFT - MESSAGE_TEXT_BG_OFFSET_RIGHT, 0));
                
                richtext_message_->formatText();
            }
            
            cocos2d::Size sizeRichText = richtext_message_->getVirtualRendererSize();
            
            cocos2d::Size sizeImageBg = sizeRichText;
            sizeImageBg.width = std::max(sizeRichText.width + MESSAGE_TEXT_BG_OFFSET_LEFT + MESSAGE_TEXT_BG_OFFSET_RIGHT, (float)MESSAGE_TEXT_BG_WIDTH_MIN);
            sizeImageBg.height = std::max(sizeRichText.height +  MESSAGE_TEXT_BG_OFFSET_TOP +  MESSAGE_TEXT_BG_OFFSET_BUTTOM, (float)MESSAGE_TEXT_BG_HEIGHT_MIN);
            
            image_chatmessage_cell_bg_->setContentSize(cocos2d::Size(sizeImageBg));
//            this->setContentSize(cocos2d::Size(750, image_chatmessage_cell_bg_->getContentSize().height));
            
            cocos2d::Point posImageMessageBg;
            if (is_mine)
            {
                posImageMessageBg.x = listview_width / 2 + MESSAGE_BG_WIDTH_MAX / 2 - image_chatmessage_cell_bg_->getContentSize().width * (1-image_chatmessage_cell_bg_->getAnchorPoint().x);
            }else
            {
                posImageMessageBg.x = listview_width / 2 - MESSAGE_BG_WIDTH_MAX / 2 + image_chatmessage_cell_bg_->getContentSize().width * image_chatmessage_cell_bg_->getAnchorPoint().x;
            }
            posImageMessageBg.y = image_chatmessage_cell_bg_->getContentSize().height * image_chatmessage_cell_bg_->getAnchorPoint().y + MESSAGE_CELL_OFFSET;
            
            image_chatmessage_cell_bg_->setPosition(posImageMessageBg);
            
            cocos2d::Point posRichText;
            posRichText.x = image_chatmessage_cell_bg_->getPositionX() - image_chatmessage_cell_bg_->getContentSize().width * image_chatmessage_cell_bg_->getAnchorPoint().x + MESSAGE_TEXT_BG_OFFSET_LEFT;
            posRichText.y = image_chatmessage_cell_bg_->getPositionY() - image_chatmessage_cell_bg_->getContentSize().height * image_chatmessage_cell_bg_->getAnchorPoint().y + MESSAGE_TEXT_BG_OFFSET_BUTTOM;
            
            richtext_message_->setPosition(posRichText);
            
            image_chatmessage_cell_bg_->setLocalZOrder(ZORDER_MESSAGE_BG);
            richtext_message_->setLocalZOrder(ZORDER_MESSAGE_TEXT);
            
            init_head_sprite();
            
            if (isDisplayMessageTime)
                display_message_time();

            break;
        }
#pragma mark MessagePicture
        case msg::MessageType::Picture:
        {
            message_cell_ui_clear();
            
            auto adjust_position = [&](Node *emotion) {
                if(vec_emotions_.empty()) {
                    cocos2d::Point posEmotion;
                    if(is_mine) {
                        posEmotion.x = listview_width / 2 + MESSAGE_BG_WIDTH_MAX / 2 - MESSAGE_TEXT_BG_OFFSET_RIGHT - emotion->getContentSize().width * (1 - emotion->getAnchorPoint().x);
                    }
                    else {
                        posEmotion.x = listview_width / 2 - MESSAGE_BG_WIDTH_MAX / 2 + MESSAGE_TEXT_BG_OFFSET_LEFT + emotion->getContentSize().width *emotion->getAnchorPoint().x;
                    }
                    posEmotion.y = emotion->getContentSize().height * emotion->getAnchorPoint().y + MESSAGE_TEXT_BG_OFFSET_BUTTOM + MESSAGE_CELL_OFFSET;
                    
                    emotion->setPosition(posEmotion);
                    
                    std::vector<Node*> vec;
                    vec.push_back(emotion);
                    vec_emotions_.push_back(vec);
                    
                    cocos2d::log("emotion empty %f, %f", emotion->getPositionX(), emotion->getPositionY());
                }
                else {
                    Node* sentry= nullptr;
                    bool out_of_range = false;
                    if(is_mine) {
                        //sentry = vec_emotions_.back().front();
                        
                        if (vec_emotions_.size() == 1)
                        {
                            sentry = vec_emotions_.back().front();
                            if(sentry->getPosition().x - sentry->getContentSize().width * sentry->getAnchorPoint().x - emotion->getContentSize().width < listview_width / 2 + MESSAGE_BG_WIDTH_MAX / 2 - MESSAGE_TEXT_BG_WIDTH_MAX + MESSAGE_TEXT_BG_OFFSET_LEFT) {
                                out_of_range = true;
                            }
                        }
                        else
                        {
                            sentry = vec_emotions_.back().back();
                            if(sentry->getPosition().x + sentry->getContentSize().width * (1 - sentry->getAnchorPoint().x) + emotion->getContentSize().width > listview_width / 2 - MESSAGE_BG_WIDTH_MAX / 2 + MESSAGE_TEXT_BG_WIDTH_MAX - MESSAGE_TEXT_BG_OFFSET_RIGHT) {
                                out_of_range = true;
                            }
                        }
                    }
                    else {
                        sentry = vec_emotions_.back().back();
                        if(sentry->getPosition().x + sentry->getContentSize().width * (1 - sentry->getAnchorPoint().x) + emotion->getContentSize().width > listview_width / 2 - MESSAGE_BG_WIDTH_MAX / 2 + MESSAGE_TEXT_BG_WIDTH_MAX - MESSAGE_TEXT_BG_OFFSET_RIGHT) {
                            out_of_range = true;
                        }
                    }
                    if(!out_of_range) {
                        auto& back = vec_emotions_.back();
                        
                        if(vec_emotions_.size() == 1) {
                            for (auto& iter : back) {
                                if(is_mine) {
                                    iter->setPosition(iter->getPosition() - Point(iter->getContentSize().width, 0));
                                }
                            }
                        }
                        
                        emotion->setPosition(back.back()->getPosition() + Point(back.back()->getContentSize().width, 0));
                        back.push_back(emotion);
                        
                        cocos2d::log("emotion !out_of_range %f, %f", emotion->getPositionX(), emotion->getPositionY());
                    }
                    else {
                        
                        for (size_t i = 0; i < vec_emotions_.size(); i++) {
                            auto& one_line_emotions = vec_emotions_.at(i);
                            for (auto& one : one_line_emotions) {
                                one->setPosition(one->getPosition() + Point(0, one->getContentSize().height));
                            }
                        }
                        
                        auto& back = vec_emotions_.back();
                        
                        std::vector<Node*> trail;
                        trail.push_back(emotion);
                        emotion->setPosition(back.front()->getPosition() - Point(0, back.front()->getContentSize().height));
                        vec_emotions_.push_back(trail);
                        
                        cocos2d::log("emotion out_of_range %f, %f", emotion->getPositionX(), emotion->getPositionY());
                    }
                }
            };
            
            
            auto vec = tools::split(chat_message_data_.messageinfo().picinfo().picname(), "#");
            for (auto& img : vec) {
                std::string path = FileUtils::getInstance()->fullPathForFilename("emotion/" + img);
                if(!path.empty()) {
                    Sprite* emotion = Sprite::create(path);
                    emotion->setAnchorPoint(Point(0.5, 0));
                    this->addChild(emotion);
                    emotion->setLocalZOrder(ZORDER_MESSAGE_TEXT);
                    adjust_position(emotion);
                    
                    cocos2d::log("emotion split %f, %f", emotion->getPositionX(), emotion->getPositionY());
                }
                else {
                    ImageView* emotion;
                    if(GDM->is_file_exist(img)) {
                        image_chatmessage_picture_ =  ImageView::create(img);
                        Size s(120, 120);
                        Size contentSize = image_chatmessage_picture_->getContentSize();
                        s.width = s.height * (contentSize.width/contentSize.height);
                        if(contentSize.width > s.width || contentSize.height > s.height) {
                            image_chatmessage_picture_->ignoreContentAdaptWithSize(false);
                            image_chatmessage_picture_->setContentSize(s);
                        }
                        
                        image_chatmessage_picture_->setAnchorPoint(Point(0.5, 0));
                        this->addChild(image_chatmessage_picture_);
                        image_chatmessage_picture_->setLocalZOrder(ZORDER_MESSAGE_TEXT);
                        adjust_position(image_chatmessage_picture_);
                        image_chatmessage_picture_->setTouchEnabled(true);
                        image_chatmessage_picture_->addClickEventListener([=](Ref*){
                            auto imageview = ImageView::create(img);
                            auto scene = Director::getInstance()->getRunningScene();
                            auto& size = Director::getInstance()->getWinSize();
                            auto layout = Layout::create();
                            layout->setContentSize(size);
                            float scale = 1.0f;
                            if(imageview->getContentSize().width > size.width) {
                                scale = std::min(scale, size.width / imageview->getContentSize().width);
                            }
                            if(imageview->getContentSize().height > size.height) {
                                scale = std::min(scale, size.height / imageview->getContentSize().height);
                            }
                            imageview->setScale(scale);
                            LayerColor* layer = LayerColor::create(Color4B(240, 239, 245, 255), size.width, size.height);
                            layout->addChild(layer, -2);
                            layout->setTouchEnabled(true);
                            layout->addClickEventListener([=](Ref*){
                                layout->removeFromParent();
                            });
                            layout->addChild(imageview, 0);
                            imageview->setPosition(Point(size) * 0.5);
                            scene->addChild(layout, 10);
                        });
                    }
                    else {
                        //
                        image_chatmessage_picture_ = ImageView::create("login_res/chat_res/image_update_wait.png");
                        Size s(120, 120);
                        Size contentSize = image_chatmessage_picture_->getContentSize();
                        s.width = s.height * (contentSize.width/contentSize.height);
                        if(contentSize.width > s.width || contentSize.height > s.height) {
                            image_chatmessage_picture_->ignoreContentAdaptWithSize(false);
                            image_chatmessage_picture_->setContentSize(s);
                        }
                        image_chatmessage_picture_->setAnchorPoint(Point(0, 0));
                        this->addChild(image_chatmessage_picture_);
                        image_chatmessage_picture_->setLocalZOrder(ZORDER_MESSAGE_TEXT);
                        adjust_position(image_chatmessage_picture_);
                        FM.download_file(img);
                    }
                    
//                    // ronaldz. 如果这里发现上传队列中有这个图片， 就需要告知用户。
//                    if (FM.is_file_in_upload_queue(img)){
//                        if (m_pLayer != nullptr){
//                            m_pLayer->removeAllChildrenWithCleanup(true);
//                            m_pLayer = NULL;
//                        }
//                        m_pLayer = Layer::create();
//                        this->addChild(m_pLayer,255);
//                        LayerColor *black = LayerColor::create(Color4B(100, 100, 100, 100), 120, 120);
//                        //black->setAnchorPoint(Point(0.5,0));
//                        black->setPositionX(image_chatmessage_picture_->getPositionX()-image_chatmessage_picture_->getContentSize().width/2);
//                        black->setPositionY(image_chatmessage_picture_->getPositionY());
//                        black->setContentSize(image_chatmessage_picture_->getContentSize());
//                        
//                        //(image_head->getPosition() - Point(image_head->getContentSize()/2));
//                        m_pLayer->addChild(black);
//                        Label *label = Label::createWithSystemFont("正在上传","Arial", 18);
//                        label->setColor(cocos2d::Color3B::WHITE);
//                        label->setPosition(Point(black->getContentSize()/2));
//                        black->addChild(label);
//                        
//                    }
                    
                }
            }
            
            cocos2d::Size sizeVecEmotions;
            if(!vec_emotions_.empty()) {
                
                for (auto vecIter : vec_emotions_[0])
                {
                    sizeVecEmotions.width = sizeVecEmotions.width + vecIter->getContentSize().width;
                }
                
                sizeVecEmotions.height = vec_emotions_.size() * vec_emotions_.front().front()->getContentSize().height;

            }
            if (image_chatmessage_cell_bg_)
            {
                
            }else
            {
                image_chatmessage_cell_bg_ = ImageView::create("login_res/chat_res/image_chatmessage_text_bg.png");
                
                this->addChild(image_chatmessage_cell_bg_);
            }
            
            image_chatmessage_cell_bg_->setScale9Enabled(true);
            image_chatmessage_cell_bg_->setCapInsets(cocos2d::Rect(35, 30, 418, 30));
            
            sizeVecEmotions.height = std::max(sizeVecEmotions.height + MESSAGE_TEXT_BG_OFFSET_TOP + MESSAGE_TEXT_BG_OFFSET_BUTTOM, (float)MESSAGE_TEXT_BG_HEIGHT_MIN);
            
            
            image_chatmessage_cell_bg_->setContentSize(cocos2d::Size(sizeVecEmotions.width + MESSAGE_TEXT_BG_OFFSET_LEFT + MESSAGE_TEXT_BG_OFFSET_RIGHT, sizeVecEmotions.height));
            
            cocos2d::Point posImageMessageBg;
            if (is_mine)
            {
                posImageMessageBg.x = listview_width / 2 + MESSAGE_BG_WIDTH_MAX / 2 - image_chatmessage_cell_bg_->getContentSize().width * (1-image_chatmessage_cell_bg_->getAnchorPoint().x);
            }else
            {
                posImageMessageBg.x = listview_width / 2 - MESSAGE_BG_WIDTH_MAX / 2 + image_chatmessage_cell_bg_->getContentSize().width * image_chatmessage_cell_bg_->getAnchorPoint().x;
            }

            posImageMessageBg.y = image_chatmessage_cell_bg_->getContentSize().height * image_chatmessage_cell_bg_->getAnchorPoint().y + MESSAGE_CELL_OFFSET;
            
            image_chatmessage_cell_bg_->setPosition(posImageMessageBg);
            image_chatmessage_cell_bg_->setLocalZOrder(ZORDER_MESSAGE_BG);

            init_head_sprite();
            
            if (isDisplayMessageTime)
            {
                display_message_time();
            }
            
            this->setContentSize(cocos2d::Size(listview_width_, image_chatmessage_cell_bg_->getContentSize().height + text_message_time_->getContentSize().height + 5));
            
            image_chatmessage_cell_bg_->setVisible(false);
            
            break;
        }
#pragma mark MessageVoice
        case msg::MessageType::Voice:
        {
            message_cell_ui_clear();
            
            if (image_chatmessage_cell_bg_)
            {
                
            }else
            {
                image_chatmessage_cell_bg_ = ImageView::create("login_res/chat_res/image_chatmessage_text_bg.png");
                
                this->addChild(image_chatmessage_cell_bg_);
            }
            
            image_chatmessage_cell_bg_->setScale9Enabled(true);
            image_chatmessage_cell_bg_->setCapInsets(cocos2d::Rect(35, 30, 418, 30));
            image_chatmessage_cell_bg_->setTouchEnabled(true);
            image_chatmessage_cell_bg_->setSwallowTouches(false);
            image_chatmessage_cell_bg_->addClickEventListener(std::bind(&ChatMessageCell::click_image_bg, this, std::placeholders::_1));
            
            if (image_voice_icon_)
            {
                
            }else
            {
                image_voice_icon_ = ImageView::create();
                this->addChild(image_voice_icon_);
            }
            
            if (is_mine)
            {
                image_voice_icon_->loadTexture("login_res/chat_res/image_voice_icon_self.png");
            }else
            {
                image_voice_icon_->loadTexture("login_res/chat_res/image_voice_icon_other.png");
            }
            
            cocos2d::Size sizeImageMessageBg;
            sizeImageMessageBg.width = MESSAGE_BG_WIDTH_MAX / 6 * 3;
            sizeImageMessageBg.height = image_voice_icon_->getContentSize().height + MESSAGE_TEXT_BG_OFFSET_TOP + MESSAGE_TEXT_BG_OFFSET_BUTTOM;
            
            image_chatmessage_cell_bg_->setContentSize(sizeImageMessageBg);
            
            cocos2d::Point posImageMessageBg;
            if (is_mine)
            {
                posImageMessageBg.x = listview_width / 2 + MESSAGE_BG_WIDTH_MAX / 2 - image_chatmessage_cell_bg_->getContentSize().width * (1-image_chatmessage_cell_bg_->getAnchorPoint().x);
            }else
            {
                posImageMessageBg.x = listview_width / 2 - MESSAGE_BG_WIDTH_MAX / 2 + image_chatmessage_cell_bg_->getContentSize().width * image_chatmessage_cell_bg_->getAnchorPoint().x;
            }
            posImageMessageBg.y = image_chatmessage_cell_bg_->getContentSize().height * image_chatmessage_cell_bg_->getAnchorPoint().y + MESSAGE_CELL_OFFSET;
            
            image_chatmessage_cell_bg_->setPosition(posImageMessageBg);
            
            cocos2d::Point posImageVoiceIcon;
            if (is_mine)
            {
                posImageVoiceIcon.x = image_chatmessage_cell_bg_->getPositionX() - image_chatmessage_cell_bg_->getContentSize().width * image_chatmessage_cell_bg_->getAnchorPoint().x + MESSAGE_TEXT_BG_OFFSET_LEFT + image_voice_icon_->getContentSize().width * image_voice_icon_->getAnchorPoint().x;
            }else
            {
                posImageVoiceIcon.x = image_chatmessage_cell_bg_->getPositionX() + image_chatmessage_cell_bg_->getContentSize().width * (1 - image_chatmessage_cell_bg_->getAnchorPoint().x) - MESSAGE_TEXT_BG_OFFSET_RIGHT - (1 - image_voice_icon_->getAnchorPoint().x) * image_voice_icon_->getContentSize().height;
            }
            
            posImageVoiceIcon.y = image_chatmessage_cell_bg_->getPositionY() - image_chatmessage_cell_bg_->getContentSize().height * image_chatmessage_cell_bg_->getAnchorPoint().y + MESSAGE_TEXT_BG_OFFSET_BUTTOM + image_voice_icon_->getContentSize().height * image_voice_icon_->getAnchorPoint().x;
            
            image_voice_icon_->setPosition(posImageVoiceIcon);
            
            image_chatmessage_cell_bg_->setLocalZOrder(ZORDER_MESSAGE_BG);
            image_voice_icon_->setLocalZOrder(ZORDER_MESSAGE_TEXT);
            
            init_head_sprite();
            
            if (isDisplayMessageTime)
            {
                display_message_time();
            }
            
            this->setContentSize(cocos2d::Size(listview_width_, image_chatmessage_cell_bg_->getContentSize().height + text_message_time_->getContentSize().height + 5));
            
            break;
        }
#pragma mark MessageCreateRoom
        case msg::MessageType::CreateRoom:
        {
            message_cell_ui_clear();
            
            image_room_icon_ = ImageView::create();
            
            switch (chat_message_data_.messageinfo().createroominfo().type()) {
                case msg::RoomType::NLHOLDEM:
                {
                    image_room_icon_->loadTexture("login_res/creategameroom_res/image_createroom_type_icon_holdem.png");
                    break;
                }
                case msg::RoomType::OFC3:
                {
                    image_room_icon_->loadTexture("login_res/creategameroom_res/image_createroom_type_icon_ofc3.png");
                    break;
                }
                case msg::RoomType::OFC4:
                {
                    image_room_icon_->loadTexture("login_res/creategameroom_res/image_createroom_type_icon_ofc3.png");
                    break;
                }
                case msg::RoomType::PLO:
                {
                    image_room_icon_->loadTexture("login_res/creategameroom_res/image_createroom_type_icon_holdem.png");
                    break;
                }
                case msg::RoomType::PLOHILO:
                {
                    image_room_icon_->loadTexture("login_res/creategameroom_res/image_createroom_type_icon_holdem.png");
                    break;
                }
                default:
                {
                    break;
                }
            }
            
            this->addChild(image_room_icon_);
            
            cocos2d::Size sizeImageRoomIcon = image_room_icon_->getContentSize();
            
            richtext_message_ = RichText::create();
            
            richtext_message_->ignoreAnchorPointForPosition(true);
            
            //std::string strMessage = chat_message_data_.messageinfo().createroominfo().roomname();
            std::string strRoomName = chat_message_data_.messageinfo().createroominfo().roomname();
            strRoomName += '\n';
            
            std::string strMessage = "";
            ;
            strMessage += '\n';
            
            strMessage += chat_message_data_.messageinfo().createroominfo().text();
            
            cocos2d::Color3B colorText;
            colorText = cocos2d::Color3B(151, 151, 151);
            cocos2d::Color3B colorRoomName;
            colorRoomName = cocos2d::Color3B(255, 255, 255);
            
            
            RichElementText * richEleText = RichElementText::create(0, colorRoomName, 255, strRoomName, "", MESSAGE_TEXT_FONT_SIZE);
            
            richtext_message_->pushBackElement(richEleText);
            
            if (!strMessage.empty())
            {
                std::u16string utf16String;
                StringUtils::UTF8ToUTF16(strMessage, utf16String);
                
                int intStrCur = 0;
                
                while (utf16String.length() - intStrCur > 125)
                {
                    std::u16string u16StrText;
                    
                    u16StrText = utf16String.substr(intStrCur, 125);
                    
                    std::string strText;
                    StringUtils::UTF16ToUTF8(u16StrText, strText);
                    
                    RichElementText * richEleText = RichElementText::create(1, colorText, 255, strText, "", MESSAGE_TEXT_FONT_SIZE);
                    
                    richtext_message_->pushBackElement(richEleText);
                    
                    intStrCur += 125;
                }
                std::u16string u16StrText;
                
                u16StrText = utf16String.substr(intStrCur, utf16String.length());
                
                std::string strText;
                StringUtils::UTF16ToUTF8(u16StrText, strText);
                
                RichElementText * richEleText = RichElementText::create(0, colorText, 255, strText, "", MESSAGE_TEXT_FONT_SIZE);
                
                richtext_message_->pushBackElement(richEleText);
            }
            this->addChild(richtext_message_);
            
            cocos2d::Size sizeRichText;
            
            richtext_message_->ignoreContentAdaptWithSize(false);
            richtext_message_->setContentSize(cocos2d::Size(MESSAGE_BG_WIDTH_MAX - MESSAGE_BG_OFFSET_LEFT - MESSAGE_BG_OFFSET_RIGHT - sizeImageRoomIcon.width - 10, 0));
            
            richtext_message_->formatText();
            sizeRichText = richtext_message_->getVirtualRendererSize();
            
            
            if (image_chatmessage_cell_bg_)
            {
                
            }else
            {
                image_chatmessage_cell_bg_ = ImageView::create("login_res/chat_res/image_chatmessage_cell_bg.png");
                
                this->addChild(image_chatmessage_cell_bg_);
            }
            
            image_chatmessage_cell_bg_->setScale9Enabled(true);
            image_chatmessage_cell_bg_->setCapInsets(cocos2d::Rect(60, 80, 415, 30));
            image_chatmessage_cell_bg_->setTouchEnabled(true);
            image_chatmessage_cell_bg_->setSwallowTouches(false);
            image_chatmessage_cell_bg_->addClickEventListener(std::bind(&ChatMessageCell::click_image_bg, this, std::placeholders::_1));
            
            cocos2d::Size sizeImageBg;
            sizeImageBg.width = MESSAGE_BG_WIDTH_MAX;
            
            sizeImageBg.height = std::max(sizeImageRoomIcon.height, sizeRichText.height) + MESSAGE_BG_OFFSET_TOP + MESSAGE_BG_OFFSET_BUTTOM;
            
            image_chatmessage_cell_bg_->setContentSize(sizeImageBg);
            
            cocos2d::Point posImageMessageBg;
            
            posImageMessageBg.x = listview_width / 2 + image_chatmessage_cell_bg_->getContentSize().width * (image_chatmessage_cell_bg_->getAnchorPoint().x - 0.5);
            
            posImageMessageBg.y = image_chatmessage_cell_bg_->getContentSize().height * image_chatmessage_cell_bg_->getAnchorPoint().y;
            
            image_chatmessage_cell_bg_->setPosition(posImageMessageBg);
            
            cocos2d::Point posImageRoomIcon;
            posImageRoomIcon.x = image_chatmessage_cell_bg_->getPositionX() - image_chatmessage_cell_bg_->getContentSize().width * image_chatmessage_cell_bg_->getAnchorPoint().x + MESSAGE_BG_OFFSET_LEFT + image_room_icon_->getContentSize().width * image_room_icon_->getAnchorPoint().x;

            posImageRoomIcon.y = image_chatmessage_cell_bg_->getPositionY() + image_chatmessage_cell_bg_->getContentSize().height * ( 1 -image_chatmessage_cell_bg_->getAnchorPoint().y) - MESSAGE_BG_OFFSET_TOP - image_room_icon_->getContentSize().height * (1 - image_room_icon_->getAnchorPoint().y);
            
            cocos2d::Point posRichText;
            posRichText.x = posImageRoomIcon.x + image_room_icon_->getContentSize().width * (1 - image_room_icon_->getAnchorPoint().x) + 10;
            posRichText.y = image_chatmessage_cell_bg_->getPositionY() + image_chatmessage_cell_bg_->getContentSize().height * ( 1 -image_chatmessage_cell_bg_->getAnchorPoint().y) - MESSAGE_BG_OFFSET_TOP - sizeRichText.height;
            
            image_room_icon_->setPosition(posImageRoomIcon);
            richtext_message_->setPosition(posRichText);
            
            image_room_icon_->setLocalZOrder(ZORDER_MESSAGE_TEXT-1);
            richtext_message_->setLocalZOrder(ZORDER_MESSAGE_TEXT);
            image_chatmessage_cell_bg_->setLocalZOrder(ZORDER_MESSAGE_BG);
            
            
//            Sprite* spriteCellBgLight = dynamic_cast<Sprite*>(this->getChildByName("spriteCellBgLight"));
//            
//            if (spriteCellBgLight == nullptr)
//            {
//                spriteCellBgLight = Sprite::create("login_res/chat_res/chat_cell_bg_animate/btn_clubchat_invite_000.png");
//                spriteCellBgLight->setPosition(image_chatmessage_cell_bg_->getPosition());
//                this->addChild(spriteCellBgLight, ZORDER_MESSAGE_BG_SPRITE, "spriteCellBgLight");
//            }
//            
//            //spriteBtnCenterStar->setLocalZOrder(ZORDER_ANIMATION_CENTER_STAR);
//            if (spriteCellBgLight->getActionByTag(1000) == nullptr)
//            {
//                Animation* animationSpriteCellBgLight = Animation::create();
//                for (int i = 0; i < 90; i++) {
//                    char fileName[128];
//                    std::sprintf(fileName, "login_res/chat_res/chat_cell_bg_animate/btn_clubchat_invite_%03d.png", i);
//                    animationSpriteCellBgLight->addSpriteFrameWithFile(fileName);
//                }
//                animationSpriteCellBgLight->setDelayPerUnit(0.05f);
//                Animate* animateSpriteCellBgLight = Animate::create(animationSpriteCellBgLight);
//                
//                auto callfuncSpriteCellBgLight1 = CallFunc::create(std::function<void()>([=](){spriteCellBgLight->setVisible(true);}));
//                
//                auto callfuncSpriteCellBgLight2 = CallFunc::create(std::function<void()>([=](){spriteCellBgLight->setVisible(false);}));
//                
//                Sequence* sequenceSpriteCellBgLight = Sequence::create(callfuncSpriteCellBgLight1, animateSpriteCellBgLight, callfuncSpriteCellBgLight2, DelayTime::create(2), NULL);
//                sequenceSpriteCellBgLight->setTag(1000);
//                
//                spriteCellBgLight->runAction(RepeatForever::create(sequenceSpriteCellBgLight));
//            }
//            
//            spriteCellBgLight->setScale(image_chatmessage_cell_bg_->getContentSize().width / spriteCellBgLight->getContentSize().width, image_chatmessage_cell_bg_->getContentSize().height / spriteCellBgLight->getContentSize().height);
            
            init_head_sprite();
            
            if (isDisplayMessageTime)
            {
                display_message_time();
            }
            
            break;
        }
#pragma mark MessageGroupNotice
        case msg::MessageType::GroupNotice:
        {
            message_cell_ui_clear();
            
            richtext_message_ = RichText::create();
            
            richtext_message_->ignoreAnchorPointForPosition(true);
            
            std::string strMessage = chat_message_data_.messageinfo().groupnoticeinfo().text();
            
            cocos2d::Color3B colorText;
            colorText = cocos2d::Color3B(255, 255, 255);

            if (!strMessage.empty())
            {
                std::u16string utf16String;
                StringUtils::UTF8ToUTF16(strMessage, utf16String);
                
                int intStrCur = 0;
                
                while (utf16String.length() - intStrCur > 125)
                {
                    std::u16string u16StrText;
                    
                    u16StrText = utf16String.substr(intStrCur, 125);
                    
                    std::string strText;
                    StringUtils::UTF16ToUTF8(u16StrText, strText);
                    
                    RichElementText * richEleText = RichElementText::create(0, colorText, 255, strText, "", MESSAGE_TEXT_FONT_SIZE);
                    
                    richtext_message_->pushBackElement(richEleText);
                    
                    intStrCur += 125;
                }
                std::u16string u16StrText;
                
                u16StrText = utf16String.substr(intStrCur, utf16String.length());
                
                std::string strText;
                StringUtils::UTF16ToUTF8(u16StrText, strText);
                
                RichElementText * richEleText = RichElementText::create(0, colorText, 255, strText, "", MESSAGE_TEXT_FONT_SIZE);
                
                richtext_message_->pushBackElement(richEleText);
            }
            this->addChild(richtext_message_);
            
            if (image_chatmessage_cell_bg_)
            {
                
            }else
            {
                image_chatmessage_cell_bg_ = ImageView::create("login_res/chat_res/image_chatmessage_cell_bg.png");
                
                this->addChild(image_chatmessage_cell_bg_);
            }
            
            image_chatmessage_cell_bg_->setScale9Enabled(true);
            image_chatmessage_cell_bg_->setCapInsets(cocos2d::Rect(60, 80, 415, 30));
            
            richtext_message_->ignoreContentAdaptWithSize(true);
            richtext_message_->formatText();
            
            if (richtext_message_->getContentSize().width < MESSAGE_BG_WIDTH_MAX - MESSAGE_BG_OFFSET_LEFT - MESSAGE_BG_OFFSET_RIGHT)
            {
                
            }else
            {
                richtext_message_->ignoreContentAdaptWithSize(false);
                richtext_message_->setContentSize(cocos2d::Size(MESSAGE_BG_WIDTH_MAX - MESSAGE_BG_OFFSET_LEFT - MESSAGE_BG_OFFSET_RIGHT, 0));
                
                richtext_message_->formatText();
            }
            
            cocos2d::Size sizeRichText = richtext_message_->getVirtualRendererSize();
            
            
            sizeRichText.height = std::max(sizeRichText.height+MESSAGE_BG_OFFSET_TOP + MESSAGE_BG_OFFSET_BUTTOM, (float)MESSAGE_BG_HEIGHT_MIN);
            
            
            image_chatmessage_cell_bg_->setContentSize(cocos2d::Size(sizeRichText.width + MESSAGE_BG_OFFSET_LEFT + MESSAGE_BG_OFFSET_RIGHT, sizeRichText.height));
            
            cocos2d::Point posImageMessageBg;
            
            posImageMessageBg.x = listview_width / 2 + image_chatmessage_cell_bg_->getContentSize().width * (image_chatmessage_cell_bg_->getAnchorPoint().x - 0.5);
            
            posImageMessageBg.y = image_chatmessage_cell_bg_->getContentSize().height * image_chatmessage_cell_bg_->getAnchorPoint().y;
            
            image_chatmessage_cell_bg_->setPosition(posImageMessageBg);
            
            cocos2d::Point posRichText;
            posRichText.x = image_chatmessage_cell_bg_->getPositionX() - image_chatmessage_cell_bg_->getContentSize().width * image_chatmessage_cell_bg_->getAnchorPoint().x + MESSAGE_BG_OFFSET_LEFT;
            posRichText.y = image_chatmessage_cell_bg_->getPositionY() - image_chatmessage_cell_bg_->getContentSize().height * image_chatmessage_cell_bg_->getAnchorPoint().y + MESSAGE_BG_OFFSET_BUTTOM;
            
            richtext_message_->setPosition(posRichText);
            
            
            image_chatmessage_cell_bg_->setLocalZOrder(ZORDER_MESSAGE_BG);
            richtext_message_->setLocalZOrder(ZORDER_MESSAGE_TEXT);
            
            display_message_time();
            
            this->setContentSize(cocos2d::Size(listview_width_, image_chatmessage_cell_bg_->getContentSize().height + text_message_time_->getContentSize().height + 5));
            
            break;
        }
        default:
            break;
    }

}

void ChatMessageCell::init_head_sprite()
{

    bool is_mine;
    
    //head
    cocos2d::Size headSize = cocos2d::Size(80, 80);
    std::string HeadPicName;
    std::string strNickName;
    if (chat_message_data_.messageinfo().type() == msg::MessageType::CreateRoom)
    {
        is_mine = (chat_message_data_.messageinfo().createroominfo().has_creater()) && (GDM->get_user_id() == chat_message_data_.messageinfo().createroominfo().creater().userid());
        HeadPicName = chat_message_data_.messageinfo().createroominfo().creater().nickname();
         strNickName = chat_message_data_.messageinfo().createroominfo().creater().nickname();
    }else
    {
        HeadPicName = chat_message_data_.userdetailinfo().picname();
        strNickName = chat_message_data_.userdetailinfo().nickname();
        is_mine = (chat_message_data_.has_userdetailinfo()) && (GDM->get_user_id() == chat_message_data_.userdetailinfo().userid());
    }
    
    image_head_file_ = HeadPicName;
    if(!image_head_file_.empty() && GDM->is_file_exist(image_head_file_)) {
        
    }
    else {
        HeadPicName = "Default/image_defualt_user_icon_large.png";
        FM.download_file(image_head_file_);
    }
    
    
    if (sprite_head_icon_)
    {
        if (sprite_head_icon_->getFileName() == HeadPicName)
        {
            cocos2d::log("ChatMessageCell:Head has create!");
        }else
        {
            sprite_head_icon_->initWithFile(HeadPicName, "Default/image_mask_circle.png");
            sprite_head_icon_->setScale(headSize.width / sprite_head_icon_->getContentSize().width, headSize.height / sprite_head_icon_->getContentSize().height);
        }
    }else
    {
        
        sprite_head_icon_ = ShaderSprite::create(HeadPicName, "Default/image_mask_circle.png");
        
        sprite_head_icon_->setScale(headSize.width / sprite_head_icon_->getContentSize().width, headSize.height / sprite_head_icon_->getContentSize().height);
        
        this->addChild(sprite_head_icon_);
    }
    
    cocos2d::Point posHeadIcon;
    if (is_mine)
    {
        posHeadIcon.x = image_chatmessage_cell_bg_->getPositionX() + image_chatmessage_cell_bg_->getContentSize().width * (1 - image_chatmessage_cell_bg_->getAnchorPoint().x);
        
        posHeadIcon.x = posHeadIcon.x + headSize.width * sprite_head_icon_->getAnchorPoint().x + 5.0;
    }else
    {
        posHeadIcon.x = image_chatmessage_cell_bg_->getPositionX() - image_chatmessage_cell_bg_->getContentSize().width * image_chatmessage_cell_bg_->getAnchorPoint().x;
        
        posHeadIcon.x = posHeadIcon.x - headSize.width * sprite_head_icon_->getAnchorPoint().x - 5.0;
    }
    
    posHeadIcon.y = image_chatmessage_cell_bg_->getPositionY() + image_chatmessage_cell_bg_->getContentSize().height * (1 -image_chatmessage_cell_bg_->getAnchorPoint().y) - headSize.height * (1 -sprite_head_icon_->getAnchorPoint().y);
    
    sprite_head_icon_->setPosition(posHeadIcon);
    
    
    if (!sprite_head_icon_->isVisible())
    {
        sprite_head_icon_->setVisible(true);
        sprite_head_icon_->setLocalZOrder(ZORDER_MESSAGE_HEAD_ICON);
    }
    
    widget_head_bg_->setAnchorPoint(sprite_head_icon_->getAnchorPoint());
    widget_head_bg_->setPosition(sprite_head_icon_->getPosition());
    widget_head_bg_->setLocalZOrder(ZORDER_MESSAGE_HEAD_ICON+1);
    if (!widget_head_bg_->isVisible())
    {
        widget_head_bg_->setVisible(true);
    }
    

    text_nick_name_->setString(strNickName);
    text_nick_name_->setLocalZOrder(ZORDER_MESSAGE_NICK_NAME);
    if (!text_nick_name_->isVisible())
    {
        text_nick_name_->setVisible(true);
    }
    
    cocos2d::Point posNickName;
    if (is_mine)
    {
        posNickName.x = widget_head_bg_->getPositionX() + widget_head_bg_->getContentSize().width * (1 - widget_head_bg_->getAnchorPoint().x) - text_nick_name_->getContentSize().width * (1 - text_nick_name_->getAnchorPoint().x);
    }else
    {
        posNickName.x = widget_head_bg_->getPositionX() - widget_head_bg_->getContentSize().width * widget_head_bg_->getAnchorPoint().x + text_nick_name_->getContentSize().width * text_nick_name_->getAnchorPoint().x;
    }
    posNickName.y = widget_head_bg_->getPositionY() + widget_head_bg_->getContentSize().height * (1 - widget_head_bg_->getAnchorPoint().y) + text_nick_name_->getContentSize().height * text_nick_name_->getAnchorPoint().y + 5;
    text_nick_name_->setPosition(posNickName);
    
    //status
    if(is_mine)
    {
        msg::MessageStatus status = chat_message_data_.status();
        switch (status) {
            case msg::MessageStatus::Success:
            {
                
                break;
            }
            case msg::MessageStatus::Sending:
            {
                sprite_sending_ = Sprite::create("waitingview/round_1.png");
                
                cocos2d::Point posSpriteSending;
                posSpriteSending.x = image_chatmessage_cell_bg_->getPositionX() -  image_chatmessage_cell_bg_->getContentSize().width * image_chatmessage_cell_bg_->getAnchorPoint().x;
                posSpriteSending.x = posSpriteSending.x - sprite_sending_->getContentSize().width * sprite_head_icon_->getAnchorPoint().x - 5.0;
                posSpriteSending.y = image_chatmessage_cell_bg_->getPositionY() + image_chatmessage_cell_bg_->getContentSize().height * (1 - image_chatmessage_cell_bg_->getAnchorPoint().y) - sprite_sending_->getContentSize().height * (1 - sprite_head_icon_->getAnchorPoint().y);
                
                sprite_sending_->setPosition(posSpriteSending);
                this->addChild(sprite_sending_);
                
                sprite_sending_->setLocalZOrder(ZORDER_MESSAGE_STATUS);
                
                Animation* animation_sprite_sending = Animation::create();
                for (int i = 1; i < 11; ++i) {
                    std::string filename = "waitingview/round_" + tools::to_string(i) + ".png";
                    animation_sprite_sending->addSpriteFrameWithFile(filename);
                }
                animation_sprite_sending->setDelayPerUnit(0.1f);
                Animate* animate = Animate::create(animation_sprite_sending);
                
                sprite_sending_->runAction(RepeatForever::create(animate));
                
                break;
            }

            case msg::MessageStatus::Failed:
            {
                btn_resend_ = Button::create("login_res/chat_res/btn_resend_normal.png");
                cocos2d::Point posSpriteSending;
                posSpriteSending.x = image_chatmessage_cell_bg_->getPositionX() -  image_chatmessage_cell_bg_->getContentSize().width * image_chatmessage_cell_bg_->getAnchorPoint().x;
                posSpriteSending.x = posSpriteSending.x - btn_resend_->getContentSize().width * btn_resend_->getAnchorPoint().x - 5.0;
                posSpriteSending.y = image_chatmessage_cell_bg_->getPositionY() + image_chatmessage_cell_bg_->getContentSize().height * (1 - image_chatmessage_cell_bg_->getAnchorPoint().y) - btn_resend_->getContentSize().height * (1 - btn_resend_->getAnchorPoint().y);
                
                btn_resend_->setPosition(posSpriteSending);
                this->addChild(btn_resend_);
                btn_resend_->setLocalZOrder(ZORDER_MESSAGE_STATUS);
                btn_resend_->addClickEventListener(std::bind(&ChatMessageCell::click_resend_message, this, std::placeholders::_1));

                break;
            }
                
            default:
                break;
        }
        
        
    }
    
}

void ChatMessageCell::display_message_time()
{
    std::string text;
    std::string submit_time = chat_message_data_.subtime();
    if(!submit_time.empty()) {
        double time = tools::stod(submit_time);
        time /= 1000;
        text = tools::time_display(time);
    }
    text_message_time_->setString(text);
    text_message_time_->setLocalZOrder(ZORDER_MESSAGE_TIME);
    text_message_time_->setPosition(cocos2d::Point(listview_width_ / 2, image_chatmessage_cell_bg_->getPositionY() + image_chatmessage_cell_bg_->getContentSize().height * (1 - image_chatmessage_cell_bg_->getAnchorPoint().y) + text_message_time_->getContentSize().height * text_message_time_->getAnchorPoint().y ));
    if (!text_message_time_->isVisible())
    {
        text_message_time_->setVisible(true);
    }
}

#pragma mark click_event
void ChatMessageCell::click_head_icon(Ref * sender)
{
    cocos2d::log("click_head_icon");
    msg::UserDetailInfo userInfo;
    if (chat_message_data_.messageinfo().type() == msg::MessageType::CreateRoom)
    {
        userInfo = chat_message_data_.messageinfo().createroominfo().creater();
    }
    else
    {
        userInfo = chat_message_data_.userdetailinfo();
    }
    SceneManager::add_layer(LayerUserInfo::create(userInfo));
    
}

void ChatMessageCell::click_image_bg(Ref * sender)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    AM->stopAll();
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    if(SoundPlayerInterface::is_playing()) {
        SoundPlayerInterface::stop_play();
    }
#endif
    
    switch (chat_message_data_.messageinfo().type()) {
        case msg::MessageType::Voice: {
            std::string filename = chat_message_data_.messageinfo().voiceinfo().voicename();
            cocos2d::log("click_image_bg_filename:%s", filename.c_str());
            std::string wav_path = GDM->get_user_directory() + filename + ".wav";
            std::string amr_path = GDM->get_user_directory() + filename + ".amr";
            if(GDM->is_file_exist(wav_path)) {
                if (select_voice_ == filename)
                {
                    select_voice_ = "";
                    return;
                }else
                {
                    select_voice_ = filename;
                }
                cocos2d::log("click_image_bg_filename1.......:%s", wav_path.c_str());
                AM->playBackgroundMusic(wav_path.c_str());
            }
            else {
                if(GDM->is_file_exist(amr_path)) {
                    if (select_voice_ == filename)
                    {
                        select_voice_ = "";
                        return;
                    }else
                    {
                        select_voice_ = filename;
                    }
                    cocos2d::log("click_image_bg_filename2.......:%s", amr_path.c_str());
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
                    FM.get_recorder()->amrToWav(amr_path, wav_path);
                    AM->playBackgroundMusic(wav_path.c_str());
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
                    SoundPlayerInterface::play_sound(amr_path);
#endif
                }
                else {
                    FM.download_file(chat_message_data_.messageinfo().voiceinfo().voicename() + ".amr");
                    TipView::showAlertView(tools::local_string("downloading", "正在下载..."));
                }
            }
            
            break;
        }
        case msg::MessageType::CreateRoom: {
//            if(in_game_) {
//                if (PDM->get_room(data_->messageinfo().createroominfo().roomid()) != nullptr) {
//                    TipView::showAlertView(tools::local_string("already_in_room", "已在此房间"));
//                }
//                else {
//                    TipView* tip = TipView::showAlertView(tools::local_string("goto_new_room", "退出当前房间进入新房间"));
//                    tip->show_cancel(true);
//                    tip->set_tipview_close_callback([=](){
//                        PDM->leave_all_room();
//                        PDM->send_enter_room(data_->messageinfo().createroominfo().roomid());
//                    });
//                }
//            }
//            else {
//                PDM->send_enter_room(data_->messageinfo().createroominfo().roomid());
//            }
            
            PDM->send_enter_room(chat_message_data_.messageinfo().createroominfo().roomid());
            
            break;
        }
        default:
            break;
    }

}

void ChatMessageCell::click_resend_message(Ref * sender)
{
    CM->resend_chat_message_by_subtime(chat_message_data_.subtime());
}

void ChatMessageCell::handle_download_event(cmd_data_pointer data)
{
    auto s = std::dynamic_pointer_cast<cmd_data_string>(data);
    if (s->get_value() == image_head_file_ && !image_head_file_.empty())
    {
        init_head_sprite();
    }else if (chat_message_data_.messageinfo().type() == msg::MessageType::Picture)
    {
        bool is_mine = (chat_message_data_.has_userdetailinfo()) && (GDM->get_user_id() == chat_message_data_.userdetailinfo().userid());
        auto vec = tools::split(chat_message_data_.messageinfo().picinfo().picname(), "#");
        if (vec.size() > 0 && s->get_value() == vec[0])
        {
            if (!vec_emotions_.empty())
            {
                for (auto vecIter : vec_emotions_)
                {
                    for (auto emotionIter : vecIter)
                    {
                        if (emotionIter)
                        {
                            emotionIter->removeFromParent();
                        }
                    }
                }
                
                vec_emotions_.clear();
            }
            if(GDM->is_file_exist(vec[0])) {
  
                image_chatmessage_picture_ = ImageView::create(vec[0]);
                
                std::vector<Node*> vecNode;
                vecNode.push_back(image_chatmessage_picture_);
                vec_emotions_.push_back(vecNode);
                
                Size s(120, 120);
                Size contentSize = image_chatmessage_picture_->getContentSize();
                s.width = s.height * (contentSize.width/contentSize.height);
                if(contentSize.width > s.width || contentSize.height > s.height) {
                    image_chatmessage_picture_->ignoreContentAdaptWithSize(false);
                    image_chatmessage_picture_->setContentSize(s);
                }
                
                image_chatmessage_picture_->setAnchorPoint(Point(0.5, 0));
                this->addChild(image_chatmessage_picture_);
                image_chatmessage_picture_->setLocalZOrder(ZORDER_MESSAGE_TEXT);

                image_chatmessage_picture_->setTouchEnabled(true);
                image_chatmessage_picture_->addClickEventListener([=](Ref*){
                    auto imageview = ImageView::create(vec[0]);
                    auto scene = Director::getInstance()->getRunningScene();
                    auto& size = Director::getInstance()->getWinSize();
                    auto layout = Layout::create();
                    layout->setContentSize(size);
                    float scale = 1.0f;
                    if(imageview->getContentSize().width > size.width) {
                        scale = std::min(scale, size.width / imageview->getContentSize().width);
                    }
                    if(imageview->getContentSize().height > size.height) {
                        scale = std::min(scale, size.height / imageview->getContentSize().height);
                    }
                    imageview->setScale(scale);
                    LayerColor* layer = LayerColor::create(Color4B(240, 239, 245, 255), size.width, size.height);
                    layout->addChild(layer, -2);
                    layout->setTouchEnabled(true);
                    layout->addClickEventListener([=](Ref*){
                        layout->removeFromParent();
                    });
                    layout->addChild(imageview, 0);
                    imageview->setPosition(Point(size) * 0.5);
                    scene->addChild(layout, 10);
                });
            }
            else {
                //
                image_chatmessage_picture_ = ImageView::create("msg_pic.png");
                std::vector<Node*> vecNode;
                vecNode.push_back(image_chatmessage_picture_);
                vec_emotions_.push_back(vecNode);
                Size s(120, 120);
                Size contentSize = image_chatmessage_picture_->getContentSize();
                s.width = s.height * (contentSize.width/contentSize.height);
                if(contentSize.width > s.width || contentSize.height > s.height) {
                    image_chatmessage_picture_->ignoreContentAdaptWithSize(false);
                    image_chatmessage_picture_->setContentSize(s);
                }
                image_chatmessage_picture_->setAnchorPoint(Point(0, 0));
                this->addChild(image_chatmessage_picture_);
                image_chatmessage_picture_->setLocalZOrder(ZORDER_MESSAGE_TEXT);

                FM.download_file(vec[0]);
            }
            cocos2d::Point posEmotion;
            if(is_mine) {
                posEmotion.x = listview_width_ / 2 + MESSAGE_BG_WIDTH_MAX / 2 - MESSAGE_TEXT_BG_OFFSET_RIGHT - image_chatmessage_picture_->getContentSize().width * (1 - image_chatmessage_picture_->getAnchorPoint().x);
            }
            else {
                posEmotion.x = listview_width_ / 2 - MESSAGE_BG_WIDTH_MAX / 2 + MESSAGE_TEXT_BG_OFFSET_LEFT + image_chatmessage_picture_->getContentSize().width * image_chatmessage_picture_->getAnchorPoint().x;
            }
            posEmotion.y = image_chatmessage_picture_->getContentSize().height * image_chatmessage_picture_->getAnchorPoint().y + MESSAGE_TEXT_BG_OFFSET_BUTTOM + MESSAGE_CELL_OFFSET;
            
            image_chatmessage_picture_->setPosition(posEmotion);
        }
        
        return;
    }
}
