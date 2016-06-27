#include "ChatMessageCell.h"
#include "GameDataManager.h"
#include "file_utils.h"
#include "AudioManager.h"
#include "FileManager.h"
#include "VoiceRecorderInterface.h"
#include "TipView.h"
#include "utils.h"
#include "SceneManager.h"
#include "cmd_def.h"
#include "PokerDataManager.h"
#include "ShaderSprite.h"
#include "Room.h"
#include "msg.pb.h"
#include "LayerFriendDetails.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "SoundPlayerInterface.h"
#endif


ChatMessageCell* ChatMessageCell::select_cell = nullptr;

bool ChatMessageCell::init()
{
    if (!Widget::init()) {
        return false;
    }
    
    this->setAnchorPoint(Point::ZERO);
    
    root_ = nullptr;
    listview = nullptr;
    
    dispatch_.register_event(EVENT_DOWLOAD_FILE,
                             BIND_FUNC(this, ChatMessageCell::update_header_image));
    
    dispatch_.register_event(EVENT_UPLOAD_FILE_SUCCESS,BIND_FUNC(this, ChatMessageCell::handle_upload_succeed));
    
    return true;
}

void ChatMessageCell::update_header_image(cmd_data_pointer)
{
    this->update_data(data_, listview_width_, in_game_);
}

void ChatMessageCell::handle_upload_succeed(cmd_data_pointer data)
{
    
    auto recv = std::dynamic_pointer_cast<cmd_data_string>(data);
    auto mydata = data_;
    std::string HeadPicName = mydata->messageinfo().picinfo().picname();
    
    // by ronaldz.
    if(recv->get_value().find(HeadPicName+" "+"Success") ==0){  // success uploaded.
        
        if (!FM.is_file_in_upload_queue(HeadPicName)){
            if (m_pLayer){
                m_pLayer->removeAllChildrenWithCleanup(true);
                m_pLayer = nullptr;
            }
        }
        return;
        
    }else if(recv->get_value().find(HeadPicName) ==0) {  // failed.   // to do , ronaldz. resend. 
        return;
        /*Button* button_resend = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "button_resend"));
         button_resend->setVisible(true);
         button_resend->addClickEventListener(std::bind(&ChatMessageCell::click_button_resend, this, std::placeholders::_1));
         button_resend->setPositionX(image_bg->getPositionX()-image_bg->getContentSize().width - 35);
         */
        
    }else { // not this file.
        return;
        
    }
    
   
    
}

//void ChatMessageCell::reset()
//{
//    for(auto& iter : element_positions_) {
//        iter.first->setPosition(iter.second.first);
//        iter.first->setContentSize(iter.second.second);
//    }
//}


void ChatMessageCell::update_data(const std::shared_ptr<msg::ChatMessage>& data, const float listview_width, const bool in_game)
{
    if(in_game) {
        log("ChatMessageCell::update_data in game");
    }
    else {
        log("ChatMessageCell::update_data in game .......");
    }
    
    listview_width_ = listview_width;
    if(root_) {
        root_->removeFromParent();
        root_ = nullptr;
    }
    in_game_ = in_game;
    data_ = data;
    emotions_.clear();
    bool is_mine = (data->has_userdetailinfo()) && (GDM->get_user_id() == data->userdetailinfo().userid());
    data_ = data;
    std::string HeadPicName = data->userdetailinfo().picname();
    switch (data->messageinfo().type()) {
        case msg::MessageType::Text: {
            if(is_mine) {
                if (in_game) {
                    root_ = CSLoader::createNode("NodeChatCellMyTextInGame.csb");
                }
                else {
                    root_ = CSLoader::createNode("NodeChatCellMyText.csb");
                }
            }
            else {
                
                if (in_game) {
                    root_ = CSLoader::createNode("NodeChatCellOtherTextInGame.csb");
                }
                else {
                    root_ = CSLoader::createNode("NodeChatCellOtherText.csb");
                }
            }
            
            this->addChild(root_);
            
            auto text_nickname = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_nickname"));
            std::string user_nickname = data->userdetailinfo().nickname();
            text_nickname->setString(user_nickname);
            
            auto image_head = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_head"));
            
            if(GDM->is_file_exist(HeadPicName)) {
                image_head->loadTexture(HeadPicName);
                //image_head->setVisible(false);
                image_head->setOpacity(0);
                
                auto sprite_icon_ = ShaderSprite::create(HeadPicName, "image/head_mask.png");
                sprite_icon_->setPosition(image_head->getPosition());
                sprite_icon_->setScale(image_head->getContentSize().width / sprite_icon_->getContentSize().width,
                                       image_head->getContentSize().height / sprite_icon_->getContentSize().height);
                
                root_->addChild(sprite_icon_, image_head->getZOrder());
            }
            else {
                download_image_ = HeadPicName;
                FM.download_file(HeadPicName);
            }
            std::string str_receive_message = data->messageinfo().textinfo().text();
            //cocos2d::log("str_receive_message_1 : %s", str_receive_message.c_str());
            auto text_message = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_message"));
            text_message->setFontName("Thonburi");
            text_message->setString(str_receive_message);

            // by ronaldz. // we want some little font size.
            text_message->setFontSize(text_message->getFontSize()-6);
            
            auto render_size = text_message->getAutoRenderSize();

            auto size = text_message->getContentSize();
            
            //如果超过了单行， 我们就把文字栏变成两行。
            size.width = std::min(render_size.width, listview_width -160 - image_head->getContentSize().width);
            
            if(render_size.width > size.width) {
//                int line_count = ceilf(render_size.width / size.width);
//                size = Size(size.width, render_size.height * line_count);
                text_message->ignoreContentAdaptWithSize(true);
                text_message->setTextAreaSize(cocos2d::Size(listview_width -160 - image_head->getContentSize().width, 0));
                
                auto size_tmp = text_message->getContentSize();
                text_message->ignoreContentAdaptWithSize(false);
                text_message->setContentSize(size_tmp);
            }
            else {
                //size = render_size;
                text_message->setContentSize(size);
            }
            
//            text_message->ignoreContentAdaptWithSize(true);
//            text_message->setTextAreaSize(cocos2d::Size(listview_width -160 - image_head->getContentSize().width, 0));
//            
//            auto size_tmp = text_message->getContentSize();
//            text_message->ignoreContentAdaptWithSize(false);
//            text_message->setContentSize(size_tmp);

            //text_message->setContentSize(size);   // ronaldz. here to adjust to the content size.

            auto image_bg = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_bg"));
            
            Rect findrect = image_bg->getCapInsets();
            //cocos2d::log("findRect %f, %f, %f, %f", findrect.origin.x, findrect.origin.y,  findrect.size.width, findrect.size.height);
            
            image_bg->setContentSize(Size(std::max(image_bg->getVirtualRendererSize().width, text_message->getContentSize().width + 70),
                                          std::max(image_bg->getVirtualRendererSize().height, text_message->getContentSize().height + 40)));
            size_ = Size(image_bg->getContentSize().width + image_head->getContentSize().width,
                         std::max(image_bg->getContentSize().height, image_head->getContentSize().height));
            setContentSize(size_ + Size(0, 55));
            
            image_bg->setTouchEnabled(true);
            image_bg->addTouchCallback([this](Ref*, Touch* touch, Widget::TouchEventType event){
                if(listview) {
                    switch (event) {
                        case Widget::TouchEventType::BEGAN:
                            listview->onTouchBegan(touch, nullptr);
                            break;
                        case Widget::TouchEventType::MOVED:
                            listview->onTouchMoved(touch, nullptr);
                            break;
                        case Widget::TouchEventType::ENDED:
                            listview->onTouchEnded(touch, nullptr);
                            break;
                        case Widget::TouchEventType::CANCELED:
                            listview->onTouchCancelled(touch, nullptr);
                            break;
                        default:
                            break;
                    }
                }
            });
            
            image_head->setTouchEnabled(true);
            image_head->addClickEventListener(std::bind(&ChatMessageCell::click_sprite_icon, this, std::placeholders::_1));
            
            if(is_mine)
            {
                msg::MessageStatus status = data->status();
                switch (status) {
                    case msg::MessageStatus::Success:
                    {
                        
                    }
                        break;
                    case msg::MessageStatus::Sending:
                    {
                        Sprite * sprite_sending = dynamic_cast<Sprite*>(CSLoader::seekNodeByName(root_, "sprite_sending"));
                        sprite_sending->setVisible(true);
                        sprite_sending->setPositionX(image_bg->getPositionX()-image_bg->getContentSize().width - 35);
                        
                    }
                        break;
                    case msg::MessageStatus::Failed:
                    {
                        Button* button_resend = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "button_resend"));
                        button_resend->setVisible(true);
                        button_resend->addClickEventListener(std::bind(&ChatMessageCell::click_button_resend, this, std::placeholders::_1));
                        button_resend->setPositionX(image_bg->getPositionX()-image_bg->getContentSize().width - 35);
                        
                    }
                        break;
                        
                    default:
                        break;
                }
                
                
            }
            
            break;
        }
            
        case msg::MessageType::Voice: {
            if(is_mine) {
                if(in_game) {
                    root_ = CSLoader::createNode("NodeChatCellMyVoiceInGame.csb");
                }
                else {
                    root_ = CSLoader::createNode("NodeChatCellMyVoice.csb");
                }
            }
            else {
                if(in_game) {
                    root_ = CSLoader::createNode("NodeChatCellOtherVoiceInGame.csb");
                }
                else {
                    root_ = CSLoader::createNode("NodeChatCellOtherVoice.csb");
                }
            }
            
            this->addChild(root_);
            
            auto text_nickname = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_nickname"));
            std::string user_nickname = data->userdetailinfo().nickname();
            text_nickname->setString(user_nickname);
            
            auto image_head = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_head"));
            
            
            if(GDM->is_file_exist(HeadPicName)) {
                image_head->loadTexture(HeadPicName);
                //image_head->setVisible(false);
                image_head->setOpacity(0);
                auto sprite_icon_ = ShaderSprite::create(HeadPicName, "image/head_mask.png");
                sprite_icon_->setPosition(image_head->getPosition());
                sprite_icon_->setScale(image_head->getContentSize().width / sprite_icon_->getContentSize().width,
                                       image_head->getContentSize().height / sprite_icon_->getContentSize().height);

                root_->addChild(sprite_icon_, image_head->getZOrder());
            }
            else {
                download_image_ = HeadPicName;
                FM.download_file(HeadPicName);
            }
            auto image_bg = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_bg"));
            image_bg->addClickEventListener(std::bind(&ChatMessageCell::click_image_bg, this, std::placeholders::_1));
            size_ = Size(image_bg->getContentSize().width + image_head->getContentSize().width,
                         std::max(image_bg->getContentSize().height, image_head->getContentSize().height));
            
            image_bg->setTouchEnabled(true);
            image_bg->addTouchCallback([this](Ref*, Touch* touch, Widget::TouchEventType event){
                switch (event) {
                    case Widget::TouchEventType::BEGAN:
                        listview->onTouchBegan(touch, nullptr);
                        break;
                    case Widget::TouchEventType::MOVED:
                        listview->onTouchMoved(touch, nullptr);
                        break;
                    case Widget::TouchEventType::ENDED:
                        listview->onTouchEnded(touch, nullptr);
                        break;
                    case Widget::TouchEventType::CANCELED:
                        listview->onTouchCancelled(touch, nullptr);
                        break;
                    default:
                        break;
                }
            });
            
            setContentSize(size_ + Size(0, 40));
            
            image_head->setTouchEnabled(true);
            image_head->addClickEventListener(std::bind(&ChatMessageCell::click_sprite_icon, this, std::placeholders::_1));
            
            if(is_mine)
            {
                msg::MessageStatus status = data->status();
                switch (status) {
                    case msg::MessageStatus::Success:
                    {
                        
                    }
                        break;
                    case msg::MessageStatus::Sending:
                    {
                        Sprite * sprite_sending = dynamic_cast<Sprite*>(CSLoader::seekNodeByName(root_, "sprite_sending"));
                        sprite_sending->setVisible(true);

                        
                    }
                        break;
                    case msg::MessageStatus::Failed:
                    {
                        Button* button_resend = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "button_resend"));
                        button_resend->setVisible(true);
                        button_resend->addClickEventListener(std::bind(&ChatMessageCell::click_button_resend, this, std::placeholders::_1));
                        
                    }
                        break;
                        
                    default:
                        break;
                }
                
                
            }

            
            break;
        }
        case msg::MessageType::Picture: {
            
            if(is_mine) {
                root_ = CSLoader::createNode("NodeChatCellMyEmotion.csb");
            }
            else {
                root_ = CSLoader::createNode("NodeChatCellOtherEmotion.csb");
            }
            
            this->addChild(root_);
            
            auto text_nickname = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_nickname"));
            std::string user_nickname = data->userdetailinfo().nickname();
            text_nickname->setString(user_nickname);
            
            auto image_head = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_head"));
            if(in_game) {
                if (is_mine) {
                    image_head->setPosition(Point(listview_width - image_head->getContentSize().width/2 - 5, image_head->getPosition().y));
                    text_nickname->setPositionX(image_head->getPositionX() - image_head->getContentSize().width * image_head->getAnchorPoint().x - 25);
                    
                }else
                {
                    text_nickname->setPositionX(image_head->getPositionX() + image_head->getContentSize().width * image_head->getAnchorPoint().x + 25);
                }
                
            }else
            {
//                if (is_mine){
//                    text_nickname->setPositionX(image_head->getPositionX() - image_head->getContentSize().width * image_head->getAnchorPoint().x - 35);
//                }else
//                {
//                    text_nickname->setPositionX(image_head->getPositionX() + image_head->getContentSize().width * image_head->getAnchorPoint().x + 35);
//                }
            }
            
            if(GDM->is_file_exist(HeadPicName)) {
                image_head->loadTexture(HeadPicName);
//                image_head->setVisible(false);
                image_head->setOpacity(0);
                
                auto sprite_icon_ = ShaderSprite::create(HeadPicName, "image/head_mask.png");
                sprite_icon_->setPosition(image_head->getPosition());
                sprite_icon_->setScale(image_head->getContentSize().width / sprite_icon_->getContentSize().width,
                                       image_head->getContentSize().height / sprite_icon_->getContentSize().height);

                root_->addChild(sprite_icon_, image_head->getZOrder());
                
            }
            else {
                auto sprite_icon_ = ShaderSprite::create(HeadPicName, "image/head_mask.png");
                sprite_icon_->setPosition(image_head->getPosition());
                sprite_icon_->setScale(image_head->getContentSize().width / sprite_icon_->getContentSize().width,
                                       image_head->getContentSize().height / sprite_icon_->getContentSize().height);
                root_->addChild(sprite_icon_, image_head->getZOrder());
                download_image_ = HeadPicName;
                FM.download_file(HeadPicName);
            }
            
            std::vector<std::vector<Node*> > emotions;
            auto adjust_position = [&](Node *emotion) {
                if(emotions.empty()) {
                    if (in_game){
                        if(is_mine) {
                            emotion->setPosition(image_head->getPosition() - Point(image_head->getContentSize().width * image_head->getAnchorPoint().x + emotion->getContentSize().width * emotion->getAnchorPoint().x + 15, image_head->getContentSize().width * image_head->getAnchorPoint().y));
                        }
                        else {
                            emotion->setPosition(image_head->getPosition() + Point(image_head->getContentSize().width * image_head->getAnchorPoint().x + emotion->getContentSize().width * emotion->getAnchorPoint().x + 15, -image_head->getContentSize().width * image_head->getAnchorPoint().y));
                        }
                        emotion->setPositionY(emotion->getPositionY() + 30);
                    }else
                    {
                        if(is_mine) {
                            emotion->setPosition(image_head->getPosition() - Point(image_head->getContentSize().width * image_head->getAnchorPoint().x + emotion->getContentSize().width * emotion->getAnchorPoint().x + 25, image_head->getContentSize().width * image_head->getAnchorPoint().y));
                        }
                        else {
                            emotion->setPosition(image_head->getPosition() + Point(image_head->getContentSize().width * image_head->getAnchorPoint().x + emotion->getContentSize().width * emotion->getAnchorPoint().x + 25, -image_head->getContentSize().width * image_head->getAnchorPoint().y));
                        }
                        emotion->setPositionY(emotion->getPositionY() + 35);
                    }
                    std::vector<Node*> vec;
                    vec.push_back(emotion);
                    emotions.push_back(vec);
                    
                    cocos2d::log("emotion empty %f, %f", emotion->getPositionX(), emotion->getPositionY());
                }
                else {
                    Node* sentry= nullptr;
                    bool out_of_range = false;
                    if(is_mine) {
                        sentry = emotions.back().front();
                        
                        if (emotions.size() == 1)
                        {
                            if(sentry->getPosition().x - sentry->getContentSize().width * sentry->getAnchorPoint().x <= emotion->getContentSize().width) {
                                out_of_range = true;
                            }
                        }else
                        {
                            if(sentry->getPosition().x + sentry->getContentSize().width * sentry->getAnchorPoint().x + emotion->getContentSize().width >= image_head->getPositionX() - image_head->getContentSize().width * image_head->getAnchorPoint().x - 10) {
                                out_of_range = true;
                            }
                        }
                    }
                    else {
                        sentry = emotions.back().back();
                        if(sentry->getPosition().x + sentry->getContentSize().width * sentry->getAnchorPoint().x + emotion->getContentSize().width >= listview_width) {
                            out_of_range = true;
                        }
                    }
                    if(!out_of_range) {
                        auto& back = emotions.back();
                        
                        if(emotions.size() == 1) {
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
                        
                        for (size_t i = 0; i < emotions.size(); i++) {
                            auto& one_line_emotions = emotions.at(i);
                            for (auto& one : one_line_emotions) {
                                one->setPosition(one->getPosition() + Point(0, one->getContentSize().height));
                            }
                        }
                        
                        auto& back = emotions.back();
                        
                        std::vector<Node*> trail;
                        trail.push_back(emotion);
                        emotion->setPosition(back.front()->getPosition() - Point(0, back.front()->getContentSize().height));
                        emotions.push_back(trail);
                        
                        cocos2d::log("emotion out_of_range %f, %f", emotion->getPositionX(), emotion->getPositionY());
                    }
                }
            };
            
            
            auto vec = tools::split(data->messageinfo().picinfo().picname(), "#");
            for (auto& img : vec) {
                std::string path = FileUtils::getInstance()->fullPathForFilename("emotion/" + img);
                if(!path.empty()) {
                    Sprite* emotion = Sprite::create(path);
                    emotion->setAnchorPoint(Point(0.5, 0));
                    root_->addChild(emotion);
                    adjust_position(emotion);
                    
                    cocos2d::log("emotion split %f, %f", emotion->getPositionX(), emotion->getPositionY());
                }
                else {
                    
                    int x, y; // pos of the image.
                    ImageView* emotion;
                    if(GDM->is_file_exist(img)) {
                        emotion =  ImageView::create(img);
                        Size s(120, 120);
                        Size contentSize = emotion->getContentSize();
                        s.height = s.width * (contentSize.height/contentSize.width);
                        if(contentSize.width > s.width || contentSize.height > s.height) {
                            emotion->ignoreContentAdaptWithSize(false);
                            emotion->setContentSize(s);
                        }
                        
                        emotion->setAnchorPoint(Point(0.5, 0));
                        root_->addChild(emotion);
                        adjust_position(emotion);
                        x = emotion->getPositionX();
                        y = emotion->getPositionY();
                        emotion->setTouchEnabled(true);
                        emotion->addClickEventListener([=](Ref*){
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
                        emotion = ImageView::create("msg_pic.png");
                        Size s(120, 120);
                        Size contentSize = emotion->getContentSize();
//                        s.height = s.width * (contentSize.height/contentSize.width);
                        if(contentSize.width > s.width || contentSize.height > s.height) {
                            emotion->ignoreContentAdaptWithSize(false);
                            emotion->setContentSize(s);
                        }
                        emotion->setAnchorPoint(Point(0, 0));
                        root_->addChild(emotion);
                        adjust_position(emotion);
                        x = emotion->getPositionX();
                        y = emotion->getPositionY();
                        FM.download_file(img);
                    }
                    
                    // ronaldz. 如果这里发现上传队列中有这个图片， 就需要告知用户。
                    if (FM.is_file_in_upload_queue(img)){
                        if (m_pLayer != nullptr){
                            m_pLayer->removeAllChildrenWithCleanup(true);
                            m_pLayer = NULL;
                        }
                        m_pLayer = Layer::create();
                        this->addChild(m_pLayer,255);
                        LayerColor *black = LayerColor::create(Color4B(100, 100, 100, 100), 120, 120);
                        //black->setAnchorPoint(Point(0.5,0));
                        black->setPositionX(emotion->getPositionX()-emotion->getContentSize().width/2);
                        black->setPositionY(emotion->getPositionY());
                        black->setContentSize(emotion->getContentSize());
                        
                        //(image_head->getPosition() - Point(image_head->getContentSize()/2));
                        m_pLayer->addChild(black);
                        Label *label = Label::createWithSystemFont("正在上传","Arial", 18);
                        label->setColor(cocos2d::Color3B::WHITE);
                        label->setPosition(Point(black->getContentSize()/2));
                        black->addChild(label);
                        
                    }
                    
                }
            }
            
            if(!emotions.empty()) {
                
                size_ = Size(listview_width, std::max(image_head->getContentSize().height, emotions.size() * emotions.front().front()->getContentSize().height + 30));
                
                log("size.width:%f, %f", size_.width, size_.height);
                setContentSize(size_ + Size(0, 50));
                
                
            }
            listview->refreshView();
            listview->jumpToBottom();
            
            
            image_head->setTouchEnabled(true);
            image_head->addClickEventListener(std::bind(&ChatMessageCell::click_sprite_icon, this, std::placeholders::_1));
            
            if(is_mine)
            {
                msg::MessageStatus status = data->status();
                switch (status) {
                    case msg::MessageStatus::Success:
                    {
                        
                    }
                        break;
                    case msg::MessageStatus::Sending:
                    {
                        Sprite * sprite_sending = dynamic_cast<Sprite*>(CSLoader::seekNodeByName(root_, "sprite_sending"));
                        sprite_sending->setVisible(true);
                        
                    }
                        break;
                    case msg::MessageStatus::Failed:
                    {
                        Button* button_resend = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "button_resend"));
                        button_resend->setVisible(true);
                        button_resend->addClickEventListener(std::bind(&ChatMessageCell::click_button_resend, this, std::placeholders::_1));
                        
                    }
                        break;
                        
                    default:
                        break;
                }
                
                
            }
            
            
            break;
        }
        case msg::MessageType::GroupNotice:  {
            root_ = CSLoader::createNode("NodeChatCellAddOrQuitGroup.csb");
            this->addChild(root_);
            auto text_message = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_message"));
            text_message->setPosition(Point(listview_width/2, text_message->getPosition().y));
            text_message->setFontName("Thonburi");
            text_message->setString(data->messageinfo().groupnoticeinfo().text());
            
            auto text_system_time = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_system_time"));
            text_system_time->setFontName("Thonburi");
            text_system_time->setPosition(Point(listview_width/2, text_system_time->getPosition().y));
            std::string text;
            std::string submit_time = data->subtime();
            if(!submit_time.empty()) {
                double time = tools::stod(submit_time);
                time /= 1000;
                text = tools::time_display(time);
            }
            text_system_time->setString(text);
            text_system_time->setPosition(Point(listview_width/2, text_system_time->getPosition().y));
            auto image_bg = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_bg"));
            image_bg->setContentSize(Size(std::max(text_message->getAutoRenderSize().width, image_bg->getContentSize().width),
                                          image_bg->getContentSize().height));
            image_bg->setPosition(Point(listview_width/2, image_bg->getPosition().y));
            image_bg->addClickEventListener(std::bind(&ChatMessageCell::click_image_bg, this, std::placeholders::_1));
            
            image_bg->setTouchEnabled(true);
            image_bg->addTouchCallback([this](Ref*, Touch* touch, Widget::TouchEventType event){
                switch (event) {
                    case Widget::TouchEventType::BEGAN:
                        listview->onTouchBegan(touch, nullptr);
                        break;
                    case Widget::TouchEventType::MOVED:
                        listview->onTouchMoved(touch, nullptr);
                        break;
                    case Widget::TouchEventType::ENDED:
                        listview->onTouchEnded(touch, nullptr);
                        break;
                    case Widget::TouchEventType::CANCELED:
                        listview->onTouchCancelled(touch, nullptr);
                        break;
                    default:
                        break;
                }
            });
            
            auto image_system_time = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_system_time"));
            image_system_time->setContentSize(Size(std::min(listview_width, image_system_time->getContentSize().width),
                                                   image_system_time->getContentSize().height));
            image_system_time->setPosition(Point(listview_width/2, image_system_time->getPosition().y));
            
            text_message->setFontSize(text_message->getFontSize() + 6);
            text_system_time->setFontSize(text_system_time->getFontSize() + 6);
            
            size_ = Size(listview_width, 150);
            setContentSize(size_);
            
            break;
        }
        case msg::MessageType::CreateRoom:{
            root_ = CSLoader::createNode("NodeChatCellCreateRoom.csb");
            this->addChild(root_);
            
            // 时间
            auto text_system_time = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_system_time"));
            text_system_time->setFontName("Thonburi");
            text_system_time->setPosition(Point(listview_width/2, text_system_time->getPosition().y));
            std::string text;
            std::string submit_time = data->subtime();
            if(!submit_time.empty()) {
                double time = tools::stod(submit_time);
                time /= 1000;
                text = tools::time_display(time);
            }
            text_system_time->setString(text);
            //text_system_time->setColor(ccBLACK);
            text_system_time->setPosition(Point(listview_width/2, text_system_time->getPosition().y));
            
            auto image_system_time = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_system_time"));
            image_system_time->setContentSize(Size(std::min(listview_width, image_system_time->getContentSize().width),
                                                   image_system_time->getContentSize().height));
            image_system_time->setPosition(Point(listview_width/2, image_system_time->getPosition().y));
            
            auto text_message_into = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_message_into"));
            text_message_into->setString(tools::local_string("click_into_room", "点击加入"));
            text_message_into->setPosition(Point(listview_width/2, text_message_into->getPosition().y));
            
            text_system_time->setFontSize(text_system_time->getFontSize() + 4);
            text_message_into->setFontSize(text_message_into->getFontSize() + 4);
            // end of 时间
            
            
            
            auto text_message = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_message"));
            text_message->setPosition(Point(listview_width/2, text_message->getPosition().y));
            text_message->setFontName("Thonburi");
            
            text_message->setFontSize(text_message->getFontSize() + 4);
            text_message->setString(data->messageinfo().createroominfo().text());
            
            auto str= text_message->getString();
            
            auto rendersize = text_message->getAutoRenderSize();
            auto size = text_message->getContentSize();
            size.width =  std::max(rendersize.width, size.width);
            text_message->setContentSize(size);
            
            
            
            auto image_bg = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_bg"));
            image_bg->setContentSize(Size(std::min(listview_width, std::max(size.width + 50, image_bg->getContentSize().width)),image_bg->getContentSize().height));
            image_bg->setPosition(Point(listview_width/2, image_bg->getPosition().y));
            image_bg->addClickEventListener(std::bind(&ChatMessageCell::click_image_bg, this, std::placeholders::_1));
            
            //image_bg->setVisible(false);
            
            image_bg->setTouchEnabled(true);
            image_bg->addTouchCallback([this](Ref*, Touch* touch, Widget::TouchEventType event){
                if(listview) {
                    switch (event) {
                        case Widget::TouchEventType::BEGAN:
                            listview->onTouchBegan(touch, nullptr);
                            break;
                        case Widget::TouchEventType::MOVED:
                            listview->onTouchMoved(touch, nullptr);
                            break;
                        case Widget::TouchEventType::ENDED:
                            listview->onTouchEnded(touch, nullptr);
                            break;
                        case Widget::TouchEventType::CANCELED:
                            listview->onTouchCancelled(touch, nullptr);
                            break;
                        default:
                            break;
                    }
                }
            });
            
            
            
            size_ = Size(listview_width, 150);
            setContentSize(size_);
            break;
        }
        default:
            break;
    }
    
    this->setTouchEnabled(true);
    this->addTouchCallback([this](Ref*, Touch* touch, Widget::TouchEventType event){
        if(listview) {
            switch (event) {
                case Widget::TouchEventType::BEGAN:
                    listview->onTouchBegan(touch, nullptr);
                    break;
                case Widget::TouchEventType::MOVED:
                    listview->onTouchMoved(touch, nullptr);
                    break;
                case Widget::TouchEventType::ENDED:
                    listview->onTouchEnded(touch, nullptr);
                    break;
                case Widget::TouchEventType::CANCELED:
                    listview->onTouchCancelled(touch, nullptr);
                    break;
                default:
                    break;
            }
        }
    });
}

void ChatMessageCell::click_image_bg(Ref* sender)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    AM->stopAll();
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    if(SoundPlayerInterface::is_playing()) {
        SoundPlayerInterface::stop_play();
    }
#endif
    
    if(select_cell == this) {
        select_cell = nullptr;
        return;
    }
    select_cell = this;
    switch (data_->messageinfo().type()) {
        case msg::MessageType::Voice: {
            std::string filename = data_->messageinfo().voiceinfo().voicename();
            cocos2d::log("click_image_bg_filename:%s", filename.c_str());
            std::string wav_path = GDM->get_user_directory() + filename + ".wav";
            std::string amr_path = GDM->get_user_directory() + filename + ".amr";
            if(GDM->is_file_exist(wav_path)) {
                cocos2d::log("click_image_bg_filename1.......:%s", wav_path.c_str());
                AM->playBackgroundMusic(wav_path.c_str());
            }
            else {
                if(GDM->is_file_exist(amr_path)) {
                    cocos2d::log("click_image_bg_filename2.......:%s", amr_path.c_str());
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
                    FM.get_recorder()->amrToWav(amr_path, wav_path);
                    AM->playBackgroundMusic(wav_path.c_str());
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
                    SoundPlayerInterface::play_sound(amr_path);
#endif
                }
                else {
                    FM.download_file(data_->messageinfo().voiceinfo().voicename() + ".amr");
                    TipView::showAlertView(tools::local_string("downloading", "正在下载..."));
                }
            }
            
            break;
        }
        case msg::MessageType::CreateRoom: {
            if(in_game_) {
                if (PDM->get_room(data_->messageinfo().createroominfo().roomid()) != nullptr) {
                    TipView::showAlertView(tools::local_string("already_in_room", "已在此房间"));
                }
                else {
                    TipView* tip = TipView::showAlertView(tools::local_string("goto_new_room", "退出当前房间进入新房间"));
                    tip->show_cancel(true);
                    tip->set_tipview_close_callback([=](){
                        PDM->leave_all_room();
                        PDM->send_enter_room(data_->messageinfo().createroominfo().roomid());
                    });
                }
            }
            else {
                PDM->send_enter_room(data_->messageinfo().createroominfo().roomid());
            }
            
            break;
        }
        default:
            break;
    }
}

void ChatMessageCell::click_sprite_icon(Ref* sender)
{
    GDM->m_isbyreachingfriend = false;
    if (is_not_touch_)
    {
        return;
    }
    
    uint32_t userid = data_->userdetailinfo().userid();
    std::string username = data_->userdetailinfo().nickname();
    std::string head_pic_name = data_->userdetailinfo().picname();
    uint32_t user_status = 0;
 

    
    LayerFriendDetails* glc = LayerFriendDetails::create();
    glc->set_show_dir(LAYER_SHOW_DIRECTION::RIGHT_TO_LEFT);
    BaseLayer* parent = static_cast<BaseLayer*>(this->getParent()->getParent()->getParent()->getParent());
    parent->getParent()->addChild(glc, 2);
//    glc->update_property(new_friend_data);
//    if (GDM->get_my_friends().find(new_friend_data->get_userid()) != GDM->get_my_friends().end())
//    {
//        glc->update_property(GDM->get_my_friends().find(userid)->second);
//        glc->update_type(3);
//    }else{
//        glc->update_type(1);
//    }
}
void ChatMessageCell::click_button_resend(Ref* sender)
{
    auto tip = TipView::showAlertView("确定要重发这条消息吗？");
    tip->show_cancel(true);
    tip->set_tipview_close_callback([this](){
        if (data_) {
            GDM->resend_chat_message(data_);
        }
        
    });
}