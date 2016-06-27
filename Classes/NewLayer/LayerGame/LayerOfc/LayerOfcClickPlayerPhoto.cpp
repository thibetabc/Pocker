//
//  LayerOfcClickPlayerPhoto.cpp
//  pokerclient
//
//  Created by zhongyu on 16/5/19.
//
//

#include "LayerOfcClickPlayerPhoto.h"
#include "utils.h"


#include "FileManager.h"
#include "GameDataManager.h"
#include "GameTools.h"
#include "PokerDataManager.h"
#include "ProcessorManager.h"
#include "FileManager.h"
#include "newOfcPokerRoom.h"
#include "TipView.h"
#include "AudioManager.h"
#include "VoiceRecorderInterface.h"
#include "PokerRoomLayer.h"


#define USER_PHOTO_IMAGE_HEIGHT 110
#define USER_PHOTO_IMAGE_WIDTH 110

bool LayerOfcClickPlayerPhoto::init()
{
    if (!BaseLayer::init())
    {
        return false;
    }
    
    root_ = CSLoader::createNode("LayerOfcClickOtherPhoto.csb");
    this->addChild(root_);
    
    btn_male_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_male"));
    btn_male_->setTouchEnabled(false);
    btn_male_->setVisible(false);
    
    btn_female_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_female"));
    btn_female_->setTouchEnabled(false);
    
    text_user_id_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_user_id"));
    text_user_id_->setString("");
    
    text_tool_desc_  = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_tool_desc"));
    text_tool_desc_->setString("");
    
    Node_user_photo_ = dynamic_cast<Node*>(CSLoader::seekNodeByName(root_, "Node_user_photo"));
    
    text_user_name_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_user_name"));
    text_user_name_->setString("");
    
    text_tool_title_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_tool_title"));
    text_tool_title_->setString("");
    
    btn_voice_play_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_voice_play"));
    btn_voice_play_->setTitleText(tools::local_string("replay_audio","语音回放"));
    btn_voice_play_->setVisible(false);
    btn_voice_play_->addClickEventListener(std::bind(&LayerOfcClickPlayerPhoto::click_btn_voice_play, this,std::placeholders::_1));
    
    btn_tick_player_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_tick_player"));
    btn_tick_player_->setTitleText(tools::local_string("kick_out","踢人"));
    btn_tick_player_->setVisible(false);
    btn_tick_player_->addClickEventListener(std::bind(&LayerOfcClickPlayerPhoto::click_btn_tick_player, this,std::placeholders::_1));
    
    image_ofc_click_other_bg_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_ofc_click_other_bg"));
    
    text_tool_title_->setString(tools::local_string("virtual_props","虚拟道具"));
    text_tool_desc_->setString(tools::local_string("HP_prop","使用道具需要100体力兑换"));
    
    layout_credit_value_ = dynamic_cast<Layout*>(CSLoader::seekNodeByName(root_, "layout_credit_value"));
    title_credit_value_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(layout_credit_value_, "title_credit_value"));
    textfield_credit_value_ = dynamic_cast<TextField*>(CSLoader::seekNodeByName(layout_credit_value_, "textfield_credit_value"));

    image_tool_bg_1_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_tool_bg_1"));
    image_tool_bg_2_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_tool_bg_2"));
    image_tool_bg_3_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_tool_bg_3"));
    image_tool_bg_4_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_tool_bg_4"));
    image_tool_bg_5_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_tool_bg_5"));
    image_tool_bg_6_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_tool_bg_6"));
    image_tool_bg_7_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_tool_bg_7"));
    image_tool_bg_8_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_tool_bg_8"));
    image_tool_bg_9_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_tool_bg_9"));
    image_tool_bg_10_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_tool_bg_10"));
    
//    tools_.insert(image_tool_bg_1_,msg::Game_Tool_Type::THROW_EGG);
    tools_.insert(std::make_pair(image_tool_bg_1_, msg::Game_Tool_Type::THROW_EGG));
    tools_.insert(std::make_pair(image_tool_bg_2_, msg::Game_Tool_Type::FLOWER));
    tools_.insert(std::make_pair(image_tool_bg_3_, msg::Game_Tool_Type::GIFT));
    tools_.insert(std::make_pair(image_tool_bg_4_, msg::Game_Tool_Type::KISS));
    tools_.insert(std::make_pair(image_tool_bg_5_, msg::Game_Tool_Type::PRAISE));
    tools_.insert(std::make_pair(image_tool_bg_6_, msg::Game_Tool_Type::CATCH_THE_CHICKEN));
    tools_.insert(std::make_pair(image_tool_bg_7_, msg::Game_Tool_Type::DIAMOND_RING));
    
    for ( auto iter : tools_ )
    {
        ImageView* shade = ImageView::create("image/ofc_game_new_res/ofc_click_other_photo_res/tool_shade.png");
        this->addChild(shade,100);
        shade->setPosition(iter.first->getPosition());
//        iter.first->setTouchEnabled(true);
//        iter.first->addClickEventListener(std::function<void (Ref*)>([=](Ref * sender){this->click_image_tool(iter.second);}));
    }
    
    
    auto img_credit_value_bg = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(layout_credit_value_, "img_credit_value_bg"));
    editbox_credit_value_ = GameTools::createEditBoxFromTextField(img_credit_value_bg, textfield_credit_value_);
    editbox_credit_value_->setReturnType(EditBox::KeyboardReturnType::DONE);
    editbox_credit_value_->setMaxLength(editbox_credit_value_->getContentSize().width);
    editbox_credit_value_->setInputMode(cocos2d::ui::EditBox::InputMode::NUMERIC);
    editbox_credit_value_->setInputFlag(cocos2d::ui::EditBox::InputFlag::SENSITIVE);
    editbox_credit_value_->setFontColor(Color3B(255, 255, 255));
    editbox_credit_value_->setPlaceholderFontColor(Color3B(151, 151, 151));
    editbox_credit_value_->setPlaceHolder("");
    editbox_credit_value_->setDelegate(this);
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(LayerOfcClickPlayerPhoto::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(LayerOfcClickPlayerPhoto::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(LayerOfcClickPlayerPhoto::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    

    return true;
}
void LayerOfcClickPlayerPhoto::click_image_tool(msg::Game_Tool_Type type)
{
    log("click_tools----%d",type);
}
void LayerOfcClickPlayerPhoto::set_player_info(const msg::UserDetailInfo info, uint32_t group_id, uint32_t owner_id, uint32_t room_id,bool is_ofc)
{
    user_info_.Clear();
    user_info_.CopyFrom(info);
    owner_id_ = owner_id;
    room_id_ = room_id;
    is_ofc_ = is_ofc;
    
    text_user_name_->setString(user_info_.nickname());
    text_user_id_->setString(tools::local_string("GoodHand","好牌")+"ID："+tools::to_string(user_info_.userid()));
    if (user_info_.sex() == msg::Sex::FEMALE)
    {
        btn_female_->setVisible(true);
        btn_male_->setVisible(false);
    }
    else if (user_info_.sex() == msg::Sex::MALE)
    {
        btn_female_->setVisible(false);
        btn_male_->setVisible(true);
    }
//    auto &ofc_room_data =dynamic_cast<newOfcPokerRoom*>(this->getParent())->GetRoom();
//    if (ofc_room_data == nullptr)
//    {
//        cocos2d::log("NodeOfcPlayer::click_btn_kick_player ofc_room_data == nullptr");
//        
//        //        btn_kick_player_->setVisible(false);
//        
////        return;
//        btn_voice_play_->setVisible(true);
//        btn_voice_play_->setPositionX(Node_user_photo_->getPositionX());
//    }
//    else
//    {
        if ( GDM->get_user_id() != owner_id_ || user_info_.userid() == GDM->get_user_id())
        {
            btn_voice_play_->setVisible(true);
            btn_voice_play_->setPositionX(Node_user_photo_->getPositionX());
        }
        else
        {
            btn_voice_play_->setVisible(true);
            btn_tick_player_->setVisible(true);
        }

//    }
    
    group_id_ = group_id;
    
    layout_credit_value_->setSwallowTouches(false);
    layout_credit_value_->setVisible(false);
    
    this->addUserPhoto();

    auto processor = PM->GetUserPermission_up(group_id_, user_info_.userid());
    send_data(processor, REQ_GET_PERMISSION);
};
void LayerOfcClickPlayerPhoto::register_events()
{
    dispatch_.register_event(EVENT_DOWLOAD_FILE,
                             BIND_FUNC(this, LayerOfcClickPlayerPhoto::handle_download_event));
    dispatch_.register_event(REQ_CHANGE_PERMISSION, BIND_FUNC(this, LayerOfcClickPlayerPhoto::handle_change_permission));
    dispatch_.register_event(REQ_GET_PERMISSION, BIND_FUNC(this, LayerOfcClickPlayerPhoto::handle_get_permission));
}

void LayerOfcClickPlayerPhoto::handle_download_event(cmd_data_pointer data)
{
    auto s = std::dynamic_pointer_cast<cmd_data_string>(data);
    if (s->get_value() != user_info_.picname()) return;
    addUserPhoto();
}

void LayerOfcClickPlayerPhoto::handle_get_permission(cmd_data_pointer data)
{
    shared_ptr<net_data_recv_package> rec = dynamic_pointer_cast<net_data_recv_package>(data);
    if(!check_request(rec)) return;
    auto processor = PM->GetUserPermission_down(rec);
    
    if (processor.has_permission())
    {
        layout_credit_value_->setVisible(true);
        editbox_credit_value_->setText(tools::to_string(processor.permission().credit()).c_str());

    }else
    {
        layout_credit_value_->setVisible(false);
    }
}

void LayerOfcClickPlayerPhoto::handle_change_permission(cmd_data_pointer data)
{
    ClearError();
    std::shared_ptr<net_data_recv_package> rec = dynamic_pointer_cast<net_data_recv_package>(data);
    if(!check_request(rec)) return;
    auto processor = PM->response_down(rec);
    switch (processor.status()){
        case msg::Status::SUCCESS:
            TipView::showAlertView(tools::local_string("operate_successed","操作成功"));
            break;
        case msg::Status::FAILED:
            ShowError(tools::local_string("operate_failed","操作失败"));
            break;
        default:
            break;
    }
}

void LayerOfcClickPlayerPhoto::addUserPhoto()
{
    std::string strPicFile = "Default/image_defualt_user_icon_large.png";
    if (user_info_.picname() != "" && GDM->is_file_exist(user_info_.picname()) && GameTools::isValidPicture(user_info_.picname()))
    {
        strPicFile = user_info_.picname();
    }else
    {
        FM.download_file(user_info_.picname());
    }
    
    if (sprite_icon_ == nullptr)
    {
        sprite_icon_ = ShaderSprite::create(strPicFile, "Default/image_mask_circle.png");
        root_->addChild(sprite_icon_);
    }else
    {
        if (sprite_icon_->getFileName() != strPicFile)
        {
            sprite_icon_->initWithFile(strPicFile, "Default/image_mask_circle.png");
        }
    }
    sprite_icon_->setPosition(Node_user_photo_->getPosition());
    auto width=sprite_icon_->getContentSize().width;
    auto height=sprite_icon_->getContentSize().height;
    float scaleX=1;
    float scaleY=1;
    if (width>USER_PHOTO_IMAGE_WIDTH)
    {
        scaleX=USER_PHOTO_IMAGE_WIDTH/width;
    }
    
    if (height>USER_PHOTO_IMAGE_HEIGHT)
    {
        scaleY=USER_PHOTO_IMAGE_HEIGHT/height;
    }
    sprite_icon_->setScale(scaleX,scaleY);
}
void LayerOfcClickPlayerPhoto::onTouchEnded(Touch* touch, Event* event)
{
    //将openGL坐标转化为node坐标
    Point touchLocation = this->convertTouchToNodeSpace(touch);
    if (image_ofc_click_other_bg_->getBoundingBox().containsPoint(touchLocation))
    {
        
    }else
    {
        this->removeFromParent();
    }
}

#pragma mark - EditBoxDetegate
void LayerOfcClickPlayerPhoto::editBoxEditingDidBegin(EditBox* editBox)
{

}

void LayerOfcClickPlayerPhoto::editBoxEditingDidEnd(EditBox* editBox)
{

}

void LayerOfcClickPlayerPhoto::editBoxTextChanged(EditBox* editBox, const std::string& text)
{
    
}

void LayerOfcClickPlayerPhoto::editBoxReturn(EditBox* editBox)
{
    std::string strCredit = editbox_credit_value_->getText();
    
    if (strCredit.empty())
    {
        
    }else
    {
        int intCredit = tools::stod(strCredit);

        msg::GroupPermission permission;
        permission.set_credit(intCredit);
        permission.set_groupid(group_id_);
        auto processor = PM->changePermission_up(user_info_.userid(), permission);
        send_data(processor, REQ_CHANGE_PERMISSION);
    }
    

}

void LayerOfcClickPlayerPhoto::click_btn_voice_play(cocos2d::Ref *sender)
{
    std::string voice_name="";
    
    if ( is_ofc_ )
    {
        auto &ofc_room_data = dynamic_cast<newOfcPokerRoom*>(this->getParent())->GetRoom();
        voice_name = ofc_room_data->get_last_voice_name(user_info_.userid());
    }
    else
    {
        auto &ofc_room_data =dynamic_cast<PokerRoomLayer*>(this->getParent())->GetRoom();
        voice_name = ofc_room_data->get_last_voice_name(user_info_.userid());
    }
//    auto &ofc_room_data =dynamic_cast<newOfcPokerRoom*>(this->getParent())->GetRoom();
//    std::string voice_name = ofc_room_data->get_last_voice_name(user_info_.userid());
    if ( voice_name == "" )
    {
        TipView::showAlertView(tools::local_string("no_speak","该玩家没有说话"));
    }
    else
    {
        std::string amr_path = GDM->get_user_directory() + voice_name;

        std::string strFileNameAMR;
        auto pos = voice_name.find(".");
        strFileNameAMR = voice_name.substr(0, pos);
        strFileNameAMR += ".wav";
        
        std::string wav_path = GDM->get_user_directory() + strFileNameAMR;
        

        if(GDM->is_file_exist(amr_path))
        {
            std::string play_path;
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
            if (!GDM->is_file_exist(wav_path))
            {
                FM.get_recorder()->amrToWav(amr_path, wav_path);
            }
            play_path = wav_path;
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
            play_path = amr_path;
#endif
            AM->play_game_voice(play_path);
        }
    }
}

void LayerOfcClickPlayerPhoto::click_btn_tick_player(cocos2d::Ref *sender)
{
//    auto &ofc_room_data =dynamic_cast<newOfcPokerRoom*>(this->getParent())->GetRoom();
//    if (ofc_room_data == nullptr)
//    {
//        cocos2d::log("NodeOfcPlayer::click_btn_kick_player ofc_room_data == nullptr");
//        
////        btn_kick_player_->setVisible(false);
//        
//        return;
//    }
    
//    uint32_t roomid = ofc_room_data->get_id();
//    uint32_t ownerid = ofc_room_data->get_owner_id();
//    bool is_tournament = ofc_room_data->get_is_tournament();
    
    if (owner_id_ == GDM->get_user_id())
    {
        TipView* tip = TipView::showAlertView(tools::local_string("kick_off_players","确定要踢走该玩家吗？"));
        tip->show_cancel(true);
        tip->set_tipview_close_callback([=](){
            PDM->send_kick_player(room_id_, user_info_.userid());
        });
    }else
    {
        cocos2d::log("newOfcPokerRoom::click_btn_kick_player not is owner but click button!");
    }
//    btn_kick_player_->setVisible(false);
    this->removeFromParent();
}
