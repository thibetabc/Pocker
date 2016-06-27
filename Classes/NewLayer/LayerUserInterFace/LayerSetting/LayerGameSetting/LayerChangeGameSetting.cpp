//
//  LayerChangeGameSetting.cpp
//  pokerclient
//
//  Created by zhongyu on 16/4/19.
//
//

#include "LayerChangeGameSetting.h"
#include "ProcessorManager.h"
#include "NET_CMD.h"
#include "utils.h"
LayerChangeGameSetting* LayerChangeGameSetting::create(msg::RoomType type, std::string name)
{
    LayerChangeGameSetting* game=new LayerChangeGameSetting();
    if(game->init(type, name))
    {
        game->autorelease();
        return game;
    }
    return nullptr;
}

bool LayerChangeGameSetting::init(msg::RoomType type, std::string name)
{
    if(!BaseLayer::init())
    {
        return false;
    }
    root_=CSLoader::createNode("LayerChangeGameSetting.csb");
    this->addChild(root_);
    type_=type;
    name_=name;
    btn_back_=dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_back"));
    btn_back_->addClickEventListener(std::bind(&LayerChangeGameSetting::click_btn_back,this,std::placeholders::_1));
    btn_submit_setting_=dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_submit_setting"));
    btn_submit_setting_->addClickEventListener(std::bind(&LayerChangeGameSetting::click_btn_submit_setting,this,std::placeholders::_1));
    btn_submit_setting_->setTitleText(tools::local_string("save","保存"));
    text_title_=dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_title"));
    text_title_->setString(name);
    
    list_=dynamic_cast<ListView*>(CSLoader::seekNodeByName(root_, "listview_game_setting"));
    auto processor = PM->null_up();
    send_data(processor, REQ_GET_USER_SETTING);
    return true;
}

void LayerChangeGameSetting::register_events()
{
    dispatch_.register_event(REQ_GET_USER_SETTING, BIND_FUNC(this, LayerChangeGameSetting::update_data));
    dispatch_.register_event(REQ_CHANGE_USER_SETTING, BIND_FUNC(this, LayerChangeGameSetting::handle_change_down));
}

void LayerChangeGameSetting::click_btn_back(cocos2d::Ref *sender)
{
    this->removeFromParent();
}

void LayerChangeGameSetting::click_btn_submit_setting(cocos2d::Ref *sender)
{
    msg::UserGameSettings settings;
    msg::UserGameSetting setting;
    setting.set_gameid(type_);
    setting.set_gamename(name_);
    setting.mutable_property()->Add();
    setting.mutable_property(0)->CopyFrom(*cell_1_->get_cell_property());
    setting.mutable_property()->Add();
    setting.mutable_property(1)->CopyFrom(*cell_2_->get_cell_property());
    setting.mutable_property()->Add();
    setting.mutable_property(2)->CopyFrom(*cell_3_->get_cell_property());
    settings.mutable_gamesetting()->Add();
    settings.mutable_gamesetting(0)->CopyFrom(setting);
    auto processor = PM->changeUserSetting_up(settings);
    send_data(processor, REQ_CHANGE_USER_SETTING);
}

void LayerChangeGameSetting::update_data(cmd_data_pointer data)
{
    ClearError();
    std::shared_ptr<net_data_recv_package> rec = dynamic_pointer_cast<net_data_recv_package>(data);
    if(!check_request(rec)) return;
    auto processor=PM->getUserSetting_down(rec);
    switch (processor.response().status()) {
        case msg::Status::SUCCESS:
        {
            game_data_.CopyFrom(processor.gamesettings().gamesetting(type_-1));
            this->update_lisview_data();
            break;
        }
        case msg::Status::FAILED:
            ShowError(tools::local_string("get_setMsg_failed","获取设置信息失败"));
            break;
        default:
            break;
    }
    
}
void LayerChangeGameSetting::update_lisview_data()
{
    float listviewHeight = 0.0;
    
    cell_1_ = NodePropertyCell::create(this);
    msg::Property property1;
    property1.set_type(msg::PropertyType::ENUM);
    property1.set_name(game_data_.property(0).name());
    property1.set_displayname(game_data_.property(0).displayname());
    property1.set_stringvalue(game_data_.property(0).stringvalue());
    property1.mutable_enumvalue()->CopyFrom(game_data_.property(0).enumvalue());
    
    cell_1_->update_data(property1);
    listviewHeight=listviewHeight+cell_1_->getContentSize().height;
    list_->pushBackCustomItem(cell_1_);
    
    cell_2_ = NodePropertyCell::create(this);
    msg::Property property2;
    property2.set_type(msg::PropertyType::BOOL);
    property2.set_displayname(game_data_.property(1).displayname());
    property2.set_name(game_data_.property(1).name());
    property2.set_stringvalue(game_data_.property(1).stringvalue());
    property2.set_boolvalue(game_data_.property(1).boolvalue());
    cell_2_->update_data(property2);
    
    listviewHeight=listviewHeight+cell_2_->getContentSize().height;
    list_->pushBackCustomItem(cell_2_);
    
    cell_3_ = NodePropertyCell::create(this);
    msg::Property property3;
    property3.set_type(msg::PropertyType::ENUM);
    property3.set_name(game_data_.property(2).name());
    property3.set_displayname(game_data_.property(2).displayname());
    property3.set_stringvalue(game_data_.property(2).stringvalue());
    property3.mutable_enumvalue()->CopyFrom(game_data_.property(2).enumvalue());
    cell_3_->update_data(property3);
    
    listviewHeight=listviewHeight+cell_3_->getContentSize().height;
    list_->pushBackCustomItem(cell_3_);

    
    list_->setContentSize(cocos2d::Size(list_->getContentSize().width, listviewHeight));
    
    
    list_->refreshView();
}
 void LayerChangeGameSetting::handle_change_down(cmd_data_pointer data)
{
    ClearError();
    std::shared_ptr<net_data_recv_package> rec=dynamic_pointer_cast<net_data_recv_package>(data);
    if(!check_request(rec)) return;
    msg::Response processor;
    processor.ParseFromArray(rec->body, rec->bodysize);
    if(processor.status()==msg::Status::SUCCESS)
    {
        this->removeFromParent();
    }
    else
    {
        ShowError(processor.message());
    }
}
