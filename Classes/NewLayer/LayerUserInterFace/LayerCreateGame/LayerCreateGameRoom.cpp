//
//  LayerCreateGameRoom.cpp
//  pokerclient
//
//  Created by lvzhangle on 16/4/5.
//
//

#include "LayerCreateGameRoom.h"
#include "utils.h"
#include "GameTools.h"
#include "ProcessorManager.h"
#include "GameDataManager.h"
#include "NET_CMD.h"
#include "PokerDataManager.h"
#include "group_data.h"

#include "LayerChooseGameType.h"
#include "LayerCreateTourBlindAndReward.h"
#include "LayerTourJoinUserList.h"
#include "TipView.h"

LayerCreateGameRoom* LayerCreateGameRoom::create(msg::RoomType room_type, bool is_tournament, const msg::GroupInfo& groupinfo, bool isCreate)
{
    LayerCreateGameRoom* result = new LayerCreateGameRoom;
    result->set_groupinfo(groupinfo);
    if(result->init(room_type, is_tournament, isCreate)) {
        result->autorelease();
        return result;
    }
    return nullptr;
}

LayerCreateGameRoom* LayerCreateGameRoom::create(msg::RoomType room_type, bool is_tournament, bool isCreate)
{
    LayerCreateGameRoom* result = new LayerCreateGameRoom;
    msg::GroupInfo nullgroup;
    nullgroup.set_groupid(0);
    result->set_groupinfo(nullgroup);
    if(result->init(room_type, is_tournament, isCreate)) {
        result->autorelease();
        return result;
    }
    return nullptr;
}

LayerCreateGameRoom* LayerCreateGameRoom::create(msg::Processor_410_EnterRoom_DOWN enterRoomInfo, bool isCreate)
{
    LayerCreateGameRoom* result = new LayerCreateGameRoom;
    
    if (enterRoomInfo.roominfo().has_groupinfo())
    {
        result->set_groupinfo(enterRoomInfo.roominfo().groupinfo());
    }else{
        msg::GroupInfo nullgroup;
        nullgroup.set_groupid(0);
        result->set_groupinfo(nullgroup);
    }
    
    result->set_enter_room_info(enterRoomInfo);
    result->set_room_info(enterRoomInfo.roominfo());
    if(result->init(enterRoomInfo.roominfo().roomtype().roomtype(), enterRoomInfo.roominfo().istournament(), isCreate)) {
        result->autorelease();
        return result;
    }
    return nullptr;
}

void LayerCreateGameRoom::set_groupinfo(const msg::GroupInfo& groupinfo)
{
    select_groupinfo_.CopyFrom(groupinfo);
}

bool LayerCreateGameRoom::init(msg::RoomType room_type, bool is_tournament, bool isCreate)
{
    if(!BaseLayer::init()) {
        return false;
    }
    
    if (room_type == msg::RoomType::ALL)
        room_type_ = msg::RoomType::NLHOLDEM;
    else
        room_type_ = room_type;
    is_tournament_ = is_tournament;
    is_create_ = isCreate;
    
    is_display_all_ = false;
    
    root_ = CSLoader::createNode("LayerCreateGameRoom.csb");
    this->addChild(root_);
    
    text_title_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_title"));
    
    btn_back_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_back"));
    btn_back_->addClickEventListener(std::bind(&LayerCreateGameRoom::click_btn_back, this, std::placeholders::_1));
    
    text_join_list_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_join_list"));
    text_join_list_->addClickEventListener(std::bind(&LayerCreateGameRoom::click_text_join_list, this, std::placeholders::_1));
    
    btn_create_game_room_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_create_game_room"));
    
    btn_create_game_room_->addClickEventListener(std::bind(&LayerCreateGameRoom::click_btn_create_game_room, this, std::placeholders::_1));
    
    image_layer_creategameroom_bg_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_layer_creategameroom_bg"));
    
    image_listview_bg_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_listview_bg"));
    
    layout_display_more_ = dynamic_cast<Layout*>(CSLoader::seekNodeByName(root_, "layout_display_more"));
    layout_display_more_->addClickEventListener(std::bind(&LayerCreateGameRoom::click_btn_display_more, this, std::placeholders::_1));
    layout_display_more_->setAnchorPoint(cocos2d::Vec2(0, 0));
    layout_display_more_->setLocalZOrder(99);
    layout_display_more_->setVisible(true);
    
    text_display_more_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_property_name"));
    text_display_more_->setString(tools::local_string("more_setting","高级"));
    listview_ = dynamic_cast<ListView*>(CSLoader::seekNodeByName(root_, "listview"));
    
    auto& win_size = Director::getInstance()->getWinSize();
    auto layout = Layout::create();
    layout->setContentSize(win_size);
    layout->setAnchorPoint(Point::ZERO);
    //layout->setAnchorPoint(Point(0, win_size.height));
    layout->setTouchEnabled(true);
    layout->setSwallowTouches(false);
    layout->addClickEventListener([](Ref*){
        GameTools::editBoxLoseInputFocus();
    });
    this->addChild(layout, 255);
    
    init_create_property();
    
    refresh_listview();
    
    register_events();
    
    return true;
}

void LayerCreateGameRoom::set_language()
{
    text_title_->setString(tools::local_string("quick_access_create_room","创建房间"));
    btn_create_game_room_->setTitleText(tools::local_string("confirm_change_password","确定"));
}

void LayerCreateGameRoom::init_create_property()
{
    const msg::CreateRoomInfoForSave* createRoomInfo = GDM->get_create_room_info(room_type_, is_tournament_);
    
    roomname.set_stringvalue(GDM->get_userinfo()->nickname() + std::string("  ") + tools::time_display(tools::date_double()));
    
    if (!is_tournament_)
    {
        btn_create_game_room_->setTitleText("确定");
        btn_create_game_room_->setVisible(true);
        btn_create_game_room_->addClickEventListener(std::bind(&LayerCreateGameRoom::click_btn_create_game_room, this, std::placeholders::_1));
        
        text_title_->setString("创建房间");
        
        propertys_ = GDM->get_Property_by_game_id(room_type_);
        if (createRoomInfo)
        {
            for (int intPropertyCount = 0; intPropertyCount < propertys_.property().size(); intPropertyCount++)
            {
                for (int intPropCount = 0; intPropCount < createRoomInfo->roomprop().property().size(); intPropCount++)
                {
                    if (propertys_.property(intPropertyCount).name() == createRoomInfo->roomprop().property(intPropCount).name())
                    {
                        propertys_.mutable_property(intPropertyCount)->set_primary(createRoomInfo->roomprop().property(intPropCount).primary());
                        switch (propertys_.property(intPropertyCount).type()) {
                            case msg::PropertyType::INT:
                            {
                                propertys_.mutable_property(intPropertyCount)->mutable_intvalue()->set_value(createRoomInfo->roomprop().property(intPropCount).intvalue().value());
                                break;
                            }
                            case msg::PropertyType::BOOL:
                            {
                                propertys_.mutable_property(intPropertyCount)->set_boolvalue(createRoomInfo->roomprop().property(intPropCount).boolvalue());
                                break;
                            }
                            case msg::PropertyType::STRING:
                            {
                                propertys_.mutable_property(intPropertyCount)->set_stringvalue(createRoomInfo->roomprop().property(intPropCount).stringvalue());
                                break;
                            }
                            case msg::PropertyType::ENUM:
                            {
                                propertys_.mutable_property(intPropertyCount)->mutable_enumvalue()->set_value(createRoomInfo->roomprop().property(intPropCount).enumvalue().value());
                                break;
                            }
                        }
                    }
                }
            }
        }
    }else
    {
        propertys_.clear_property();
        
        const msg::RoomInfomation createRoomInfomation = GDM->get_room_infomations();
        
        if (is_create_)
        {
            btn_create_game_room_->setVisible(true);
            btn_create_game_room_->setTitleText("确定");
            btn_create_game_room_->addClickEventListener(std::bind(&LayerCreateGameRoom::click_btn_create_game_room, this, std::placeholders::_1));
            
            text_title_->setString("创建房间");
            
            int_time_input_ = tools::date_double();
        
            if (createRoomInfo)
            {
                tournament_info_.CopyFrom(createRoomInfo->tournamentinfo());
                template_size_name_ = createRoomInfo->templatesizename();
                template_speed_name_ = createRoomInfo->templatespeedname();
            }
            else
            {
                const msg::TournamentInfo templateSizeDefault = createRoomInfomation.tournamentsizetemplate(0);
                tournament_info_.CopyFrom(templateSizeDefault);
                template_size_name_ = templateSizeDefault.templatename();
                
                const msg::TournamentInfo templateSpeedDefault = createRoomInfomation.tournamentspeedtemplate(0);
                tournament_info_.set_maxaddonlevel(templateSpeedDefault.maxaddonlevel());
                tournament_info_.set_startbb(templateSpeedDefault.startbb());
                tournament_info_.mutable_tournamentlevel()->CopyFrom(templateSpeedDefault.tournamentlevel());
                template_speed_name_ = templateSpeedDefault.templatename();
                
                tournament_info_.set_actiontime(60);
                tournament_info_.set_prizepercent(100);
                tournament_info_.set_playerpertable(10);
            }
        }else{
            tournament_info_.CopyFrom(enter_room_info_.tournamentinfo());
            int_time_input_ = tools::stod(tournament_info_.starttime()) / 1000;
            
            if (room_info_.createrinfo().userid() == GDM->get_user_id() &&
                room_info_.roomstatus() == msg::RoomStatus::WAITINGTOSTART)
            {
                btn_create_game_room_->setVisible(true);
                btn_create_game_room_->setTitleText(tools::local_string("cancle_game","取消比赛"));
                btn_create_game_room_->addClickEventListener(std::bind(&LayerCreateGameRoom::click_btn_cancel_tournament, this, std::placeholders::_1));
            }else
            {
                btn_create_game_room_->setVisible(false);
            }
            
            text_title_->setString(tools::local_string( "game_info","比赛信息"));
            
            roomname.set_stringvalue(room_info_.roomname());
        }
        
        //IsSitAndGo 坐满即玩
        msg::Property* propertyIsSitAndGo = propertys_.add_property();
        propertyIsSitAndGo->set_name("IsSitAndGo");
        propertyIsSitAndGo->set_displayname(tools::local_string("need_full","是否坐满即玩"));
        propertyIsSitAndGo->set_type(msg::PropertyType::BOOL);
        propertyIsSitAndGo->set_boolvalue(tournament_info_.issitandgo());
        propertyIsSitAndGo->set_primary(true);
        
        //PrizePercent 奖池比例
        msg::Property* propertyPrizePercent = propertys_.add_property();
        propertyPrizePercent->set_name("PrizePercent");
        propertyPrizePercent->set_displayname(tools::local_string("rewards_pool","奖池比例"));
        propertyPrizePercent->set_type(msg::PropertyType::INT);
        propertyPrizePercent->mutable_intvalue()->set_value(tournament_info_.prizepercent());
        propertyPrizePercent->mutable_intvalue()->set_min(tools::to_string(1));
        propertyPrizePercent->mutable_intvalue()->set_max(tools::to_string(100));

        //Buyin 报名费用
        msg::Property* propertyBuyin = propertys_.add_property();
        propertyBuyin->set_name("Buyin");
        propertyBuyin->set_displayname(tools::local_string("registration_fee","报名费用"));
        propertyBuyin->set_type(msg::PropertyType::INT);
        propertyBuyin->mutable_intvalue()->set_value(tournament_info_.buyin());
        propertyBuyin->set_primary(true);
        
        //MinPlayerNum 最少人数
        msg::Property* propertyMinPlayerNum = propertys_.add_property();
        propertyMinPlayerNum->set_name("MinPlayerNum");
        propertyMinPlayerNum->set_displayname("最少人数");
        propertyMinPlayerNum->set_type(msg::PropertyType::INT);
        propertyMinPlayerNum->mutable_intvalue()->set_value(tournament_info_.minplayernum());
        propertyMinPlayerNum->set_primary(true);
        
        //MaxPlayerNum 最大人数
        msg::Property* propertyMaxPlayerNum = propertys_.add_property();
        propertyMaxPlayerNum->set_name("MaxPlayerNum");
        propertyMaxPlayerNum->set_displayname("最大人数");
        propertyMaxPlayerNum->set_type(msg::PropertyType::INT);
        propertyMaxPlayerNum->mutable_intvalue()->set_value(tournament_info_.maxplayernum());
        propertyMaxPlayerNum->set_primary(true);
        
        //ActionTime 操作时间
        msg::Property* propertyActionTime = propertys_.add_property();
        propertyActionTime->set_name("ActionTime");
        propertyActionTime->set_displayname(tools::local_string("operate_time","操作时间"));
        propertyActionTime->set_type(msg::PropertyType::INT);
        propertyActionTime->mutable_intvalue()->set_value(tournament_info_.actiontime());
        propertyActionTime->mutable_intvalue()->add_defaultvalue(15);
        propertyActionTime->mutable_intvalue()->add_defaultvalue(30);
        propertyActionTime->mutable_intvalue()->add_defaultvalue(45);
        propertyActionTime->mutable_intvalue()->add_defaultvalue(60);
        propertyActionTime->mutable_intvalue()->add_defaultvalue(90);
        propertyActionTime->mutable_intvalue()->add_defaultvalue(120);
        propertyActionTime->mutable_intvalue()->set_min("15");
        propertyActionTime->mutable_intvalue()->set_max("120");
        
        //PlayerPerTable 每桌最大人数
        msg::Property* propertyPlayerPerTable = propertys_.add_property();
        propertyPlayerPerTable->set_name("PlayerPerTable");
        propertyPlayerPerTable->set_displayname(tools::local_string("PlayerPerTable","每桌最大人数"));
        propertyPlayerPerTable->set_type(msg::PropertyType::INT);
        propertyPlayerPerTable->mutable_intvalue()->set_value(tournament_info_.playerpertable());
        propertyPlayerPerTable->mutable_intvalue()->set_min(tools::to_string(3));
        propertyPlayerPerTable->mutable_intvalue()->set_max(tools::to_string(10));
        
        //StartBigBlind 初始筹码
        msg::Property* propertyStartBigBlind = propertys_.add_property();
        propertyStartBigBlind->set_name("StartBigBlind");
        propertyStartBigBlind->set_displayname("初始筹码");
        propertyStartBigBlind->set_type(msg::PropertyType::INT);
        propertyStartBigBlind->mutable_intvalue()->set_value(tournament_info_.startbb());
        
        //MaxAddonLevel 最大重买入级别
        msg::Property* propertyMaxAddonLevel = propertys_.add_property();
        propertyMaxAddonLevel->set_name("MaxAddonLevel");
        propertyMaxAddonLevel->set_displayname("最大重买入级别");
        propertyMaxAddonLevel->set_type(msg::PropertyType::INT);
        propertyMaxAddonLevel->mutable_intvalue()->set_value(tournament_info_.maxaddonlevel());
        
        if (is_create_)
        {
            //TemplateSize 规模
            msg::Property* propertyTemplateSize = propertys_.add_property();
            propertyTemplateSize->set_name("TemplateSize");
            propertyTemplateSize->set_displayname(tools::local_string("TemplateSize","规模"));
            propertyTemplateSize->set_type(msg::PropertyType::ENUM);
            
            msg::EnumItem * enumItemsSizeCustom = propertyTemplateSize->mutable_enumvalue()->add_items();
            enumItemsSizeCustom->set_value(0);
            enumItemsSizeCustom->set_text("自定义");
            
            int intTemplateSizeCurrent = 0;
            for (int intTemplateSizeCount = 0; intTemplateSizeCount < createRoomInfomation.tournamentsizetemplate().size(); intTemplateSizeCount++)
            {
                if (template_size_name_ == createRoomInfomation.tournamentsizetemplate(intTemplateSizeCount).templatename())
                {
                    intTemplateSizeCurrent = intTemplateSizeCount + 1;
                }
                
                msg::EnumItem * enumItems = propertyTemplateSize->mutable_enumvalue()->add_items();
                enumItems->set_value(intTemplateSizeCount + 1);
                enumItems->set_text(createRoomInfomation.tournamentsizetemplate(intTemplateSizeCount).templatename());
            }
            propertyTemplateSize->mutable_enumvalue()->set_value(intTemplateSizeCurrent);
            propertyTemplateSize->set_primary(true);
            
//            //TemplateSpeed 速度
//            msg::Property* propertyTemplateSpeed = propertys_.add_property();
//            propertyTemplateSpeed->set_name("TemplateSpeed");
//            propertyTemplateSpeed->set_displayname(tools::local_string("TemplateSpeed","速度"));
//            propertyTemplateSpeed->set_type(msg::PropertyType::ENUM);
//            
//            msg::EnumItem * enumItemsSpeedCustom = propertyTemplateSpeed->mutable_enumvalue()->add_items();
//            enumItemsSpeedCustom->set_value(0);
//            enumItemsSpeedCustom->set_text("自定义");
//            
//            int intTemplateSpeedCurrent = 0;
//            for (int intTemplateSpeedCount = 0; intTemplateSpeedCount < createRoomInfomation.tournamentspeedtemplate().size(); intTemplateSpeedCount++)
//            {
//                if (template_speed_name_ == createRoomInfomation.tournamentspeedtemplate(intTemplateSpeedCount).templatename())
//                {
//                    intTemplateSpeedCurrent = intTemplateSpeedCount + 1;
//                }
//                
//                msg::EnumItem * enumItems = propertyTemplateSpeed->mutable_enumvalue()->add_items();
//                enumItems->set_value(intTemplateSpeedCount + 1);
//                enumItems->set_text(createRoomInfomation.tournamentspeedtemplate(intTemplateSpeedCount).templatename());
//            }
//            propertyTemplateSpeed->mutable_enumvalue()->set_value(intTemplateSpeedCurrent);
//            propertyTemplateSpeed->set_primary(true);
            
            //
            msg::Property * propertyBlindTime = propertys_.add_property();
            propertyBlindTime->CopyFrom(createRoomInfomation.blindtime());
            propertyBlindTime->mutable_intvalue()->set_value(tournament_info_.blindtime()
                                                         );
            propertyBlindTime->set_primary(true);
        }
    }
}

void LayerCreateGameRoom::refresh_listview()
{
    listview_->removeAllItems();
    
    cocos2d::Size sizeLayoutDisplayMore = layout_display_more_->getContentSize();
    
    float listviewHeight = 0.0;
    
    float listviewHeightMin = 150.0 + sizeLayoutDisplayMore.height;
    float listviewHeightMax = 920.0 - sizeLayoutDisplayMore.height;
    
    if (is_tournament_)
    {
        text_join_list_->setVisible(!is_create_);
        
        NodePropertyCell * cellRoomType = NodePropertyCell::create(this, is_create_);
        cellRoomType->update_roomtype_data(room_type_, is_tournament_);
        cellRoomType->set_select_roomtype_callback(std::bind(&LayerCreateGameRoom::AfterSelectRoomType, this, std::placeholders::_1, std::placeholders::_2));
        if (!is_create_)
        {
            cellRoomType->setTouchEnabled(false);
        }
        listview_->pushBackCustomItem(cellRoomType);
        
        listviewHeight += cellRoomType->getContentSize().height;
        
        NodePropertyCell * cellGroupInfo = NodePropertyCell::create(this, is_create_);
        cellGroupInfo->update_group_data(select_groupinfo_);
        cellGroupInfo->set_select_group_callback(std::bind(&LayerCreateGameRoom::afterSelectGroup, this, std::placeholders::_1));
        if (!is_create_)
        {
            cellGroupInfo->setTouchEnabled(false);
        }
        listview_->pushBackCustomItem(cellGroupInfo);
        
        listviewHeight += cellGroupInfo->getContentSize().height;
        
        NodePropertyCell * cellRoomName = NodePropertyCell::create(this, is_create_);
        roomname.set_type(msg::PropertyType::STRING);
        roomname.set_displayname(tools::local_string("game_name","比赛名称"));
        cellRoomName->update_data(roomname);
        cellRoomName->set_property_callback(std::bind(&LayerCreateGameRoom::afterChangeRoomName, this, std::placeholders::_1));
        listview_->pushBackCustomItem(cellRoomName);
        if (!is_create_)
        {
            cellRoomName->setTouchEnabled(false);
        }
        listviewHeight += cellRoomName->getContentSize().height;
        
        NodePropertyCell * cellStartTime = NodePropertyCell::create(this, is_create_);
        cellStartTime->update_time_input(int_time_input_);
        cellStartTime->set_select_time_callback(std::bind(&LayerCreateGameRoom::afterSelectTime, this, std::placeholders::_1));
        listview_->pushBackCustomItem(cellStartTime);
        if (!is_create_)
        {
            cellStartTime->setTouchEnabled(false);
        }
        listviewHeight += cellStartTime->getContentSize().height;
        
        for (int i = 0; i< propertys_.property().size(); i++) {
            if (!is_display_all_ && !propertys_.property(i).primary())
            {
                continue;
            }
            NodePropertyCell * propertyCell = NodePropertyCell::create(this, is_create_);
            
            if (propertys_.property(i).name() == "TemplateSize")
            {
                propertyCell->set_property_callback(std::bind(&LayerCreateGameRoom::afterSelectTemplateSize, this, std::placeholders::_1));
            }else if (propertys_.property(i).name() == "TemplateSpeed")
            {
                propertyCell->set_property_callback(std::bind(&LayerCreateGameRoom::afterSelectTemplateSpeed, this, std::placeholders::_1));
            }else
            {
                propertyCell->set_property_callback(std::bind(&LayerCreateGameRoom::afterChangeProp, this, std::placeholders::_1));
            }
            
            propertyCell->update_data(propertys_.property(i));
            
            if (propertys_.property(i).type() == msg::PropertyType::INT &&
                propertys_.property(i).intvalue().defaultvalue().size() > 0)
                update_cell_int_value_min_max(propertyCell);
            
            
            listviewHeight += propertyCell->getContentSize().height;
            if (!is_create_)
            {
                propertyCell->setTouchEnabled(false);
            }
            listview_->pushBackCustomItem(propertyCell);
        }
        
        if (is_display_all_)
        {
            NodePropertyCell * cellBlindInfo = NodePropertyCell::create(this);
            cellBlindInfo->update_click_cell(tools::local_string("the_Blinds_level","盲注级别"));
            cellBlindInfo->set_click_cell_callback([&](){
                LayerCreateTourBlindAndReward* layerTourBlindAndReward = LayerCreateTourBlindAndReward::create(tournament_info_, is_create_, LayerCreateTourBlindAndReward::SELECT_TYPE::BLIND_INFO);
                layerTourBlindAndReward->set_level_change_callback(std::bind(&LayerCreateGameRoom::afterChangeTourBlind, this, std::placeholders::_1));
                layerTourBlindAndReward->set_prize_change_callback(std::bind(&LayerCreateGameRoom::afterChangeTourReward, this, std::placeholders::_1));
                this->addChild(layerTourBlindAndReward, 0, layerTourBlindAndReward->GetName());
            });
            listview_->pushBackCustomItem(cellBlindInfo);
            
            listviewHeight += cellBlindInfo->getContentSize().height;
            
            
            NodePropertyCell * cellRewardInfo = NodePropertyCell::create(this);
            cellRewardInfo->update_click_cell(tools::local_string("pond_details","奖池结构"));
            cellRewardInfo->set_click_cell_callback([&](){
                LayerCreateTourBlindAndReward* layerTourBlindAndReward = LayerCreateTourBlindAndReward::create(tournament_info_, is_create_, LayerCreateTourBlindAndReward::SELECT_TYPE::REWARD_INFO);
                layerTourBlindAndReward->set_level_change_callback(std::bind(&LayerCreateGameRoom::afterChangeTourBlind, this, std::placeholders::_1));
                layerTourBlindAndReward->set_prize_change_callback(std::bind(&LayerCreateGameRoom::afterChangeTourReward, this, std::placeholders::_1));
                this->addChild(layerTourBlindAndReward, 0, layerTourBlindAndReward->GetName());
            });
            listview_->pushBackCustomItem(cellRewardInfo);
            
            listviewHeight += cellRewardInfo->getContentSize().height;
        }
    }else
    {
        text_join_list_->setVisible(false);
        
        NodePropertyCell * cell1 = NodePropertyCell::create(this);
        cell1->update_roomtype_data(room_type_, is_tournament_);
        cell1->set_select_roomtype_callback(std::bind(&LayerCreateGameRoom::AfterSelectRoomType, this, std::placeholders::_1, std::placeholders::_2));
        listview_->pushBackCustomItem(cell1);
        
        listviewHeight += cell1->getContentSize().height;
        
        NodePropertyCell * cell2 = NodePropertyCell::create(this);
        cell2->update_group_data(select_groupinfo_);
        cell2->set_select_group_callback(std::bind(&LayerCreateGameRoom::afterSelectGroup, this, std::placeholders::_1));
        
        listview_->pushBackCustomItem(cell2);
        
        listviewHeight += cell2->getContentSize().height;
        
        NodePropertyCell * cell3 = NodePropertyCell::create(this);
        roomname.set_type(msg::PropertyType::STRING);
        roomname.set_displayname(tools::local_string("text_room", "房间名称"));
        cell3->update_data(roomname);
        cell3->set_property_callback(std::bind(&LayerCreateGameRoom::afterChangeRoomName, this, std::placeholders::_1));
        listview_->pushBackCustomItem(cell3);
        
        listviewHeight += cell3->getContentSize().height;
        

        for (int i=0; i< propertys_.property().size(); i++) {
            if (!is_display_all_ && !propertys_.property(i).primary())
            {
                continue;
            }
            NodePropertyCell * propertyCell = NodePropertyCell::create(this);
            propertyCell->set_property_callback(std::bind(&LayerCreateGameRoom::afterChangeProp, this, std::placeholders::_1));
            propertyCell->update_data(propertys_.property(i));
            
            if (propertys_.property(i).type() == msg::PropertyType::INT &&
                propertys_.property(i).intvalue().defaultvalue().size() > 0)
                update_cell_int_value_min_max(propertyCell);
            
            
            listviewHeight += propertyCell->getContentSize().height;
            listview_->pushBackCustomItem(propertyCell);
        }
    }
    
    listviewHeight = std::max(listviewHeight, listviewHeightMin);
    listviewHeight = std::min(listviewHeight, listviewHeightMax);
    
    listview_->setContentSize(cocos2d::Size(listview_->getContentSize().width, listviewHeight));
    image_listview_bg_->setContentSize(cocos2d::Size(image_listview_bg_->getContentSize().width, listviewHeight + 80 + sizeLayoutDisplayMore.height));
    
    layout_display_more_->setPositionY(listview_->getPositionY() - listviewHeight - sizeLayoutDisplayMore.height);
    
    listview_->refreshView();
}

void LayerCreateGameRoom::update_cell_int_value_min_max(NodePropertyCell* cell)
{
    int min=0, max=0;
    const msg::Property* prop = cell->get_cell_property();
    if (tools::IsNumberic(prop->intvalue().min()))
        min = tools::stoi(prop->intvalue().min());
    if (tools::IsNumberic(prop->intvalue().max()))
        max = tools::stoi(prop->intvalue().max());
    for (int i = 0; i < propertys_.property().size(); i++)
    {
        if (propertys_.property(i).name() == prop->intvalue().max())
            max = propertys_.property(i).intvalue().value();
        else if (propertys_.property(i).name() == prop->intvalue().min())
            min = propertys_.property(i).intvalue().value();
        
    }
    cell->update_value_min_max(min, max);
}

void LayerCreateGameRoom::update_enter_room_info(msg::Processor_410_EnterRoom_DOWN enterRoomInfo, bool isCreate)
{
    
    if (!enterRoomInfo.roominfo().has_groupinfo())
    {
        msg::GroupInfo nullgroup;
        nullgroup.set_groupid(0);
        
        set_groupinfo(nullgroup);
    }else
    {
        set_groupinfo(enterRoomInfo.roominfo().groupinfo());
    }
    
    
    set_enter_room_info(enterRoomInfo);
    set_room_info(enterRoomInfo.roominfo());
    
    room_type_ = enterRoomInfo.roominfo().roomtype().roomtype();
    is_tournament_ = enterRoomInfo.roominfo().istournament();
    is_create_ = isCreate;
    
    init_create_property();
    
    refresh_listview();
    
    
    LayerCreateTourBlindAndReward* layerTourBlAndRe = dynamic_cast<LayerCreateTourBlindAndReward*>(getChildByName(typeid(LayerCreateTourBlindAndReward).name()));
    if (layerTourBlAndRe)
    {
        layerTourBlAndRe->update_data(tournament_info_, is_create_);
    }
    LayerTourJoinUserList* layerTourUserList = dynamic_cast<LayerTourJoinUserList*>(getChildByName(typeid(LayerTourJoinUserList).name()));
    if (layerTourUserList)
    {
        std::vector<msg::UserDetailInfo> vecUserInfos;
        for (int intUserCount = 0; intUserCount < enter_room_info_.userinfo().size(); intUserCount++)
        {
            vecUserInfos.push_back(enter_room_info_.userinfo(intUserCount));
        }
        
        layerTourUserList->update_data(room_info_.roomid(), vecUserInfos);
    }
}

#pragma mark click_event
void LayerCreateGameRoom::click_btn_back(Ref * sender)
{
    this->removeFromParent();
}

void LayerCreateGameRoom::click_text_join_list(Ref * sender)
{
    std::vector<msg::UserDetailInfo> vecUserInfos;
    for (int intUserCount = 0; intUserCount < enter_room_info_.userinfo().size(); intUserCount++)
    {
        vecUserInfos.push_back(enter_room_info_.userinfo(intUserCount));
    }
    LayerTourJoinUserList* tourJoinList = LayerTourJoinUserList::create(room_info_.roomid(), vecUserInfos);
    this->addChild(tourJoinList, 0, tourJoinList->GetName());
}

void LayerCreateGameRoom::click_btn_display_more(Ref * sender)
{
    cocos2d::log("click_btn_display_more");
    
    is_display_all_ = !is_display_all_;
    
    std::string more_setting = tools::local_string("more_setting","高级");
    std::string simple_setting = tools::local_string("simple_setting","收起");
    text_display_more_->setString(is_display_all_ ? simple_setting : more_setting);
    
    refresh_listview();
}

void LayerCreateGameRoom::afterSelectGroup(const msg::GroupInfo& groupinfo)
{
    select_groupinfo_.CopyFrom(groupinfo);
}

void LayerCreateGameRoom::afterChangeRoomName(const msg::Property& prop)
{
    roomname.CopyFrom(prop);
}

void LayerCreateGameRoom::afterChangeProp(const msg::Property& prop)
{
    for (int i=0; i< propertys_.property().size(); i++) {
        if (propertys_.property(i).name() == prop.name())
            propertys_.mutable_property(i)->CopyFrom(prop);
    }
    for (int i=0; i< propertys_.property().size(); i++) {
        if (propertys_.property(i).type() == msg::PropertyType::INT &&
            propertys_.property(i).intvalue().defaultvalue().size() > 0)
        {
            if (propertys_.property(i).intvalue().max() == prop.name() || propertys_.property(i).intvalue().min() == prop.name()){
                for (auto item: listview_->getItems()){
                    NodePropertyCell* cell = dynamic_cast<NodePropertyCell*>(item);
                    if (cell->get_cell_property() && cell->get_cell_property()->name() == propertys_.property(i).name())
                        update_cell_int_value_min_max(cell);
                }
            }
        }
    }
}

void LayerCreateGameRoom::AfterSelectRoomType(msg::RoomType room_type, bool is_tournament)
{

    if (room_type_ == room_type && is_tournament_ == is_tournament)
    {
        
    }else
    {
        room_type_ = room_type;
        is_tournament_ = is_tournament;

        init_create_property();
        
        refresh_listview();
    }
    
    auto layer = getChildByName(typeid(LayerChooseGameType).name());
    if (layer)
        layer->removeFromParent();
}

void LayerCreateGameRoom::afterChangeTourBlind(msg::TournamentLevel tourLevel)
{
    if (is_tournament_)
    {
        tournament_info_.mutable_tournamentlevel()->CopyFrom(tourLevel);
    }
}

void LayerCreateGameRoom::afterChangeTourReward(msg::TournamentPrize tourPrize)
{
    if (is_tournament_)
    {
        tournament_info_.mutable_tournamentprize()->CopyFrom(tourPrize);
    }
}

void LayerCreateGameRoom::afterSelectTemplateSize(const msg::Property& prop)
{
    if (prop.enumvalue().value() > 0)
    {
        const msg::RoomInfomation createRoomInfomation = GDM->get_room_infomations();
        
        const msg::TournamentInfo selectTemplateSize = createRoomInfomation.tournamentsizetemplate(prop.enumvalue().value() - 1);
        
        template_size_name_ = selectTemplateSize.templatename();
        
        for (int iterPropCount = 0; iterPropCount < propertys_.property().size(); iterPropCount++)
        {
            if (propertys_.property(iterPropCount).name() == "MinPlayerNum")
            {
                propertys_.mutable_property(iterPropCount)->mutable_intvalue()->set_value(selectTemplateSize.minplayernum());
            }
            else if (propertys_.property(iterPropCount).name() == "MaxPlayerNum")
            {
                propertys_.mutable_property(iterPropCount)->mutable_intvalue()->set_value(selectTemplateSize.maxplayernum());
            }
        }
        
        //tournament_info_.mutable_tournamentprize()->CopyFrom(selectTemplateSize.tournamentprize());
    }
    for (int i=0; i< propertys_.property().size(); i++) {
        if (propertys_.property(i).name() == prop.name())
            propertys_.mutable_property(i)->CopyFrom(prop);
    }
    
    refresh_listview();
}

void LayerCreateGameRoom::afterSelectTemplateSpeed(const msg::Property& prop)
{
    if (prop.enumvalue().value() > 0)
    {
        const msg::RoomInfomation createRoomInfomation = GDM->get_room_infomations();
        
        const msg::TournamentInfo selectTemplateSpeed = createRoomInfomation.tournamentspeedtemplate(prop.enumvalue().value() - 1);
        
        template_speed_name_ = selectTemplateSpeed.templatename();
        
        for (int iterPropCount = 0; iterPropCount < propertys_.property().size(); iterPropCount++)
        {
            if (propertys_.property(iterPropCount).name() == "MaxAddonLevel")
            {
                propertys_.mutable_property(iterPropCount)->mutable_intvalue()->set_value(selectTemplateSpeed.maxaddonlevel());
            }
            else if (propertys_.property(iterPropCount).name() == "StartBigBlind")
            {
                propertys_.mutable_property(iterPropCount)->mutable_intvalue()->set_value(selectTemplateSpeed.startbb());
            }
        }
        tournament_info_.mutable_tournamentlevel()->CopyFrom(selectTemplateSpeed.tournamentlevel());

    }
    for (int i=0; i< propertys_.property().size(); i++) {
        if (propertys_.property(i).name() == prop.name())
            propertys_.mutable_property(i)->CopyFrom(prop);
    }
    
    refresh_listview();

}

void LayerCreateGameRoom::afterSelectTime(long long int selectTime)
{
    int_time_input_ = selectTime;
}

void LayerCreateGameRoom::click_btn_create_game_room(Ref * sender)
{
    if(roomname.stringvalue().empty()) {
        ShowError(tools::local_string("roomname_empty", "房间名称不能为空"));
        return;
    }
    
    msg::RoomInfo roominfo;
    roominfo.mutable_groupinfo()->set_groupid(select_groupinfo_.groupid());
    roominfo.mutable_roomtype()->set_roomtype(room_type_);
    roominfo.set_roomname(roomname.stringvalue());
    
    if (!is_tournament_)
    {
        roominfo.set_istournament(false);
        auto processor = PM->CreateRoom_UP(roominfo, propertys_);
        send_data(processor,REQ_CREATE_ROOM);
        GDM->set_createroom_info(room_type_, false, roominfo, propertys_);
    }else{
        roominfo.set_istournament(true);
        
        for (auto iter : propertys_.property())
        {
            if (iter.name() == "IsSitAndGo")
            {
                tournament_info_.set_issitandgo(iter.boolvalue());
            }else if (iter.name() == "PrizePercent")
            {
                tournament_info_.set_prizepercent(iter.intvalue().value());
            }else if (iter.name() == "Buyin")
            {
                tournament_info_.set_buyin(iter.intvalue().value());
            }else if (iter.name() == "ActionTime")
            {
                tournament_info_.set_actiontime(iter.intvalue().value());
            }else if (iter.name() == "MinPlayerNum")
            {
                tournament_info_.set_minplayernum(iter.intvalue().value());
            }else if (iter.name() == "MaxPlayerNum")
            {
                tournament_info_.set_maxplayernum(iter.intvalue().value());
            }else if (iter.name() == "PlayerPerTable")
            {
                tournament_info_.set_playerpertable(iter.intvalue().value());
            }else if (iter.name() == GDM->get_room_infomations().blindtime().name())
            {
                tournament_info_.set_blindtime(iter.intvalue().value());
            }
        }
        
        //cocos2d::log("%s", tools::time_display(int_time_input_).c_str());
        tournament_info_.set_starttime(tools::to_string(int_time_input_ * 1000));
        
        auto processor = PM->CreateTournament_UP(roominfo, tournament_info_);
        send_data(processor, REQ_CREATE_ROOM);
        GDM->set_createroom_info(room_type_, true, roominfo, template_size_name_, template_speed_name_, tournament_info_);
    }
}

void LayerCreateGameRoom::click_btn_cancel_tournament(Ref * sender)
{
    auto processor = PM->CancelTournament_UP(room_info_.roomid());
    send_data(processor, REQ_CANCEL_MATCH);
}

void LayerCreateGameRoom::register_events()
{
    dispatch_.register_event(REQ_CREATE_ROOM,
                             BIND_FUNC(this, LayerCreateGameRoom::handle_create_room));
    dispatch_.register_event(REQ_CANCEL_MATCH,
                             BIND_FUNC(this, LayerCreateGameRoom::handle_cancel_tournament));
}

void LayerCreateGameRoom::handle_create_room(cmd_data_pointer data)
{
    ClearError();
    std::shared_ptr<net_data_recv_package> rec = dynamic_pointer_cast<net_data_recv_package>(data);
    if (!check_request(rec)) return;
    
    msg::Processor_152_CreateRoom_down processor;
    processor.ParseFromArray(rec->body, rec->bodysize);
    switch (processor.response().status()) {
        case msg::Status::SUCCESS: {
            PDM->send_enter_room(processor.roomid());
            if (dis_miss_callback_)
            {
                dis_miss_callback_();
            }
            else
            {
                cocos2d::log("LayerCreateGameRoom::handle_create_room removeFromParent");
                this->removeFromParent();
            }
            break;
        }
        case msg::Status::FAILED: {
            ShowError(processor.response().message());
            
            break;
        }
            
    }
    
}

void LayerCreateGameRoom::handle_cancel_tournament(cmd_data_pointer data)
{
    ClearError();
    
    std::shared_ptr<net_data_recv_package> rec = dynamic_pointer_cast<net_data_recv_package>(data);
    if(!check_request(rec)) return;
    
    auto processor = PM->response_down(rec);
    
    
    processor.ParseFromArray(rec->body, rec->bodysize);
    switch (processor.status()) {
        case msg::Status::SUCCESS: {
            TipView::showAlertView(processor.message());
            this->removeFromParent();
            
            break;
        }
        case msg::Status::FAILED: {
            ShowError(processor.message());

            break;
        }
            
    }
}

#pragma mark ConnectStatus
void LayerCreateGameRoom::RefreshLayer()
{
    
}

void LayerCreateGameRoom::LayerDisconnect()
{

}

