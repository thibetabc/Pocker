//  创建房间
#include "RoomCreateLayer.h"
#include "GameTools.h"
#include "utils.h"
#include "GameDataManager.h"
#include "GameNameCell.h"
#include "cmd_def.h"
#include "RoomPropertyCell.h"
#include "TipView.h"
#include "RoomTitle.h"
#include "LayerEditName.h"
#include "msg.pb.h"

#define LISTVIEW_TAG 1000

bool RoomCreateLayer::init()
{
    if(!BaseLayer::init()) {
        return false;
    }
    
    select_game_id_ = msg::RoomType::OFC3;
    
    //
    auto size = Director::getInstance()->getWinSize();
    size = Size(std::min(size.width, size.height), std::max(size.width, size.height));
    
    auto layout_move = ui::Layout::create();
    layout_move->setContentSize(size);
    layout_move->setTouchEnabled(true);
    layout_move->setSwallowTouches(false);
    layout_move->setAnchorPoint(Point::ZERO);
    layout_move->addTouchCallback(std::bind(&RoomCreateLayer::touch_image, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    this->addChild(layout_move, 9);
    
    auto root = CSLoader::createNode("LayerRoomSetting.csb");
    this->addChild(root);
    
    auto text_save = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_save"));
    text_save->setString(tools::local_string("save", "保存"));
    
    text_save->addClickEventListener(std::bind(&RoomCreateLayer::click_text_save, this, std::placeholders::_1));
    
    auto btn_back = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "btn_back"));
    btn_back->addClickEventListener([this](Ref*) {
        this->removeFromParent();
    });
    listview_ = dynamic_cast<ListView*>(CSLoader::seekNodeByName(root, "listview"));
    listview_->setTag(LISTVIEW_TAG);
    listview_->addEventListener(std::bind(&RoomCreateLayer::touchListView, this, std::placeholders::_1, std::placeholders::_2));
    listview_->addTouchCallback(std::bind(&RoomCreateLayer::touch_image, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    
    
    
    auto text_room_name = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_room_name"));
    text_room_name->setString(tools::local_string("text_room", "房间名字"));
    
    auto tf_room_name = dynamic_cast<TextField*>(CSLoader::seekNodeByName(root, "tf_room_name"));
    tf_room_name_ = GameTools::createEditBoxFromTextField(root, tf_room_name);
    tf_room_name_->setInputMode(EditBox::InputMode::SINGLE_LINE);
    
    auto room_info = GDM->get_room_infomations();
    select_game_id_ = room_info.createroominfo(0).roomtype().roomtype();
    update_room_type_list(nullptr);
    
    register_events();
    
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
    
    return true;
}
void RoomCreateLayer::touch_image(Ref* sender, Touch* touch, ui::Widget::TouchEventType event)
{
   
}

void RoomCreateLayer::click_item(msg::RoomType idx)
{
    select_game_id_ = idx;
    update_data(cmd_data_pointer());
}
void RoomCreateLayer::touchListView222(Ref* sender, Touch* touch, ui::Widget::TouchEventType event)
{
    
}
void RoomCreateLayer::touchListView(Ref* sender, ListView::EventType event)
{
    
    if(event != ListView::EventType::ON_SELECTED_ITEM_END)
    return;
    
    size_t index = listview_->getCurSelectedIndex();
    auto property = propertys_->mutable_property(index);
    
    if(property->type() == msg::PropertyType::BOOL)
    return;
    
    LayerEditName* len = LayerEditName::create(LayerEditName::EDIT_OTHER);
    len->set_show_dir(LAYER_SHOW_DIRECTION::LEFT_TO_RIGHT);
    this->addChild(len);
    
    
    
    len->set_title(property->displayname());
    len->set_number_keyboard();
    
    len->set_defalut_value(tools::to_string(property->intvalue().value()));
    auto item = dynamic_cast<RoomPropertyCell*>(listview_->getItems().at(index));
    
    len->set_dismiss_callback([=](){
        if(item) {
            if (len->GetValueConfirmed()){
                property->mutable_intvalue()->set_value(tools::stoi(len->get_value()));
                item->update_data(property);
            }
        }
    });
}

void RoomCreateLayer::click_text_save(Ref* sender)
{
    const std::string room_name = tf_room_name_->getText();
    if(room_name.empty()) {
        TipView::showAlertView(tools::local_string("roomname_empty", "房间名称不能为空！"));
        return;
    }
    
    //GDM->send_create_room(GDM->get_select_group()->get_id(),select_game_id_,propertys_,room_name);
    this->removeFromParent();
}

void RoomCreateLayer::click_image_left(Ref* sender)
{
    
}
void RoomCreateLayer::click_image_right(Ref* sender)
{
    
}

void RoomCreateLayer::update_data(cmd_data_pointer)
{
    listview_->removeAllItems();
    propertys_ = GDM->get_Property_by_game_id(select_game_id_);
    
    for (int i=0; i<propertys_->property_size(); i++) {
        RoomPropertyCell* cell = RoomPropertyCell::create(listview_->getTag(), i);
        cell->setTouchEnabled(true);
        cell->update_data(propertys_->mutable_property(i));
        listview_->pushBackCustomItem(cell);
    }
}

void RoomCreateLayer::update_room_type_list(cmd_data_pointer)
{
}

void RoomCreateLayer::onEnterTransitionDidFinish()
{
    BaseLayer::onEnterTransitionDidFinish();
    update_room_type_list(nullptr);
}


#pragma mark private
void RoomCreateLayer::register_events()
{
    dispatch_.register_event(EVENT_HANDLE_ROOM_INFO_FINISHED,
                             BIND_FUNC(this, RoomCreateLayer::update_data));
    dispatch_.register_event(EVENT_HANDLE_ROOM_INFO_FINISHED,
                             BIND_FUNC(this, RoomCreateLayer::update_room_type_list));
}

#pragma mark tableview datasource --
TableViewCell* RoomCreateLayer::tableCellAtIndex(TableView *table, ssize_t idx)
{
}

ssize_t RoomCreateLayer::numberOfCellsInTableView(TableView *table)
{
    return room_datas_.size();
}
#pragma mark tableview delegate --
void RoomCreateLayer::tableCellTouched(TableView* table, TableViewCell* cell)
{
    
}

