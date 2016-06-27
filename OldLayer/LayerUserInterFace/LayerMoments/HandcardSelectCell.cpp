//
//  HandcardSelectCell.cpp
//  pokerclient
//
//  Created by admin on 15-9-7.
//
//

#include "HandcardSelectCell.h"
#include "utils.h"


bool HandcardSelectCell::init()
{
    if(!TableViewCell::init()) {
        return false;
    }
    
    auto root = CSLoader::createNode("NodeSelectHandcardCell.csb");
    this->addChild(root);
    
    auto image_bg = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root, "image_bg"));
    
    text_group_name_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "Text_group_name"));
    text_room_name_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "Text_room_name"));
    text_player_name_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "Text_player_name"));
    text_pool_chip_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "Text_dichi"));
    text_ju_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "Text_ju"));
    
    LayerColor *bg = LayerColor::create(Color4B::WHITE, image_bg->getContentSize().width, image_bg->getContentSize().height - 2);
    bg->setAnchorPoint(Point::ZERO);
    bg->setPosition(Point::ZERO);
    root->addChild(bg, -1);
    
    auto btn_replay = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "Button_huifang"));
    btn_replay->addClickEventListener(std::bind(&HandcardSelectCell::click_btn_replay, this, std::placeholders::_1));
    
    return true;
}

void HandcardSelectCell::click_btn_replay(Ref* sender)
{

}
