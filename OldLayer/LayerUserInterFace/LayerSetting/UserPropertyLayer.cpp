#include "UserPropertyLayer.h"
#include "GameNameCell.h"
#include "GameDataManager.h"
#include "RoomPropertyCell.h"
#include "RoomTitle.h"
#include "GameTools.h"
#include "cmd_def.h"
#include "PokerDataManager.h"
#define LISTVIEW_TAG 1000

UserPropertyLayer::~UserPropertyLayer()
{
    if (this->setting)
    {
        delete this->setting;
    }
}
UserPropertyLayer* UserPropertyLayer::create(const bool in_game)
{
    UserPropertyLayer* result = new UserPropertyLayer;
    if(result->init(in_game)) {
        result->autorelease();
        return result;
    }
    return nullptr;
}

bool UserPropertyLayer::init(const bool in_game)
{
    if(!Layer::init()) {
        return false;
    }
    this->setting = NULL;
    auto size = Director::getInstance()->getWinSize();
    
    size = Size(std::min(size.width, size.height), std::max(size.width, size.height));
    
    //    auto layout = ui::Layout::create();
    //    layout->setContentSize(size);
    //    layout->setTouchEnabled(true);
    //    layout->setAnchorPoint(Point::ZERO);
    //    layout->addClickEventListener([](Ref*){
    //        GameTools::editBoxLoseInputFocus();
    //    });
    //    this->addChild(layout, 0);
    
    
    float width = size.width;
    float height = size.height;
    float top_height = 128;
    if(in_game) {
        width /= 2;
        height /= 2;
        top_height /= 2;
    }
    
    LayerColor* layer = LayerColor::create(Color4B(240, 239, 245, 255), width, size.height);
    this->addChild(layer, -2);
    
    in_game_ = in_game;
    
    LayerColor* top = LayerColor::create(COLOR_TOP, width, top_height);
    top->setAnchorPoint(Vec2(0, 0));
    top->setPosition(0, DESIGN_HEIGHT - top->getContentSize().height);
    this->addChild(top, -1);
    
    auto layout_move = ui::Layout::create();
    layout_move->setContentSize(size);
    layout_move->setTouchEnabled(true);
    layout_move->setSwallowTouches(false);
    layout_move->setAnchorPoint(Point::ZERO);
    layout_move->addTouchCallback(std::bind(&UserPropertyLayer::touch_image, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    this->addChild(layout_move, 9);
    
    std::string csb_name = "LayerUserProperty.csb";
    if (in_game) {
        csb_name = "LayerUserPropertyInGame.csb";
    }
    auto root = CSLoader::createNode(csb_name);
    this->addChild(root, 1);
    
    auto listview_name = dynamic_cast<ListView*>(CSLoader::seekNodeByName(root, "listview_name"));
    listview_property_ = dynamic_cast<ListView*>(CSLoader::seekNodeByName(root, "listview_property"));
    listview_property_->setTag(LISTVIEW_TAG);
    listview_property_->addClickEventListener([](Ref*){
        GameTools::editBoxLoseInputFocus();
    });
    text_save_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_save"));
    btn_back_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "btn_back"));
    //image_left_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root, "image_left"));
    //image_right_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root, "image_right"));
    btn_close_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "btn_close"));
    
    btn_back_->addClickEventListener(std::bind(&UserPropertyLayer::click_btn_back, this, std::placeholders::_1));
    text_save_->addClickEventListener(std::bind(&UserPropertyLayer::click_text_save, this, std::placeholders::_1));
    //image_left_->addClickEventListener(std::bind(&UserPropertyLayer::click_image_left, this, std::placeholders::_1));
    //image_right_->addClickEventListener(std::bind(&UserPropertyLayer::click_image_right, this, std::placeholders::_1));
    btn_close_->addClickEventListener(std::bind(&UserPropertyLayer::click_btn_back, this, std::placeholders::_1));
    
    //select_game_index_ = 1;
    
    
    btn_close_->setVisible(false);
    
    this->setContentSize(size);
    
    room_title_ = RoomTitle::create();
    if(in_game) {
        room_title_->set_view_size(Size(200, 19));
    }
    room_title_->setPosition(listview_name->getPosition());
    room_title_->set_click_item(std::bind(&UserPropertyLayer::click_item, this, std::placeholders::_1));
    root->addChild(room_title_);
    room_title_->setContentSize(listview_name->getContentSize());
    listview_name->removeFromParent();
    
    update_data();
    
    return true;
}

void UserPropertyLayer::set_click_item(msg::RoomType game_idx)
{
    room_title_->click_item(game_idx);
}

void UserPropertyLayer::touch_image(Ref* sender, Touch* touch, ui::Widget::TouchEventType event)
{
    switch (event) {
        case Widget::TouchEventType::BEGAN:
        move_ = false;
        began_location_ = touch->getLocation();
        break;
        case Widget::TouchEventType::MOVED:
        move_ = true;
        break;
        case Widget::TouchEventType::ENDED:{
            Point p = touch->getLocation();
            float move = p.x - began_location_.x;
            if(move > 420) {
                room_title_->move_to_left();
            }
            else if(move < -420) {
                room_title_->move_to_right();
            }
            break;
        }
        case Widget::TouchEventType::CANCELED:
        
        break;
        
        default:
        break;
    }
}

void UserPropertyLayer::update_data(cmd_data_pointer data)
{
    room_title_->clear_all_items();
    
    
    GameNameCell* cell_front = GameNameCell::create();
    cell_front->update_data();
    room_title_->push_back_item(cell_front);

    for (int i =0; i< gamesettings.gamesetting_size(); i++) {
        GameNameCell* cell = GameNameCell::create();
        cell->set_in_game(in_game_);
        msg::UserGameSetting setting = gamesettings.gamesetting(i);
        cell->update_data(setting.gameid(), setting.gamename());
        if(i == 0) {
            cell->set_selected(true);
            select_game_index_ = setting.gameid();
        }
        
        room_title_->push_back_item(cell);
    }
    
    GameNameCell* cell_back = GameNameCell::create();
    cell_back->update_data();
    room_title_->push_back_item(cell_back);
    
    refresh_data();
}

void UserPropertyLayer::refresh_data()
{
    listview_property_->removeAllItems();
    this->setting = nullptr;
    
    
    if (gamesettings.gamesetting().size() == 0)
    {
        return;
    }
    for (auto it : gamesettings.gamesetting())
    {
        if (it.gameid() == select_game_index_ && it.property().size() > 0)
        {
            this->setting = new msg::UserGameSetting(it);
            break;
        }
    }
    
    if (setting)
    {
        for (int i = 0; i < this->setting->property_size(); i++) {
            RoomPropertyCell* cell = RoomPropertyCell::create(listview_property_->getTag(), i);
            cell->update_data(this->setting->mutable_property(i));
            listview_property_->pushBackCustomItem(cell);
            
            if(in_game_) {
                cell->setScale(0.5);
                cell->setAnchorPoint(Point(0, 0));
                cell->setContentSize(cell->getContentSize() / 2);
            }
        }
    }
}

void UserPropertyLayer::click_item(msg::RoomType game_idx)
{
    if (game_idx == 0)
        return;
    select_game_index_ = game_idx;
    refresh_data();
}

void UserPropertyLayer::click_text_save(Ref* sender)
{
    uint32_t game_id = gamesettings.gamesetting(select_game_index_ - 1).gameid();
    if (this->setting)
    {
        //GDM->send_change_user_setting(gamesettings);
    }
}
void UserPropertyLayer::click_image_left(Ref* sender)
{
    room_title_->move_to_left();
}
void UserPropertyLayer::click_image_right(Ref* sender)
{
    room_title_->move_to_right();
}

void UserPropertyLayer::click_btn_back(Ref* sender)
{
    this->removeFromParent();
}

void UserPropertyLayer::register_events()
{
    
}