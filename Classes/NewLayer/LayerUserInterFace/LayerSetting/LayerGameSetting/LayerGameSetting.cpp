//
//  LayerGameSetting.cpp
//  pokerclient
//
//  Created by zhongyu on 16/4/19.
//
//

#include "LayerGameSetting.h"
#include "NodeGameSettingGameTypeCell.h"
#include "utils.h"
#include "LayerChangeGameSetting.h"
#include "GameDataManager.h"
#include "AudioManager.h"
#include "LayerLanguageSetting.h"
#include "GameTools.h"


LayerGameSetting::LayerGameSetting() : isHideLanguageSetting(false)
{
    
}

LayerGameSetting::~LayerGameSetting()
{
    
}

LayerGameSetting *LayerGameSetting::create(bool iHideLanguageSetting)
{
    auto layer = new (std::nothrow) LayerGameSetting();
    layer->isHideLanguageSetting = iHideLanguageSetting;
    if(layer && layer->init())
    {
        layer->autorelease();
        return layer;
    }
    
    delete layer;
    layer = nullptr;
    return nullptr;
    
}

bool LayerGameSetting::init()
{
    if(!BaseLayer::init())
    {
        return false;
    }
    root_=CSLoader::createNode("LayerGameSetting.csb");
    this->addChild(root_);
    btn_back_=dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_back"));
    btn_back_->addClickEventListener(std::bind(&LayerGameSetting::click_btn_back,this,std::placeholders::_1));
    this->addDataSource();
    text_title_=dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_title"));
    text_title_->setString(tools::local_string("usersetting_game_setting","游戏设置"));
    
    auto text_system_title=dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_system_title"));
    text_system_title->setString(tools::local_string("system_setting","系统设置"));
    
    list_game_type_ = dynamic_cast<ListView*>(CSLoader::seekNodeByName(root_, "listview_game_type"));
    tableview_gametype_ = TableView::create(this, list_game_type_->getContentSize());
    tableview_gametype_->setAnchorPoint(list_game_type_->getAnchorPoint());
    tableview_gametype_->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    tableview_gametype_->setPosition(list_game_type_->getPosition());
    tableview_gametype_->setDelegate(this);
    root_->addChild(tableview_gametype_);
    list_game_type_->removeFromParent();
    
    system_game_setting_= dynamic_cast<ListView*>(CSLoader::seekNodeByName(root_, "list_view_system_set"));
    this->add_game_setting_data();
    
    return true;
}
void LayerGameSetting::addDataSource()
{
    std::string names[]={tools::local_string("holdempoker", "德州扑克"),tools::local_string("omaha", "底池限注奥马哈"),tools::local_string("omaha_hl", "底池限注奥马哈 高/低")};
    msg::RoomType types[]={msg::RoomType::NLHOLDEM,msg::RoomType::PLO,msg::RoomType::PLOHILO};
    for(int i=0;i<3;i++)
    {
        dataSource data;
        data.name=names[i];
        data.type=types[i];
        data_source_.push_back(data);
    }
    data_source_.at(0).isSelected=true;
}
void LayerGameSetting::click_btn_back(cocos2d::Ref *sender)
{
    this->removeFromParent();
}
TableViewCell* LayerGameSetting::tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx)
{
    NodeGameSettingGameTypeCell* cell = dynamic_cast<NodeGameSettingGameTypeCell*>(table->dequeueCell());
    if(!cell) {
        cell = NodeGameSettingGameTypeCell::create();
    }
    cell->set_text_game_type(data_source_.at(idx).name, data_source_.at(idx).type);
    if (data_source_.at(idx).isSelected)
    {
        cell->set_btn_choose_status(true);
    }
    else
    {
        cell->set_btn_choose_status(false);
    }
    return cell;
}
ssize_t LayerGameSetting::numberOfCellsInTableView(cocos2d::extension::TableView *table)
{
    return data_source_.size();
}
void LayerGameSetting::tableCellTouched(cocos2d::extension::TableView *table, cocos2d::extension::TableViewCell *cell)
{
    auto clickCell=dynamic_cast<NodeGameSettingGameTypeCell*>(cell);
    if(clickCell->getIdx()!=current_select_index_)
    {
        data_source_.at(current_select_index_).isSelected=false;
        current_select_index_=clickCell->getIdx();
        data_source_.at(current_select_index_).isSelected=true;
        tableview_gametype_->reloadData();
    }
    LayerChangeGameSetting* game=LayerChangeGameSetting::create(data_source_.at(current_select_index_).type, data_source_.at(current_select_index_).name);
    this->addChild(game);
}

void LayerGameSetting::add_game_setting_data()
{
    float height=0.0;
    
    NodePropertyCell* cell_1=NodePropertyCell::create(this);
    msg::Property pro_1;
    pro_1.set_type(msg::PropertyType::BOOL);
    pro_1.set_boolvalue(GDM->get_is_newcard());
    pro_1.set_displayname(tools::local_string("Four_color_deck","开启四色手牌"));
    cell_1->update_data(pro_1);
    cell_1->set_property_callback(std::bind(&LayerGameSetting::new_card_selected_callback,this, std::placeholders::_1));
    
    height=height+cell_1->getContentSize().height;
    system_game_setting_->pushBackCustomItem(cell_1);
    
    cell_2_=NodePropertyCell::create(this);
    msg::Property pro_2;
    pro_2.set_displayname(tools::local_string("music_volume","音乐"));
    cell_2_->update_slider_data(pro_2);
    cell_2_->set_property_callback(std::bind(&LayerGameSetting::music_callback,this, std::placeholders::_1));
    cell_2_->get_Slider()->setPercent(UserDefault::getInstance()->getFloatForKey("effects_volume")*100);
    
    height=height+cell_2_->getContentSize().height;
    system_game_setting_->pushBackCustomItem(cell_2_);
    
    cell_3_=NodePropertyCell::create(this);
    msg::Property pro_3;
    pro_3.set_displayname(tools::local_string("effects_volume","音效"));
    cell_3_->update_slider_data(pro_3);
    cell_3_->set_property_callback(std::bind(&LayerGameSetting::volume_callback,this, std::placeholders::_1));
    cell_3_->get_Slider()->setPercent(UserDefault::getInstance()->getFloatForKey("music_volume")*100);
    height=height+cell_3_->getContentSize().height;
    system_game_setting_->pushBackCustomItem(cell_3_);
    
    auto layout = Layout::create();
    layout->setContentSize(cell_3_->getContentSize());
    auto btnSetLanguage = Button::create("login_res/mainlayer_res/btn_mainpage_other_normal.png", "login_res/mainlayer_res/btn_mainpage_pressed.png","login_res/mainlayer_res/btn_mainpage_other_disable.png");
    //    std::string languageName = "中文";
    //    auto language = (LanguageType)GameTools::getLanguage();
    //    switch (language) {
    //        case LanguageType::CHINESE:
    //            languageName = "中文";
    //            break;
    //        case LanguageType::ENGLISH:
    //            languageName = "English";
    //            break;
    //        default:
    //            break;
    //    }
    //    btnSetLanguage->setTitleText("选择语言(" + languageName + ")");
    btnSetLanguage->setTitleText(tools::local_string("choose_language","选择语言"));
    btnSetLanguage->setEnabled(isHideLanguageSetting);
    btnSetLanguage->setTitleFontSize(26);
    btnSetLanguage->addClickEventListener([&](Ref *ref)
                                          {
                                              log("选择语言");
                                              AddGlobalLayer(LayerLanguageSetting::create());
                                          });
    auto sizeScreen = Director::getInstance()->getVisibleSize();
    btnSetLanguage->setPosition(Size(sizeScreen.width/2, cell_3_->getContentSize().height / 2));
    layout->addChild(btnSetLanguage);
    system_game_setting_->pushBackCustomItem(layout);
    height=height+cell_3_->getContentSize().height;
    
    system_game_setting_->setPositionY(system_game_setting_->getPositionY() - cell_3_->getContentSize().height - 30);
    system_game_setting_->setContentSize(Size(system_game_setting_->getContentSize().width, height));
    system_game_setting_->refreshView();
}
void LayerGameSetting::new_card_selected_callback(msg::Property property)
{
    if(property.boolvalue())
    {
        GDM->set_is_newcard(true);
        UserDefault::getInstance()->setBoolForKey("is_newcard", true);
        UserDefault::getInstance()->flush();
    }
    else
    {
        GDM->set_is_newcard(false);
        UserDefault::getInstance()->setBoolForKey("is_newcard", false);
        UserDefault::getInstance()->flush();
    }
    if (change_setting_callback_)
    {
        change_setting_callback_();
    }
}
void LayerGameSetting::volume_callback(msg::Property property)
{
    Slider* slider=cell_3_->get_Slider();
    float effects_volume = slider->getPercent() / 100.0;
    
//    cocos2d::log("LayerGameSetting::slider_slider_effects_volume %f", effects_volume);
    
    AM->setEffectsVolume(effects_volume);
}
void LayerGameSetting::music_callback(msg::Property property)
{
     Slider* slider=cell_2_->get_Slider();
    float music_volume = slider->getPercent() / 100.0;
    
//    cocos2d::log("LayerGameSetting::slider_slider_music_volume %f", music_volume);
    
    AM->setBackgroundMusicVolume(music_volume);
    
}
