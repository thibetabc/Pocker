#include "ChipNode.h"
#include "utils.h"
#include "GameTools.h"


bool ChipNode::init()
{
    if (!Node::init()) {
        return false;
    }
    
    auto root = CSLoader::createNode("NodeChip.csb");
    this->addChild(root,255);
    sprite_chip_ = dynamic_cast<Sprite* >(CSLoader::seekNodeByName(root, "sprite_chip"));
    text_amount_ = dynamic_cast<Text* >(CSLoader::seekNodeByName(root, "text_amount"));
    bg_ = dynamic_cast<ImageView* >(CSLoader::seekNodeByName(root, "Image_1"));
    return true;
}

void ChipNode::update_data(const int amount, const bool pool)
{
    setVisible(amount != 0);
    _amount = amount;
    text_amount_->setString(tools::to_string(amount));
    if(pool)
    {
        sprite_chip_->setTexture("game_res/chips/fenchi.png");
        bg_->setVisible(false);
        text_amount_->setFontSize(25);
        text_amount_->setPosition(Point(10, 12));
    }
    else
    {
        sprite_chip_->setTexture(GameTools::get_chip_image_name(amount));
    }
}
void ChipNode::setHorizontal()
{
    sprite_chip_->setPosition(-20, 0);
    text_amount_->setAnchorPoint(Vec2(0, 0.5f));
    text_amount_->setTextHorizontalAlignment(cocos2d::TextHAlignment::LEFT);
    text_amount_->cocos2d::Node::setPosition(0, 0);
    
}
