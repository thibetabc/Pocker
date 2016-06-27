//
//  LayerChatNew.cpp
//  pokerclient
//
//  Created by zhongyukeji on 16/4/11.
//
//

#include "LayerChat.h"

#include "GameTools.h"
#include "utils.h"
#include "VoiceRecorderInterface.h"
#include "NET_CMD.h"

#include "GameDataManager.h"
#include "AudioManager.h"
#include "FileManager.h"
#include "ChatMessageManager.h"
#include "ProcessorManager.h"

#include "ChatMessageCell.h"
#include "RecordAnimationLayer.h"
#include "LayerEmotion.h"
#include "TipView.h"
#include "LayerChoosePhoto.h"
#include "LayerScoreList.h"
#include "LayerChooseGameRoom.h"
#include "LayerGroupSetting.h"
#include "LayerUserInfo.h"
#include "Toast.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "SoundPlayerInterface.h"
#endif

#define ANIMATION_LAYER_TAG 1

#define ZORDER_BG 0
#define ZORDER_TABLEVIEW 1
#define ZORDER_BUTTON 2
#define ZORDER_CHAT_BAR_BG 3
#define ZORDER_CHAT_INPUT_BG 4
#define ZORDER_CHAT_INPUT 5
#define ZORDER_CHAT_BUTTON 6
#define ZORDER_EMOTION 7
#define ZORDER_RECORD_ANIMATION 8
#define ZORDER_NEW_LAYER 9

LayerChat* LayerChat::createByGroup(const msg::GroupInfo & group_info)
{
    LayerChat* result = new LayerChat;
    if(result->init()) {
        result->autorelease();
        result->update_chat_status_by_group(group_info);
        return result;
    }
    return nullptr;
}

LayerChat* LayerChat::createByFriend(const msg::UserDetailInfo & user_info)
{
    LayerChat* result = new LayerChat;
    if(result->init()) {
        result->autorelease();
        result->update_chat_status_by_friend(user_info);
        return result;
    }
    return nullptr;
}

LayerChat* LayerChat::createByGroup(const uint32_t group_id)
{
    LayerChat* result = new LayerChat;
    if(result->init()) {
        result->autorelease();
        result->update_chat_status_by_group(group_id);
        return result;
    }
    return nullptr;
}
LayerChat* LayerChat::createByFriend(const uint32_t friend_id)
{
    LayerChat* result = new LayerChat;
    if(result->init()) {
        result->autorelease();
        result->update_chat_status_by_friend(friend_id);
        return result;
    }
    return nullptr;
}

LayerChat* LayerChat::createByGroup()
{
    LayerChat* result = new LayerChat;
    if(result->init()) {
        result->autorelease();
        result->update_chat_status_by_group();
        return result;
    }
    return nullptr;
}

LayerChat* LayerChat::createByFriend()
{
    LayerChat* result = new LayerChat;
    if(result->init()) {
        result->autorelease();
        result->update_chat_status_by_friend();
        return result;
    }
    return nullptr;
}


bool LayerChat::init()
{
    if(!BaseLayer::init()) {
        return false;
    }
 
    root_ = CSLoader::createNode("LayerChat.csb");
    this->addChild(root_);
    
    group_id_ = 0;
    friend_id_ = 0;
    
    chat_status_ = CHAT_STATUS::NONE;
    
    btn_back_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_back"));
    btn_back_->addClickEventListener(std::bind(&LayerChat::click_btn_back, this, std::placeholders::_1));
    btn_back_->setLocalZOrder(ZORDER_BUTTON);

    text_title_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_title"));
    text_title_->addClickEventListener(std::bind(&LayerChat::click_btn_chat_setting, this, std::placeholders::_1));
    
    text_title_->setLocalZOrder(ZORDER_BUTTON);
    
    text_chat_status_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_chat_status"));
    text_chat_status_->setLocalZOrder(ZORDER_CHAT_BUTTON);
    text_chat_status_->setVisible(false);
    text_chat_status_->setString(tools::local_string("clud_mute","目前群禁言，只有群主和管理能发言"));
    
    image_layer_chat_bg_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_layer_chat_bg"));
    image_layer_chat_bg_->setLocalZOrder(ZORDER_BG);
    image_chat_bar_bg_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_chat_bar_bg"));
    image_chat_bar_bg_->setLocalZOrder(ZORDER_CHAT_BAR_BG);
    
    btn_voice_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_voice"));
    btn_voice_->addClickEventListener(std::bind(&LayerChat::click_btn_voice, this, std::placeholders::_1));
    btn_voice_->setLocalZOrder(ZORDER_CHAT_BUTTON);

    btn_keyboard_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_keyboard"));
    btn_keyboard_->addClickEventListener(std::bind(&LayerChat::click_btn_keyboard, this, std::placeholders::_1));
    btn_keyboard_->setLocalZOrder(ZORDER_CHAT_BUTTON);
    
    btn_add_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_add"));
    btn_add_->addClickEventListener(std::bind(&LayerChat::click_btn_add, this, std::placeholders::_1));
    btn_add_->setLocalZOrder(ZORDER_CHAT_BUTTON);
    
    btn_chat_setting_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_chat_setting"));
    btn_chat_setting_->addClickEventListener(std::bind(&LayerChat::click_btn_chat_setting, this, std::placeholders::_1));
    btn_chat_setting_->setLocalZOrder(ZORDER_BUTTON);
    
    btn_chat_emotion_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_chat_emotion"));
    btn_chat_emotion_->addClickEventListener(std::bind(&LayerChat::click_btn_chat_emotion, this, std::placeholders::_1));
    btn_chat_emotion_->setLocalZOrder(ZORDER_CHAT_BUTTON);

    btn_group_match_info_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_group_match_info"));
    btn_group_match_info_->addClickEventListener(std::bind(&LayerChat::click_btn_group_match_info,this,std::placeholders::_1));
    btn_group_match_info_->setLocalZOrder(ZORDER_BUTTON);
    
    btn_record_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_record"));
    btn_record_->addTouchCallback(std::bind(&LayerChat::touch_btn_record, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    btn_record_->setLocalZOrder(ZORDER_CHAT_BUTTON);

    image_input_bg_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_input_bg"));
    image_input_bg_->setLocalZOrder(ZORDER_CHAT_INPUT_BG);
    
    tf_input_= dynamic_cast<TextField*>(CSLoader::seekNodeByName(root_, "tf_input"));
    
    editbox_input_ = GameTools::createEditBoxFromTextField(this, tf_input_);
    editbox_input_->setReturnType(EditBox::KeyboardReturnType::SEND);
    editbox_input_->setInputMode(EditBox::InputMode::SINGLE_LINE);
    editbox_input_->setDelegate(this);
    editbox_input_->setMaxLength(250);
    //editbox_input_->setPlaceholderFontColor(cocos2d::Color3B(151, 151, 151));
    editbox_input_->setFontColor(cocos2d::Color3B(255, 255, 255));
    editbox_input_->addClickEventListener([this](Ref*){
        isTouchLayerout_ = false;
    });
    editbox_input_->setLocalZOrder(ZORDER_CHAT_INPUT);
    tf_input_->removeFromParentAndCleanup(true);
    
    btn_play_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_play"));
    btn_play_->addClickEventListener(std::bind(&LayerChat::click_btn_play, this, std::placeholders::_1));
    btn_play_->setLocalZOrder(ZORDER_BUTTON);
    
    image_play_count_bg_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_play_count_bg"));
    image_play_count_bg_->setVisible(false);
    text_play_count_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_play_count"));
    
    listview_ = dynamic_cast<ListView*>(CSLoader::seekNodeByName(root_, "listview"));
    tableview_ = TableView::create(this, listview_->getContentSize());
    tableview_->setAnchorPoint(listview_->getAnchorPoint());
    tableview_->setContentOffset(Vec2(0, 0));
    tableview_->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);

    tableview_->setPosition(listview_->getPosition());
    tableview_->setDelegate(this);
    
//    auto layoutView = Layout::create();
//    layoutView->setContentSize(tableview_->getContentSize());
//    layoutView->setTouchEnabled(true);
//    layoutView->setAnchorPoint(tableview_->getAnchorPoint());
//    layoutView->setSwallowTouches(true);
//    layoutView->setPosition(tableview_->getPosition());
//    layoutView->setClippingEnabled(true);
//    this->addChild(layoutView, ZORDER_TABLEVIEW);
//    layoutView->addChild(tableview_);
    
    this->addChild(tableview_, ZORDER_TABLEVIEW);
    listview_->removeFromParent();
    
    
//    tableview_->touchBeganCallback_ = std::bind(&LayerChat::ChatViewOnTouchBegan, this, std::placeholders::_1, std::placeholders::_2);
//    tableview_->touchMovedCallback_ = std::bind(&LayerChat::ChatViewOnTouchMoved, this, std::placeholders::_1, std::placeholders::_2);
//    tableview_->touchEndedCallback_ = std::bind(&LayerChat::ChatViewOnTouchEnded, this, std::placeholders::_1, std::placeholders::_2);
//    tableview_->touchCancelledCallback_ = std::bind(&LayerChat::ChatViewOnTouchCancelled, this, std::placeholders::_1, std::placeholders::_2);
  
    
    
    auto& size = Director::getInstance()->getWinSize();
    auto layout = Layout::create();
    layout->setContentSize(size);
    layout->setTouchEnabled(true);
    layout->setAnchorPoint(Point::ZERO);
    layout->setSwallowTouches(false);
    layout->addClickEventListener([this](Ref*){
        isTouchLayerout_ = true;
        GameTools::editBoxLoseInputFocus();
    });
    
    this->addChild(layout, 10);
    
    node_about_textfileds_.insert(std::make_pair(btn_voice_, btn_voice_->getPosition()));
    node_about_textfileds_.insert(std::make_pair(btn_chat_emotion_, btn_chat_emotion_->getPosition()));
    node_about_textfileds_.insert(std::make_pair(editbox_input_, editbox_input_->getPosition()));
    node_about_textfileds_.insert(std::make_pair(image_input_bg_, image_input_bg_->getPosition()));
    node_about_textfileds_.insert(std::make_pair(tableview_, tableview_->getPosition()));
    node_about_textfileds_.insert(std::make_pair(btn_keyboard_, btn_keyboard_->getPosition()));
    node_about_textfileds_.insert(std::make_pair(btn_record_, btn_record_->getPosition()));
    node_about_textfileds_.insert(std::make_pair(btn_add_, btn_add_->getPosition()));

    click_btn_keyboard(nullptr);
    GameTools::editBoxLoseInputFocus();

    //没有获取到信息之前功能键无效
    has_info_ = false;
    
    register_events();
    
    return true;
}

void LayerChat::init_test()
{
    double t = tools::date_double() * 1000;
    char time_str[1024];
    sprintf(time_str, "%f", t);
    
    // by lvzhangle
    msg::ChatMessage chatmessage1;
    
    chatmessage1.mutable_userdetailinfo()->set_userid(GDM->get_user_id());
    chatmessage1.mutable_userdetailinfo()->set_nickname("天作码农");
    chatmessage1.mutable_userdetailinfo()->set_picname("login_res/mark.png");
    chatmessage1.mutable_messageinfo()->mutable_textinfo()->set_text("协议我已经拟好了，来签字吧!");
    chatmessage1.mutable_messageinfo()->set_type(msg::MessageType::Text);
    chatmessage1.set_subtime(time_str);
    
    chat_message_datas_.push_back(chatmessage1);
    
    
    msg::ChatMessage chatmessage2;
    
    chatmessage2.mutable_userdetailinfo()->set_userid(111111111);
    chatmessage2.mutable_userdetailinfo()->set_nickname("路人甲");
    chatmessage2.mutable_userdetailinfo()->set_picname("login_res/mark.png");
    chatmessage2.mutable_messageinfo()->mutable_textinfo()->set_text("柳宗元有不少记述当时失意者的1111文章，他这是借此抒发心志，表达他对这个黑暗社会的不满和控诉。《覃季子墓铭》写一个穷困一生的读书人。覃季子特别喜欢读书，虽家境贫穷，但为人耿直，气节不俗，从不接受别人的施舍。他精通经史，熟读司马迁《史记》和班固的《汉书》，上下纵横贯通。他集数十家之说，作书《覃子史纂》。覃季子学识渊博，不仅精通儒、墨、名、法，还涉略动植物学，凡是有益于世人的东西，他都收录在自己的书里。可他不喜欢作官取士这些事。他因著书闻名，曾被人表荐做了太子校书的小官，后来不幸死于永州。临死时，他慨叹说：是希望出名一生穷困呢，还是不求闻名只求富有呢？是持正耿直一生不顺利，还是糊里糊涂、趋炎附势呢？可见自古做人处世的艰难。覃季子死后若干年，柳宗宗元遭贬来到永州，他感伤覃季子书文不能大行于世，于是为他写墓志铭，想让他的事传留下去。柳宗元在墓志铭里说：覃季子为了名望就得甘守穷困孤独，而想富有就得忍受耻辱啊！这其实表达的是柳宗元自己的人生价值取向和不俯仰时俗的心态。");
    chatmessage2.mutable_messageinfo()->set_type(msg::MessageType::Text);
    chatmessage2.set_subtime(time_str);
    chat_message_datas_.push_back(chatmessage2);
    
    
    
    msg::ChatMessage chatmessage3;
    
    chatmessage3.mutable_userdetailinfo()->set_userid(2222222222);
    chatmessage3.mutable_userdetailinfo()->set_nickname("路人乙");
    chatmessage3.mutable_userdetailinfo()->set_picname("login_res/mark.png");
    chatmessage3.mutable_messageinfo()->mutable_textinfo()->set_text("The Chinese military aircraft landed on Yongshu Reef, also known as Fiery Cross Reef, to give emergency assistance to three severely ill civilian workers, said Lu Kang, spokesman with China's Ministry of Foreign Affairs. The three construction workers were airlifted Sunday to a hospital on Hainan island, according to Xinhua, the Chinese state-run news agency.!");
    chatmessage3.mutable_messageinfo()->set_type(msg::MessageType::Text);
    chatmessage3.set_subtime(time_str);
    chat_message_datas_.push_back(chatmessage3);
    
    
    msg::ChatMessage chatmessage4;
    chatmessage4.mutable_userdetailinfo()->set_userid(GDM->get_user_id());
    chatmessage4.mutable_userdetailinfo()->set_nickname("天作码农");
    chatmessage4.mutable_userdetailinfo()->set_picname("login_res/mark.png");
    chatmessage4.mutable_messageinfo()->mutable_voiceinfo()->set_voicename("voice.amr");
    chatmessage4.mutable_messageinfo()->mutable_voiceinfo()->set_voicetime(5);
    chatmessage4.mutable_messageinfo()->set_type(msg::MessageType::Voice);
    chatmessage4.set_subtime(time_str);
    chat_message_datas_.push_back(chatmessage4);
    
    msg::ChatMessage chatmessage5;
    chatmessage5.mutable_userdetailinfo()->set_userid(111123123);
    chatmessage5.mutable_userdetailinfo()->set_nickname("路人丙");
    chatmessage5.mutable_userdetailinfo()->set_picname("login_res/mark.png");
    chatmessage5.mutable_messageinfo()->mutable_voiceinfo()->set_voicename("voice.amr");
    chatmessage5.mutable_messageinfo()->mutable_voiceinfo()->set_voicetime(5);
    chatmessage5.mutable_messageinfo()->set_type(msg::MessageType::Voice);
    chatmessage5.set_subtime(time_str);
    chat_message_datas_.push_back(chatmessage5);
    
    msg::ChatMessage chatmessage6;
    chatmessage6.mutable_userdetailinfo()->set_userid(GDM->get_user_id());
    chatmessage6.mutable_userdetailinfo()->set_nickname("天作码农");
    chatmessage6.mutable_userdetailinfo()->set_picname("login_res/mark.png");
    chatmessage6.mutable_messageinfo()->mutable_voiceinfo()->set_voicename("voice.amr");
    chatmessage6.mutable_messageinfo()->mutable_voiceinfo()->set_voicetime(5);
    chatmessage6.mutable_messageinfo()->set_type(msg::MessageType::Voice);
    chatmessage6.set_subtime(time_str);
    chat_message_datas_.insert(chat_message_datas_.begin(), chatmessage6);
    
    msg::ChatMessage chatmessage7;
    chatmessage7.mutable_messageinfo()->mutable_groupnoticeinfo()->set_text("系统通知：听说涨工资了");
    chatmessage7.mutable_messageinfo()->set_type(msg::MessageType::GroupNotice);
    chatmessage7.set_subtime(time_str);
    chat_message_datas_.push_back(chatmessage7);
    
    msg::ChatMessage chatmessage8;
    chatmessage8.mutable_messageinfo()->mutable_groupnoticeinfo()->set_text("新华社北京4月19日电 中共中央总书记、国家主席、中央军委主席、中央网络安全和信息化领导小组组长习近平19日上午在京主持召开网络安全和信息化工作座谈会并发表重要讲话，强调按照创新、协调、绿色、开放、共享的发展理念推动我国经济社会发展，是当前和今后一个时期我国发展的总要求和大趋势，我国网信事业发展要适应这个大趋势，在践行新发展理念上先行一步，推进网络强国建设，推动我国网信事业发展，让互联网更好造福国家和人民。");
    chatmessage8.mutable_messageinfo()->set_type(msg::MessageType::GroupNotice);
    chatmessage8.set_subtime(time_str);
    chat_message_datas_.push_back(chatmessage8);
    
    
    
    msg::ChatMessage chatmessage9;
    chatmessage9.mutable_userdetailinfo()->set_userid(GDM->get_user_id());
    chatmessage9.mutable_userdetailinfo()->set_nickname("天作码农");
    chatmessage9.mutable_userdetailinfo()->set_picname("login_res/mark.png");
    
    chatmessage9.mutable_messageinfo()->mutable_picinfo()->set_picname("ep_blush.png#ep_deng.png#ep_dizzy.png#ep_evilish.png#ep_grin.png#ep_he.png");
    chatmessage9.mutable_messageinfo()->set_type(msg::MessageType::Picture);
    chatmessage9.set_subtime(time_str);
    chat_message_datas_.push_back(chatmessage9);
    
    msg::ChatMessage chatmessage10;
    chatmessage10.mutable_userdetailinfo()->set_userid(123123123);
    chatmessage10.mutable_userdetailinfo()->set_nickname("黑非洲撒哈拉以南经济促进和贸易发展联合组织");
    chatmessage10.mutable_userdetailinfo()->set_picname("login_res/mark.png");
    
    chatmessage10.mutable_messageinfo()->mutable_picinfo()->set_picname("ep_he.png");
    chatmessage10.mutable_messageinfo()->set_type(msg::MessageType::Picture);
    chatmessage10.set_subtime(time_str);
    chat_message_datas_.push_back(chatmessage10);
    
    msg::ChatMessage chatmessage11;
    chatmessage11.mutable_userdetailinfo()->set_userid(GDM->get_user_id());
    chatmessage11.mutable_userdetailinfo()->set_nickname("天作码农");
    chatmessage11.mutable_userdetailinfo()->set_picname("login_res/mark.png");
    
    chatmessage11.mutable_messageinfo()->mutable_picinfo()->set_picname("ep_blush.png#ep_deng.png#ep_dizzy.png#ep_evilish.png#ep_grin.png#ep_he.png#ep_blush.png#ep_deng.png#ep_dizzy.png#ep_evilish.png#ep_grin.png#ep_he.png#ep_blush.png");
    chatmessage11.mutable_messageinfo()->set_type(msg::MessageType::Picture);
    chatmessage11.set_subtime(time_str);
    chat_message_datas_.push_back(chatmessage11);
    
    msg::ChatMessage chatmessage12;
    chatmessage12.mutable_userdetailinfo()->set_userid(111234);
    chatmessage12.mutable_userdetailinfo()->set_nickname("安拉胡阿克巴");
    chatmessage12.mutable_userdetailinfo()->set_picname("login_res/mark.png");
    
    chatmessage12.mutable_messageinfo()->mutable_picinfo()->set_picname("ep_blush.png#ep_deng.png#ep_dizzy.png#ep_evilish.png#ep_grin.png#ep_he.png#ep_blush.png#ep_deng.png#ep_dizzy.png#ep_evilish.png#ep_grin.png#ep_he.png#ep_blush.png");
    chatmessage12.mutable_messageinfo()->set_type(msg::MessageType::Picture);
    chatmessage12.set_subtime(time_str);
    chat_message_datas_.push_back(chatmessage12);
    
    msg::ChatMessage chatmessage13;
    chatmessage13.mutable_messageinfo()->mutable_createroominfo()->set_text("玩家 天作码农 创建了房间 我在天作十五层");
    chatmessage13.mutable_messageinfo()->mutable_createroominfo()->set_roomname("我在天作十五层");
    chatmessage13.mutable_messageinfo()->mutable_createroominfo()->set_type(msg::RoomType::OFC3);
    chatmessage13.mutable_messageinfo()->mutable_createroominfo()->mutable_creater()->set_userid(GDM->get_user_id());
    chatmessage13.mutable_messageinfo()->mutable_createroominfo()->mutable_creater()->set_nickname("天作码农");
    chatmessage13.mutable_messageinfo()->mutable_createroominfo()->mutable_creater()->set_picname("login_res/mark.png");
    chatmessage13.mutable_messageinfo()->set_type(msg::MessageType::CreateRoom);
    chatmessage13.set_subtime(time_str);
    chat_message_datas_.push_back(chatmessage13);
    
    refresh_tableview();
}

void LayerChat::update_chat_status_by_group()
{
    chat_status_ = CHAT_STATUS::GROUP_CHAT;
    
    chat_message_datas_.clear();
    
    if (!btn_group_match_info_->isVisible())
    {
        btn_group_match_info_->setVisible(true);
    }
    if (!btn_play_->isVisible())
    {
        btn_play_->setVisible(true);
    }
    
    //init_test();
}

void LayerChat::update_chat_status_by_friend()
{
    chat_status_ = CHAT_STATUS::FRIEND_CHAT;
    
    chat_message_datas_.clear();
    
    if (btn_group_match_info_->isVisible())
    {
        btn_group_match_info_->setVisible(false);
    }
    if (btn_play_->isVisible())
    {
        btn_play_->setVisible(false);
    }
    
    //init_test();
}

void LayerChat::update_chat_status_by_group(const msg::GroupInfo & group_info)
{
    update_chat_status_by_group();
    group_info_ = group_info;
    group_id_ = group_info_.groupid();
    refresh_group_info_view();
    set_is_allow_chat(group_info_.allowchat());
    update_chat_message_datas();
    send_get_group_info();
    send_get_room_info();
}

void LayerChat::update_chat_status_by_friend(const msg::UserDetailInfo & user_info)
{
    update_chat_status_by_friend();
    user_info_ = user_info;
    friend_id_ = user_info_.userid();
    refresh_user_info_view();
    update_chat_message_datas();
}

void LayerChat::update_chat_status_by_group(const uint32_t group_id)
{
    update_chat_status_by_group();
    group_id_ = group_id;
    msg::GroupInfo nullgroup;
    nullgroup.set_groupid(group_id_);
    nullgroup.set_allowchat(false);
    nullgroup.set_allowcreateroom(false);
    group_info_.CopyFrom(nullgroup);
    set_is_allow_chat(false);
    update_chat_message_datas();
    send_get_group_info();
    send_get_room_info();
}

void LayerChat::update_chat_status_by_friend(const uint32_t friend_id)
{
    update_chat_status_by_friend();
    friend_id_ = friend_id;
    update_chat_message_datas();
}

void LayerChat::update_chat_message_datas()
{
    switch (chat_status_) {
        case CHAT_STATUS::GROUP_CHAT:
        {
            CM->clear_unreal_message_by(group_id_, ChatMessageManager::MESSAGETYPE::GROUPCHAT);
            std::vector<msg::ChatMessage> vec_message;
            const msg::ChatMessages* chatmessages = CM->get_chats(group_id_, ChatMessageManager::MESSAGETYPE::GROUPCHAT);
            if (chatmessages)
            {
                for (auto messageIter : chatmessages->message())
                {
                    vec_message.push_back(messageIter);
                }
            }
            update_chat_message_datas(vec_message);
            
            break;
        }
        case CHAT_STATUS::FRIEND_CHAT:
        {
            CM->clear_unreal_message_by(friend_id_, ChatMessageManager::MESSAGETYPE::FRIENDCHAT);
            std::vector<msg::ChatMessage> vec_message;
            const msg::ChatMessages* chatmessages = CM->get_chats(friend_id_, ChatMessageManager::MESSAGETYPE::FRIENDCHAT);
            if (chatmessages)
            {
                for (auto messageIter : chatmessages->message())
                {
                    vec_message.push_back(messageIter);
                }
            }
            update_chat_message_datas(vec_message);
            
            break;
        }
        default:
            break;
    }
}

void LayerChat::update_chat_message_datas(const std::vector<msg::ChatMessage> & chat_message_datas)
{
    chat_message_datas_ = chat_message_datas;
    refresh_tableview();
}

void LayerChat::send_chatmessage_text(std::string chat_text)
{
    msg::ChatMessage sendMessage;
    sendMessage.mutable_messageinfo()->set_type(msg::MessageType::Text);
    sendMessage.mutable_messageinfo()->mutable_textinfo()->set_text(chat_text);
    
    if (chat_status_ == CHAT_STATUS::GROUP_CHAT)
    {
        sendMessage.set_groupid(group_id_);
    }else
    {
        sendMessage.set_receiveuserid(friend_id_);
    }
    
    CM->send_chat_message(sendMessage);
}

void LayerChat::send_chatmessage_picture(std::string pic_file_name)
{
    msg::ChatMessage sendMessage;
    sendMessage.mutable_messageinfo()->set_type(msg::MessageType::Picture);
    sendMessage.mutable_messageinfo()->mutable_picinfo()->set_picname(pic_file_name);
    
    if (chat_status_ == CHAT_STATUS::GROUP_CHAT)
    {
        sendMessage.set_groupid(group_id_);
    }else
    {
        sendMessage.set_receiveuserid(friend_id_);
    }
    
    CM->send_chat_message(sendMessage);
}

void LayerChat::send_chatmessage_voice(std::string voice_file_name, int voice_time)
{
    msg::ChatMessage sendMessage;
    
    sendMessage.mutable_messageinfo()->set_type(msg::MessageType::Voice);
    sendMessage.mutable_messageinfo()->mutable_voiceinfo()->set_voicename(voice_file_name);
    sendMessage.mutable_messageinfo()->mutable_voiceinfo()->set_voicetime(voice_time);
    
    if (chat_status_ == CHAT_STATUS::GROUP_CHAT)
    {
        sendMessage.set_groupid(group_id_);
    }else
    {
        sendMessage.set_receiveuserid(friend_id_);
    }
    
    CM->send_chat_message(sendMessage);
}

bool LayerChat::is_display_time(uint32_t idx)
{
    if (idx == 0)
    {
        return true;
    }
    
    bool is_display_time = false;
    double current_time = 0.0;
    std::string submit_time = chat_message_datas_[0].subtime();
    if(!submit_time.empty()) {
        current_time = tools::stod(submit_time);
    }
    
    for (int i = 0; i < idx; i++)
    {
        submit_time = chat_message_datas_[i].subtime();
        if(!submit_time.empty()) {
            double iter_time = tools::stod(submit_time);
            if (iter_time - current_time - (10 * 60 * 1000) > 0)
            {
                //cocos2d::log("CurrentTime > IterTime idx:%u", idx);
                current_time = iter_time;
            }
        }
    }
    
    double idx_time;
    submit_time = chat_message_datas_[idx].subtime();
    if(!submit_time.empty()) {
        double idx_time = tools::stod(submit_time);
        //cocos2d::log("idx_time %s", tools::time_display(idx_time / 1000).c_str());
        if (idx_time - current_time - (10 * 60 * 1000) > 0)
        {
            is_display_time = true;
        }
    }
    
    
    cocos2d::log("***IsDisplayTime %s, return %s  ***", tools::time_display(idx_time / 1000).c_str(), is_display_time ? "true" : "false");
    return is_display_time;
}

void LayerChat::refresh_user_info_view()
{
    text_title_->setString(user_info_.nickname());
    
    btn_chat_setting_->setPositionX(text_title_->getPositionX() + text_title_->getContentSize().width * (1 - text_title_->getAnchorPoint().x) + btn_chat_setting_->getContentSize().width * btn_chat_setting_->getAnchorPoint().x + 10.0);
}

void LayerChat::refresh_group_info_view()
{
    text_title_->setString(group_info_.groupname());
    
    btn_chat_setting_->setPositionX(text_title_->getPositionX() + text_title_->getContentSize().width * (1 - text_title_->getAnchorPoint().x) + btn_chat_setting_->getContentSize().width * btn_chat_setting_->getAnchorPoint().x + 10.0);
}

void LayerChat::RefreshLayer()
{
    if (chat_status_ == CHAT_STATUS::GROUP_CHAT)
    {
        send_get_group_info();
        send_get_room_info();
    }else if (chat_status_ == CHAT_STATUS::FRIEND_CHAT)
    {
        
    }
}

void LayerChat::set_is_allow_chat(bool isAllowChat)
{
    if (isAllowChat)
    {
        btn_keyboard_->setVisible(false);
        btn_voice_->setVisible(true);
        
        btn_record_->setVisible(false);
        
        image_input_bg_->setVisible(true);
        editbox_input_->setVisible(true);
        
        btn_chat_emotion_->setVisible(true);
        btn_add_->setVisible(true);
        
        text_chat_status_->setVisible(false);
    }else
    {
        btn_keyboard_->setVisible(false);
        btn_voice_->setVisible(false);
        
        btn_record_->setVisible(false);
        
        editbox_input_->setVisible(false);
        image_input_bg_->setVisible(false);
        
        btn_chat_emotion_->setVisible(false);
        btn_add_->setVisible(false);
        
        text_chat_status_->setVisible(true);
    }
}

#pragma mark click_event
void LayerChat::click_btn_keyboard(Ref * sender)
{
    btn_keyboard_->setVisible(false);
    btn_voice_->setVisible(true);
    btn_record_->setVisible(false);
    //tf_chat_->setPositionY(m_pEditBoxPosition.y);
    editbox_input_->setVisible(true);
}

void LayerChat::touch_btn_record(Ref* sender, Touch* touch, Widget::TouchEventType event)
{
    unsigned long time = tools::date();
    uint32_t user_id = GDM->get_user_id();
    
    int record_time = 0;
    switch (event) {
        case cocos2d::ui::Widget::TouchEventType::BEGAN: {
            AM->begin_recording();
            
            touch_record_begin_position_ = touch->getLocation();
            record_filename_ = tools::to_string(user_id) + "_" + tools::to_string(time);
            log("filename1....:%s", record_filename_.c_str());
            FM.get_recorder()->beginRecordByFileName(record_filename_);
            btn_record_->setTitleText(tools::local_string("release_send", "松开  发送"));
            RecordAnimationLayer* record_animation_layer_ = (RecordAnimationLayer*)getChildByTag(ANIMATION_LAYER_TAG);
            if(!record_animation_layer_) {
                record_animation_layer_ = RecordAnimationLayer::create();
                addChild(record_animation_layer_, ZORDER_RECORD_ANIMATION, ANIMATION_LAYER_TAG);
                record_animation_layer_->record();
            }
            break;
        }
        case cocos2d::ui::Widget::TouchEventType::MOVED: {
            Point p = touch->getLocation();
            if(!btn_record_->isHighlighted()) {
                RecordAnimationLayer* record_animation_layer_ = (RecordAnimationLayer*)getChildByTag(ANIMATION_LAYER_TAG);
                if(record_animation_layer_) {
                    float sub = p.y - touch_record_begin_position_.y;
                    log("sub:%f", sub);
                    if(fabsf(p.y - touch_record_begin_position_.y) < 100) {
                        record_animation_layer_->cancel();
                    }
                    else {
                        record_animation_layer_->giveup();
                    }
                }
            }
            break;
        }
        case cocos2d::ui::Widget::TouchEventType::ENDED: {
            
            record_time = FM.get_recorder()->stopRecord();
            if(record_time!=0) {
                const std::string upload_filename = record_filename_ + ".amr";
                FM.upload_file(upload_filename);
                // by lvzhangle
                send_chatmessage_voice(record_filename_, record_time);
                
//                ChatMessage->mutable_userdetailinfo()->set_userid(GDM->get_user_id());
//                ChatMessage->mutable_userdetailinfo()->set_nickname(GDM->get_user_data()->get_nickname());
//                ChatMessage->mutable_userdetailinfo()->set_picname(GDM->get_user_data()->get_head_pic_name());
//                ChatMessage->mutable_userdetailinfo()->set_sex(GDM->get_user_data()->get_sex());
//                
//                ChatMessage->mutable_messageinfo()->mutable_voiceinfo()->set_voicename(record_filename_);
//                ChatMessage->mutable_messageinfo()->mutable_voiceinfo()->set_voicetime(record_time);
//                ChatMessage->mutable_messageinfo()->set_type(msg::MessageType::Voice);
//                if (GDM->get_chat_status() == 1){
//                    ChatMessage->set_groupid(GDM->get_select_group_id());
//                }else if (GDM->get_chat_status() == 2){
//                    ChatMessage->set_receiveuserid(GDM->get_select_friend_id());
//                }
//                GDM->send_chat_message(*ChatMessage);
                
                
            }
            else {
                Toast::ToastShow(tools::local_string("record_time_too_shop", "录音时间太短"));
            }
            record_filename_ = "";
            btn_record_->setTitleText(tools::local_string("press_voice", "按住  说话"));
            
            RecordAnimationLayer* record_animation_layer_ = (RecordAnimationLayer*)getChildByTag(ANIMATION_LAYER_TAG);
            if(record_animation_layer_)
                record_animation_layer_->removeFromParent();
            AM->end_recording();
            break;
        }
        case cocos2d::ui::Widget::TouchEventType::CANCELED:
            RecordAnimationLayer* record_animation_layer_ = (RecordAnimationLayer*)getChildByTag(ANIMATION_LAYER_TAG);
            if(record_animation_layer_)
                record_animation_layer_->removeFromParent();
            btn_record_->setTitleText(tools::local_string("press_voice", "按住  说话"));
            AM->end_recording();
            break;
    }
}

void LayerChat::click_btn_voice(Ref * sender)
{
    btn_record_->setTitleText(tools::local_string("press_voice", "按住  说话"));
    btn_voice_->setVisible(false);
    btn_keyboard_->setVisible(true);
    btn_record_->setVisible(true);
//    m_pEditBoxPosition = tf_chat_->getPosition();
//    tf_chat_->setPositionY(-100);
    editbox_input_->setVisible(false);
}

void LayerChat::click_btn_back(Ref * sender)
{
    this->removeFromParent();
}

void LayerChat::click_btn_chat_emotion(Ref * sender)
{
    editbox_input_->setVisible(false);
    LayerEmotion* el = LayerEmotion::create();
    el->set_dismiss_callback(std::bind(&LayerChat::remove_emotion_layer, this));
    el->set_voice_callback([this](){
        for (auto& iter : node_about_textfileds_) {
            if(iter.first != tableview_) {
                iter.first->setVisible(true);
            }
        }
        this->keyBoardWillHide(0.2f, 0);
        this->click_btn_voice(nullptr);
        
    });
    //el->set_chat_callback(std::bind(&LayerChat::show_keyboard, this));
    el->set_chat_callback([=](){
        this->remove_emotion_layer();
        this->show_keyboard();
    });
    el->set_emoition_callback(std::bind(&LayerChat::send_chatmessage_picture, this, std::placeholders::_1));
    
    this->addChild(el, ZORDER_EMOTION);
    
    for (auto& iter : node_about_textfileds_) {
        if(iter.first != tableview_) {
            iter.first->setVisible(false);
        }
    }
    

    float dis = 425;
//    auto& items = listview_->getItems();
//    if(!items.empty()) {
//        auto back = items.back();
//        Point p = back->getPosition() + node_about_textfileds_.at(tableview_);
//        if(p.y < dis) {
//            Point p1 = node_about_textfileds_.at(listview_) + Point(0, dis - p.y + editbox_input_->getContentSize().height + 20);
//            tableview_->stopAllActions();
//            tableview_->runAction(MoveTo::create(0.2f, p1));
//        }
//    }
    
    tableview_->stopAllActions();
    tableview_->runAction(MoveTo::create(0.2f, cocos2d::Point(tableview_->getPositionX(), tableview_->getPositionY() + dis)));
}

void LayerChat::click_btn_add(Ref * sender)
{
    editbox_input_->setVisible(false);
    LayerChoosePhoto* as = LayerChoosePhoto::create();
    as->setIsChat(true);
    as->set_choose_photo_callback(std::bind(&LayerChat::send_chatmessage_picture, this, std::placeholders::_1));
    as->set_dismiss_callback([this](){
        editbox_input_->setVisible(true);
        GameTools::editBoxLoseInputFocus();
    });
    this->addChild(as, ZORDER_NEW_LAYER);
}

void LayerChat::click_btn_chat_setting(Ref* sender)
{
    if(CHAT_STATUS::GROUP_CHAT == chat_status_)
    {
        //普通群
        if(group_info_.type() == msg::GroupType::NORMALGROUP)
        {
            AddGlobalLayer(LayerGroupSetting::create(group_id_));
        }
    }
    else if (CHAT_STATUS::FRIEND_CHAT == chat_status_)
    {
        AddGlobalLayer(LayerUserInfo::create(user_info_));
    }
}

void LayerChat::click_btn_group_match_info(Ref* sender)
{
    LayerScoreList* list=LayerScoreList::create(true,group_id_,group_info_.groupname());
    this->addChild(list, ZORDER_NEW_LAYER, list->GetName());
}

void LayerChat::click_btn_play(Ref* sender)
{
    LayerChooseGameRoom * layerroom = LayerChooseGameRoom::create(msg::RoomType::ALL, false, group_info_);
    
    AddGlobalLayer(layerroom);
}

#pragma mark tableview datasource --
TableViewCell* LayerChat::tableCellAtIndex(TableView *table, ssize_t idx)
{
    ChatMessageCell * cell = dynamic_cast<ChatMessageCell*>(table->dequeueCell());
    if (!cell)
    {
        cell = ChatMessageCell::create();
    }
    cell->update_data(chat_message_datas_[idx], tableview_->getContentSize().width, is_display_time(idx));
    
    return cell;
}

Size LayerChat::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
    msg::ChatMessage chat_data = chat_message_datas_[idx];
    
    float cellOffSet = idx == 0 ? MESSAGE_CELL_OFFSET : MESSAGE_CELL_OFFSET * 2;
    
    switch (chat_data.messageinfo().type()) {
        case msg::MessageType::Text:
        {
            std::string strMessage = chat_data.messageinfo().textinfo().text();
            
            RichText* richtext_message_ = RichText::create();
            
            richtext_message_->ignoreAnchorPointForPosition(true);

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
                    
                    RichElementText * richEleText = RichElementText::create(0, cocos2d::Color3B::WHITE, 100, strText, "", MESSAGE_TEXT_FONT_SIZE);
                    
                    richtext_message_->pushBackElement(richEleText);
                    
                    intStrCur += 125;
                }
                std::u16string u16StrText;
                
                u16StrText = utf16String.substr(intStrCur, utf16String.length());
                
                std::string strText;
                StringUtils::UTF16ToUTF8(u16StrText, strText);
                
                RichElementText * richEleText = RichElementText::create(0, cocos2d::Color3B::WHITE, 100, strText, "", MESSAGE_TEXT_FONT_SIZE);
                
                richtext_message_->pushBackElement(richEleText);
            }
  
            
            ImageView* image_chatmessage_cell_bg_ = ImageView::create("login_res/chat_res/image_chatmessage_cell_bg.png");
            
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
            
            Text* text_nick_name_ = Text::create(chat_data.userdetailinfo().nickname(), "", MESSAGE_NICK_NAME_FONT_SIZE);
            
             sizeImageBg.height = sizeImageBg.height + text_nick_name_->getContentSize().height + 5;
            
            //cocos2d::log("LayerChat::tableCellSizeForIndex %f, %f", 750.0, sizeImageBg.height);
            
            return cocos2d::Size(750, sizeImageBg.height + cellOffSet);

            break;
        }
        case msg::MessageType::Voice:
        {
            ImageView* image_voice_icon_ = ImageView::create();
            
            image_voice_icon_->loadTexture("login_res/chat_res/image_voice_icon_self.png");
            
            cocos2d::Size sizeImageMessageBg;
            sizeImageMessageBg.height = image_voice_icon_->getContentSize().height + MESSAGE_TEXT_BG_OFFSET_TOP + MESSAGE_TEXT_BG_OFFSET_BUTTOM;
            
            Text* text_nick_name_ = Text::create(chat_data.userdetailinfo().nickname(), "", MESSAGE_NICK_NAME_FONT_SIZE);
            
            sizeImageMessageBg.height = sizeImageMessageBg.height + text_nick_name_->getContentSize().height + 5;
            
            cocos2d::log("LayerChat::tableCellSizeForIndex %f, %f", 750.0, sizeImageMessageBg.height);
            
            return cocos2d::Size(750, sizeImageMessageBg.height + cellOffSet);
            
            break;
        }
        case msg::MessageType::GroupNotice:
        {
            std::string strMessage = chat_data.messageinfo().groupnoticeinfo().text();

            RichText* richtext_message_ = RichText::create();
            
            richtext_message_->ignoreAnchorPointForPosition(true);
            
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
                    
                    RichElementText * richEleText = RichElementText::create(0, cocos2d::Color3B::WHITE, 100, strText, "", MESSAGE_TEXT_FONT_SIZE);
                    
                    richtext_message_->pushBackElement(richEleText);
                    
                    intStrCur += 125;
                }
                std::u16string u16StrText;
                
                u16StrText = utf16String.substr(intStrCur, utf16String.length());
                
                std::string strText;
                StringUtils::UTF16ToUTF8(u16StrText, strText);
                
                RichElementText * richEleText = RichElementText::create(0, cocos2d::Color3B::WHITE, 100, strText, "", MESSAGE_TEXT_FONT_SIZE);
                
                richtext_message_->pushBackElement(richEleText);
            }
            
            
            ImageView* image_chatmessage_cell_bg_ = ImageView::create("login_res/chat_res/image_chatmessage_cell_bg.png");
            
            image_chatmessage_cell_bg_->setScale9Enabled(true);
            image_chatmessage_cell_bg_->setCapInsets(cocos2d::Rect(60, 80, 415, 30));
            image_chatmessage_cell_bg_->setEnabled(false);
            
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
            
            std::string strTime;
            std::string submit_time = chat_data.subtime();
            if(!submit_time.empty()) {
                double dbTime = tools::stod(submit_time);
                dbTime /= 1000;
                strTime = tools::time_display(dbTime);
            }

            
            Text* text_message_time = Text::create(strTime, "", MESSAGE_MESSAGE_TIME_FONT_SIZE);
            
            sizeRichText.height = sizeRichText.height + text_message_time->getContentSize().height + 5;
            
            cocos2d::log("LayerChat::tableCellSizeForIndex %f, %f", 750.0, sizeRichText.height);
            
            return cocos2d::Size(750, sizeRichText.height);
            
            break;
        }
        case msg::MessageType::Picture:
        {
            std::string strTime;
            std::string submit_time = chat_data.subtime();
            if(!submit_time.empty()) {
                double dbTime = tools::stod(submit_time);
                dbTime /= 1000;
                strTime = tools::time_display(dbTime);
            }
            
            Text* text_message_time = Text::create(strTime, "", MESSAGE_MESSAGE_TIME_FONT_SIZE);
            
            auto vecImageFile = tools::split(chat_data.messageinfo().picinfo().picname(), "#");
            if (vecImageFile.size() > 0)
            {
                if (vecImageFile.size() == 1)
                {
                    std::string path = FileUtils::getInstance()->fullPathForFilename("emotion/" + vecImageFile[0]);
                    
                    Size s(120, 120);
                    if (path.empty())
                    {
//                        ImageView* emotion;
//                        if(GDM->is_file_exist(vecImageFile[0])) {
////                            emotion =  ImageView::create(vecImageFile[0]);
////                            Size s(120, 120);
////                            Size contentSize = emotion->getContentSize();
////                            s.height = s.width * (contentSize.height/contentSize.width);
////                            if(contentSize.width > s.width || contentSize.height > s.height) {
////                                emotion->ignoreContentAdaptWithSize(false);
////                                emotion->setContentSize(s);
////                            }
//                            
//                        }
//                        else {
////                            emotion = ImageView::create("msg_pic.png");
////                            Size s(120, 120);
////                            Size contentSize = emotion->getContentSize();
////                            if(contentSize.width > s.width || contentSize.height > s.height) {
////                                emotion->ignoreContentAdaptWithSize(false);
////                                emotion->setContentSize(s);
////                            }
//                            
//                        }
                        return cocos2d::Size(750, s.height + MESSAGE_TEXT_BG_OFFSET_TOP + MESSAGE_TEXT_BG_OFFSET_BUTTOM + text_message_time->getContentSize().height + 5 + cellOffSet);
                    }else
                    {
                        
                        return cocos2d::Size(750, s.height + MESSAGE_TEXT_BG_OFFSET_TOP + MESSAGE_TEXT_BG_OFFSET_BUTTOM + text_message_time->getContentSize().height + 5 + cellOffSet);
                    }

                }else
                {
                    cocos2d::Size sizeEmotion = cocos2d::Size(120, 120);
                    
                    int emotionColumn = (MESSAGE_TEXT_BG_WIDTH_MAX - MESSAGE_TEXT_BG_OFFSET_LEFT - MESSAGE_TEXT_BG_OFFSET_RIGHT) / sizeEmotion.width;
                    
                    int emotionLines = vecImageFile.size() / emotionColumn;
                    
                    if (vecImageFile.size() % emotionColumn != 0)
                    {
                        emotionLines++;
                    }
                    
                    float messageHeight = sizeEmotion.height * emotionLines;
                    
                    return cocos2d::Size(750, messageHeight + MESSAGE_TEXT_BG_OFFSET_TOP + MESSAGE_TEXT_BG_OFFSET_BUTTOM + text_message_time->getContentSize().height + 5 + cellOffSet);

                }
            }else
            {
                return cocos2d::Size(750, MESSAGE_TEXT_BG_OFFSET_TOP + MESSAGE_TEXT_BG_OFFSET_BUTTOM + text_message_time->getContentSize().height + 5 + cellOffSet);
            }

            
            break;
        }
        case msg::MessageType::CreateRoom:
        {
            
            ImageView* image_room_icon = ImageView::create();
            
            switch (chat_data.messageinfo().createroominfo().type()) {
                case msg::RoomType::NLHOLDEM:
                {
                    image_room_icon->loadTexture("login_res/creategameroom_res/image_createroom_type_icon_holdem.png");
                    break;
                }
                case msg::RoomType::OFC3:
                {
                    image_room_icon->loadTexture("login_res/creategameroom_res/image_createroom_type_icon_ofc3.png");
                    break;
                }
                case msg::RoomType::OFC4:
                {
                    image_room_icon->loadTexture("login_res/creategameroom_res/image_createroom_type_icon_ofc3.png");
                    break;
                }
                case msg::RoomType::PLO:
                {
                    image_room_icon->loadTexture("login_res/creategameroom_res/image_createroom_type_icon_holdem.png");
                    break;
                }
                case msg::RoomType::PLOHILO:
                {
                    image_room_icon->loadTexture("login_res/creategameroom_res/image_createroom_type_icon_holdem.png");
                    break;
                }
                default:
                {
                    break;
                }
            }
            
            
            cocos2d::Size sizeImageRoomIcon = image_room_icon->getContentSize();
            
            RichText* richtext_message_ = RichText::create();
            
            richtext_message_->ignoreAnchorPointForPosition(true);
            
            std::string strMessage = chat_data.messageinfo().createroominfo().roomname();
            
            strMessage += '\n';
            strMessage += '\n';
            strMessage += chat_data.messageinfo().createroominfo().text();
            
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
                    
                    RichElementText * richEleText = RichElementText::create(0, colorText, 100, strText, "", MESSAGE_TEXT_FONT_SIZE);
                    
                    richtext_message_->pushBackElement(richEleText);
                    
                    intStrCur += 125;
                }
                std::u16string u16StrText;
                
                u16StrText = utf16String.substr(intStrCur, utf16String.length());
                
                std::string strText;
                StringUtils::UTF16ToUTF8(u16StrText, strText);
                
                RichElementText * richEleText = RichElementText::create(0, colorText, 100, strText, "", MESSAGE_TEXT_FONT_SIZE);
                
                richtext_message_->pushBackElement(richEleText);
            }
            
            richtext_message_->ignoreContentAdaptWithSize(false);
            richtext_message_->setContentSize(cocos2d::Size(MESSAGE_BG_WIDTH_MAX - MESSAGE_BG_OFFSET_LEFT - MESSAGE_BG_OFFSET_RIGHT - sizeImageRoomIcon.width - 10, 0));
            
            richtext_message_->formatText();
            cocos2d::Size sizeRichText = richtext_message_->getVirtualRendererSize();
            
            cocos2d::Size sizeImageBg;
            
            sizeImageBg.height = std::max(sizeImageRoomIcon.height, sizeRichText.height) + MESSAGE_BG_OFFSET_TOP + MESSAGE_BG_OFFSET_BUTTOM;

            Text* text_nick_name_ = Text::create(chat_data.messageinfo().createroominfo().creater().nickname(), "", MESSAGE_NICK_NAME_FONT_SIZE);
            
            sizeImageBg.height = sizeImageBg.height + text_nick_name_->getContentSize().height + 5;
            
            
            return cocos2d::Size(750, sizeImageBg.height);
        }
    }
    
    return cocos2d::Size(750, 100);
}

ssize_t LayerChat::numberOfCellsInTableView(TableView *table)
{
    return chat_message_datas_.size();
}

#pragma mark tableview delegate --
void LayerChat::tableCellTouched(TableView* table, TableViewCell* cell)
{
    cocos2d::log("CellTouched Idx:%zd", cell->getIdx());
}

#pragma mark EditBox delegate
void LayerChat::editBoxEditingDidBegin(EditBox* editBox)
{
    
}

void LayerChat::editBoxEditingDidEnd(EditBox* editBox)
{
    
}
void LayerChat::editBoxTextChanged(EditBox* editBox, const std::string& text)
{
    isTouchLayerout_ = false;
}

#pragma mark OnTouchEvent
bool LayerChat::ChatViewOnTouchBegan(Touch * touch, Event* event)
{
    return true;
}

void LayerChat::ChatViewOnTouchMoved(Touch * touch, Event* event)
{
    
}

void LayerChat::ChatViewOnTouchEnded(Touch * touch, Event* event)
{
 
}

void LayerChat::ChatViewOnTouchCancelled(Touch * touch, Event* event)
{

}

void LayerChat::editBoxReturn(EditBox* editBox)
{
    if (!isTouchLayerout_)
    {
        std::string content = editbox_input_->getText();
        if(!content.empty()) {
            if (content.size() > 1000) {
                TipView::showAlertView(tools::local_string("too_many_info","输入内容过长，请重新输入"));
                return;
            }
            
            send_chatmessage_text(content);
            editbox_input_->setText("");
        }
    }
}

#pragma mark ViewFunc
void LayerChat::keyBoardWillShow(const float duration, const float distance)
{

    float dis = distance;

    if(SceneManager::get_landscape()) {
        dis *= 2;
        dis += 20;
    }
    
//    auto& items = tableview_->getItems();
//    if(!items.empty()) {
//        auto back = items.back();
//        Point p = back->convertToWorldSpace(Point::ZERO);
//        if(p.y < dis + editbox_input_->getContentSize().height) {
//            Point p1 = listview_->getPosition() + Point(0, dis - p.y + editbox_input_->getContentSize().height + back->getContentSize().height/2);
//            tableview_->stopAllActions();
//            tableview_->runAction(MoveTo::create(duration, p1));
//        }
//    }
    
    for (auto& iter : node_about_textfileds_) {
//        if(iter.first != tableview_) {
//            Point p = iter.first->getPosition() + Point(0, dis);
//            iter.first->stopAllActions();
//            iter.first->runAction(MoveTo::create(duration, p));
//        }
        Point p = iter.first->getPosition() + Point(0, dis);
        iter.first->stopAllActions();
        iter.first->runAction(MoveTo::create(duration, p));

    }
}
void LayerChat::keyBoardWillHide(const float duration, const float distance)
{
    float dis = distance;
    if (dis < 0.0f) dis = 0.0f;
    
    for (auto& iter : node_about_textfileds_) {
//        if(iter.first != tableview_) {
//            iter.first->stopAllActions();
//            iter.first->runAction(MoveTo::create(duration, iter.second));
//        }
        iter.first->stopAllActions();
        iter.first->runAction(MoveTo::create(duration, iter.second));
    }
}

void LayerChat::show_keyboard()
{
    editbox_input_->setVisible(true);
    GameTools::editBoxLoseInputFocus();
    for (auto& iter : node_about_textfileds_) {
        if(iter.first != tableview_) {
            iter.first->setVisible(true);
        }
    }
    click_btn_keyboard(btn_keyboard_);
    
    editbox_input_->touchDownAction(editbox_input_, cocos2d::ui::Widget::TouchEventType::ENDED);
    
}

void LayerChat::remove_emotion_layer()
{
    for (auto& iter : node_about_textfileds_) {
        if(iter.first != tableview_) {
            iter.first->setVisible(true);
        }
    }
    
    click_btn_keyboard(btn_keyboard_);
    this->keyBoardWillHide(0.2f, 0);
    
}

void LayerChat::refresh_tableview()
{
    tableview_->reloadData();
    
    cocos2d::Size sizeContainer = tableview_->getContainer()->getContentSize();
    cocos2d::Size sizeTableView = tableview_->getViewSize();
    if (sizeContainer.height - sizeTableView.height > 1)
    {
        tableview_->setContentOffset(cocos2d::Point(0, 0));
    }
}

void LayerChat::register_events()
{
    dispatch_.register_event(REQ_GET_GROUP_INFO,
                             BIND_FUNC(this, LayerChat::handle_get_group_info));
    dispatch_.register_event(REQ_GET_ROOM_INFO,
                             BIND_FUNC(this, LayerChat::handle_get_room_info));
}

#pragma mark SEND
void LayerChat::send_get_room_info()
{
    if (CHAT_STATUS::GROUP_CHAT == chat_status_ && group_id_ != 0)
    {
        auto processor = PM->GetRoomInfo_UP(group_id_);
        send_data(processor, REQ_GET_ROOM_INFO);
    }
}

void LayerChat::send_get_group_info()
{
    if (CHAT_STATUS::GROUP_CHAT == chat_status_ && group_id_ != 0)
    {
        auto processor = PM->GetGroupInfo_UP(group_id_);
        send_data(processor, REQ_GET_GROUP_INFO);
    }
}

#pragma mark HANDLE
void LayerChat::handle_get_room_info(cmd_data_pointer data)
{
    std::shared_ptr<net_data_recv_package> rec = dynamic_pointer_cast<net_data_recv_package>(data);
    if (!check_request(rec)) return;
    msg::Processor_150_GetRoomInfo_DOWN processor;
    processor.ParseFromArray(rec->body, rec->bodysize);
    switch (processor.response().status()) {
        case msg::Status::SUCCESS: {
            uint32_t uintRoomCount = processor.roominfo().size();
            if (uintRoomCount == 0)
            {
                if (image_play_count_bg_->isVisible())
                {
                    image_play_count_bg_->setVisible(false);
                }
            }else
            {
                if (!image_play_count_bg_->isVisible())
                {
                    image_play_count_bg_->setVisible(true);
                }
                text_play_count_->setString(tools::to_string(uintRoomCount));
            }
            
            break;
        }
        case msg::Status::FAILED: {
            //TipView::showAlertView(tools::local_string("get_room_list_failed", "获取房间列表失败"));
            break;
        }
        default:
            break;
    }
}

void LayerChat::handle_get_user_info(cmd_data_pointer data)
{
    
}

void LayerChat::handle_get_group_info(cmd_data_pointer data)
{
    std::shared_ptr<net_data_recv_package> rec = dynamic_pointer_cast<net_data_recv_package>(data);
    if (!check_request(rec)) return;
    msg::Processor_145_GetGroupInfo_DOWN processor;
    processor.ParseFromArray(rec->body, rec->bodysize);
    switch (processor.response().status()) {
        case msg::Status::SUCCESS: {
            if (processor.groupinfo().groupid() == group_id_)
            {
                group_info_.CopyFrom(processor.groupinfo());
                
                text_title_->setString(processor.groupinfo().groupname() + std::string("(") + tools::to_string(processor.groupusernum()) + std::string(")"));
                
                btn_chat_setting_->setPositionX(text_title_->getPositionX() + text_title_->getContentSize().width * (1 - text_title_->getAnchorPoint().x) + btn_chat_setting_->getContentSize().width * btn_chat_setting_->getAnchorPoint().x + 10.0);

                btn_chat_setting_->setVisible(group_info_.type() == msg::GroupType::NORMALGROUP);

                set_is_allow_chat(group_info_.allowchat());
                
                GameTools::editBoxLoseInputFocus();
            }
            break;
        }
        case msg::Status::FAILED: {
            //TipView::showAlertView(processor.response().message());
            break;
        }
    }
}

#pragma mark OnExit
void LayerChat::onExit()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    AM->stopAll();
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

    if(SoundPlayerInterface::is_playing()) {
        SoundPlayerInterface::stop_play();
    }
    
#endif
    BaseLayer::onExit();
}