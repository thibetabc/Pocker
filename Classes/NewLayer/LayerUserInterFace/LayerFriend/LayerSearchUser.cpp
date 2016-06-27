//
//  LayerSearchUser.cpp
//  pokerclient
//
//  Created by zhongyukeji on 16/4/11.
//
//

#include "LayerSearchUser.h"
#include "TipView.h"
#include "ProcessorManager.h"
#include "NET_CMD.h"
#include "utils.h"
#include "LayerInvitationList.h"
#include "GameTools.h"
#include "LayerUserInfo.h"
#include "ChatMessageManager.h"
#include "LayerFriendContactsList.h"
#include "ShaderSprite.h"

LayerSearchUser::LayerSearchUser() : isSearch(true)
{
    
}

bool LayerSearchUser::init()
{
    if(!BaseLayer::init())
    {
        return false;
    }
    
    auto root = CSLoader::createNode("LayerGroupSearch.csb");
    this->addChild(root);
    
    auto title = (Text*)root->getChildByName("text_title");
    auto input = (TextField*)root->getChildByName("tf_keyword");
    inputIcon = root->getChildByName("image_search_icon");
    auto btnBack = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "btn_back"));
    auto btnSearch = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "btn_search"));
    auto addContacts = (Text *)root->getChildByName("text_contacts"); //添加手机联系人
    addContacts->setTouchEnabled(true);
    addContacts->setString(tools::local_string("add_phone_contacts","添加手机联系人"));
    auto listview = root->getChildByName("listview");
    Size tableviewSize = Size(listview->getContentSize().width, listview->getContentSize().height - 150);
    tableview_ = TableView::create(this, tableviewSize);
    tableview_->setAnchorPoint(listview->getAnchorPoint());
    tableview_->setContentOffset(Vec2(0, 0));
    tableview_->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    tableview_->setPosition(listview->getPosition());
    tableview_->setDelegate(this);
    tableview_->setClippingToBounds(true);
    root->addChild(tableview_);
    listview->removeFromParent();
    
    title->setString(tools::local_string("Search_Friend", "搜索好友"));
    tf_keyword_ = GameTools::createEditBoxFromTextField(root, input);
    tf_keyword_->setPlaceHolder(tools::local_string("Input_Friends_Nickname","     请输入好友昵称").c_str());
    tf_keyword_->setDelegate(this);
    tf_keyword_->setInputMode(EditBox::InputMode::SINGLE_LINE);
    tf_keyword_->setReturnType(EditBox::KeyboardReturnType::SEARCH);
    btnBack->addClickEventListener([&](Ref *ref)
    {
        isSearch = false;
        this->removeFromParent();
    });
    btnSearch->setTitleText(tools::local_string("text_search", "查找"));
    btnSearch->addClickEventListener([&](Ref* ref)
                                     {
                                         if(isSearch)
                                         {
                                             if (strlen(tf_keyword_->getText()) == 0)
                                             {
                                                 inputIcon->setVisible(true);
                                             }
                                             else
                                             {
                                                 GameTools::editBoxLoseInputFocus();
                                                 sendMsg(tf_keyword_->getText());
                                             }
                                         }
                                     });
    addContacts->addClickEventListener([&](Ref* ref)
    {
        isSearch = false;
        GameTools::editBoxLoseInputFocus();
        //手机联系人
        AddGlobalLayer(LayerFriendContactsList::create());
    });
    
    
    return true;
}



#pragma mark - tableviewDatasource
TableViewCell* LayerSearchUser::tableCellAtIndex(TableView *table, ssize_t idx)
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
    auto info = mUserDetailInfo[idx];
    int userId = info.userid();
    imgIcon->setSpriteTexture(info.picname());//头像
    spriteSex->setVisible(true);
    spriteSex->setTexture((info.sex() == msg::Sex::MALE ) ? gMaleIcon: gFemaleIcon);
    textName->setString(info.nickname());
    textRemark->setString("ID: " + tools::boost_any_to_string(userId));
    textTime->setVisible(true);
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
    return cell;
}

ssize_t LayerSearchUser::numberOfCellsInTableView(TableView *table)
{
    return mUserDetailInfo.size();
}


#pragma mark - tableviewDelegate
void LayerSearchUser::tableCellTouched(TableView* table, TableViewCell* cell)
{
    //去个人信息
    AddGlobalLayer(LayerUserInfo::create(mUserDetailInfo[cell->getIdx()]));
}

void LayerSearchUser::tableCellHighlight(TableView* table, TableViewCell* cell)
{
    
    auto imageBg = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(cell, "img_group_cell"));
    imageBg->loadTexture("image_club/club_group_bg_press.png");
}

void LayerSearchUser::tableCellUnhighlight(TableView* table, TableViewCell* cell)
{
    
    auto imageBg = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(cell, "img_group_cell"));
    imageBg->loadTexture("image_club/club_group_bg.png");
}

void LayerSearchUser::editBoxEditingDidBegin(EditBox* editBox)
{
    isSearch = true;
    inputIcon->setVisible(false);
}

void LayerSearchUser::editBoxEditingDidEnd(EditBox* editBox)
{
}
void LayerSearchUser::editBoxTextChanged(EditBox* editBox, const std::string& text) {
}

void LayerSearchUser::editBoxReturn(EditBox* editBox)
{
    log("editBox = %s, tf_keyword_ = %s", editBox->getText(), tf_keyword_->getText());
}

void LayerSearchUser::sendMsg(const std::string nick)
{
    auto processor = PM->SearchUser_up(nick);
    send_data(processor, REQ_SEARCH_USER);
}

void LayerSearchUser::register_events()
{
    dispatch_.register_event(REQ_SEARCH_USER,
                             BIND_FUNC(this, LayerSearchUser::handle_search_user));
}

void LayerSearchUser::handle_search_user(cmd_data_pointer data) 
{
    std::shared_ptr<net_data_recv_package> rec = dynamic_pointer_cast<net_data_recv_package>(data);
    if (!check_request(rec)) return;
    auto processor = PM->SearchUser_down(rec);
    switch (processor.response().status())
    {
        case msg::Status::SUCCESS:
        {            
            auto userdetailinfo = processor.userdetailinfo();
            mUserDetailInfo.assign(userdetailinfo.begin(), userdetailinfo.end());
            tableview_->reloadData();
            break;
        }
        case msg::Status::FAILED:
            TipView::showAlertView(tools::local_string("account_exist","该用户不存在"));
            break;
        default:
            break;
    }
    
}