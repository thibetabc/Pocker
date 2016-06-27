//
//  LayerFriendContactsList.cpp
//  pokerclient
//
//  Created by HuangFeng on 16/4/30.
//
//

#include "LayerFriendContactsList.h"
#include "TipView.h"
#include "ProcessorManager.h"
#include "NET_CMD.h"
#include "GameDataManager.h"
#include "GroupCell.h"
#include "InvitationManager.h"
#include "PlatformSdk.h"
#include "LayerUserInfo.h"
#include "utils.h"
#include "LayerFriendList.h"
#include "GameTools.h"
#include "PlatformSdk.h"

LayerFriendContactsList::LayerFriendContactsList():connectingView(nullptr)
{
    
}

LayerFriendContactsList::~LayerFriendContactsList()
{
    
}

bool LayerFriendContactsList::init()
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
    
    btnSearch->setVisible(false);
    btnCreate->setVisible(false);
    btnBack->addClickEventListener([&](Ref *ref)
                                   {
                                       this->removeFromParent();
                                   });
    
    title->setString(tools::local_string("phone_contacts","手机联系人"));
    this->scheduleOnce(std::bind(&LayerFriendContactsList::accessContants, this, std::placeholders::_1), 0.2f, "LayerFriendContactsList");
    connectingView = ConnectingView::create();
    this->addChild(connectingView, 9999);
    return true;
}

void LayerFriendContactsList::accessContants(float dt)
{
    bool isAuthorized = PS->getPhoneContacts();
    if(!isAuthorized)
    {
        auto tipLayer = TipView::showAlertView(tools::local_string("set_contacts","你未开启”允许好牌访问通讯录“，请到系统设置-隐私-通讯录中开启"));
        tipLayer->set_tipview_close_callback([this]()
                                             {
                                                 this->removeFromParent();
                                             });
        connectingView->removeFromParent();
    }
    else
    {
        requestFriendContactsList();
    }
}

void LayerFriendContactsList::RefreshLayer()
{
    requestFriendContactsList();
}


#pragma mark - tableviewDatasource
TableViewCell* LayerFriendContactsList::tableCellAtIndex(TableView *table, ssize_t idx)
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
    
    //手机联系人
    auto data = mAppOfPhoneUsers;
    auto contact = data[idx].appUsers;
    auto userStatus = data[idx].userStatus;
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
    spriteSex->setVisible(false);//性别
    buttonOperate->setVisible(true);//加好友
    spriteSelect->setVisible(false);//多选项
    textTime->setVisible(false);
    spriteChat->setVisible(false);
    
    
    textName->setVisible(false);
    textName->setString(contact.nickname());
    spriteSex->setVisible(true);
    spriteSex->setTexture(contact.sex() == msg::Sex::MALE ? gMaleIcon : gFemaleIcon);
    
    textRemark->setVisible(true);
    textRemark->setString(data[idx].name);
    textLastestChat->setVisible(false);
    std::string pic = contact.picname();
    imgIcon->setSpriteTexture(pic.empty() ? "Default/image_defualt_user_icon_large.png" : pic);
    if (userStatus == USER_STATUS::NON_FRIEND)
    {
        buttonOperate->setEnabled(true);
        buttonOperate->setTitleText(tools::local_string("add","添加"));
        buttonOperate->addClickEventListener([&,idx](Ref*ref)
                                             {
                                                 //添加好友
                                                 msg::Request request;
                                                 request.set_type(msg::RequestType::ADDFRIEND);
                                                 int userId = mAppOfPhoneUsers[idx].appUsers.userid();
                                                 
                                                 log("手机联系人 添加好友ID = %d", userId);
                                                 request.mutable_userdetailinfo()->set_userid(GDM->get_user_id());
                                                 request.set_receiverid(userId);
                                                 auto processor = PM->SendRequest_up(request);
                                                 send_data(processor, REQ_ADD_FRIEND);
                                             });
    }
    else if(userStatus == USER_STATUS::FRIEND)
    {
        buttonOperate->setEnabled(false);
        buttonOperate->setTitleText(tools::local_string("add","添加"));
    }
    else
    {
        buttonOperate->setEnabled(true);
        buttonOperate->setTitleText(tools::local_string("invite","邀请"));buttonOperate->addClickEventListener([&,idx](Ref*ref)
                                                                               {
                                                                                   std::string phone = mAppOfPhoneUsers[idx].telephone;
                                                                                   PlatformSdk::getInstance()->showMessageView(phone, "你好", "交友玩牌两不误，【好牌app】最fashion的约局神器，约出来的牌友情，猛戳链接进入 http://haopaia.cn/");
                                                                               });
    }
    
    return cell;
}

ssize_t LayerFriendContactsList::numberOfCellsInTableView(TableView *table)
{
  return mAppOfPhoneUsers.size();
}

#pragma mark - tableviewDelegate
void LayerFriendContactsList::tableCellTouched(TableView* table, TableViewCell* cell)
{
    sizeOffset = table->getContentOffset();
    auto idx = cell->getIdx();
    log("idx = %zd", idx);
    auto data = mAppOfPhoneUsers;
    auto userStatus = data[idx].userStatus;
    if(userStatus != USER_STATUS::NON_APP_USER)
    {
        auto contact = data[idx].appUsers;
        addChild(LayerUserInfo::create(contact));
    }
    
}

void LayerFriendContactsList::tableCellHighlight(TableView* table, TableViewCell* cell)
{
    auto imageBg = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(cell, "img_group_cell"));
    imageBg->loadTexture("image_club/club_group_bg_press.png");
}

void LayerFriendContactsList::tableCellUnhighlight(TableView* table, TableViewCell* cell)
{
    auto imageBg = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(cell, "img_group_cell"));
    imageBg->loadTexture("image_club/club_group_bg.png");
    
}


//保存手机联系人
void LayerFriendContactsList::setContacts(std::vector<Contacts> contacts)
{
    
    const size_t PHONE_NUM = 11;
    mContacts.clear();
    Contacts tContacts;
    for(auto contact : contacts)
    {
        tContacts.allTelephone.clear();
        for(auto phone : contact.allTelephone)
        {
            string str = GameTools::getStringDigit(phone);
            if(!str.empty())
            {
                string::size_type len = str.size();
                if(len > PHONE_NUM)
                {
                    //找后11位
                    str = str.substr(len - PHONE_NUM,PHONE_NUM);
                }
                
                if(str.size() == PHONE_NUM)
                {
                    tContacts.allTelephone.push_back(str);
                }
                else
                {
                    log("error phone -----> %s", str.c_str());
                }
            }
        }
        
        if(tContacts.allTelephone.size() > 0)
        {
            tContacts.allEmail = contact.allEmail;
            tContacts.name = contact.name;
            mContacts.push_back(tContacts);
        }
    }
    
//    int index = 0;
//    log("===========得到通讯录数据(%lu)===================", mContacts.size());
//    for(auto person : mContacts)
//    {
//        for(auto telephone : person.allTelephone)
//        {
//            log("%s : %s", person.name.c_str(), telephone.c_str());
//            
//            //假数据
//            USER_STATUS status;
//            switch(index % 3)
//            {
//                case 0:
//                    status = USER_STATUS::FRIEND;
//                    break;
//                case 1:
//                    status = USER_STATUS::NON_FRIEND;
//                    break;
//                default:
//                    status = USER_STATUS::NON_APP_USER;
//                    break;
//            }
//            
//            msg::UserDetailInfo users;
//            users.set_nickname(person.name);
//            users.set_picname("");
//            users.set_sex(index % 2 == 0 ? msg::Sex::FEMALE : msg::Sex::MALE);
//            users.set_userid(index);
//            mAppOfPhoneUsers.push_back(AppOfPhoneUsers{person.name, telephone, "email", status, users});
//            ++index;
//        }
//        
//    }
    mTableview->reloadData();
}



//根据手机号找名字
LayerFriendContactsList::Contacts LayerFriendContactsList::getContactsByPhone(std::string phone)
{
    Contacts tmp;
    for(auto contacts : mContacts)
    {
        for (auto telephone : contacts.allTelephone)
        {
            if(phone == telephone)
            {
                return contacts;
            }
        }
    }
    
    return tmp;
}


void LayerFriendContactsList::register_events()
{
    dispatch_.register_event(REQ_PHONE_CONTACTS_LIST,
                             BIND_FUNC(this, LayerFriendContactsList::receiveFriendContactsList));
    dispatch_.register_event(REQ_ADD_FRIEND,
                             BIND_FUNC(this, LayerFriendContactsList::receiveAddFriend));
    
}
#pragma mark - protobuf
//请求手机联系人
void LayerFriendContactsList::requestFriendContactsList()
{
    std::vector<std::string> phoneNumber;
    for (auto iter : mContacts)
    {
        for (auto phone : iter.allTelephone)
        {
            phoneNumber.push_back(phone);
        }
    }
    if(!phoneNumber.empty())
    {
        auto processor = PM->SearchFriendByPhoneNumber_up(phoneNumber);
        send_data(processor, REQ_PHONE_CONTACTS_LIST);
    }
    
}


void LayerFriendContactsList::receiveFriendContactsList(cmd_data_pointer data)
{
    
    std::shared_ptr<net_data_recv_package> rec = dynamic_pointer_cast<net_data_recv_package>(data);
    auto processor = PM->SearchFriendByPhoneNumber_down(rec);
    
    mAppOfPhoneUsers.clear();
    std::string phoneNum;
    cocos2d::log("获取到服务器,手机联系人数量: %d", processor.friends().size());

    log("===========得到通讯录数据(%lu)===================", mContacts.size());
    for(auto person : mContacts)
    {
        for(auto telephone : person.allTelephone)
        {
            // log("%s : %s", person.name.c_str(), telephone.c_str());
            msg::UserDetailInfo users;
            USER_STATUS status = USER_STATUS::NON_APP_USER;
            
            for(auto friendInfo : processor.friends())
            {
                
                if(telephone == friendInfo.phonenumber())
                {
                    users = friendInfo.users();
                    status = friendInfo.isfriend() ? USER_STATUS::FRIEND : USER_STATUS::NON_FRIEND;
                    log("%s : %s", person.name.c_str(), telephone.c_str());
                    break;
                }
                
            }
            
            if(status == USER_STATUS::NON_APP_USER)
            {
                users.set_nickname(person.name);
                users.set_picname("");
                users.set_sex(msg::Sex::MALE);
                users.set_userid(0);
            }
            mAppOfPhoneUsers.push_back(AppOfPhoneUsers{person.name, telephone, "email", status, users});
            
        }
        
    }
    if(mAppOfPhoneUsers.size() > 0)
    {
        sort(mAppOfPhoneUsers.begin(), mAppOfPhoneUsers.end(), [](AppOfPhoneUsers data1, AppOfPhoneUsers data2)
        {
            if(data1.userStatus == USER_STATUS::NON_FRIEND)
            {
                return true;
            }
            if (data1.userStatus == USER_STATUS::NON_FRIEND)
            {
                return true;
            }
            if (data1.userStatus == USER_STATUS::FRIEND || data2.userStatus == USER_STATUS::FRIEND)
            {
                return true;
            }
            return false;
        });
    }
    else
    {
        ShowTipMessage(true, tools::local_string("no_phone_contact", "没找到手机联系人"));
    }
    
    if(connectingView)
    {
        connectingView->removeFromParent();
        connectingView = nullptr;
    }
    
    mTableview->reloadData();
    if(!sizeOffset.equals(Size::ZERO))
    {
        mTableview->setContentOffset(sizeOffset);
    }
}

void LayerFriendContactsList::receiveAddFriend(cmd_data_pointer data)
{
    
    std::shared_ptr<net_data_recv_package> rec = dynamic_pointer_cast<net_data_recv_package>(data);
    msg::Response processor;
    processor.ParseFromArray(rec->body, rec->bodysize);
    TipView::showAlertView(processor.message());
    
}


