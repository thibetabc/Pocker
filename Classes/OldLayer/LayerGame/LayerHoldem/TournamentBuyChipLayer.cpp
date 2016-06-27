//
//  TournamentBuyChipLayer.cpp
//  pokerclient
//
//  Created by ll on 15/12/3.
//
//

#include "TournamentBuyChipLayer.h"
#include "utils.h"
#include "PokerDataManager.h"
#include "Room.h"
#include "GameDataManager.h"
#include "user_data.h"

bool TournamentBuyChipLayer::init()
{
    if(!PokerChildLayer::init()) {
        return false;
    }
    
    auto root = CSLoader::createNode("LayerTourmentBuyChip.csb");
    this->addChild(root);
    auto btn_cancel = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "btn_cancel"));
    auto btn_confirm = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "btn_confirm"));
    btn_cancel->addClickEventListener(std::bind(&TournamentBuyChipLayer::click_btn_cancel, this, std::placeholders::_1));
    btn_confirm->addClickEventListener(std::bind(&TournamentBuyChipLayer::click_btn_confirm, this, std::placeholders::_1));
    
    text_buyin_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_buy_chip"));

    return true;
}

void TournamentBuyChipLayer::set_text_string(std::string text_string)
{
    text_buyin_->setString(text_string);
}

void TournamentBuyChipLayer::click_btn_cancel(Ref* sender)
{
    auto &poker_room_data = this->GetRoom();
    uint32_t roomID = poker_room_data->get_id();
    
    PDM->send_poker_tournament_addon(roomID, false);
    this->removeFromParent();
}


void TournamentBuyChipLayer::handle_android_back()
{
    click_btn_cancel(nullptr);
    
    this->removeFromParent();
}

void TournamentBuyChipLayer::click_btn_confirm(Ref* sender)
{
    auto &poker_room_data = this->GetRoom();
    uint32_t roomID = poker_room_data->get_id();
    
    PDM->send_poker_tournament_addon(roomID, true);
    this->removeFromParent();
}