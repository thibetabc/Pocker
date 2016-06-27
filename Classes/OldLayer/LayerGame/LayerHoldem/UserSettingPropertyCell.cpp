#include "UserSettingPropertyCell.h"
#include "PokerDataManager.h"


bool UserSettingPropertyCell::init()
{
    if(!Widget::init()) {
        return false;
    }
    
    auto root1 = CSLoader::createNode("NodeUserSetting.csb");
    this->addChild(root1);
    
    auto root2 = CSLoader::createNode("NodeUserSetting.csb");
    root2->setPosition(Point(180, 0));
    this->addChild(root2);
    
    auto root3 = CSLoader::createNode("NodeUserSetting.csb");
    root3->setPosition(Point(360, 0));
    this->addChild(root3);
    
    roots_.push_back(root1);
    roots_.push_back(root2);
    roots_.push_back(root3);
    
    this->setContentSize(Size(500, 40));
    
    return true;
}


void UserSettingPropertyCell::update_data(const std::vector<std::shared_ptr<msg::Property> >& properties)
{
    
    for (auto& root : roots_) {
        root->setVisible(false);
    }
    
    for (size_t i = 0; i < properties.size(); i++) {
        auto& property = properties.at(i);
        auto root = roots_.at(i);
        root->setVisible(true);
        auto checkbox = dynamic_cast<CheckBox*>(CSLoader::seekNodeByName(root, "check_box"));
        auto text_note = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_note"));
        checkbox->setTag(i);
        checkbox->setSelected(property->boolvalue());
        text_note->setString(property->displayname());
        checkbox->addEventListener(std::bind(&UserSettingPropertyCell::click_checkbox, this, std::placeholders::_1, std::placeholders::_2));
        text_note->addClickEventListener([=](Ref*){
            checkbox->setSelected(!checkbox->isSelected());
            CheckBox::EventType    event = checkbox->isSelected() ? CheckBox::EventType::UNSELECTED : CheckBox::EventType::SELECTED;
            this->click_checkbox(checkbox, event);
            
        });
    }
    properties_ = properties;
}

void UserSettingPropertyCell::click_checkbox(Ref* sender, CheckBox::EventType event)
{
    auto cb = dynamic_cast<CheckBox*>(sender);
    
    if(cb)
    {
        //TODO:设置属性和room_id
        msg::PokerPlayerSetting properties;
        uint32_t room_id;
        PDM->send_send_set_change(properties,room_id);
    }
    
}