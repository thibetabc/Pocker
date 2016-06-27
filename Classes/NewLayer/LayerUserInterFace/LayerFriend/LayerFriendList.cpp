//
//  LayerFriendList.cpp
//  pokerclient
//
//  Created by zhongyukeji on 16/4/11.
//
//

#include "LayerFriendList.h"
#include "TipView.h"
#include "utils.h"
#include "LayerSearchUser.h"
#include "LayerInvitationList.h"
#include "LayerUserInfo.h"
#include "InvitationManager.h"
#include "LayerChat.h"
#include "GameDataManager.h"
#include "ChatMessageManager.h"
#include "ShaderSprite.h"
#include "LayerPublichPriendsCircle.h"
#include "LayerShowImage.h"
LayerFriendList::LayerFriendList():mType(Type::NONE),select_callback_(nullptr),mLastIndex(0),sizeOffset(Size::ZERO)
{
    
}

LayerFriendList *LayerFriendList::create(Type type, std::vector<uint32_t> invalidId)
{
    auto layer = new LayerFriendList;
    layer->mType = type;
    layer->mInvalidId = invalidId;
    if(layer && layer->init())
    {
        layer->autorelease();
        return layer;
    }
    
    delete layer;
    layer = nullptr;
    return nullptr;
}

bool LayerFriendList::init()
{
    if(!BaseLayer::init())
    {
        return false;
    }
    is_show_edit=false;
    auto root = CSLoader::createNode("LayerFriendList.csb");
    this->addChild(root);
    auto btnBack = (Button *)root->getChildByName("btn_back");
    
    title_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_title"));
    title_->setString(tools::local_string("my_friend","好友"));
    
    btnSearchFriend = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "btn_group_create"));
    btnNewFriend = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "btn_new_friend"));
    auto titleNewFriend = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "txt_group_latest_chat"));
    titleNewFriend->setString(tools::local_string("friend_request_message","好友请求消息"));
    imgNewFriendMsgIcon = dynamic_cast<Sprite*>(CSLoader::seekNodeByName(root, "img_char"));
    textNewFriendMsgUnreadCount = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "txt_msg_tip"));
    auto textOk = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_ok"));
    auto listview = root->getChildByName("listview");
    textOk->setVisible(false);
    btnFriendCircle =dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "btn_friend_circle"));
    btnAddFriends =dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "btn_add_friends"));
    layout_friend=dynamic_cast<Layout*>(CSLoader::seekNodeByName(root, "Panel_friend"));
    layout_circle=dynamic_cast<Layout*>(CSLoader::seekNodeByName(root, "Panel_circle"));
    text_btn_circle= dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "Text_btn_circle"));
    text_btn_friends= dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "Text_btn_friends"));
    text_btn_circle->setString(tools::local_string("friends_circle","朋友圈"));
    text_btn_friends->setString(tools::local_string("my_friend","好友"));
    image_chat_bar_bg = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root, "image_chat_bar_bg"));
    btn_send = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "btn_send"));
    image_input_bg = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root, "image_input_bg"));
    btn_send->setTitleText(tools::local_string("send","发送"));
    text_tip_title= dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_tip_title"));
    tf_input = dynamic_cast<TextField*>(CSLoader::seekNodeByName(root, "tf_input"));
    image_chat_bar_bg->setVisible(false);
    layout_friend->setTouchEnabled(true);
    layout_circle->setTouchEnabled(true);
    text_btn_friends->setColor(Color3B(76, 96, 115));
    text_btn_circle->setColor(Color3B(255, 255, 255));
    btnFriendCircle->setVisible(false);
    btnAddFriends->setVisible(false);
    image_chat_bar_bg->setLocalZOrder(9999);
    image_chat_bar_bg->setSwallowTouches(true);
    
    
    ed_property_value_ = GameTools::createEditBoxFromTextField(image_chat_bar_bg, tf_input);
    ed_property_value_->setReturnType(EditBox::KeyboardReturnType::DONE);
    ed_property_value_->setMaxLength(tf_input->getContentSize().width);
    ed_property_value_->setInputFlag(cocos2d::ui::EditBox::InputFlag::SENSITIVE);
    //ed_property_value_->setInputMode(cocos2d::ui::EditBox::InputMode::DECIMAL );
    ed_property_value_->setFontColor(Color3B(255, 255, 255));
    ed_property_value_->setPlaceholderFontColor(Color3B(151, 151, 151));
    
    ed_property_value_->setVisible(false);
    ed_property_value_->setDelegate(this);
    
    ed_property_value_->setSwallowTouches(true);
    
    auto& size = Director::getInstance()->getWinSize();
    auto layout = Layout::create();
    layout->setContentSize(size);
    layout->setTouchEnabled(true);
    layout->setAnchorPoint(Point::ZERO);
    layout->setSwallowTouches(false);
    layout->addClickEventListener([this](Ref*){
        image_chat_bar_bg->setVisible(false);
        GameTools::editBoxLoseInputFocus();
    });
    root->addChild(layout, 9000);

    
    is_edit_=true;
    
    if (!is_edit_)
    {
        ed_property_value_->setTouchEnabled(false);
    }
    inputCommentInfo="";
    btn_send->addClickEventListener([&](Ref *ref)
                                    {
                                       this->send_comment_req();//发送评论
                                    });
    //返回
    btnBack->addClickEventListener([&](Ref *ref)
    {
        this->removeFromParent();
    });
    btnSearchFriend->addClickEventListener([&](Ref *)
    {
        //搜索好友
        printf("123123");
        this->addChild(LayerSearchUser::create());
    });
    btnNewFriend->addClickEventListener([&](Ref *ref)
    {
        //好友消息,有 同意/已添加 按钮
        this->addChild(LayerInvitationList::create());
    });
    int count = IM->get_unread_invitation_count();
    imgNewFriendMsgIcon->setVisible(count != 0);
    textNewFriendMsgUnreadCount->setString(tools::to_string(count));
    
    ifShowFriendCircle=false;
    
    
    auto friend_circle_listview =root->getChildByName("friend_circle_listView");
    friend_circle_listview->setLocalZOrder(900);
    auto layerfriendcircleinfo =new LayerFriendsCircleDetail;
    layerfriendinfo =layerfriendcircleinfo->create();
    layerfriendinfo->setPosition(friend_circle_listview->getPosition());
    //this->addChild(layerfriendinfo);
    root->addChild(layerfriendinfo);
    layerfriendinfo->setVisible(false);
    layerfriendinfo->setLocalZOrder(900);
    
    friend_circle_listview->removeFromParent();
    
    listview->setLocalZOrder(990);
    
    tableview_ = TableView::create(this,listview->getContentSize());
    tableview_->setAnchorPoint(listview->getAnchorPoint());
    tableview_->setContentOffset(Vec2(0, 0));
    tableview_->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    tableview_->setPosition(listview->getPosition());
    tableview_->setDelegate(this);
    tableview_->setClippingToBounds(true);
    tableview_->setTag(200);
    tableview_->setDataSource(this);
    root->addChild(tableview_);
    tableview_->setLocalZOrder(990);
    
    tableview_->setVisible(true);
    btnNewFriend->setVisible(false);
    layout_circle->addClickEventListener([&](Ref *ref)
                                        {
                                            this->getOnlineData(false);
                                            ifShowFriendCircle=true;
                                            this->setButtonState();
                                            btnNewFriend->setVisible(false);
                                            tableview_->setVisible(false);
                                            layerfriendinfo->setVisible(true);
                                            if(ifShowFriendCircle==true){return;}
                                            else
                                            {
                                                ifShowFriendCircle=true;
                                                this->setShowFriendCircle(true);
                                            }
                                        });
    layout_friend->addClickEventListener([&](Ref *ref)
                                           {
                                               ifShowFriendCircle=false;
                                               this->setButtonState();
                                               tableview_->setVisible(true);
                                               btnNewFriend->setVisible(true);
                                               layerfriendinfo->setVisible(false);
                                               if(ifShowFriendCircle==false){return;}
                                               else
                                               {
                                                   ifShowFriendCircle=false;
                                                   this->setShowFriendCircle(false);
                                               }
                                           });
    listview->removeFromParent();
    this->setButtonState();
    
    if(Type::SELECT_SINGLE == mType || Type::SELECT_MULTIPLE == mType)
    {
       
        auto title = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_title"));
        title->setText(tools::local_string("my_friend","好友"));
        btnNewFriend->setVisible(false);
        btnSearchFriend->setVisible(false);
        if(Type::SELECT_MULTIPLE == mType)
        {
            title->setString(tools::local_string("club_inviting","俱乐部邀请"));
        }
        
        textOk->setVisible(true);
        textOk->setString(tools::local_string("confirm","确认"));
        textOk->addClickEventListener([&](Ref *ref)
        {
            if(select_callback_)
            {
                std::vector<msg::UserDetailInfo> tUserDetailInfo;
                for(auto iter : mCustomData)
                {
                    if(iter.isValid && iter.isSelect)
                    {
                        tUserDetailInfo.push_back(iter.userDetailInfo);
                        
                        log("返回好友名字：%s, id = %d", iter.userDetailInfo.nickname().c_str(),iter.userDetailInfo.userid());
                    }
                }
                select_callback_(tUserDetailInfo);
            }
        });
        
    }
   // this->getOnlineData(false);//获取朋友圈网络信息
    send_msg();
    this->setShowFriendCircle(false);
    
    return true;
}

void LayerFriendList::setButtonState()
{
    
    if(ifShowFriendCircle)
    {
        text_btn_friends->setColor(Color3B(255, 255, 255));
        text_btn_circle->setColor(Color3B(76, 96, 115));
        layout_friend->setTouchEnabled(true);
        layout_circle->setTouchEnabled(false);
        btnSearchFriend->addClickEventListener([&](Ref *ref)
                                            {
                                                //好友消息,有 同意/已添加 按钮
                                                this->addChild(LayerPublichPriendsCircle::create());
                                            });
        title_->setString(tools::local_string("friends_circle","朋友圈"));
    }
    else
    {
        text_btn_friends->setColor(Color3B(76, 96, 115));
        text_btn_circle->setColor(Color3B(255, 255, 255));
        layout_friend->setTouchEnabled(false);
        layout_circle->setTouchEnabled(true);
        btnSearchFriend->addClickEventListener([&](Ref *ref)
                                            {
                                                //好友消息,有 同意/已添加 按钮
                                                this->addChild(LayerSearchUser::create());
                                            });
        title_->setString(tools::local_string("my_friend","好友"));
    }
        
}

void LayerFriendList::handle_get_comment_req(cmd_data_pointer data)
{
    ShowTipMessage(false);
    std::shared_ptr<net_data_recv_package> rec=dynamic_pointer_cast<net_data_recv_package>(data);
    if(!check_request(rec)) return;
    
    auto processor = PM->response_down(rec);
    
        switch (processor.status()){
            case msg::Status::SUCCESS:
                ed_property_value_->setVisible(false);
                image_chat_bar_bg->setVisible(false);
                GameTools::editBoxLoseInputFocus();
                Toast::ToastShow(tools::local_string("operate_successed","操作成功"));
                //this->sendComment(momentID_);
                this->getOnlineData(true);
                layerfriendinfo->tableview_->reloadData();
                break;
            case msg::Status::FAILED:
                Toast::ToastShow(tools::local_string("operate_failed","操作失败"));
                break;
            default:
                break;
        }

}

void LayerFriendList::sendComment(int commentID)
{
    msg::Processor_204_GetComment_UP processor; //发布评论
    msg::Request request;
    
    processor.set_momentid(momentID_);
    send_data(processor, REQ_GET_COMMENT);
    
    
}
void LayerFriendList::send_comment_req()//发送评论
{
    this->editBoxReturn(ed_property_value_);
    if(inputCommentInfo=="") return;
    
    msg::Processor_203_PublishComment processor; //发布评论
    auto myProcessor = processor.mutable_commentinfo();
    
    myProcessor->set_momentid(momentID_);
    if(sendType_==2)
    {
        myProcessor->set_type(::msg::CommentType::RESPOND);
        myProcessor->set_parentcomment(parentID_);
    }
    if(sendType_==1)
    {
        myProcessor->set_type(::msg::CommentType::COMMENT);
    }
    myProcessor->set_content(inputCommentInfo);
    send_data(processor, REQ_PUBLISH_COMMENT);
}

void LayerFriendList::RefreshLayer()
{
    send_msg();
    int count = IM->get_unread_invitation_count();
    imgNewFriendMsgIcon->setVisible(count != 0);
    textNewFriendMsgUnreadCount->setString(tools::to_string(count));
}

void LayerFriendList::update_unread_invitation_count()
{
    int count = IM->get_unread_invitation_count();
    imgNewFriendMsgIcon->setVisible(count != 0);
    textNewFriendMsgUnreadCount->setString(tools::to_string(count));
}

//网络请求
void LayerFriendList::getOnlineData(bool ifResetTableviewData)
{
    printf("get online data");
    
    if(ifResetTableviewData==false)
    {
        circle_posY=0;
    }
    else
    {
        float posY = layerfriendinfo->tableview_->getContainer()->getPositionY();
        circle_posY = posY;
    }
    uint32_t momentID=0;
    bool isDown=false;
    momentType_=msg::MomentDisplayType::FRIEND;
    auto processor=PM->GetMoment_UP(momentType_, momentID, isDown);
    send_data(processor, REQ_GET_MOMENT);
    
}

void LayerFriendList::send_circle_info()
{
//    msg::Moment moment;
//    msg::Processor_200_PublishMoment processor;
//    msg::Request request;
//    processor.mutable_moment()->set_displaytype(msg::MomentDisplayType::MINE);
////    processor.mutable_moment()->set_allocated_momentinfo(&momentInfo);
//    processor.mutable_moment()->mutable_momentinfo()->set_momenttype(1);
//    processor.mutable_moment()->mutable_momentinfo()->mutable_normalmomentinfo()->set_wordcontent("123456");
////    processor.mutable_moment()->Add()->CopyFrom(moment);
////    auto processor=PM->PublishMoment_UP(moment);
//    send_data(processor, REQ_PUBLISH_MOMENT);
    
}

void LayerFriendList::update_unread_message_count()
{
    Vector<TableViewCell*> cellsUsed = tableview_->get_cellsUsed();
    if (cellsUsed.size() > 0)
    {
        for (auto cell_iter : cellsUsed)
        {
            
            auto idx = cell_iter->getIdx();
            auto userInfo = mCustomData[idx].userDetailInfo;
            uint32_t unread_message_count = CM->get_unread_message_count(userInfo.userid(), ChatMessageManager::MESSAGETYPE::FRIENDCHAT);
            
            auto spriteRedBall = dynamic_cast<Sprite*>(CSLoader::seekNodeByName(cell_iter, "img_msg_tip"));
            auto textUnreadCount = dynamic_cast<Text*>(CSLoader::seekNodeByName(cell_iter, "txt_msg_tip"));
            spriteRedBall->setVisible(unread_message_count != 0);
            textUnreadCount->setString(tools::to_string(unread_message_count));
        }
    }
}


TableViewCell* LayerFriendList::tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx)
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
        textRemark->setPositionX(textRemark->getPositionX() + 40);
        spriteSex = dynamic_cast<Sprite*>(CSLoader::seekNodeByName(cell, "sprite_sex"));
        
    }
    else
    {
        
        imageBg = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(cell, "img_group_cell"));
        imgIcon = dynamic_cast<ShaderSprite*>(CSLoader::seekNodeByName(cell, "shader_sprite"));
        spriteSex = dynamic_cast<Sprite*>(CSLoader::seekNodeByName(cell, "sprite_sex"));
        textRemark = dynamic_cast<Text*>(CSLoader::seekNodeByName(cell, "text_group_remark"));
        
    }
    
    
    //data
    auto info = mCustomData[idx].userDetailInfo;
    auto userId = info.userid();
    
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
    imgIcon->setSpriteTexture(info.picname());//头像
    
    spriteSex->setTexture((info.sex() == msg::Sex::MALE ) ? gMaleIcon : gFemaleIcon);
    textName->setString(info.nickname());
    textRemark->setString("ID: " + tools::boost_any_to_string(userId));
    buttonOperate->setVisible(false);
    spriteSelect->setVisible(false);
    
    
    auto lastmessage = CM->get_last_message(userId, ChatMessageManager::MESSAGETYPE::FRIENDCHAT);
    if(lastmessage)
    {
        
        std::string str ="";
        if (lastmessage->has_userdetailinfo()){
            str.append(lastmessage->userdetailinfo().nickname().c_str());
            str.append(":");
        }
        if (lastmessage->messageinfo().type() == msg::MessageType::Text)
            str.append(lastmessage->messageinfo().textinfo().text().c_str());
        else if (lastmessage->messageinfo().type() == msg::MessageType::Picture)
            str.append(tools::local_string("Received_picture", "收到图片"));
        else if (lastmessage->messageinfo().type() == msg::MessageType::Voice)
            str.append(tools::local_string("Received_voice", "收到语音"));
        //最后聊天信息
        textLastestChat->setString(str.empty() ? " " : str);
        
        //最后聊天时间
        const std::string submit_time = lastmessage->subtime();
        std::string text = "";
        if(!submit_time.empty()) {
            double time = tools::stod(submit_time);
            time /= 1000;
            text = tools::time_display(time);
        }
        textTime->setString(text);
    }
    else
    {
        
        textTime->setString(" ");
        textLastestChat->setString(" ");
    }
    
    //未读消息
    uint32_t unread_message_count = CM->get_unread_message_count(userId, ChatMessageManager::MESSAGETYPE::FRIENDCHAT);
    spriteRedBall->setVisible(unread_message_count != 0);
    textUnreadCount->setString(tools::to_string(unread_message_count));
    
    if(Type::SELECT_SINGLE == mType || Type::SELECT_MULTIPLE == mType)
    {
        textLastestChat->setVisible(false);
        textTime->setVisible(false);
        spriteRedBall->setVisible(false);
        spriteChat->setVisible(false);
        spriteSelect->setVisible(true);
        
        if(mCustomData[idx].isValid)
        {
            spriteSelect->setTexture(mCustomData[idx].isSelect ? "login_res/selftrackrecord/btn_choose_status.png" : "login_res/selftrackrecord/btn_not_choose_status_normal.png");
        }
        else
        {
            spriteSelect->setTexture("login_res/selftrackrecord/btn_not_choose_status_invaild.png");//不可选
        }
    }
    
    
    return cell;
    
}

ssize_t LayerFriendList::numberOfCellsInTableView(cocos2d::extension::TableView *table)
{
    return  mCustomData.size();
}

void LayerFriendList::tableCellTouched(TableView* table, TableViewCell* cell)
{
    log("sadhjsakdha");
    if(mType == Type::NONE)
    {
        sizeOffset = table->getContentOffset();
        log("进入俱乐部聊天");
        
        auto friendInfo = mCustomData[cell->getIdx()].userDetailInfo;
        LayerChat* glc = LayerChat::createByFriend(friendInfo);
        glc->set_show_dir(LAYER_SHOW_DIRECTION::RIGHT_TO_LEFT);
        AddGlobalLayer(glc);
    }
    else
    {
        auto spriteSelect = dynamic_cast<Sprite*>(CSLoader::seekNodeByName(cell,"sprite_select"));
        auto idx = cell->getIdx();
        if(mCustomData[idx].isValid)
        {
            mCustomData[idx].isSelect = !mCustomData[idx].isSelect;
            if(mCustomData[idx].isValid)
            {
                spriteSelect->setTexture(mCustomData[idx].isSelect ? "login_res/selftrackrecord/btn_choose_status.png" : "login_res/selftrackrecord/btn_not_choose_status_normal.png");
            }
            else
            {
                spriteSelect->setTexture("login_res/selftrackrecord/btn_not_choose_status_invaild.png");//不可选
            }
            if(mType == Type::SELECT_SINGLE)
            {
               
                if (mLastIndex != idx)
                {
                    auto cell = table->cellAtIndex(mLastIndex);
                    if(cell)
                    {
                        auto spriteSelect = dynamic_cast<Sprite*>(CSLoader::seekNodeByName(cell,"sprite_select"));
                        if(spriteSelect)
                        {
                            spriteSelect->setTexture("login_res/selftrackrecord/btn_not_choose_status_normal.png");
                        }
                    }
                    mCustomData[mLastIndex].isSelect = false;
                    mLastIndex = idx;
                }
            }
        }
    }
}

void LayerFriendList::tableCellHighlight(TableView* table, TableViewCell* cell)
{
    auto imageBg = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(cell, "img_group_cell"));
    imageBg->loadTexture("image_club/club_group_bg_press.png");
}

void LayerFriendList::tableCellUnhighlight(TableView* table, TableViewCell* cell)
{
    auto imageBg = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(cell, "img_group_cell"));
    imageBg->loadTexture("image_club/club_group_bg.png");
}

//是否在无效列表里
bool LayerFriendList::isInValidList(uint32_t userId)
{
    for(auto iter : mInvalidId)
    {
        if(userId == iter)
        {
            return true;
        }
    }
    return false;
}

#pragma mark - protobuf

void LayerFriendList::send_msg()
{
    msg::Response processor;
    processor.set_status(msg::Status::SUCCESS);
    send_data(processor,REQ_GET_FRIENDS_LIST);
    
}


void LayerFriendList::register_events()
{
    dispatch_.register_event(REQ_GET_FRIENDS_LIST,
                             BIND_FUNC(this, LayerFriendList::handle_get_friends_list));
    dispatch_.register_event(REQ_GET_MOMENT, BIND_FUNC(this,LayerFriendList::handle_get_friends_circle_info));
    dispatch_.register_event(REQ_PUBLISH_MOMENT,BIND_FUNC(this,LayerFriendList::handle_get_publish_circle_info));
    dispatch_.register_event(REQ_PUBLISH_COMMENT,BIND_FUNC(this,LayerFriendList::handle_get_comment_req));
    dispatch_.register_event(REQ_GET_COMMENT,BIND_FUNC(this,LayerFriendList::handle_get_commentinfo_req));
    dispatch_.register_event(REQ_DELETE_MOMENT,BIND_FUNC(this,LayerFriendList::handle_delete_comment_info));//删除朋友圈
    dispatch_.register_event(REQ_DELETE_COMMENT,BIND_FUNC(this,LayerFriendList::handle_delete_comment_info));//删除评论
    
}

void LayerFriendList::handle_delete_comment_info(cmd_data_pointer data)
{
    circle_tableview_change=true;
    circle_posY=layerfriendinfo->tableview_->getContentOffset().y;
    ShowTipMessage(false);
    std::shared_ptr<net_data_recv_package> rec=dynamic_pointer_cast<net_data_recv_package>(data);
    if(!check_request(rec)) return;
    auto processor = PM->response_down(rec);
    switch (processor.status()){
        case msg::Status::SUCCESS:
            this->getOnlineData(true);
            layerfriendinfo->tableview_->reloadData();
            Toast::ToastShow(tools::local_string("operate_successed","操作成功"));
            
            break;
        case msg::Status::FAILED:
            Toast::ToastShow(tools::local_string("operate_failed","操作失败"));
            break;
        default:
            break;
    }
}
void LayerFriendList::handle_get_commentinfo_req(cmd_data_pointer data)
{
    std::shared_ptr<net_data_recv_package> rec=dynamic_pointer_cast<net_data_recv_package>(data);
    if(!check_request(rec)) return;
    auto processor=PM->GetComment_Down(rec);
    std::vector<msg::CommentInfo> commentInfo;
    int size=processor.commentinfo().size();
    auto info = processor.commentinfo();
    
}

void LayerFriendList::handle_get_publish_circle_info(cmd_data_pointer data)
{
    ShowTipMessage(false);
    std::shared_ptr<net_data_recv_package> rec=dynamic_pointer_cast<net_data_recv_package>(data);
    if(!check_request(rec)) return;
}

void LayerFriendList::handle_get_friends_circle_info(cmd_data_pointer data)
{
    ShowTipMessage(false);
    std::shared_ptr<net_data_recv_package> rec=dynamic_pointer_cast<net_data_recv_package>(data);
    if(!check_request(rec)) return;
    auto processor=PM->GetMoment_Down(rec);
    auto moment = processor.moment();
    
    std::vector<msg::Moment> mVectorMoment;
    std::vector<float> heightData;
    for (auto iter : moment)
    {
        mVectorMoment.push_back(iter);
    }
    layerfriendinfo->setShowData(mVectorMoment ,ifShowFriendCircle , circle_posY);
}

void LayerFriendList::handle_get_friends_list(cmd_data_pointer data)
{
    std::shared_ptr<net_data_recv_package> rec = dynamic_pointer_cast<net_data_recv_package>(data);
    if (!check_request(rec)) return;
    auto processor = PM->getFriendList_down(rec);
    switch (processor.response().status()) {
        case msg::Status::SUCCESS: {
            
            auto friendVec = processor.friendinfo();
            countFriendList = mCustomData.size();
            mCustomData.clear();
            struct CustomData customData;
            for(auto iter : friendVec)
            {
                customData.isOnline = iter.isonline();
                customData.userDetailInfo = iter.userdetailinfo();
                customData.isSelect = false;
                customData.isValid = !isInValidList(iter.userdetailinfo().userid());
                mCustomData.push_back(customData);
            }
            //聊天时间排序
            auto sortFunc = [&](const struct CustomData &data1, const struct CustomData data2)
            {
                auto lastmessage1 = CM->get_last_message(data1.userDetailInfo.userid(), ChatMessageManager::MESSAGETYPE::FRIENDCHAT);
                if(!lastmessage1)
                {
                    return false;
                }
                auto lastmessage2 = CM->get_last_message(data2.userDetailInfo.userid(), ChatMessageManager::MESSAGETYPE::FRIENDCHAT);
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
            
            std::sort(mCustomData.begin(), mCustomData.end(), sortFunc);
            tableview_->reloadData();
            if(countFriendList == mCustomData.size() && !sizeOffset.equals(Size::ZERO))
            {
                tableview_->setContentOffset(sizeOffset);
            }
            break;
        }
        case msg::Status::FAILED:
            TipView::showAlertView(tools::local_string("refresh_failed","刷新好友列表失败"));
            break;
        default:
            break;
    }
}

void LayerFriendList::setShowFriendCircle(bool showOrNot)
{
    if(ifShowFriendCircle)
    {
        btnNewFriend->setVisible(false);
        tableview_->setVisible(false);
        layerfriendinfo->setVisible(true);
        
    }
    else
    {
        btnNewFriend->setVisible(true);
        tableview_->setVisible(true);
        layerfriendinfo->setVisible(false);
        
    }
    
}

#pragma mark EditBox delegate
void LayerFriendList::editBoxEditingDidBegin(EditBox* editBox)
{
    editBox->setText("");
    inputCommentInfo="";
}
void LayerFriendList::editBoxEditingDidEnd(EditBox* editBox)
{
    
}
void LayerFriendList::editBoxTextChanged(EditBox* editBox, const std::string& text)
{
    
}
void LayerFriendList::editBoxReturn(EditBox* editBox)
{
    if (strlen(editBox->getText()) == 0) {
        
    } else {
        std::string key_word = editBox->getText();
        if(key_word.empty()) {
            
        }
        else {
            inputCommentInfo=key_word;
            log("input info is %s",inputCommentInfo.c_str());
        }
    }
    editBox->setText("");
}


void LayerFriendList::shareInfo(const std::string& info)
{
    auto layerShare = LayerShare::create();
    layerShare->setShareRecordDetail(info);
    AddGlobalLayer(layerShare);
}

void LayerFriendList::RefreshMessage()
{
    auto map = CM->get_messages_by_type(ChatMessageManager::MESSAGETYPE::FRIENDCHAT);
    bool bNeedUpdate = false;
    for (auto iter = map->begin(); iter != map->end(); iter++){
        bool bFind = false;
        for (auto data: mCustomData)
            if (data.userDetailInfo.userid() == iter->first){
                bFind = true;
                break;
            }
        if (!bFind){
            bNeedUpdate = true;
            break;
        }
    }
    if (bNeedUpdate)
        send_msg();
    else
        tableview_->reloadData();
}

void LayerFriendList::setCircleImageShow(std::string imageName)
{
    auto layerShowImage = LayerShowImage::create();
    AddGlobalLayer(layerShowImage);
    layerShowImage->showImage(imageName);
    
}

void LayerFriendList::setComment(int MomentId,int commentID ,int sendType , int parentID)//
{
    commentID_= commentID;
    momentID_= MomentId;
    sendType_=sendType;
    parentID_=parentID;
    
    ed_property_value_->setVisible(true);
    image_chat_bar_bg->setVisible(true);
    bool ifShow = image_chat_bar_bg->isVisible();
    int zorder = image_chat_bar_bg->getLocalZOrder();
    is_show_edit=true;
  
}

void LayerFriendList::setDeleteFriendCircleInfo(int type,int momentID,int commentID)//删除朋友圈，评论，和点赞
{
    std::string showstring="";
    switch(type)
    {
        case 1:
            showstring=tools::local_string("delete_Moment","确定删除该条朋友圈信息?");
            break;
        case 2:
            showstring=tools::local_string("delete_comment","删除该条评论?");
            
            break;
        case 3:
            showstring=tools::local_string("delete_support","取消点赞?");
            break;
        default:
            break;
    }
    deleteMoment=momentID;
    deleteComentID_=commentID;
    auto tipLayer = TipView::showAlertView(showstring,true);
    if(type==1)
    {
    tipLayer->set_tipview_close_callback([this]()
                                         {
                                             this->deleteMomment(deleteMoment);
                                         });
    }
    else
    {
        tipLayer->set_tipview_close_callback([this]()
                                             {
                                                 this->deleteComment(deleteMoment, deleteComentID_);
                                             });
    }
}

void LayerFriendList::jumpToFriendInfoPage(msg::UserDetailInfo user_info_)
{
    AddGlobalLayer(LayerUserInfo::create(user_info_));
}

void LayerFriendList::deleteComment(int MomentID,int CommentID)
{
    auto processor = PM->DeleteComent_UP(CommentID , MomentID);
    send_data(processor, REQ_DELETE_COMMENT);
}

void LayerFriendList::deleteMomment(int momentID)
{
    auto processor = PM->DeleteMoment_UP(momentID);
    send_data(processor, REQ_DELETE_MOMENT);
}
