//
//  LayerMain.cpp
//  pokerclient
//
//  Created by lvzhangle on 16/3/25.
//
//

#include "LayerMain.h"

#include "ProcessorManager.h"
#include "SceneManager.h"
#include "ChatMessageManager.h"
#include "PokerDataManager.h"
#include "InvitationManager.h"

#include "utils.h"
#include "NET_CMD.h"

#include "NodeMainLayerRoomPage.h"
#include "LayerChooseGameType.h"
#include "LayerChooseGameRoom.h"
#include "LayerQuickAccess.h"
#include "LayerUserSetting.h"
#include "LayerGroupList.h"
#include "LayerFriendList.h"
#include "TipView.h"
#include "Toast.h"
#include "GameTools.h"

#define MAX_MAINPAGE_ROTATION 8

#define ZORDER_UI_TABLEVIEW 10
#define ZORDER_UI 20
#define ZORDER_UI_BUTTON 30
#define ZORDER_BTN_BOTTOM 40
#define ZORDER_ANIMATION_BOTTOM 50
#define ZORDER_ANIMATION_CENTER_LIGHT 60
#define ZORDER_BTN_CENTER 70
#define ZORDER_ANIMATION_CENTER_STAR 80

#define TAG_SPRITE_BOTTOM 1000
#define TAG_SPRITE_CENTER 2000

LayerMain::LayerMain():exitTime_(0)
{
    
}

bool LayerMain::init()
{
    if(!BaseLayer::init()) {
        return false;
    }
    
    root_ = CSLoader::createNode("LayerMain.csb");
    this->addChild(root_);
    
    auto listview = dynamic_cast<ListView*>(CSLoader::seekNodeByName(root_, "listview"));
    
    tableview_ = TableView::create(this, listview->getContentSize());
    tableview_->setAnchorPoint(listview->getAnchorPoint());
    tableview_->setContentOffset(Vec2(0, 0));
    tableview_->setDirection(cocos2d::extension::ScrollView::Direction::HORIZONTAL);
    tableview_->setVerticalFillOrder(TableView::VerticalFillOrder::BOTTOM_UP);
    tableview_->setPosition(listview->getPosition());
    tableview_->setDelegate(this);
    tableview_->setDataSource(this);
    tableview_->setBounceable(false);
    tableview_->setInertiaScrollEnabled(false);

    root_->addChild(tableview_);
    tableview_->touchBeganCallback_ = std::bind(&LayerMain::RoomPageOnTouchBegan, this, std::placeholders::_1, std::placeholders::_2);
    tableview_->touchMovedCallback_ = std::bind(&LayerMain::RoomPageOnTouchMoved, this, std::placeholders::_1, std::placeholders::_2);
    tableview_->touchEndedCallback_ = std::bind(&LayerMain::RoomPageOnTouchEnded, this, std::placeholders::_1, std::placeholders::_2);
    tableview_->touchCancelledCallback_ = std::bind(&LayerMain::RoomPageOnTouchCancelled, this, std::placeholders::_1, std::placeholders::_2);
    
    tableview_->addGUIScrollViewCallback(std::bind(&LayerMain::RoomPageOnScroll, this, std::placeholders::_1));
    
    tableview_->setLocalZOrder(ZORDER_UI_TABLEVIEW);
    
    listview->removeFromParent();
    
    tableview_->reloadData();
    
    text_title_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_title"));
    text_title_->setLocalZOrder(ZORDER_UI);
    
    btn_user_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_user"));
    btn_user_->setLocalZOrder(ZORDER_UI_BUTTON);
    btn_user_->addClickEventListener(std::bind(&LayerMain::click_btn_user, this, std::placeholders::_1));
    
    image_user_message_icon_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_user_message_icon"));
    
    text_user_message_count_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_user_message_count"));
    
    image_group_message_icon_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_group_message_icon"));
    
    text_group_message_count_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_group_message_count"));
    
    btn_setting_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_setting"));
    btn_setting_->setLocalZOrder(ZORDER_UI_BUTTON);
    btn_setting_->addClickEventListener(std::bind(&LayerMain::click_btn_setting, this, std::placeholders::_1));
    
    btn_bar_left_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_bar_left"));
    btn_bar_left_->setLocalZOrder(ZORDER_BTN_BOTTOM);
    btn_bar_left_->addClickEventListener(std::bind(&LayerMain::click_btn_bar_left, this, std::placeholders::_1));
    
    btn_bar_right_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_bar_right"));
    btn_bar_right_->setLocalZOrder(ZORDER_BTN_BOTTOM);
    btn_bar_right_->addClickEventListener(std::bind(&LayerMain::click_btn_bar_right, this, std::placeholders::_1));
    
    btn_center_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_center"));
    btn_center_->setLocalZOrder(ZORDER_BTN_CENTER);
    btn_center_->addClickEventListener(std::bind(&LayerMain::click_btn_center, this, std::placeholders::_1));
//    btn_center_->addTouchCallback(CC_CALLBACK_3(LayerMain::onTouchEventSprite, this));
    text_bar_left_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_bar_left"));
    text_bar_left_->setLocalZOrder(ZORDER_BTN_BOTTOM + 1);
    
    text_bar_right_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_bar_right"));
    text_bar_right_->setLocalZOrder(ZORDER_BTN_BOTTOM + 1);
    
    send_get_room_info();
    
    update_unread_message_count();
    set_luanguage_info();
    
    display_layer_animation(true);
    
    return true;
}


Sprite* LayerMain::getMaskedSprite(cocos2d::Rect rect){
    
    Size size = Director::getInstance()->getVisibleSize();
    RenderTexture* texture = RenderTexture::create((int)size.width, (int)size.height,Texture2D::PixelFormat::RGBA8888);
    texture->begin();
    this->getParent()->visit();
    texture->end();
    log("rect(%.1f, %.1f, %.1f, %.1f)", rect.origin.x,rect.origin.y, rect.size.width, rect.size.height);
    Sprite *textureSprite = Sprite::createWithTexture(texture->getSprite()->getTexture(), rect);
//    textureSprite->setBlendFunc( BlendFunc{GL_ONE, GL_ZERO} );
    
    return textureSprite;
//
//    Sprite *textureSprite = nullptr;
//    utils::captureScreen([&textureSprite](bool succeed, const std::string& outputFile)
//    {
//        if(succeed)
//        {
//      textureSprite = Sprite::create(outputFile);
//        }
//    }, "ScreenShot.png");
    
    //    Sprite* textureSprite=Sprite::create("mark_test/bg.png", rect);
    Sprite* maskSprite=Sprite::create("mark_test/mask.png");
    
    //创建RenderTexture,尺寸符合mask
    
    RenderTexture * rt = RenderTexture::create( maskSprite->getContentSize().width,
                                               maskSprite->getContentSize().height );
    
    maskSprite->setPosition(maskSprite->getContentSize().width/2,
                            maskSprite->getContentSize().height/2);
    textureSprite->setPosition(textureSprite->getContentSize().width/2,
                               textureSprite->getContentSize().height/2);
    
    //对于mask,需要先渲染到RenderTexture上,源因子1,目标因子0
    maskSprite->setBlendFunc( BlendFunc{GL_ONE, GL_ZERO} );
    //对于原图,渲染的时候透明度采用mask的透明度,这样在mask透明的地方原图也透明,实现蒙板效果,目标因子0去掉mask
    textureSprite->setBlendFunc( BlendFunc{GL_DST_ALPHA, GL_ZERO} );
    
    //开始渲染,先是mask,然后原图
    rt->begin();
    maskSprite->visit();
    textureSprite->visit();
    rt->end();
    
    //提取sprite
    Sprite* final =Sprite::createWithTexture(rt->getSprite()->getTexture());
    //反转Y轴,纹理创建时Y轴是反的
    final->setFlippedY(true);
    return final;
}


void LayerMain::onTouchEventSprite(Ref*ref, Touch*touch, Widget::TouchEventType type){
    
    switch(type)
    {
        case Widget::TouchEventType::BEGAN:
        {
            Rect rect=Rect(touch->getLocationInView().x-50, touch->getLocationInView().y-50, 100, 100);
            showdetail=getMaskedSprite(rect);
            showdetail->setScale(3.0f);
            showdetail->setPosition(touch->getLocation());
            addChild(showdetail);
            
        }

            break;
        case Widget::TouchEventType::MOVED:
        {
            
            Rect rect=Rect(touch->getLocation().x-50, touch->getLocationInView().y-50, 100, 100);
            showdetail->setTexture(getMaskedSprite(rect)->getTexture()) ;
            showdetail->setPosition(touch->getLocation());
        }
            break;
        case Widget::TouchEventType::ENDED:
            showdetail->removeFromParentAndCleanup(true);
            break;
        case Widget::TouchEventType::CANCELED:
            showdetail->removeFromParentAndCleanup(true);
            break;
    }
}

void LayerMain::set_luanguage_info(){
    text_title_->setString(tools::local_string("GoodHand","好牌"));
    text_bar_left_->setString(tools::local_string("playground","游乐场"));
    text_bar_right_->setString(tools::local_string("title_club","俱乐部"));
}

void LayerMain::send_get_room_info()
{
    auto processor = PM->GetRoomInfo_UP(0);
    send_data(processor, REQ_GET_ROOM_INFO);
}

void LayerMain::update_unread_message_count()
{
    uint32_t uintUnreadUserCount = 0;
    uint32_t uintUnreadUserMessage = CM->get_unread_message_count(0, ChatMessageManager::MESSAGETYPE::FRIENDCHAT, true);
    uint32_t uintUnreadUserInvitation = IM->get_unread_invitation_count();
    
    uintUnreadUserCount = uintUnreadUserMessage + uintUnreadUserInvitation;
    
    if (uintUnreadUserCount == 0)
    {
        if (image_user_message_icon_->isVisible())
        {
            image_user_message_icon_->setVisible(false);
        }
    }else
    {
        if (!image_user_message_icon_->isVisible())
        {
            image_user_message_icon_->setVisible(true);
        }
        text_user_message_count_->setString(tools::to_string(uintUnreadUserCount));
    }
    
    uint32_t uintUnreadGroupMessage = CM->get_unread_message_count(0, ChatMessageManager::MESSAGETYPE::GROUPCHAT, true);
    
    if (uintUnreadGroupMessage == 0)
    {
        if (image_group_message_icon_->isVisible())
        {
            image_group_message_icon_->setVisible(false);
        }
    }else
    {
        if (!image_group_message_icon_->isVisible())
        {
            image_group_message_icon_->setVisible(true);
        }
        text_group_message_count_->setString(tools::to_string(uintUnreadGroupMessage));
    }
}

void LayerMain::register_events()
{
    dispatch_.register_event(REQ_GET_ROOM_INFO,
                             BIND_FUNC(this, LayerMain::handle_get_room_info));
    dispatch_.register_event(REQ_SEND_JOIN,
                             BIND_FUNC(this, LayerMain::handle_join));
}

void LayerMain::RefreshLayer()
{
    send_get_room_info();
}

void LayerMain::handle_get_room_info(cmd_data_pointer data)
{
    ClearError();
    std::shared_ptr<net_data_recv_package> rec = dynamic_pointer_cast<net_data_recv_package>(data);
    if (!check_request(rec)) return;
    msg::Processor_150_GetRoomInfo_DOWN processor;
    processor.ParseFromArray(rec->body, rec->bodysize);
    switch (processor.response().status()) {
        case msg::Status::SUCCESS: {
            GDM->add_room_server(processor.roomserver().ip(), processor.roomserver().ipv6(), processor.roomserver().port());
            std::vector<msg::RoomInfo> allRooms;
            for (auto room: processor.roominfo())
                allRooms.push_back(room);
            uint32_t lastGroupID = GDM->get_last_game_group_id();
            uint32_t lastRoomID = GDM->get_last_game_group_id();
            bool lastGameIsTournament = GDM->get_last_game_is_tournament();
            msg::RoomType lastGameType = GDM->get_last_game_room_type();
            
            auto roomListSortFunc = [&](msg::RoomInfo room1, msg::RoomInfo room2){
                int score1 = 0;
                int score2 = 0;
                if (room1.roomid() == lastRoomID)
                    score1 += 0x100;
                else if (room1.roomtype().roomtype() == lastGameType && room1.istournament() == lastGameIsTournament)
                    score1 += 0x10;
                else if (room1.groupinfo().groupid() == lastGroupID)
                    score1 += 0x1;
                if (room2.roomid() == lastRoomID)
                    score2 += 0x100;
                else if (room2.roomtype().roomtype() == lastGameType && room2.istournament() == lastGameIsTournament)
                    score2 += 0x10;
                else if (room2.groupinfo().groupid() == lastGroupID)
                    score2 += 0x1;
                
                if (score1 == score2)
                    return room1.roomid() > room2.roomid();
                else
                    return score1 > score2;
            };
            
            std::sort(allRooms.begin(), allRooms.end(), roomListSortFunc);
            
            main_room_page_datas_ = allRooms;
            
            tableview_->reloadData();
            if (processor.roominfo_size()==0)
            {
                ShowTipMessage(true,tools::local_string("no_game_room","亲，您当前没有任何牌局\n快点击下面按钮\n约小伙伴们一起玩耍吧~"));
            }
            else
            {
                ShowTipMessage(false);
            }
            break;
        }
        case msg::Status::FAILED: {
            TipView::showAlertView(tools::local_string("get_room_list_failed", "获取房间列表失败"));
            break;
        }
        default:
            break;
    }
}

void LayerMain::handle_join(cmd_data_pointer data)
{
    std::shared_ptr<net_data_recv_package> rec = dynamic_pointer_cast<net_data_recv_package>(data);
    if(!check_request(rec)) return;
    auto processor = PM->response_down(rec);
    TipView::showAlertView(processor.message());
}

#pragma mark click event --
void LayerMain::click_btn_setting(Ref * sender)
{
    AddGlobalLayer(LayerUserSetting::create());
}
void LayerMain::click_btn_user(Ref * sender)
{
    AddGlobalLayer(LayerFriendList::create());
}
void LayerMain::click_btn_bar_left(Ref * sender)
{
    LayerChooseGameType* chooseGameType = LayerChooseGameType::create();
    addChild(chooseGameType);
    chooseGameType->set_choose_call_func(std::bind(&LayerMain::select_choose_room_type, this, std::placeholders::_1, std::placeholders::_2));
}

void LayerMain::click_select_cell(uint32_t idx)
{
    if (main_room_page_datas_.size() == 1 && idx == 1)
    {
        idx = 0;
    }
    
    if (idx < main_room_page_datas_.size())
    {
        auto select_room = main_room_page_datas_[idx];
        
        if (select_room.istournament())
        {
            PDM->send_enter_room(select_room.roomid());
  
        }else
        {
            //PDM->send_enter_room(select_room.roomid());
        }
    }
}

void LayerMain::click_select_button(uint32_t idx)
{
    if (main_room_page_datas_.size() == 1 && idx == 1)
    {
        idx = 0;
    }
    
    if (idx < main_room_page_datas_.size())
    {
        auto select_room = main_room_page_datas_[idx];
        
        if (select_room.istournament())
        {
            if (select_room.roomstatus() == msg::RoomStatus::WAITINGTOSTART)
            {
                auto processor = PM->JoinTournament_UP(select_room.roomid(), true);
                send_data(processor, REQ_SEND_JOIN);
                
            }else
            {
                PDM->send_enter_room(select_room.roomid());
            }

        }else
        {
            PDM->send_enter_room(select_room.roomid());
        }
    }
}

void LayerMain::select_choose_room_type(msg::RoomType room_type, bool is_tournament)
{
    auto layer = getChildByName("LayerChooseGameType");
    if (layer)
        layer->removeFromParent();
    LayerChooseGameRoom* chooseGameRoom = LayerChooseGameRoom::create(room_type, is_tournament);
    AddGlobalLayer(chooseGameRoom);
}

void LayerMain::click_btn_bar_right(Ref * sender)
{
    auto gl = LayerGroupList::create();
    AddGlobalLayer(gl);
}

void LayerMain::click_btn_center(Ref * sender)
{
    LayerQuickAccess* quickaccess=LayerQuickAccess::create();
    AddGlobalLayer(quickaccess);
}

#pragma mark tableview scroll --
void LayerMain::RoomPageOnScroll(Ref * sender)
{
    Vector<TableViewCell*> cellsUsed (tableview_->get_cellsUsed());
    if (cellsUsed.size() != 0)
    {
        for (auto cell_iter : cellsUsed)
        {
            if (cell_iter)
            {
                Point point_world_cell_iter = cell_iter->getParent()->convertToWorldSpace(cell_iter->getPosition());
                cocos2d::Size size_cell_iter = cell_iter->getContentSize();
                
                if (cell_iter->getIdx() == 0)
                {
                    point_world_cell_iter = cocos2d::Point(point_world_cell_iter.x + 116, point_world_cell_iter.y);
                }else if (cell_iter->getIdx() == main_room_page_datas_.size() - 1)
                {
                    
                }
                float distance_iter = 375.0 - (point_world_cell_iter.x + size_cell_iter.width * (0.5 - cell_iter->getAnchorPoint().x));
                
                
                if (distance_iter >= 0)  //left
                {
                    NodeMainLayerRoomPage* roompage_cell = dynamic_cast<NodeMainLayerRoomPage*>(cell_iter);
                    if (roompage_cell)
                    {
                        float distance_rotation = std::fabs(distance_iter);
                        float distance_rotation_mark = distance_rotation - size_cell_iter.width/2;

                        
                        if (distance_rotation_mark - 100.0 >= 0) //超出
                        {
                            roompage_cell->setNodeMainLayerRoomRotation(NodeMainLayerRoomPage::ROOMPAGE_ROTATION_TYPE::LEFT, -MAX_MAINPAGE_ROTATION);
    
                        }else if (distance_rotation_mark <= 0)
                        {
                            roompage_cell->setNodeMainLayerRoomRotation(NodeMainLayerRoomPage::ROOMPAGE_ROTATION_TYPE::LEFT, 0);

                        }else
                        {
                            roompage_cell->setNodeMainLayerRoomRotation(NodeMainLayerRoomPage::ROOMPAGE_ROTATION_TYPE::LEFT, -distance_rotation_mark / 100.0 * MAX_MAINPAGE_ROTATION);

                        }
                    }
                    
                }else if (distance_iter < 0) //right
                {
                    NodeMainLayerRoomPage* roompage_cell = dynamic_cast<NodeMainLayerRoomPage*>(cell_iter);
                    if (roompage_cell)
                    {
                        float distance_rotation = std::fabs(distance_iter);
                        
                        float distance_rotation_mark = distance_rotation - size_cell_iter.width/2;
                        
                        
                        if (distance_rotation_mark - 100.0 >= 0) //超出
                        {
                            roompage_cell->setNodeMainLayerRoomRotation(NodeMainLayerRoomPage::ROOMPAGE_ROTATION_TYPE::RIGHT, MAX_MAINPAGE_ROTATION);

                        }else if (distance_rotation_mark <= 0)
                        {
                            roompage_cell->setNodeMainLayerRoomRotation(NodeMainLayerRoomPage::ROOMPAGE_ROTATION_TYPE::RIGHT, 0);

                        }else
                        {
                            roompage_cell->setNodeMainLayerRoomRotation(NodeMainLayerRoomPage::ROOMPAGE_ROTATION_TYPE::RIGHT, distance_rotation_mark / 100.0 * MAX_MAINPAGE_ROTATION);
                        }
                        
                    }
                }
            }
        }
    }
}
#pragma mark tableview touch --
bool LayerMain::RoomPageOnTouchBegan(Touch * touch, Event* event)
{
    return true;
}

void LayerMain::RoomPageOnTouchMoved(Touch * touch, Event* event)
{
    
}

void LayerMain::RoomPageOnTouchEnded(Touch * touch, Event* event)
{
    RoomPageTouchEndedFunc(touch, event);
}

void LayerMain::RoomPageOnTouchCancelled(Touch * touch, Event* event)
{
    RoomPageTouchEndedFunc(touch, event);
}

void LayerMain::RoomPageTouchEndedFunc(Touch * touch, Event* event)
{
    Vector<TableViewCell*> cellsUsed = tableview_->get_cellsUsed();
    if (cellsUsed.size() > 1)
    {
        TableViewCell* select_cell = cellsUsed.at(0);
        for (auto cell_iter : cellsUsed)
        {
            if (cell_iter)
            {
                Point point_container = tableview_->getContainer()->getPosition();
                
                Point point_world_cell_select = select_cell->getParent()->convertToWorldSpace(select_cell->getPosition());
                Point point_world_cell_iter = cell_iter->getParent()->convertToWorldSpace(cell_iter->getPosition());
                
                cocos2d::Size size_cell_select = select_cell->getContentSize();
                cocos2d::Size size_cell_iter = cell_iter->getContentSize();
                
                if (select_cell->getIdx() == 0)
                {
                    point_world_cell_select = cocos2d::Point(point_world_cell_select.x + 116, point_world_cell_select.y);
                }else if (select_cell->getIdx() == main_room_page_datas_.size() - 1)
                {
                    
                }
                
                if (cell_iter->getIdx() == 0)
                {
                    point_world_cell_iter = cocos2d::Point(point_world_cell_iter.x + 116, point_world_cell_iter.y);
                }else if (cell_iter->getIdx() == main_room_page_datas_.size() - 1)
                {
                    
                }
                
                float distance_iter = std::fabs(375.0 - (point_world_cell_iter.x + size_cell_iter.width * (0.5 - cell_iter->getAnchorPoint().x)));
                float distance_select = std::fabs(375.0 - (point_world_cell_select.x + size_cell_select.width * (0.5 - select_cell->getAnchorPoint().x)));
                
                if ((distance_iter - distance_select) < 0)
                {
                    select_cell = cell_iter;
                }
                cocos2d::log("select_cell idx %ld", select_cell->getIdx());
                
            }
        }
        
        
        cocos2d::Size size_cell_select = select_cell->getContentSize();
        
        cocos2d::log("content cell idx %ld", select_cell->getIdx());
        cocos2d::Point point_content_offset;
        //= cocos2d::Point(select_cell->getPosition().x + (0.5 - select_cell->getAnchorPoint().x) * select_cell->getContentSize().width - 375, select_cell->getPosition().y);
        if(select_cell->getIdx() == 0)
        {
            point_content_offset = cocos2d::Point(select_cell->getPosition().x, select_cell->getPosition().y);
        }else
        {
            point_content_offset = cocos2d::Point(select_cell->getPosition().x + (0.5 - select_cell->getAnchorPoint().x) * select_cell->getContentSize().width - 375, select_cell->getPosition().y);
        }
        
        cocos2d::log("content_point %f, %f", point_content_offset.x, point_content_offset.y);
        
        //cocos2d::Point xx = select_cell->convertToNodeSpace(select_cell->getParent()->getPosition());
        
        tableview_->setContentOffset(cocos2d::Point(-1 * point_content_offset.x, 0), true);
        
        cocos2d::log("container : %f, %f", tableview_->getContainer()->getPosition().x, tableview_->getContainer()->getPosition().y);
    }
}

#pragma mark tableview datasource --
TableViewCell* LayerMain::tableCellAtIndex(TableView *table, ssize_t idx)
{
    NodeMainLayerRoomPage * cell = dynamic_cast<NodeMainLayerRoomPage*>(table->dequeueCell());
    if (!cell)
    {
        cell = NodeMainLayerRoomPage::create();
        cell->set_click_btn_callback(std::bind(&LayerMain::click_select_button, this, std::placeholders::_1));
        cell->set_click_cell_callback(std::bind(&LayerMain::click_select_cell, this, std::placeholders::_1));
    }
    cell->update_data(main_room_page_datas_[idx]);
    
    if (main_room_page_datas_.size() == 1)
    {
        cell->setNodeMainLayerRoomPageIndex(1);
    }else
    {
        cell->setNodeMainLayerRoomPageIndex((uint32_t)idx);
    }
    
    return cell;
}

Size LayerMain::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
    if (idx == 0 || idx == main_room_page_datas_.size()-1) {
        return cocos2d::Size(518 + 116, 648);
    }else
    {
        return cocos2d::Size(518, 648);
    }
}

ssize_t LayerMain::numberOfCellsInTableView(TableView *table)
{
    return main_room_page_datas_.size();
}

#pragma mark tableview delegate --
void LayerMain::tableCellTouched(TableView* table, TableViewCell* cell)
{
    
}

#pragma mark animation --
void LayerMain::display_layer_animation(bool isOpen)
{
    if (isOpen)
    {
        Sprite* spriteBtnBottom = Sprite::create("login_res/mainlayer_res/btn_home_bottom/btn_home_bottom_000.png");

        spriteBtnBottom->setLocalZOrder(ZORDER_ANIMATION_BOTTOM);
        
        root_->addChild(spriteBtnBottom);
        
        Animation* animationSpriteBtnBottom  = Animation::create();
        for (int i = 0; i < 30; i++) {
            char fileName[128];
            std::sprintf(fileName, "login_res/mainlayer_res/btn_home_bottom/btn_home_bottom_%03d.png", i);
            animationSpriteBtnBottom ->addSpriteFrameWithFile(fileName);
        }
        animationSpriteBtnBottom->setDelayPerUnit(0.1f);
        Animate* animateSpriteBtnBottom = Animate::create(animationSpriteBtnBottom);
        
        auto callfuncSpriteBtnBottom1 = CallFunc::create(std::function<void()>([=](){spriteBtnBottom->setPosition(btn_bar_left_->getPosition()); spriteBtnBottom->setVisible(true);}));
        //auto callfuncSpriteBtnBottom2 = CallFunc::create(std::function<void()>([=](){spriteBtnBottom->setVisible(false);}));
        auto callfuncSpriteBtnBottom3 = CallFunc::create(std::function<void()>([=](){spriteBtnBottom->setPosition(btn_bar_right_->getPosition()); }));
        auto callfuncSpriteBtnBottom4 = CallFunc::create(std::function<void()>([=](){spriteBtnBottom->setVisible(false);}));
        
        Sequence* sequenceSpriteBtnBottom = Sequence::create(callfuncSpriteBtnBottom1, animateSpriteBtnBottom, callfuncSpriteBtnBottom3, animateSpriteBtnBottom, callfuncSpriteBtnBottom4, DelayTime::create(4), NULL);
        
        spriteBtnBottom->runAction(RepeatForever::create(sequenceSpriteBtnBottom));

        //////////////////////////////////////////
        Sprite* spriteBtnCenterLight = Sprite::create("login_res/mainlayer_res/btn_home_chip_light/btn_home_chip_light_000.png");
        spriteBtnCenterLight->setAnchorPoint(cocos2d::Vec2(0.5, 0));
        
        spriteBtnCenterLight->setLocalZOrder(ZORDER_ANIMATION_CENTER_LIGHT);
        
        cocos2d::Point posSpriteBtnCenterLight;
        posSpriteBtnCenterLight.x = btn_center_->getPosition().x;
        posSpriteBtnCenterLight.y = 0;
        
        spriteBtnCenterLight->setPosition(posSpriteBtnCenterLight);
        root_->addChild(spriteBtnCenterLight);
        
        Animation* animationSpriteBtnCenterLight = Animation::create();
        for (int i = 0; i < 270; i++) {
            char fileName[128];
            std::sprintf(fileName, "login_res/mainlayer_res/btn_home_chip_light/btn_home_chip_light_%03d.png", i);
            animationSpriteBtnCenterLight->addSpriteFrameWithFile(fileName);
        }
        animationSpriteBtnCenterLight->setDelayPerUnit(0.1f);
        Animate* animateSpriteBtnCenterLight = Animate::create(animationSpriteBtnCenterLight);
        
        spriteBtnCenterLight->runAction(RepeatForever::create(animateSpriteBtnCenterLight));
        spriteBtnCenterLight->setVisible(true);
        
//////////////////////////////////////////
        Sprite* spriteBtnCenterStar = Sprite::create("login_res/mainlayer_res/btn_home_chip_star/btn_home_chip_star_000.png");
        spriteBtnCenterStar->setAnchorPoint(cocos2d::Vec2(0.5, 0));

        spriteBtnCenterStar->setLocalZOrder(ZORDER_ANIMATION_CENTER_STAR);
        
        cocos2d::Point posSpriteBtnCenterStar;
        posSpriteBtnCenterStar.x = btn_center_->getPosition().x;
        posSpriteBtnCenterStar.y = 0;

        spriteBtnCenterStar->setPosition(posSpriteBtnCenterStar);
        root_->addChild(spriteBtnCenterStar);
        
        Animation* animationSpriteBtnCenterStar = Animation::create();
        for (int i = 0; i < 270; i++) {
            char fileName[128];
            std::sprintf(fileName, "login_res/mainlayer_res/btn_home_chip_star/btn_home_chip_star_%03d.png", i);
            animationSpriteBtnCenterStar->addSpriteFrameWithFile(fileName);
        }
        animationSpriteBtnCenterStar->setDelayPerUnit(0.1f);
        Animate* animateSpriteBtnCenterStar = Animate::create(animationSpriteBtnCenterStar);
        
        spriteBtnCenterStar->runAction(RepeatForever::create(animateSpriteBtnCenterStar));
        spriteBtnCenterStar->setVisible(true);

    }else
    {
        
    }
}


void LayerMain::handle_android_back()
{
    if ((GameTools::millisecondNow() - exitTime_) > 2000L)
    {
        exitTime_ = GameTools::millisecondNow();
        Toast::ToastShow(tools::local_string("android_click_back","再按一次退出应用"));
    }
    else
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        Director::getInstance()->end();        
#endif
    }
}