//
//  LayerUserInfo.cpp
//  pokerclient
//
//  Created by zhongyukeji on 16/4/11.
//
//

#include "LayerUserInfo.h"
#include "NET_CMD.h"
#include "TipView.h"
#include "GroupCell.h"
#include "utils.h"
#include "ProcessorManager.h"
#include "LayerChat.h"
#include "InvitationManager.h"
#include "GameDataManager.h"
#include "LayerInvitationList.h"
#include "GameTools.h"
#include "Toast.h"


LayerUserInfo::LayerUserInfo():isBlockMoment(false),mIsFriend(false)
{
    
}

LayerUserInfo *LayerUserInfo::create(msg::UserDetailInfo info)
{
    auto layer = new LayerUserInfo();
    layer->mInfo = info;
    layer->IsInvitation = false;
    for (auto iter: IM->get_invitations_()){
        if (iter.second.request().userdetailinfo().userid() == info.userid() && iter.second.status() == msg::InvitationStatus::Waiting && iter.second.request().type() == msg::RequestType::ADDFRIEND){
            layer->IsInvitation = true;
            break;
        }
    }
    if(layer && layer->init())
    {
        layer->autorelease();
        return layer;
    }
    else
    {
        delete layer;
        layer = nullptr;
        return nullptr;
    }
}


bool LayerUserInfo::init()
{
    if(!BaseLayer::init())
    {
        return false;
    }
    
    auto root = CSLoader::createNode("LayerFriendDetails.csb");
    addChild(root);
    auto btnBack = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "btn_back"));
    auto tmpImgIcon = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root, "image_user_avatar"));
    auto imgHead = ShaderSprite::createAndDownload("Default/image_defualt_user_icon_large.png", "Default/image_defualt_user_icon_large.png", "Default/image_mask_circle.png");
    imgHead->setPosition(tmpImgIcon->getPosition());
    root->addChild(imgHead, tmpImgIcon->getLocalZOrder());
    tmpImgIcon->removeFromParent();
    
    auto text_title= dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_title"));
    auto nickName = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "title_nick_name"));
    auto account = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "title_account"));
    auto spriteSex = dynamic_cast<Sprite*>(CSLoader::seekNodeByName(root, "sprite_sex"));
    auto textNickname = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_nike_name"));
    auto textAccount = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_account"));
    auto textSex = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "title_friend_session"));
    auto titleDiamond = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "title_diamond"));
    auto imgTextfieldBg = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root, "img_diamond_bg"));
    auto textfieldDiamond = dynamic_cast<TextField*>(CSLoader::seekNodeByName(root, "textfield_diamond"));
    mBtnConfirm = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "btn_diamond"));
    
    text_title->setString(tools::local_string("personal_info","个人资料"));
    nickName->setString(tools::local_string("nickname","昵称"));
    account->setString(tools::local_string("account","账号"));
    mBtnConfirm->setTitleText(tools::local_string("confirm_change_password","确定"));
    titleDiamond->setString(tools::local_string("give_diamond","赠送钻石"));
    mCheckboxHiteMoments = dynamic_cast<CheckBox*>(CSLoader::seekNodeByName(root, "checkbox_friend_session"));
    mBtnSendMsg = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "btn_send_message"));
    mBtnDeleteFriend = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "btn_delete_friend"));
    
    btnBack->addClickEventListener([&](Ref *ref)
    {
        if(IsInvitation)
        {
            auto layer = dynamic_cast<LayerInvitationList*>(this->getParent());
            if(layer)
            {
                layer->refresh_deal_data();
            }
        }
        this->removeFromParent();
    });
    imgHead->setSpriteTexture(mInfo.picname());
    spriteSex->setTexture(mInfo.sex() == msg::Sex::MALE ? gMaleIcon : gFemaleIcon);
    textSex->setString(mInfo.sex() == msg::Sex::MALE ? tools::local_string("block_his_moment", "屏蔽他的朋友圈") : tools::local_string("block_her_moment", "屏蔽她的朋友圈"));
    textNickname->setString(mInfo.nickname());
    spriteSex->setPositionX(textNickname->getPositionX() - textNickname->getContentSize().width - 30);
    textAccount->setString(tools::to_string(mInfo.userid()));
    mCheckboxHiteMoments->setSelected(false);
    setCheckBoxTextures(mCheckboxHiteMoments);
    mCheckboxHiteMoments->addClickEventListener([&](Ref *ref)
    {
        auto checkbox = dynamic_cast<CheckBox*>(ref);
        bool isSelect = checkbox->isSelected();
        
        
        
        int userId = mInfo.userid();
        log("屏蔽userId = %d", userId);
        auto processor = PM->BlockUser_up(userId, isSelect);
        send_data(processor, REQ_SHIELD_FRIEND);
    });
    mEditboxtDiamond = GameTools::createEditBoxFromTextField(imgTextfieldBg, textfieldDiamond);
    mEditboxtDiamond->setInputMode(EditBox::InputMode::NUMERIC);
    mEditboxtDiamond->setReturnType(EditBox::KeyboardReturnType::DONE);
    mEditboxtDiamond->setMaxLength(10);
    mEditboxtDiamond->setFontColor(Color3B::WHITE);
    mEditboxtDiamond->setPlaceHolder("0");
    mEditboxtDiamond->setDelegate(this);
    
    mBtnConfirm->addClickEventListener([&](Ref *ref){
        
        log("send Diamond: %s, len = %d", mEditboxtDiamond->getText(), strlen(mEditboxtDiamond->getText()));
        if(strlen(mEditboxtDiamond->getText()) <= 0 || !GameTools::isRegexMath(mEditboxtDiamond->getText(), GameTools::INPUT_TYPE::NUMBER))
        {
            Toast::ToastShow(tools::local_string("input_whole","只支持数字"));
            return;
        }
        GameTools::editBoxLoseInputFocus();
        requestDiamond();
        
    });
    //处理搜索消息
    if(!IsInvitation)
        InitNormal();
    else
    {
        
        //处理请求消息
        mBtnSendMsg->setTitleText(tools::local_string("operate", "同意"));
        mBtnSendMsg->addClickEventListener([&](Ref *ref)
                                          {
                                              sendInvitationFriend(true);
                                          });
        mBtnDeleteFriend->setTitleText(tools::local_string("disagree_friend_request", "拒绝"));
        mBtnDeleteFriend->addClickEventListener([&](Ref *ref)
                                          {
                                              sendInvitationFriend(false);
                                          });
        
    }
    send_data(PM->null_up(), REQ_MOMENT_BLOCK_LIST);//每次都请求已屏蔽的朋友圈列表
    return true;
}

void LayerUserInfo::InitNormal()
{
    mBtnSendMsg->setTitleText(tools::local_string("send_friend_message", "发送消息"));
    mBtnDeleteFriend->setTitleText(tools::local_string("delete_friend","删除好友"));
    //本人
    if(mInfo.userid() == GDM->get_user_id())
    {
        mBtnSendMsg->setVisible(false);
        mBtnDeleteFriend->setVisible(false);
        mCheckboxHiteMoments->setVisible(false);
        mBtnConfirm->setVisible(false);
    }
    else
    {
        mBtnSendMsg->setVisible(false);
        mBtnSendMsg->addClickEventListener([&](Ref *ref)
                                           {
                                               if(!mIsFriend)
                                               {
                                                   sendAddFriend();
                                               }
                                               else
                                               {
                                                   
                                                   log("发送消息");
                                                   auto layer = dynamic_cast<LayerChat*>(SceneManager::get_layer_by_name(typeid(LayerChat).name()));
                                                   if(layer)
                                                   {
                                                       layer->removeFromParent();
                                                   }
                                                   
                                                   LayerChat* glc = LayerChat::createByFriend(mInfo);
                                                   glc->set_show_dir(LAYER_SHOW_DIRECTION::RIGHT_TO_LEFT);
                                                   AddGlobalLayer(glc);
                                                    
                                                   auto layer1 = dynamic_cast<LayerUserInfo*>(SceneManager::get_layer_by_name(typeid(LayerUserInfo).name()));
                                                   if(layer1)
                                                   {
                                                       layer1->removeFromParent();
                                                   }
                                                   
                                               }
                                           });
        
        mBtnDeleteFriend->setVisible(false);
        mBtnDeleteFriend->addClickEventListener([&](Ref *ref)
                                                {
                                                    sendDelFriend();
                                                });
        sendFriendList();
    }
}

#pragma mark - EditBoxDelegate
void LayerUserInfo::editBoxEditingDidEnd(EditBox* editBox)
{
    
}

void LayerUserInfo::editBoxReturn(EditBox* editBox)
{
    mEditboxtDiamond->setText(editBox->getText());
    log("input: %s", mEditboxtDiamond->getText());
}

void LayerUserInfo::setCheckBoxTextures(CheckBox *checkbox)
{
    const std::string CHECKBOX_SELECT = "login_res/creategameroom_res/checkbox_cell_selected.png";
    const std::string CHECKBOX_UNSELECT = "login_res/creategameroom_res/checkbox_cell_unselected.png";
    if(!checkbox->isSelected())
    {
        checkbox->loadTextures(CHECKBOX_SELECT, CHECKBOX_SELECT, CHECKBOX_SELECT, CHECKBOX_SELECT, CHECKBOX_SELECT);
    }
    else
    {
        checkbox->loadTextures(CHECKBOX_UNSELECT, CHECKBOX_UNSELECT, CHECKBOX_UNSELECT, CHECKBOX_UNSELECT, CHECKBOX_UNSELECT);
    }
}

void LayerUserInfo::RefreshLayer()
{
    
    if(!IsInvitation)
    {
      sendFriendList();
    }
    send_data(PM->null_up(), REQ_MOMENT_BLOCK_LIST);//每次都请求已屏蔽的朋友圈列表
    
}

void LayerUserInfo::register_events()
{
    
    dispatch_.register_event(REQ_MOMENT_BLOCK_LIST,
                             BIND_FUNC(this, LayerUserInfo::handle_all_block_user_list));
    dispatch_.register_event(REQ_ADD_FRIEND,
                             BIND_FUNC(this, LayerUserInfo::handle_add_friend));
    dispatch_.register_event(REQ_DELETE_FRIEND,
                             BIND_FUNC(this, LayerUserInfo::handle_delete_friend));
    dispatch_.register_event(REQ_SHIELD_FRIEND,
                             BIND_FUNC(this, LayerUserInfo::handle_block_user));
    dispatch_.register_event(REQ_DEAL_REQUEST,
                             BIND_FUNC(this, LayerUserInfo::handle_deal_request));
    //SearchFriend
    dispatch_.register_event(REQ_GET_FRIENDS_LIST,
                             BIND_FUNC(this, LayerUserInfo::handle_get_friends_list));
    dispatch_.register_event(REQ_GIFT_DIAMOUNT, BIND_FUNC(this, LayerUserInfo::receiverDiamond));
}



void LayerUserInfo::sendAddFriend()
{
    //添加好友
    msg::Request tRequest;
    tRequest.set_type(msg::RequestType::ADDFRIEND);
    tRequest.mutable_userdetailinfo()->set_userid(GDM->get_user_id());
    int userId = mInfo.userid();
    tRequest.set_receiverid(userId);
    log("添加好友，userid = %d, requestId = %d ", GDM->get_user_id(), userId);
    
    auto processor = PM->SendRequest_up(tRequest);
    send_data(processor, REQ_ADD_FRIEND);
    
}

void LayerUserInfo::sendDelFriend()
{
    auto processor = PM->DeleteFriend_up(mInfo.userid());
    send_data(processor, REQ_DELETE_FRIEND);
}

//同意/拒绝好友申请
void LayerUserInfo::sendInvitationFriend(bool isAgree)
{
    log("124协议【%s】好友 request_id = %d", isAgree ? "同意" : "拒绝", IM->getID());
    auto processor = PM->DealRequest_up(IM->getID(), isAgree);
    send_data(processor, REQ_DEAL_REQUEST);
}

void LayerUserInfo::sendFriendList()
{
    msg::Response processor;
    processor.set_status(msg::Status::SUCCESS);
    send_data(processor,REQ_GET_FRIENDS_LIST);
}

void LayerUserInfo::requestDiamond()
{
    int amount = atoi(mEditboxtDiamond->getText());
    auto processer = PM->Send_Diamond_UP(mInfo.userid(), amount);
    send_data(processer, REQ_GIFT_DIAMOUNT);
}

void LayerUserInfo::receiverDiamond(cmd_data_pointer data)
{
    std::shared_ptr<net_data_recv_package> rec = dynamic_pointer_cast<net_data_recv_package>(data);
    if (!check_request(rec)) return;
    auto processor = PM->response_down(rec);
    switch (processor.status()) {
        case msg::Status::SUCCESS: {
            Toast::ToastShow(tools::local_string("give_successfully","赠送成功"));
            break;
        }
        case msg::Status::FAILED:
            Toast::ToastShow(processor.message());
            break;
        default:
            break;
    }
}




void LayerUserInfo::handle_deal_request(cmd_data_pointer data)
{
    std::shared_ptr<net_data_recv_package> rec = dynamic_pointer_cast<net_data_recv_package>(data);
    if (!check_request(rec)) return;
    auto processor = PM->DealRequest_down(rec);
    
    switch (processor.response().status()) {
        case msg::Status::SUCCESS:
            Toast::ToastShow(tools::local_string("request_has_aready_send","已发送请求"));
            IM->deal_request(processor.requestid(), processor.accepted());
            break;
        case msg::Status::FAILED:
            Toast::ToastShow(tools::local_string("request_failed","请求失败"));
            break;
            
        default:
            break;
    }
    
    InitNormal();
}

//接收已屏蔽的朋友圈列表
void LayerUserInfo::handle_all_block_user_list(cmd_data_pointer data)
{
    std::shared_ptr<net_data_recv_package> rec = dynamic_pointer_cast<net_data_recv_package>(data);
    if (!check_request(rec)) return;
    auto processor = PM->GetMomentBlockUsers_down(rec);
    
    isBlockMoment = true;
    for (auto userInfo: processor.users())
    {
        if(userInfo.userid() == mInfo.userid())
        {
            isBlockMoment = false;
            break;
        }
    }
    
    mCheckboxHiteMoments->setSelected(isBlockMoment);
    setCheckBoxTextures(mCheckboxHiteMoments);
    
}

// 添加好友
void LayerUserInfo::handle_add_friend(cmd_data_pointer data)
{
    std::shared_ptr<net_data_recv_package> rec = dynamic_pointer_cast<net_data_recv_package>(data);
    if (!check_request(rec)) return;
    auto processor = PM->response_down(rec);
    TipView::showAlertView(processor.message());
    
}

//删除好友
void LayerUserInfo::handle_delete_friend(cmd_data_pointer data) 
{
    ClearError();
    std::shared_ptr<net_data_recv_package> rec = dynamic_pointer_cast<net_data_recv_package>(data);
    if (!check_request(rec)) return;
    auto processor = PM->response_down(rec);
    switch (processor.status()) {
        case msg::Status::SUCCESS: {
            Toast::ToastShow(tools::local_string("delete_friend_successed","删除好友成功"));
            InitNormal();
            break;
        }
        case msg::Status::FAILED:
            Toast::ToastShow(processor.message());
            break;
        default:
            break;
    }
}

//屏蔽好友
void LayerUserInfo::handle_block_user(cmd_data_pointer data)
{
    ClearError();
    std::shared_ptr<net_data_recv_package> rec = dynamic_pointer_cast<net_data_recv_package>(data);
    if (!check_request(rec)) return;
    auto processor = PM->response_down(rec);
    switch (processor.status()) {
        case msg::Status::SUCCESS: {
            // 获取约友圈
//            if(GameDataManager::TYPE_SQUARE == GDM->circletype){
//                GDM->send_get_moment(0, 0, msg::MomentDisplayType::SQUARE);
//            }
//            else if(GameDataManager::TYPE_RECOMMEND == GDM->circletype){
//                GDM->send_get_moment(0, 0, msg::MomentDisplayType::RECOMMEND);
//            }
//            else if(GameDataManager::TYPE_FRIEND == GDM->circletype){
//                GDM->send_get_moment(0, 0, msg::MomentDisplayType::FRIEND);
//            }
            isBlockMoment = !isBlockMoment;
            mCheckboxHiteMoments->setSelected(isBlockMoment);
            setCheckBoxTextures(mCheckboxHiteMoments);
            break;
        }
        case msg::Status::FAILED:
            ShowError(tools::local_string("operate_failed","操作失败"));
            break;
        default:
            break;
    }

}

//获得好友列表
void LayerUserInfo::handle_get_friends_list(cmd_data_pointer data)
{
    std::shared_ptr<net_data_recv_package> rec = dynamic_pointer_cast<net_data_recv_package>(data);
    if (!check_request(rec)) return;
    auto processor = PM->getFriendList_down(rec);
    switch (processor.response().status()) {
        case msg::Status::SUCCESS: {
            
            bool isFriend = false;
            auto friendVec = processor.friendinfo();
            for(auto tFriend : friendVec)
            {
                if(tFriend.userdetailinfo().userid() == mInfo.userid())
                {
                    isFriend = true;
                    break;
                }
            }
            mIsFriend = isFriend;
            mBtnSendMsg->setVisible(true);
            if(mIsFriend)
            {
                mBtnDeleteFriend->setVisible(true);
            }
            else
            {
                mBtnSendMsg->setTitleText(tools::local_string("add_friend","添加好友"));
            }
            break;
        }
        case msg::Status::FAILED:
            break;
        default:
            break;
    }
}



