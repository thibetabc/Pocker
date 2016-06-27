#include "MatchScroeCell.h"
#include "utils.h"
#include "PokerDataManager.h"
#include "Room.h"
bool MatchScroeCell::init()
{
    if(!Widget::init()) {
        return false;
    }

    return true;
}

void MatchScroeCell::update_data(const std::shared_ptr<MatchScroeNode::MATCH_OBJECT>& obj)
{
    if(obj->room_type == msg::RoomType::OFC3 || obj->room_type == msg::RoomType::OFC4 || obj->room_type == msg::RoomType::SIChUANMahjong)
    {
        auto root = CSLoader::createNode("NodeOfcMatchScoreCell.csb");
        this->addChild(root);
        
        this->setContentSize(Size(367, 60));
        
        text_nickname_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_nickname"));
        text_win_chip_amount_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_player_score"));
        
        text_nickname_->setString(obj->nickname);
        text_win_chip_amount_->setString(tools::to_string(obj->win_amount));
    }else
    {
        auto root = CSLoader::createNode("NodeTimeRecord.csb");
        this->addChild(root);
        
        this->setContentSize(Size(643, 60));
        
        text_nickname_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "Text_1"));
        text_buy_chip_amount_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "Text_2"));
        text_win_chip_amount_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "Text_2_0"));
        
        text_nickname_->setString(obj->nickname);
        text_buy_chip_amount_->setString(tools::to_string(obj->buy_amount));
        text_win_chip_amount_->setString(tools::to_string(obj->win_amount));
    }

}
