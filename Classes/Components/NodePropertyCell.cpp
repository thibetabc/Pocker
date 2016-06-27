//
//  NodePropertyCell.cpp
//  pokerclient
//
//  Created by lvzhangle on 16/3/27.
//
//

#include "NodePropertyCell.h"
#include "GameTools.h"
#include "utils.h"
#include "LayerGroupList.h"
#include "LayerChooseGameType.h"
#include "LayerMenu.h"

NodePropertyCell* NodePropertyCell::create(Node* ParentLayer, bool is_edit)
{
    NodePropertyCell* result = new NodePropertyCell;
    result->set_ParentLayer(ParentLayer);
    if(result->init(is_edit)) {
        result->autorelease();
        return result;
    }
    return nullptr;
}

void NodePropertyCell::set_ParentLayer(Node* ParentLayer)
{
    ParentLayer_ = ParentLayer;
}

const msg::Property* NodePropertyCell::get_cell_property()
{
    if (cell_type_ >= VALUE_STRING)
        return &property_;
    else
        return nullptr;
}

bool NodePropertyCell::init(bool is_edit)
{
    if(!Widget::init()) {
        return false;
    }
    
    is_edit_ = is_edit;
    cell_type_ = NONE;
    int_value_max_ = 0;
    int_value_min_ = 0;
    
    this->setAnchorPoint(Vec2::ZERO);
    
    this->setTouchEnabled(true);
    
    //    this->addTouchCallback([this](Ref*, Touch* touch, Widget::TouchEventType event){
    //        ListView* listview = (ListView*) getParent();
    //        if(listview) {
    //            switch (event) {
    //                case Widget::TouchEventType::BEGAN:
    //                    listview->onTouchBegan(touch, nullptr);
    //                    break;
    //
    //                case Widget::TouchEventType::MOVED:
    //                    listview->onTouchMoved(touch, nullptr);
    //                    break;
    //                case Widget::TouchEventType::ENDED:
    //                    listview->onTouchEnded(touch, nullptr);
    //                    break;
    //                case Widget::TouchEventType::CANCELED:
    //                    listview->onTouchCancelled(touch, nullptr);
    //                    break;
    //                default:
    //                    break;
    //            }
    //        }
    //    });
    
    return true;
}

void NodePropertyCell::TouchGroup(Ref *sender, Widget::TouchEventType touchtype)
{
    if (!ParentLayer_ || touchtype != Widget::TouchEventType::ENDED) return;
    LayerGroupList* lgl = LayerGroupList::create();
    lgl->set_group_layer_type(LayerGroupList::GAMESELECTGROUP);
    lgl->set_select_group_callback(std::bind(&NodePropertyCell::AfterSelectGroup, this, std::placeholders::_1));
    ParentLayer_->addChild(lgl, 0, lgl->getName());
}

void NodePropertyCell::TouchRoomType(Ref *sender, Widget::TouchEventType touchtype)
{
    if (!ParentLayer_ || touchtype != Widget::TouchEventType::ENDED) return;
    LayerChooseGameType * choosegametype = LayerChooseGameType::create();
    choosegametype->set_choose_call_func(std::bind(&NodePropertyCell::AfterSelectRoomType, this, std::placeholders::_1, std::placeholders::_2));
    ParentLayer_->addChild(choosegametype, 0, choosegametype->GetName());
}

void NodePropertyCell::click_click_cell(Ref * sender)
{
    if (click_cell_callback_)
    {
        click_cell_callback_();
    }
}

void NodePropertyCell::AfterSelectGroup(const msg::GroupInfo& groupinfo)
{
    update_group_data(groupinfo);
    if (select_group_callback_)
    {
        select_group_callback_(groupinfo);
    }
}

void NodePropertyCell::AfterSelectRoomType(msg::RoomType room_type, bool is_tournament)
{
    cocos2d::log("NodePropertyCell : AfterSelectRoomType");
    update_roomtype_data(room_type, is_tournament);
    if (select_roomtype_callback_)
    {
        select_roomtype_callback_(room_type, is_tournament);
    }
}

void NodePropertyCell::update_roomtype_data(msg::RoomType room_type, bool is_tournament)
{
    if (cell_type_ != GAME_TYPE)
    {
        cell_type_ = GAME_TYPE;
        
        root_ = CSLoader::createNode("NodeCreateRoomCell_GameType.csb");
        this->addChild(root_);
        
        image_property_icon_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_property_icon"));
        image_cell_line_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_createroomcell_line"));
        text_property_name_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_property_name"));
        image_select_icon_1_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_select_icon_1"));
        
        text_property_name_->setTouchEnabled(true);
        
        text_property_name_->addTouchCallback([this](Ref*, Touch* touch, Widget::TouchEventType event){
            ListView* listview = (ListView*) getParent();
            if(listview) {
                switch (event) {
                    case Widget::TouchEventType::BEGAN:
                        listview->onTouchBegan(touch, nullptr);
                        break;
                        
                    case Widget::TouchEventType::MOVED:
                        listview->onTouchMoved(touch, nullptr);
                        break;
                    case Widget::TouchEventType::ENDED:
                        listview->onTouchEnded(touch, nullptr);
                        break;
                    case Widget::TouchEventType::CANCELED:
                        listview->onTouchCancelled(touch, nullptr);
                        break;
                    default:
                        break;
                }
            }
        });
        
        this->setContentSize(cocos2d::Size(750 ,110));
        addTouchEventListener(std::bind(&NodePropertyCell::TouchRoomType, this, std::placeholders::_1, std::placeholders::_2));
    }
    
    if (is_tournament)
    {
        
    }else
    {
        switch(room_type){
            case msg::RoomType::NLHOLDEM:
            {
                image_property_icon_->loadTexture("login_res/creategameroom_res/image_createroom_type_icon_holdem.png");
                text_property_name_->setString(tools::local_string("holdempoker", "德州扑克"));
                break;
            }
            case msg::RoomType::OFC3:
            {
                image_property_icon_->loadTexture("login_res/creategameroom_res/image_createroom_type_icon_ofc3.png");
                text_property_name_->setString(tools::local_string("pineapplepoker", "大菠萝"));
                break;
            }
            case msg::RoomType::OFC4:
            {
                image_property_icon_->loadTexture("login_res/creategameroom_res/image_createroom_type_icon_ofc3.png");
                text_property_name_->setString(tools::local_string("openfacechinesepoker", "明牌十三张"));
                break;
            }
            case msg::RoomType::PLO:
            {
                image_property_icon_->loadTexture("login_res/creategameroom_res/image_createroom_type_icon_holdem.png");
                text_property_name_->setString(tools::local_string("omaha", "底池限注奥马哈"));
                break;
            }
            case msg::RoomType::PLOHILO:
            {
                image_property_icon_->loadTexture("login_res/creategameroom_res/image_createroom_type_icon_holdem.png");
                text_property_name_->setString(tools::local_string("omaha_hl", "底池限注奥马哈 高/低"));
                break;
            }
            case msg::RoomType::SIChUANMahjong:
            {
                text_property_name_->setString(tools::local_string("sichuan_mahjong", "四川麻将"));
                image_property_icon_->loadTexture("login_res/choosegametype_res/image_gametype_icon_holdem.png");
                break;
            }
            default:
            {
                break;
            }
        }
    }
    
    float imagePropertyIconPositionX = text_property_name_->getPositionX() + (1 - text_property_name_->getAnchorPoint().x) * text_property_name_->getContentSize().width + image_property_icon_->getAnchorPoint().x * image_property_icon_->getContentSize().width + 30;
    image_property_icon_->setPositionX(imagePropertyIconPositionX);
}

void NodePropertyCell::update_group_data(const msg::GroupInfo& groupinfo)
{
    if (cell_type_ == SELECT_GROUP)
    {
        if (groupinfo.groupid() == 0)
        {
            text_property_name_->setString(tools::local_string("temporary_room","临时房间"));
        }else
        {
            text_property_name_->setString(tools::local_string("group_name","俱乐部名称：") + groupinfo.groupname());
        }
    }else
    {
        cell_type_ = SELECT_GROUP;
        
        root_ = CSLoader::createNode("NodeCreateRoomCell_SelectGroup.csb");
        this->addChild(root_);
        
        image_cell_line_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_createroomcell_line"));
        text_property_name_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_property_name"));
        image_select_icon_1_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_select_icon_1"));
        
        if (groupinfo.groupid() == 0)
        {
            text_property_name_->setString(tools::local_string("temporary_room","临时房间"));
        }else
        {
            text_property_name_->setString(tools::local_string("group_name","俱乐部名称：") + groupinfo.groupname());
        }
        
        this->setContentSize(cocos2d::Size(750 ,110));
        
        addTouchEventListener(std::bind(&NodePropertyCell::TouchGroup, this, std::placeholders::_1, std::placeholders::_2));
    }
}

void NodePropertyCell::update_click_cell(const std::string displayName)
{
    if (cell_type_ == CLICK_CELL)
    {
        text_property_name_->setString(displayName);
    }else
    {
        cell_type_ = CLICK_CELL;
        
        root_ = CSLoader::createNode("NodeCreateRoomCell_SelectGroup.csb");
        this->addChild(root_);
        
        image_cell_line_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_createroomcell_line"));
        text_property_name_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_property_name"));
        image_select_icon_1_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_select_icon_1"));
        
        text_property_name_->setString(displayName);
        
        this->setContentSize(cocos2d::Size(750 ,110));
        
        addClickEventListener(std::bind(&NodePropertyCell::click_click_cell, this, std::placeholders::_1));
    }
}

void NodePropertyCell::update_time_input(long long int timeSec)
{
    int_time_input_ = timeSec;
    
    if (cell_type_ == VALUE_TIME_INPUT)
    {
        
    }else
    {
        cell_type_ = VALUE_TIME_INPUT;
        
        root_ = CSLoader::createNode("NodeCreateRoomCell_TimeInput.csb");
        this->addChild(root_);
        
        image_cell_line_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_createroomcell_line"));
        text_property_name_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_property_name"));
        text_property_name_->setString(tools::local_string("time","时间"));

        image_property_input_bg_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_property_input_bg"));
        auto text_year=dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_property_year"));
        auto text_mon=dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_property_month"));
        auto text_day=dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_property_day"));
        auto text_hour=dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_property_hour"));
        auto text_min=dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_property_min"));
        text_year->setString(tools::local_string("year","年"));
        text_mon->setString(tools::local_string("month","月"));
        text_day->setString(tools::local_string("day","日"));
        text_hour->setString(tools::local_string("hour","时"));
        text_min->setString(tools::local_string("minute","分"));
        
        tf_property_year_ = dynamic_cast<TextField*>(CSLoader::seekNodeByName(root_, "tf_property_year"));
        ed_property_year_ = GameTools::createEditBoxFromTextField(this, tf_property_year_);
        ed_property_year_->setReturnType(EditBox::KeyboardReturnType::DONE);
        ed_property_year_->setMaxLength(ed_property_year_->getContentSize().width);
        ed_property_year_->setInputMode(cocos2d::ui::EditBox::InputMode::NUMERIC);
        ed_property_year_->setInputFlag(cocos2d::ui::EditBox::InputFlag::SENSITIVE);
        ed_property_year_->setFontColor(Color3B(255, 255, 255));
        ed_property_year_->setPlaceholderFontColor(Color3B(151, 151, 151));
        ed_property_year_->setPlaceHolder("");
        ed_property_year_->setDelegate(this);
        
        tf_property_month_ = dynamic_cast<TextField*>(CSLoader::seekNodeByName(root_, "tf_property_month"));
        ed_property_month_ = GameTools::createEditBoxFromTextField(this, tf_property_month_);
        ed_property_month_->setReturnType(EditBox::KeyboardReturnType::DONE);
        ed_property_month_->setMaxLength(ed_property_month_->getContentSize().width);
        ed_property_month_->setInputMode(cocos2d::ui::EditBox::InputMode::NUMERIC);
        ed_property_month_->setInputFlag(cocos2d::ui::EditBox::InputFlag::SENSITIVE);
        ed_property_month_->setFontColor(Color3B(255, 255, 255));
        ed_property_month_->setPlaceholderFontColor(Color3B(151, 151, 151));
        ed_property_month_->setPlaceHolder("");
        ed_property_month_->setDelegate(this);
        
        tf_property_day_ = dynamic_cast<TextField*>(CSLoader::seekNodeByName(root_, "tf_property_day"));
        ed_property_day_ = GameTools::createEditBoxFromTextField(this, tf_property_day_);
        ed_property_day_->setReturnType(EditBox::KeyboardReturnType::DONE);
        ed_property_day_->setMaxLength(ed_property_day_->getContentSize().width);
        ed_property_day_->setInputMode(cocos2d::ui::EditBox::InputMode::NUMERIC);
        ed_property_day_->setInputFlag(cocos2d::ui::EditBox::InputFlag::SENSITIVE);
        ed_property_day_->setFontColor(Color3B(255, 255, 255));
        ed_property_day_->setPlaceholderFontColor(Color3B(151, 151, 151));
        ed_property_day_->setPlaceHolder("");
        ed_property_day_->setDelegate(this);
        
        tf_property_hour_ = dynamic_cast<TextField*>(CSLoader::seekNodeByName(root_, "tf_property_hour"));
        ed_property_hour_ = GameTools::createEditBoxFromTextField(this, tf_property_hour_);
        ed_property_hour_->setReturnType(EditBox::KeyboardReturnType::DONE);
        ed_property_hour_->setMaxLength(ed_property_day_->getContentSize().width);
        ed_property_hour_->setInputMode(cocos2d::ui::EditBox::InputMode::NUMERIC);
        ed_property_hour_->setInputFlag(cocos2d::ui::EditBox::InputFlag::SENSITIVE);
        ed_property_hour_->setFontColor(Color3B(255, 255, 255));
        ed_property_hour_->setPlaceholderFontColor(Color3B(151, 151, 151));
        ed_property_hour_->setPlaceHolder("");
        ed_property_hour_->setDelegate(this);
        
        tf_property_min_ = dynamic_cast<TextField*>(CSLoader::seekNodeByName(root_, "tf_property_min"));
        ed_property_min_ = GameTools::createEditBoxFromTextField(this, tf_property_min_);
        ed_property_min_->setReturnType(EditBox::KeyboardReturnType::DONE);
        ed_property_min_->setMaxLength(ed_property_min_->getContentSize().width);
        ed_property_min_->setInputMode(cocos2d::ui::EditBox::InputMode::NUMERIC);
        ed_property_min_->setInputFlag(cocos2d::ui::EditBox::InputFlag::SENSITIVE);
        ed_property_min_->setFontColor(Color3B(255, 255, 255));
        ed_property_min_->setPlaceholderFontColor(Color3B(151, 151, 151));
        ed_property_min_->setPlaceHolder("");
        ed_property_min_->setDelegate(this);
        
        if (!is_edit_)
        {
            image_property_input_bg_->setVisible(false);
            ed_property_year_->setTouchEnabled(false);
            ed_property_month_->setTouchEnabled(false);
            ed_property_day_->setTouchEnabled(false);
            ed_property_hour_->setTouchEnabled(false);
            ed_property_min_->setTouchEnabled(false);
        }
        
        this->setContentSize(cocos2d::Size(750 ,170));
    }
    
    time_t timep = timeSec;
    tm* timeStruct = localtime(&timep);
    
    int intYear = timeStruct->tm_year + 1900;
    int intMonth = timeStruct->tm_mon + 1;
    int intDay = timeStruct->tm_mday;
    int intHour = timeStruct->tm_hour;
    int intMin = timeStruct->tm_min;

    ed_property_year_->setText(tools::to_string(intYear).c_str());
    ed_property_month_->setText(tools::to_string(intMonth).c_str());
    ed_property_day_->setText(tools::to_string(intDay).c_str());
    ed_property_hour_->setText(tools::to_string(intHour).c_str());
    ed_property_min_->setText(tools::to_string(intMin).c_str());
    
    
}

void NodePropertyCell::update_data(const msg::Property& property)
{
    property_.CopyFrom(property);
    
    cocos2d::log("propertyName:%s", property_.name().c_str());
    switch (property_.type()) {
        case msg::PropertyType::INT:
        {
            if (property_.intvalue().defaultvalue().size() != 0)
            {
                cell_type_ = VALUE_INT_SLIDER;
                root_ = CSLoader::createNode("NodeCreateRoomCell_IntSlider.csb");
                this->addChild(root_);
                
                image_cell_line_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_createroomcell_line"));
                text_property_name_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_property_name"));
                text_slider_cur_min_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_slider_cur_min"));
                text_slider_cur_max_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_slider_cur_max"));
                text_slider_cur_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_slider_cur"));
                slider_ = dynamic_cast<Slider*>(CSLoader::seekNodeByName(root_, "slider"));
                
                text_property_name_->setString(property_.displayname());
                
                text_slider_cur_min_->setString(property_.intvalue().min());
                
                text_slider_cur_max_->setString(property_.intvalue().max());
                
                text_slider_cur_->setString(tools::to_string(property_.intvalue().value()));
                
                slider_->addEventListener(std::bind(&NodePropertyCell::property_int_slider, this, std::placeholders::_1));
                
                if (!is_edit_)
                {
                    slider_->setTouchEnabled(false);
                }
                
                refresh_data();
                
                this->setContentSize(cocos2d::Size(750 ,170));
                
            }else
            {
                cell_type_ = VALUE_INT_INPUT;
                
                root_ = CSLoader::createNode("NodeCreateRoomCell_IntInput.csb");
                this->addChild(root_);
                
                image_cell_line_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_createroomcell_line"));
                text_property_name_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_property_name"));
                
                image_property_input_bg_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_property_input_bg"));
                tf_property_value_ = dynamic_cast<TextField*>(CSLoader::seekNodeByName(root_, "tf_property_value"));
                
                ed_property_value_ = GameTools::createEditBoxFromTextField(this, tf_property_value_);
                ed_property_value_->setReturnType(EditBox::KeyboardReturnType::DONE);
                
                ed_property_value_->setMaxLength(ed_property_value_->getContentSize().width);
                ed_property_value_->setInputMode(cocos2d::ui::EditBox::InputMode::NUMERIC);
                ed_property_value_->setInputFlag(cocos2d::ui::EditBox::InputFlag::SENSITIVE);
                ed_property_value_->setFontColor(Color3B(255, 255, 255));
                ed_property_value_->setPlaceholderFontColor(Color3B(151, 151, 151));
                
                ed_property_value_->setText(tools::to_string(property_.intvalue().value()).c_str());
                
                ed_property_value_->setDelegate(this);
                
                text_property_name_->setString(property_.displayname());
                
                if (!is_edit_)
                {
                    ed_property_value_->setTouchEnabled(false);
                }
                
                this->setContentSize(cocos2d::Size(750 ,110));
            }
            break;
        }
        case msg::PropertyType::BOOL:
        {
            root_ = CSLoader::createNode("NodeCreateRoomCell_Bool.csb");
            this->addChild(root_);
            
            cell_type_ = VALUE_BOOL;
            image_cell_line_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_createroomcell_line"));
            text_property_name_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_property_name"));
            
            checkbox_ = dynamic_cast<CheckBox*>(CSLoader::seekNodeByName(root_, "checkbox"));
            
            checkbox_->addEventListener(std::bind(&NodePropertyCell::property_bool_checkbox, this, std::placeholders::_1));
            
            text_property_name_->setString(property_.displayname());
            
            checkbox_->setSelected(property_.boolvalue());
            
            if (checkbox_->isSelected())
            {
                checkbox_->loadTextures("login_res/creategameroom_res/checkbox_cell_selected.png", "login_res/creategameroom_res/checkbox_cell_selected.png", "login_res/creategameroom_res/checkbox_cell_selected.png", "login_res/creategameroom_res/checkbox_cell_selected.png", "login_res/creategameroom_res/checkbox_cell_selected.png");
                
            }else
            {
                checkbox_->loadTextures("login_res/creategameroom_res/checkbox_cell_unselected.png", "login_res/creategameroom_res/checkbox_cell_unselected.png", "login_res/creategameroom_res/checkbox_cell_unselected.png", "login_res/creategameroom_res/checkbox_cell_unselected.png", "login_res/creategameroom_res/checkbox_cell_unselected.png");
            }
            
            if (!is_edit_)
            {
                checkbox_->setTouchEnabled(false);
            }
            this->setContentSize(cocos2d::Size(750 ,110));
            
            break;
        }
        case msg::PropertyType::ENUM:
        {
            if (cell_type_ == VALUE_ENUM)
            {
                
            }else
            {
                cell_type_ = VALUE_ENUM;
                
                root_ = CSLoader::createNode("NodeCreateRoomCell_Enum.csb");
                this->addChild(root_);
                
                image_cell_line_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_createroomcell_line"));
                text_property_name_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_property_name"));
                btn_enum_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_enum"));
                
                text_property_name_->setString(property_.displayname());
                
                this->setContentSize(cocos2d::Size(750 ,110));
                
                btn_enum_->setSwallowTouches(false);
                
                btn_enum_->addClickEventListener(std::bind(&NodePropertyCell::click_btn_enum, this, std::placeholders::_1));
                
                for (auto item : property_.enumvalue().items())
                {
                    if (item.value() == property_.enumvalue().value())
                    {
                        btn_enum_->setTitleText(item.text());
                    }
                }
                
                if (!is_edit_)
                {
                    btn_enum_->setTouchEnabled(false);
                }
                
            }
            break;
        }
        case msg::PropertyType::STRING:
        {
            if (cell_type_ == VALUE_STRING)
            {
                text_property_name_->setString(property_.displayname());
                ed_property_value_->setText(property_.stringvalue().c_str());
            }else
            {
                cell_type_ = VALUE_STRING;
                
                root_ = CSLoader::createNode("NodeCreateRoomCell_RoomName.csb");
                this->addChild(root_);
                
                tf_property_value_ = dynamic_cast<TextField*>(CSLoader::seekNodeByName(root_, "tf_room_name"));
                image_property_input_bg_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_room_name_input_bg"));
                
                image_cell_line_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_createroomcell_line"));
                text_property_name_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_property_name"));
                text_property_name_->setString(property_.displayname());
                
                ed_property_value_ = GameTools::createEditBoxFromTextField(this, tf_property_value_);
                ed_property_value_->setReturnType(EditBox::KeyboardReturnType::DONE);
                
                ed_property_value_->setMaxLength(ed_property_value_->getContentSize().width);
                ed_property_value_->setInputMode(cocos2d::ui::EditBox::InputMode::ANY);
                ed_property_value_->setInputFlag(cocos2d::ui::EditBox::InputFlag::SENSITIVE);
                ed_property_value_->setFontColor(Color3B(255, 255, 255));
                ed_property_value_->setPlaceholderFontColor(Color3B(151, 151, 151));
                ed_property_value_->setText(property_.stringvalue().c_str());
                
                ed_property_value_->setDelegate(this);
                
                if (!is_edit_)
                {
                    ed_property_value_->setTouchEnabled(false);
                }
                
                this->setContentSize(cocos2d::Size(750, 110));
                addTouchEventListener(nullptr);
            }
        }
        default:
            break;
    }
    addTouchEventListener(nullptr);
}

void NodePropertyCell::update_slider_data(const msg::Property& property)
{
    property_.CopyFrom(property);
    cell_type_ = VALUE_INT_SLIDER_SECOND;
    root_ = CSLoader::createNode("NodeCreateRoomCell_IntSlider.csb");
    this->addChild(root_);
    
    image_cell_line_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_createroomcell_line"));
    text_property_name_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_property_name"));
    text_slider_cur_min_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_slider_cur_min"));
    text_slider_cur_min_->setVisible(false);
    text_slider_cur_max_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_slider_cur_max"));
    text_slider_cur_max_->setVisible(false);
    text_slider_cur_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_slider_cur"));
    text_slider_cur_->setVisible(false);
    
    slider_ = dynamic_cast<Slider*>(CSLoader::seekNodeByName(root_, "slider"));
    text_property_name_->setString(property_.displayname());
    
    slider_->addEventListener(std::bind(&NodePropertyCell::property_int_slider, this, std::placeholders::_1));
    this->setContentSize(cocos2d::Size(750 ,170));
}

void NodePropertyCell::update_value_min_max(const int value_min, const int value_max)
{
    int_value_min_ = value_min;
    int_value_max_ = value_max;
    
    refresh_data();
}

void NodePropertyCell::update_select_enum(const msg::Property& property)
{
    property_ = property;
    for (auto item : property_.enumvalue().items())
    {
        if (item.value() == property_.enumvalue().value())
        {
            btn_enum_->setTitleText(item.text());
        }
    }
    if (property_callback_)
    {
        property_callback_(property_);
    }
}

void NodePropertyCell::refresh_data()
{
    switch (cell_type_) {
        case NONE:
        {
            
            break;
        }
        case GAME_TYPE:
        {
            
            
            break;
        }
        case VALUE_STRING:
        {
            
            
            break;
        }
        case SELECT_GROUP:
        {
            
            break;
        }
        case VALUE_BOOL:
        {
            
            checkbox_->setSelected(property_.boolvalue());
            break;
        }
        case VALUE_ENUM:
        {
            
            
            break;
        }
        case VALUE_INT_INPUT:
        {
            
            ed_property_value_->setText(tools::to_string(property_.intvalue().value()).c_str());
            break;
        }
        case VALUE_INT_SLIDER:
        {
            std::vector<int> vecIntValueDefaults;
            
            for (auto valueIter : property_.intvalue().defaultvalue())
            {
                vecIntValueDefaults.push_back(valueIter);
            }
            
            for (auto iter = vecIntValueDefaults.begin(); iter != vecIntValueDefaults.end(); )
            {
                if (*iter > int_value_max_ ||
                    *iter < int_value_min_ )
                {
                    iter = vecIntValueDefaults.erase(iter);
                }else
                {
                    iter++;
                }
            }
            
            int propertyIntValueDefaultSize = vecIntValueDefaults.size();
            
            if (propertyIntValueDefaultSize > 0)
            {
                text_slider_cur_min_->setString(tools::to_string(vecIntValueDefaults[0]));
                text_slider_cur_max_->setString(tools::to_string(vecIntValueDefaults[propertyIntValueDefaultSize-1]));
                
            }
            
            if (propertyIntValueDefaultSize == 0)
            {
                slider_->setPercent(50.0);
                
                text_slider_cur_->setString(tools::to_string(property_.intvalue().value()));
                if (text_slider_cur_max_->isVisible())
                {
                    text_slider_cur_max_->setVisible(false);
                }
                if (text_slider_cur_min_->isVisible())
                {
                    text_slider_cur_min_->setVisible(false);
                }
                if (!text_slider_cur_->isVisible())
                {
                    text_slider_cur_->setVisible(true);
                }
            }else if (propertyIntValueDefaultSize == 1) //只有一个默认值
            {
                slider_->setPercent(50.0);
                
                property_.mutable_intvalue()->set_value(vecIntValueDefaults[0]);
                text_slider_cur_->setString(tools::to_string(property_.intvalue().value()));
                if (text_slider_cur_max_->isVisible())
                {
                    text_slider_cur_max_->setVisible(false);
                }
                if (text_slider_cur_min_->isVisible())
                {
                    text_slider_cur_min_->setVisible(false);
                }
                
                if (!text_slider_cur_->isVisible())
                {
                    text_slider_cur_->setVisible(true);
                }
            }else if (propertyIntValueDefaultSize == 2)
            {
                if (property_.intvalue().value() >= vecIntValueDefaults[1] ||
                    property_.intvalue().value() > ((vecIntValueDefaults[0] + vecIntValueDefaults[1]) / 2))
                {
                    slider_->setPercent(100.0);
                    if (text_slider_cur_->isVisible())
                    {
                        text_slider_cur_->setVisible(false);
                    }
                    text_slider_cur_max_->setColor(cocos2d::Color3B(255, 255, 255));
                    text_slider_cur_min_->setColor(cocos2d::Color3B(151, 151, 151));
                }else
                {
                    slider_->setPercent(0.0);
                    if (text_slider_cur_->isVisible())
                    {
                        text_slider_cur_->setVisible(false);
                    }
                    text_slider_cur_max_->setColor(cocos2d::Color3B(151, 151, 151));
                    text_slider_cur_min_->setColor(cocos2d::Color3B(255, 255, 255));
                }
                if (!text_slider_cur_max_->isVisible())
                {
                    text_slider_cur_max_->setVisible(true);
                }
                if (!text_slider_cur_min_->isVisible())
                {
                    text_slider_cur_min_->setVisible(true);
                }
                
            }else if (propertyIntValueDefaultSize > 2)
            {
                
                int currentValueItem = 0;
                
                for (auto valueIter : vecIntValueDefaults)
                {
                    if (valueIter == property_.intvalue().value())
                    {
                        break;
                    }
                    currentValueItem++;
                }
                
                float sliderDisplayPercent = 1.0 * currentValueItem / (propertyIntValueDefaultSize - 1);
                
                text_slider_cur_->setString(tools::to_string(property_.intvalue().value()));
                
                if (sliderDisplayPercent == 0.0)
                {
                    text_slider_cur_max_->setColor(cocos2d::Color3B(151, 151, 151));
                    text_slider_cur_min_->setColor(cocos2d::Color3B(255, 255, 255));
                    
                    if (text_slider_cur_->isVisible())
                        text_slider_cur_->setVisible(false);
                    
                }else if (sliderDisplayPercent >= 1.0)
                {
                    text_slider_cur_max_->setColor(cocos2d::Color3B(255, 255, 255));
                    text_slider_cur_min_->setColor(cocos2d::Color3B(151, 151, 151));
                    
                    if (text_slider_cur_->isVisible())
                        text_slider_cur_->setVisible(false);
                }else
                {
                    text_slider_cur_max_->setColor(cocos2d::Color3B(151, 151, 151));
                    text_slider_cur_min_->setColor(cocos2d::Color3B(151, 151, 151));
                    
                    if (!text_slider_cur_->isVisible())
                        text_slider_cur_->setVisible(true);
                }
                
                slider_->setPercent(sliderDisplayPercent * 100.0);
                
                if (!text_slider_cur_max_->isVisible())
                {
                    text_slider_cur_max_->setVisible(true);
                }
                if (!text_slider_cur_min_->isVisible())
                {
                    text_slider_cur_min_->setVisible(true);
                }
            }
            
            float sliderWidth = slider_->getContentSize().width;
            float sliderPositionX = slider_->getPositionX() - (1.0 - slider_->getAnchorPoint().x) * sliderWidth;
            
            float textCurPositionX  = sliderPositionX + sliderWidth * slider_->getPercent() / 100.0;
            
            text_slider_cur_->setPositionX(textCurPositionX);
            
            break;
        }
        case VALUE_INT_SLIDER_SECOND:
        {
            break;
        }
        case VALUE_TIME_INPUT:
        {
            break;
        }
        default:
        {
            break;
        }
    }
}

void NodePropertyCell::property_bool_checkbox(Ref * sender)
{
    property_.set_boolvalue(checkbox_->isSelected());
    if (checkbox_->isSelected())
    {
        checkbox_->loadTextures("login_res/creategameroom_res/checkbox_cell_selected.png", "login_res/creategameroom_res/checkbox_cell_selected.png", "login_res/creategameroom_res/checkbox_cell_selected.png", "login_res/creategameroom_res/checkbox_cell_selected.png", "login_res/creategameroom_res/checkbox_cell_selected.png");
        
    }else
    {
        checkbox_->loadTextures("login_res/creategameroom_res/checkbox_cell_unselected.png", "login_res/creategameroom_res/checkbox_cell_unselected.png", "login_res/creategameroom_res/checkbox_cell_unselected.png", "login_res/creategameroom_res/checkbox_cell_unselected.png", "login_res/creategameroom_res/checkbox_cell_unselected.png");
    }
    if(property_callback_)
    {
        property_callback_(property_);
    }
}

void NodePropertyCell::property_int_slider(Ref * sender)
{
    float sliderPercent = slider_->getPercent();
    
    std::vector<int> vecIntValueDefaults;
    
    for (auto valueIter : property_.intvalue().defaultvalue())
    {
        vecIntValueDefaults.push_back(valueIter);
    }
    
    for (auto iter = vecIntValueDefaults.begin(); iter != vecIntValueDefaults.end(); )
    {
        if (*iter > int_value_max_ ||
            *iter < int_value_min_ )
        {
            iter = vecIntValueDefaults.erase(iter);
        }else
        {
            iter++;
        }
    }
    
    int propertyIntValueDefaultSize = vecIntValueDefaults.size();
    
    if (propertyIntValueDefaultSize == 1) //只有一个默认值
    {
        property_.mutable_intvalue()->set_value(vecIntValueDefaults[0]);
        
    }else if (propertyIntValueDefaultSize == 2)
    {
        if (sliderPercent >= 50.0)
        {
            property_.mutable_intvalue()->set_value(vecIntValueDefaults[1]);
        }else
        {
            property_.mutable_intvalue()->set_value(vecIntValueDefaults[0]);
        }
    }else if (propertyIntValueDefaultSize > 2)
    {
        
        int currentValueItem = sliderPercent / 100.0 * (propertyIntValueDefaultSize-1);
        
        property_.mutable_intvalue()->set_value(vecIntValueDefaults[currentValueItem]);
        
    }
    
    refresh_data();
    
    if (property_callback_)
        property_callback_(property_);
}

void NodePropertyCell::click_btn_enum(Ref * sender)
{
    LayerMenu *lm = LayerMenu::create(convertToWorldSpace(btn_enum_->getPosition()), btn_enum_->getContentSize(), property_);
    ParentLayer_->addChild(lm);
    
    lm->set_property_callback(std::bind(&NodePropertyCell::update_select_enum, this, std::placeholders::_1));
//    Point base = convertToWorldSpace(btn_enum_->getPosition());
//    Point bg_base = convertToWorldSpace(btn_enum_->getPosition());
//    pBg->update(bg_base, base, distance, property_);
}

#pragma mark EditBox delegate
void NodePropertyCell::editBoxEditingDidBegin(EditBox* editBox)
{
    
}

void NodePropertyCell::editBoxEditingDidEnd(EditBox* editBox)
{
    
}

void NodePropertyCell::editBoxTextChanged(EditBox* editBox, const std::string& text)
{
    if (cell_type_ == VALUE_STRING) {
        property_.set_stringvalue(text);
    }else if (cell_type_ == VALUE_INT_INPUT)
    {
        property_.mutable_intvalue()->set_value(tools::stod(ed_property_value_->getText()));
    }
    if (property_callback_)
        property_callback_(property_);
}

void NodePropertyCell::editBoxReturn(EditBox* editBox)
{
    if (cell_type_ == VALUE_TIME_INPUT) {
        time_t timep = int_time_input_;
        tm* timeStruct = localtime(&timep);
        
        int intOldYear = timeStruct->tm_year;
        int intOldMonth = timeStruct->tm_mon;
        int intOldDay = timeStruct->tm_mday;
        int intOldHour = timeStruct->tm_hour;
        int intOldMin = timeStruct->tm_min;
        
        int intNewYear;
        int intNewMonth;
        int intNewDay;
        int intNewHour;
        int intNewMin;
        
        std::string strNewYear = ed_property_year_->getText();
        std::string strNewMonth = ed_property_month_->getText();
        std::string strNewDay = ed_property_day_->getText();
        std::string strNewHour = ed_property_hour_->getText();
        std::string strNewMin = ed_property_min_->getText();

        if (strNewYear.empty())
        {
            intNewYear = intOldYear;
        }else
        {
            intNewYear = tools::stoi(strNewYear);
        }
        if (strNewMonth.empty())
        {
            intNewMonth = intOldMonth;
        }else
        {
            intNewMonth = tools::stoi(strNewMonth);
        }
        if (strNewDay.empty())
        {
            intNewDay = intOldDay;
        }else
        {
            intNewDay = tools::stoi(strNewDay);
        }
        if (strNewHour.empty())
        {
            intNewHour = intOldHour;
        }else
        {
            intNewHour = tools::stoi(strNewHour);
        }
        if (strNewMin.empty())
        {
            intNewMin = intOldMin;
        }else
        {
            intNewMin = tools::stoi(strNewMin);
        }
        
        if (intNewYear < intOldYear)
        {
            intNewYear = intOldYear;
        }
        if (intNewMonth <= 0)
        {
            intNewMonth = 1;
        }else if (intNewMonth > 12)
        {
            intNewMonth = 12;
        }
        
        if (intNewDay <= 0)
        {
            intNewDay = 1;
        }else if (intNewDay > 31)
        {
            intNewDay = 31;
        }
        
        if ((intNewMonth == 4 || intNewMonth == 6 || intNewMonth == 9 || intNewMonth == 11) && intNewDay > 30)
        {
            intNewDay = 30;
        }else if (intNewMonth == 2 && intNewDay >= 29)
        {
            if (intNewYear % 4 != 0 || (intNewYear % 100 == 0 && intNewYear % 400 != 0))
            {
                intNewDay = 28;
            }else
            {
                intNewDay = 29;
            }
        }
        
        if (intNewHour < 0)
        {
            intNewHour = 0;
        }else if (intNewHour > 23)
        {
            intNewHour = 23;
        }
        
        if (intNewMin < 0)
        {
            intNewMin = 0;
        }else if (intNewMin > 59)
        {
            intNewMin = 59;
        }
        
        ed_property_year_->setText(tools::to_string(intNewYear).c_str());
        ed_property_month_->setText(tools::to_string(intNewMonth).c_str());
        ed_property_day_->setText(tools::to_string(intNewDay).c_str());
        ed_property_min_->setText(tools::to_string(intNewMin).c_str());
        
        struct timeval timeTemp;
        gettimeofday(&timeTemp, 0);
        time_t timeTemp_t = timeTemp.tv_sec + timeTemp.tv_usec / 1000000.0;
        struct tm * newTime = localtime(&timeTemp_t);
        
        newTime->tm_year = intNewYear - 1900;
        newTime->tm_mon = intNewMonth - 1;
        newTime->tm_mday = intNewDay;
        newTime->tm_hour = intNewHour;
        newTime->tm_min = intNewMin;
        
        time_t timeResult = mktime(newTime);
        
        int_time_input_ = timeResult;

        cocos2d::log("edit time is %s", tools::time_display(int_time_input_).c_str());
        
        if (select_time_callback_)
        {
            select_time_callback_(int_time_input_);
        }
    }
    GameTools::editBoxLoseInputFocus();
}