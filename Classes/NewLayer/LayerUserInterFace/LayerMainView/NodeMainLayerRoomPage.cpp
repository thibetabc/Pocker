//
//  NodeMainLayerRoomPage.cpp
//  pokerclient
//
//  Created by lvzhangle on 16/3/26.
//
//

#include "NodeMainLayerRoomPage.h"
#include "NodeMainLayerRoomPageCell.h"
#include "GameDataManager.h"
#include "PokerDataManager.h"
#include "utils.h"

bool NodeMainLayerRoomPage::init()
{
    if (!TableViewCell::init()) {
        return false;
    }
    
    root_ = CSLoader::createNode("NodeMainLayerRoomPage.csb");
    
    image_mainpage_top_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_mainpage_top"));
    image_mainpage_bg_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_mainpage_bg"));
    image_mainpage_bg_->setTouchEnabled(true);
    image_mainpage_bg_->setSwallowTouches(false);
    //image_mainpage_bg_->addClickEventListener(std::bind(&NodeMainLayerRoomPage::click_cell_bg, this, std::placeholders::_1));
    listview_ = dynamic_cast<ListView*>(CSLoader::seekNodeByName(root_, "listview"));
    listview_->setSwallowTouches(false);
    btn_other_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_other"));
    text_top_title_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_top_title"));
    text_top_title_->setString(tools::local_string("desk_num","桌号"));
    text_title_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_title"));
    image_mainpage_icon_1_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_mainpage_icon_1"));
    image_mainpage_icon_2_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_mainpage_icon_2"));
    
    root_->ignoreAnchorPointForPosition(true);
    
    root_->setContentSize(image_mainpage_bg_->getContentSize());
    
    root_->setPosition(cocos2d::Point(0, 225));
    
    this->setContentSize(image_mainpage_bg_->getContentSize());
    
    this->addChild(root_);
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(false);
    listener->onTouchBegan = CC_CALLBACK_2(NodeMainLayerRoomPage::cellOnTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(NodeMainLayerRoomPage::cellOnTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(NodeMainLayerRoomPage::cellOnTouchEnded, this);
    listener->onTouchCancelled = CC_CALLBACK_2(NodeMainLayerRoomPage::cellOnTouchCancelled, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    return true;
}

void NodeMainLayerRoomPage::click_cell_bg(Ref * sender)
{
    if (click_cell_callback_)
    {
        click_cell_callback_(intRoomPageIdx_);
    }
}

void NodeMainLayerRoomPage::click_btn_other(Ref * sender)
{
    if (click_btn_callback_)
    {
        click_btn_callback_(intRoomPageIdx_);
    }
}

void NodeMainLayerRoomPage::update_data(msg::RoomInfo room_data)
{
    
    
    listview_->removeAllChildren();
    room_page_cell_datas_.clear();
    
    room_data_ = room_data;
    
    //roomName
    std::string roomName = room_data_.roomname();
    text_top_title_->setString(roomName);
    
    //groupName
    if (room_data_.groupinfo().has_groupname())
    {
        text_title_->setString(room_data_.groupinfo().groupname());
    }else
    {
        text_title_->setString(tools::to_string(room_data_.roomid()));
    }
    
    if (room_data.roomstatus() == msg::RoomStatus::OPEN)
    {
        image_mainpage_icon_2_->loadTexture("login_res/mainlayer_res/image_mainpage_heart_icon.png");
        //image_mainpage_icon_2_->loadTexture("login_res/mainlayer_res/image_mainpage_spade_icon.png");
    }else if (room_data_.roomstatus() == msg::RoomStatus::WAITINGTOSTART)
    {
        image_mainpage_icon_2_->loadTexture("login_res/mainlayer_res/image_mainpage_heart_icon.png");
    }else
    {
        image_mainpage_icon_2_->loadTexture("login_res/mainlayer_res/image_mainpage_spade_icon.png");
    }
    
    //listview
    
    if (room_data_.roomtype().roomtype() == msg::RoomType::OFC3 ||
        room_data_.roomtype().roomtype() == msg::RoomType::OFC4)
    {
        image_mainpage_icon_1_->loadTexture("login_res/choosegameroom_res/image_choosegameroom_icon_ofc3.png");
    }else if (room_data_.roomtype().roomtype() == msg::RoomType::NLHOLDEM ||
              room_data_.roomtype().roomtype() == msg::RoomType::PLO ||
              room_data_.roomtype().roomtype() == msg::RoomType::PLOHILO)
    {
        image_mainpage_icon_1_->loadTexture("login_res/choosegameroom_res/image_choosegameroom_icon_holdem.png");
    }
    std::pair<std::string, std::string> pairRoomType;
    std::string strRoomTypeName = room_data_.roomtype().roomtypename();
    pairRoomType.first = tools::local_string("gametype","游戏类型");
    pairRoomType.second = strRoomTypeName;
    room_page_cell_datas_.push_back(pairRoomType);
    
    //时间
//    std::pair<std::string, std::string> pairStartTimeStart;
//    double dbTimeStart = room_data_.starttime();
//    dbTimeStart /= 1000;
//    std::string textTimeStart = tools::time_display(dbTimeStart, tools::TIMEDISPLAYTYPE::ALLTIME);
//    textTimeStart = tools::digital2time(room_data_.starttime());
//    
//    pairStartTimeStart.first = "时间";
//    pairStartTimeStart.second = textTimeStart;
//    room_page_cell_datas_.push_back(pairStartTimeStart);
    
    
    std::pair<std::string, std::string> pairStartTimeEnd;
    if (room_data_.istournament())
    {
        long long int intTimeStart = room_data_.starttime();
        intTimeStart /= 1000;

        std::string textTimeStart;
        textTimeStart = tools::time_display(intTimeStart);
        pairStartTimeEnd.first = tools::local_string("starting_at", "开始时间：");
        pairStartTimeEnd.second = textTimeStart;
        
    }else
    {
        double dbTimeEnd = room_data_.endtime();
        dbTimeEnd /= 1000;
        
        if (dbTimeEnd < 0)
        {
            pairStartTimeEnd.first = tools::local_string("accomplished","即将结束");
            pairStartTimeEnd.second = "";
        }else
        {
            std::string textTimeEnd;
            textTimeEnd = tools::to_string((int)(dbTimeEnd / 60)) + tools::local_string("minutes","分钟");
            pairStartTimeEnd.first = tools::local_string("remaining_time","剩余时间");
            pairStartTimeEnd.second = textTimeEnd;
        }
    }
    
    room_page_cell_datas_.push_back(pairStartTimeEnd);
    
    //人数
    std::pair<std::string, std::string> pairPlayerNum;
    pairPlayerNum.first =tools::local_string("current_players","当前人数：");
    pairPlayerNum.second = tools::to_string(room_data_.playernum()) + std::string("/") + tools::to_string(room_data_.maxplayernum());
    room_page_cell_datas_.push_back(pairPlayerNum);
    
    //note
    std::pair<std::string, std::string> pairRoomNote;
    pairRoomNote.first = tools::local_string("remark","备注");
    pairRoomNote.second = room_data_.roomnote();
    room_page_cell_datas_.push_back(pairRoomNote);
    
    for (auto page_cell_data_iter : room_page_cell_datas_)
    {
        NodeMainLayerRoomPageCell* roomPageCell = NodeMainLayerRoomPageCell::create();
        roomPageCell->update_data(page_cell_data_iter);
        listview_->pushBackCustomItem(roomPageCell);
    }
    listview_->refreshView();
    
    if (room_data_.istournament())
    {
        if (room_data_.roomstatus() == msg::RoomStatus::WAITINGTOSTART)
        {
            btn_other_->setTitleText(tools::local_string("join","报名"));

        }else
        {
            btn_other_->setTitleText(tools::local_string("other_info","其他信息"));

        }
    }else
    {
        btn_other_->setTitleText(tools::local_string("enter_room","进入房间"));
    }
    btn_other_->addClickEventListener(std::bind(&NodeMainLayerRoomPage::click_btn_other, this, std::placeholders::_1));
    
    display_node_animation(true);
    cocos2d::log("NodeMainLayerRoomPage isRunning %s", root_->isRunning() ? "true" : "false");
    
}

void NodeMainLayerRoomPage::setNodeMainLayerRoomRotation(ROOMPAGE_ROTATION_TYPE rotation_type, float rotation)
{
    switch (rotation_type) {
        case ROOMPAGE_ROTATION_TYPE::LEFT:
        {
            root_->setAnchorPoint(Vec2(1, 0));
            root_->setRotation(rotation);
            break;
        }
        case ROOMPAGE_ROTATION_TYPE::RIGHT:
        {
            root_->setAnchorPoint(Vec2(0, 0));
            root_->setRotation(rotation);
            break;
        }
        default:
        {
            break;
        }
    }
}

void NodeMainLayerRoomPage::setNodeMainLayerRoomPageIndex(uint32_t idx)
{
    intRoomPageIdx_ = idx;
    if (intRoomPageIdx_ == 0)
    {
        root_->setPositionX(0 + 116);
    }else
    {
        root_->setPositionX(0);
    }
}

void NodeMainLayerRoomPage::display_node_animation(bool isOpen)
{
    if (isOpen)
    {
        //////////////////////////////////////////
        Sprite* spriteBtnOtherLight = dynamic_cast<Sprite*>(root_->getChildByName("spriteBtnOtherLight"));
        
        if (spriteBtnOtherLight == nullptr)
        {
            spriteBtnOtherLight = Sprite::create("login_res/mainlayer_res/btn_mainpage_other_light/btn_homecard_000.png");
            spriteBtnOtherLight->setPosition(btn_other_->getPosition());
            root_->addChild(spriteBtnOtherLight, 0, "spriteBtnOtherLight");
        }
        
        //spriteBtnCenterStar->setLocalZOrder(ZORDER_ANIMATION_CENTER_STAR);
        if (spriteBtnOtherLight->getActionByTag(1000) == nullptr)
        {
            Animation* animationSpriteBtnOtherLight = Animation::create();
            for (int i = 0; i < 40; i++) {
                char fileName[128];
                std::sprintf(fileName, "login_res/mainlayer_res/btn_mainpage_other_light/btn_homecard_%03d.png", i);
                animationSpriteBtnOtherLight->addSpriteFrameWithFile(fileName);
            }
            animationSpriteBtnOtherLight->setDelayPerUnit(0.1f);
            Animate* animateSpriteBtnOtherLight = Animate::create(animationSpriteBtnOtherLight);
            
            auto callfuncSpriteBtnOtherLight1 = CallFunc::create(std::function<void()>([=](){spriteBtnOtherLight->setVisible(true);}));
            
            auto callfuncSpriteBtnOtherLight2 = CallFunc::create(std::function<void()>([=](){spriteBtnOtherLight->setVisible(false);}));
            
            Sequence* sequenceSpriteOtherLight = Sequence::create(callfuncSpriteBtnOtherLight1, animateSpriteBtnOtherLight, callfuncSpriteBtnOtherLight2, DelayTime::create(3), NULL);
            sequenceSpriteOtherLight->setTag(1000);
            
            spriteBtnOtherLight->runAction(RepeatForever::create(sequenceSpriteOtherLight));
        }
        
    }else
    {
        
    }
    
}


bool NodeMainLayerRoomPage::cellOnTouchBegan(Touch * touch, Event* event)
{
    Point touchLocation = root_->convertTouchToNodeSpace(touch);
    
    if (image_mainpage_bg_->getBoundingBox().containsPoint(touchLocation))
    {
        pos_touch_record_ = touchLocation;
        is_click_cell_ = true;
        return true;
    }
    
    return false;
}

void NodeMainLayerRoomPage::cellOnTouchMoved(Touch * touch, Event* event)
{
    Point touchLocation = root_->convertTouchToNodeSpace(touch);
    
    if (!image_mainpage_bg_->getBoundingBox().containsPoint(touchLocation))
    {
        is_click_cell_ = false;
    }
}

void NodeMainLayerRoomPage::cellOnTouchEnded(Touch * touch, Event* event)
{
    Point touchLocation = root_->convertTouchToNodeSpace(touch);
    
    if (image_mainpage_bg_->getBoundingBox().containsPoint(touchLocation))
    {
        if (fabs(touchLocation.x - pos_touch_record_.x) - 10 < 0 && fabs(touchLocation.y - pos_touch_record_.y) - 10 < 0)
        {
            if (click_cell_callback_)
            {
                click_cell_callback_(intRoomPageIdx_);
            }
        }
    }
    
    is_click_cell_ = false;
}

void NodeMainLayerRoomPage::cellOnTouchCancelled(Touch * touch, Event* event)
{
    is_click_cell_ = false;
}