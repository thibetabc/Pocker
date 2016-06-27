//
//  RecordDetailCell.cpp
//  pokerclient
//
//  Created by NaughtyDog on 16/3/11.
//
//

#include "RecordDetailCell.h"
#include "RecordObject.h"
#include "utils.h"
#include "PokerDataManager.h"
#include "cmd_data.h"
#include "GameDataManager.h"
#include "Gametools.h"

Size RecordDetailCell::size()
{
    static Size s(321, 60);
    return s;
}

bool RecordDetailCell::init()
{
    if(!TableViewCell::init()) {
        return false;
    }
    
    root = CSLoader::createNode("NodeRecordDetailCell.csb");
    this->addChild(root);
    
    text_nickname_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "Text_name"));
    text_amount_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "Text_win_amount"));
    text_hand_type_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "Text_hand_type"));
    sprite_card_bg_ = dynamic_cast<Node*>(CSLoader::seekNodeByName(root, "Node_card"));
    auto win_text = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "Text_win"));
    win_text->setString(tools::local_string("win","赢取"));

    this->setContentSize(Size(321, 60));
    gr_cell_ = NULL;
    return true;
}


void RecordDetailCell::update_data(msg::Processor_300_PokerGetHandHistory_DOWN& records, ssize_t idx,bool isDetail ,float offset)
{
    
    if (isDetail) {
        root->setPositionY(offset);
    }
    else
    {
        root->setPositionY(0);
        if (detailNode) {
            detailNode->removeFromParent();
            detailNode = nullptr;
        }
    }
    
    int userid = 0;
    std::string picname = "";
    if (records.has_players()) {

        msg::ReplayPlayer player = records.players().replayplayer(idx);
        text_nickname_->setString(player.userdetailinfo().nickname());
        
        picname = player.userdetailinfo().picname();
        userid = player.userdetailinfo().userid();
        
    }
    
    if (records.has_handresultinfo()) {
        int32_t winchip = records.handresultinfo().playerresult(idx).playerwinchips();
        int32_t betchip = records.handresultinfo().playerresult(idx).playerbetchips();
        text_amount_->setString(tools::to_string(winchip - betchip));
    }
    
    int cardSize = records.steps().replaystep(0).propupdate().pokerplayerinfo(idx).holecards_size();
    
    for (int i = 0; i < cardSize; i++) {
        int index = records.steps().replaystep(0).propupdate().pokerplayerinfo(idx).holecards(i);
        const std::string image_name = PDM->get_card_image(index);
        Sprite* card = Sprite::create(image_name);
        card->setScale(0.6);

        card->setPosition(0 + card->getContentSize().width * (i * 0.6f), 0);
        sprite_card_bg_->addChild(card, 1);
    }
    
    int stepSize = records.steps().replaystep_size();
    
    int infoSize = records.steps().replaystep(stepSize - 1).propupdate().pokerplayerinfo_size();
    
    if (infoSize > idx) {
        std::string card_type = records.steps().replaystep(stepSize - 1).propupdate().pokerplayerinfo(idx).hihandtype();
        std::string low_card_type =  records.steps().replaystep(stepSize - 1).propupdate().pokerplayerinfo(idx).lohandtype();
        
        card_type = tools::trim(card_type);
        low_card_type = tools::trim(low_card_type);
        std::string result;
        if(!card_type.empty() && !low_card_type.empty()) {
            result = card_type + "/" + low_card_type;
        }
        if (result.empty()) {
            result = card_type.empty() ? low_card_type : card_type;
        }
        
        text_hand_type_->setString(result);
    }
    else
    {
        text_hand_type_->setString("");
    }
    
    if (isDetail) {
        
        if (detailNode) {
            detailNode->removeFromParent();
            detailNode = nullptr;
        }
        
        detailNode = Node::create();
        this->addChild(detailNode);
        
        
        if(GDM->is_file_exist(picname)) {
            
//            auto sprite_icon_ = ShaderSprite::create(picname, "login_res/game_res/avatar_mask_circle.png");
//            sprite_icon_->setPosition(Vec2(76,offset -40));
//            sprite_icon_->setScale(76 / sprite_icon_->getContentSize().width,
//                                   76 / sprite_icon_->getContentSize().height);
//            
//            detailNode->addChild(sprite_icon_);
        }
        else
        {
            Sprite * sprite_icon_ = Sprite::create("main_avatar_bg.png");
            sprite_icon_->setPosition(Vec2(76,offset -40));
            detailNode->addChild(sprite_icon_);
        }
        

        
        
        std::vector<int> lines(4,0);
        
        if (records.has_players()) {
            
            
            for(auto& replayStep : records.steps().replaystep())
            {
                if (replayStep.pokerstep().has_blinds()) {
                    for(auto& blind : replayStep.pokerstep().blinds().blind())
                    {
                        if (blind.userid() == userid) {
                            switch (replayStep.pokerstep().round()) {
                                case msg::PokerRoundType::Preflop:
                                {
                                    lines.at(0)++;
                                    
                                }
                                    break;
                                case msg::PokerRoundType::Flop:
                                {
                                    lines.at(1)++;
                                }
                                    break;
                                case msg::PokerRoundType::Turn:
                                {
                                    lines.at(2)++;
                                }
                                    break;
                                case msg::PokerRoundType::River:
                                {
                                    lines.at(3)++;
                                }
                                    break;
                                    
                                default:
                                    break;
                            }
                            
                            
                            std::string title = "";
                            switch (blind.type()) {
                                case msg::BlindType::Smallblind:
                                {
                                    title = tools::local_string("small_blind","小盲");
                                    
                                }
                                    break;
                                case msg::BlindType::Bigblind:
                                {
                                    
                                    title = tools::local_string("big_blind","大盲");
                                    
                                }
                                    break;
                                case msg::BlindType::Ante:
                                {
                                    title = "Ante";
                                    
                                }
                                    break;
                                case msg::BlindType::Straddle:
                                {
                                    title = tools::local_string("chatch","抓");
                                    
                                    
                                }
                                    break;
                                    
                                default:
                                    break;
                            }
                            
                            
                            Text *text = Text::create(title + tools::to_string(blind.amount()), "Arial", 24);
                            text->setColor(Color3B::GRAY);
                            text->setPosition(Vec2(200 + 120 * replayStep.pokerstep().round(),offset + 10 - lines.at(replayStep.pokerstep().round()) * 30));
                            detailNode->addChild(text);
                            
                        }
                    }
                    
                }
                
                if (replayStep.pokerstep().has_playeraction())
                {
                    if (replayStep.pokerstep().playeraction().userid() == userid) {
                        switch (replayStep.pokerstep().round()) {
                            case msg::PokerRoundType::Preflop:
                            {
                                lines.at(0)++;
                                
                            }
                                break;
                            case msg::PokerRoundType::Flop:
                            {
                                lines.at(1)++;
                            }
                                break;
                            case msg::PokerRoundType::Turn:
                            {
                                lines.at(2)++;
                            }
                                break;
                            case msg::PokerRoundType::River:
                            {
                                lines.at(3)++;
                            }
                                break;
                                
                            default:
                                break;
                        }
                        
                        std::string title = "";
                        switch (replayStep.pokerstep().playeraction().action()) {
                            case msg::ActionStatus::FOLD:
                            {
                                title = tools::local_string("fold","弃牌");
                            }
                                break;
                            case msg::ActionStatus::CHECK:
                            {
                                
                                title = tools::local_string("check","让牌");
                                
                            }
                                break;
                            case msg::ActionStatus::BET:
                            {
                                title = tools::local_string("bet","下注") + tools::to_string(replayStep.pokerstep().playeraction().amount());
                            }
                                break;
                            case msg::ActionStatus::CALL:
                            {
                                title = tools::local_string("raise","跟注") + tools::to_string(replayStep.pokerstep().playeraction().amount());
                            }
                                break;
                            case msg::ActionStatus::RAISE:
                            {
                                title = tools::local_string("raise","加注") + tools::to_string(replayStep.pokerstep().playeraction().amount());
                            }
                                break;
                            case msg::ActionStatus::ALLIN:
                            {
                                title = tools::local_string("all_in","全下") + tools::to_string(replayStep.pokerstep().playeraction().amount());
                            }
                                break;
                                
                            default:
                                break;
                        }
                        
                        Text *text = Text::create(title, "Arial", 24);
                        text->setColor(Color3B::GRAY);
                        text->setPosition(Vec2(200 + replayStep.pokerstep().round() * 120,offset + 10 - lines.at(replayStep.pokerstep().round()) * 30));
                        detailNode->addChild(text);

                        
                    }
                    
                }
                
            }
            
            
        }
    
    }
    
}
void RecordDetailCell::select(const bool s)
{
}