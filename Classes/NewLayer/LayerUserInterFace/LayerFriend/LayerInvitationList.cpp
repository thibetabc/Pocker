//
//  LayerInvitationList.cpp
//  pokerclient
//
//  Created by zhongyukeji on 16/4/11.
//
//

#include "LayerInvitationList.h"
#include "TipView.h"
#include "ProcessorManager.h"
#include "NET_CMD.h"
#include "GameDataManager.h"
#include "ShaderSprite.h"
#include "InvitationManager.h"
#include "PlatformSdk.h"
#include "LayerUserInfo.h"
#include "utils.h"
#include "LayerFriendList.h"
#include "ChatMessageManager.h"
#include "Toast.h"

bool LayerInvitationList::init()
{
    if(!BaseLayer::init())
    {
        return false;
    }
    auto root = CSLoader::createNode("LayerGroup_0.csb");
    this->addChild(root);
    
    auto btnBack = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "btn_back"));
    auto title = dynamic_cast<Text *>(CSLoader::seekNodeByName(root, "text_title"));
    auto btnSearch = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "btn_group_search"));
    auto btnCreate = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "btn_group_create"));
    auto listview = dynamic_cast<ListView*>(CSLoader::seekNodeByName(root, "listview"));
    
    btnSearch->setVisible(false);
    btnCreate->setVisible(false);
    btnBack->addClickEventListener([&](Ref *ref)
                                   {
                                       //刷新数据
                                       auto layer = dynamic_cast<LayerFriendList*>(this->getParent());
                                       if(layer)
                                       {
                                           layer->send_msg();
                                       }
                                       this->removeFromParent();
                                   });
    
    
    IM->clear_unread_invitation_count(); //清理未读消息
    mInvitations = IM->get_vec_invitations_(); //好友列表
    sortByTime();
    title->setString(tools::local_string("friend_request_message", "好友请求消息"));
    if (mInvitations.size() == 0 )
    {
        ShowTipMessage(true,tools::local_string("No_friend_message", "亲，当前暂无好友添加消息\n快去添加好友找其他小伙伴一起玩耍吧~"));
    }
    else
    {
        ShowTipMessage(false);
    }
    
    sizeOffset = Size::ZERO;
    mTableview = TableView::create(this, listview->getContentSize());
    mTableview->setAnchorPoint(listview->getAnchorPoint());
    mTableview->setContentOffset(Vec2(0, 0));
    mTableview->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    mTableview->setPosition(listview->getPosition());
    mTableview->setDelegate(this);
    mTableview->setClippingToBounds(true);
    root->addChild(mTableview);
    listview->removeFromParent();
    
    
    
    return true;
}


void LayerInvitationList::RefreshLayer()
{
    refresh_deal_data();
}

//请求好友列表同意回调
void LayerInvitationList::refresh_deal_data()
{
    mInvitations = IM->get_vec_invitations_();
    sortByTime();
    mTableview->reloadData();
    if(!sizeOffset.equals(Size::ZERO))
    {
        mTableview->setContentOffset(sizeOffset);
    }
}

#pragma mark - tableviewDatasource
TableViewCell* LayerInvitationList::tableCellAtIndex(TableView *table, ssize_t idx)
{
    TableViewCell *cell = table->dequeueCell();
    ImageView *imageBg;
    ShaderSprite *imgIcon;
    Text *textRemark;
    Sprite *spriteSex;
    if(!cell)
    {
        auto root = CSLoader::createNode("NodeGroupCell_0.csb");
        cell = new TableViewCell();
        cell->autorelease();
        cell->addChild(root);
        
        auto tmpImgIcon = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(cell, "img_group_head"));
        imageBg = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(cell, "img_group_cell"));
        imgIcon = ShaderSprite::createAndDownload("Default/image_defualt_user_icon_large.png", "Default/image_defualt_user_icon_large.png", "Default/image_mask_circle.png");
        imgIcon->setPosition(tmpImgIcon->getPosition());
        //调整cell的width
        root->setPositionX((DESIGN_WIDTH - imageBg->getContentSize().width)/2);
        root->addChild(imgIcon, tmpImgIcon->getLocalZOrder(), "shader_sprite");
        tmpImgIcon->removeFromParent();
        
        textRemark = dynamic_cast<Text*>(CSLoader::seekNodeByName(cell, "text_group_remark"));
        spriteSex = dynamic_cast<Sprite*>(CSLoader::seekNodeByName(cell, "sprite_sex"));
        textRemark->setPositionX(textRemark->getPositionX() + spriteSex->getContentSize().width / 2);
        
        
    }
    else
    {
        imageBg = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(cell, "img_group_cell"));
        imgIcon = dynamic_cast<ShaderSprite*>(CSLoader::seekNodeByName(cell, "shader_sprite"));
        spriteSex = dynamic_cast<Sprite*>(CSLoader::seekNodeByName(cell, "sprite_sex"));
        textRemark = dynamic_cast<Text*>(CSLoader::seekNodeByName(cell, "text_group_remark"));
    }
    
    
    
    //ui
    auto spriteChat = dynamic_cast<Sprite*>(CSLoader::seekNodeByName(cell, "img_chat"));
    auto spriteRedBall = dynamic_cast<Sprite*>(CSLoader::seekNodeByName(cell, "img_msg_tip"));
    auto textUnreadCount = dynamic_cast<Text*>(CSLoader::seekNodeByName(cell, "txt_msg_tip"));
    auto textName = dynamic_cast<Text*>(CSLoader::seekNodeByName(cell, "txt_group_name"));
    auto textLastestChat = dynamic_cast<Text*>(CSLoader::seekNodeByName(cell, "txt_group_latest_chat"));
    auto textTime = dynamic_cast<Text*>(CSLoader::seekNodeByName(cell, "text_time"));
    auto buttonOperate = dynamic_cast<Button*>(CSLoader::seekNodeByName(cell, "btn_operate"));
    auto spriteSelect = dynamic_cast<Sprite*>(CSLoader::seekNodeByName(cell,"sprite_select"));
    
    imageBg->loadTexture("image_club/club_group_bg.png");
    buttonOperate->setVisible(false);
    spriteSelect->setVisible(false);
    textTime->setVisible(false);
    spriteChat->setVisible(false);
    //好友消息
    auto data = mInvitations[idx];
    auto request = data.request();
    auto userInfo = request.userdetailinfo();
    auto status = data.status();
    imgIcon->setSpriteTexture(userInfo.picname());
    textName->setVisible(true);
    textName->setString(userInfo.nickname());
    textName->setVisible(false);
    spriteSex->setVisible(true);
    spriteSex->setTexture(userInfo.sex() == msg::Sex::MALE ? gMaleIcon : gFemaleIcon);
    textRemark->setVisible(true);
    switch (request.type()) {
        case msg::RequestType::ADDFRIEND:
            textRemark->setString(tools::local_string("request_add_friend","请求添加好友"));
            break;
        case msg::RequestType::ENTERGROUP:
            textRemark->setString(tools::local_string("ask_enter_club","请求加入俱乐部"));
            break;
        case msg::RequestType::INVITETOGROUP:
            textRemark->setString(tools::local_string("invite_enter_club","邀请你加入俱乐部"));
            break;
            
        default:
            break;
    }
    textLastestChat->setVisible(true);
    textLastestChat->setString("ID: " + tools::to_string(userInfo.userid()));
    buttonOperate->setVisible(true);
    if(status == msg::InvitationStatus::Accepted)
    {
        buttonOperate->setTitleText(tools::local_string("agreed","已同意"));
        buttonOperate->setEnabled(false);
    }
    else if(status == msg::InvitationStatus::Refused)
    {
        buttonOperate->setTitleText(tools::local_string("disagreed","已拒绝"));
        buttonOperate->setEnabled(false);
    }
    else
    {
        buttonOperate->setTitleText(tools::local_string("operate" , "同意"));
        buttonOperate->setEnabled(true);
        buttonOperate->addClickEventListener([&,request](Ref *ref)
                                             {
                                                 operation_request_id = request.id();
                                                 log("124协议同意添加好友 request_id = %d", operation_request_id);
                                                 //发送同意，接收在InvitationManager
                                                 auto processor = PM->DealRequest_up(operation_request_id, true);
                                                 send_data(processor, REQ_DEAL_REQUEST);
                                             });
    }
    
    
    
    
    return cell;
}

ssize_t LayerInvitationList::numberOfCellsInTableView(TableView *table)
{
    
    return mInvitations.size();
    
}

#pragma mark - tableviewDelegate
void LayerInvitationList::tableCellTouched(TableView* table, TableViewCell* cell)
{
    
    sizeOffset = table->getContentOffset();
    auto idx = cell->getIdx();
    log("idx = %zd", idx);
    auto request = mInvitations[idx].request();
    log("request.id = %d", request.id());
    IM->setID(request.id());
    addChild(LayerUserInfo::create(request.userdetailinfo()));
    
}

void LayerInvitationList::tableCellHighlight(TableView* table, TableViewCell* cell)
{
    
    auto imageBg = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(cell, "img_group_cell"));
    imageBg->loadTexture("image_club/club_group_bg_press.png");
}

void LayerInvitationList::tableCellUnhighlight(TableView* table, TableViewCell* cell)
{
    
    auto imageBg = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(cell, "img_group_cell"));
    imageBg->loadTexture("image_club/club_group_bg.png");
}

void LayerInvitationList::register_events()
{
    dispatch_.register_event(REQ_DEAL_REQUEST,
                             BIND_FUNC(this, LayerInvitationList::handle_deal_request));
    
}

void LayerInvitationList::handle_deal_request(cmd_data_pointer data)
{
    std::shared_ptr<net_data_recv_package> rec = dynamic_pointer_cast<net_data_recv_package>(data);
    if (!check_request(rec)) return;
    auto processor = PM->DealRequest_down(rec);
    switch (processor.response().status()) {
        case msg::Status::SUCCESS:
        {
            Toast::ToastShow(tools::local_string("request_has_aready_send","已发送请求"));
            IM->deal_request(processor.requestid(), processor.accepted());
            for (auto& iter: mInvitations){
                if (iter.request().id() == operation_request_id)
                    iter.set_status(processor.accepted() ? msg::InvitationStatus::Accepted : msg::InvitationStatus::Refused);
            }
            mInvitations = IM->get_vec_invitations_();
            sortByTime();
            if (mInvitations.size() == 0 )
            {
                ShowTipMessage(true,tools::local_string("No_friend_message","亲，当前暂无好友添加消息\n快去添加好友找其他小伙伴一起玩耍吧~"));
            }
            else
            {
                ShowTipMessage(false);
            }
            
            mTableview->reloadData();
            if(!sizeOffset.equals(Size::ZERO))
            {
                mTableview->setContentOffset(sizeOffset);
            }
        }
            break;
        case msg::Status::FAILED:
            Toast::ToastShow(tools::local_string("request_failed","请求失败"));
            break;
        default:
            break;
    }
}


void LayerInvitationList::sortByTime()
{
    //聊天时间排序
    auto sortFunc = [&](const struct msg::Invitation &data1, const struct msg::Invitation data2)
    {
        auto lastmessage1 = CM->get_last_message(data1.request().userdetailinfo().userid(), ChatMessageManager::MESSAGETYPE::FRIENDCHAT);
        if(!lastmessage1)
        {
            return false;
        }
        auto lastmessage2 = CM->get_last_message(data2.request().userdetailinfo().userid(), ChatMessageManager::MESSAGETYPE::FRIENDCHAT);
        if(!lastmessage2)
        {
            return true;
        }
        
        const std::string submit_time1 = lastmessage1->subtime();
        if(submit_time1.empty())
        {
            return false;
        }
        const std::string submit_time2 = lastmessage2->subtime();
        if(submit_time2.empty())
        {
            return true;
        }
        
        return tools::stod(submit_time1) >  tools::stod(submit_time2);
    };
    
    std::sort(mInvitations.begin(), mInvitations.end(), sortFunc);
}







