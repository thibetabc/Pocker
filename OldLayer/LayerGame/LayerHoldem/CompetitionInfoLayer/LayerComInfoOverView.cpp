//
//  LayerComInfoOverView.cpp
//  pokerclient
//
//  Created by ll on 15/11/26.
//
//

#include "LayerComInfoOverView.h"
#include "utils.h"

bool LayerComInfoOverView::init()
{
    if(!Layer::init()) {
        return false;
    }
    
    root_ = CSLoader::createNode("LayerComInfoOverViewPortrait.csb");
    this->addChild(root_);
    
    text_note_1_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_note_1"));
    text_note_2_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_note_2"));
    text_note_3_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_note_3"));
    text_note_4_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_note_4"));
    text_note_5_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_note_5"));
    text_note_6_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_note_6"));
    text_note_7_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_note_7"));
    
    text_note_1_->setString("");
    text_note_2_->setString("");
    text_note_3_->setString("");
    text_note_4_->setString("");
    text_note_5_->setString("");
    text_note_6_->setString("");
    text_note_7_->setString("");
    
    text_note_1_->setVisible(false);
    text_note_2_->setVisible(false);
    text_note_3_->setVisible(false);
    text_note_4_->setVisible(false);
    text_note_5_->setVisible(false);
    text_note_6_->setVisible(false);
    text_note_7_->setVisible(false);
    
    return true;
}

void LayerComInfoOverView::update_data(const msg::Processor_530_PokerGetTourmentStatus_DOWN&tourment_status)
{
//    当前人数 totalplayernum
//    总筹码
//    平均筹码
//    if currentlevel > maxaddon  不可重买入
//        currentlevel < maxaddon 可以重买入(买入为buyin)
//        当前级别 小盲/大盲/ante 剩余xx分钟
//        下一级别 小盲/大盲/ante
//        
//        如果是最后一个，就只显示当前级别，且不显示剩余时间
    
    int32_t current_players_num = tourment_status.totalplayernum();
    int32_t total_chips = tourment_status.totalchips();
    
    int32_t average_chips;
    if (current_players_num != 0)
        average_chips = total_chips / current_players_num;
    else
        average_chips = total_chips;
    
    int32_t buyin = tourment_status.tournamentinfo().buyin();
    uint32_t current_level = tourment_status.currentlevel();
    
    text_note_1_->setString(tools::local_string("current_players", "当前人数：") + tools::to_string(current_players_num));
    text_note_1_->setVisible(true);
    text_note_2_->setString(tools::local_string("total_chips", "总筹码：") + tools::to_string(total_chips));
    text_note_2_->setVisible(true);
    text_note_3_->setString(tools::local_string("average_chips", "平均筹码：") + tools::to_string(average_chips));
    text_note_3_->setVisible(true);
    
    if (current_level > tourment_status.tournamentinfo().maxaddonlevel())
    {
        text_note_4_->setString(tools::local_string("Addon_no_rebuy", "Addon：不可重买入"));
    }else
    {
        text_note_4_->setString(std::string(tools::local_string("Addon_rebuy_amount", "Addon：可以重买入（买入:")) + tools::to_string(buyin) + std::string("）"));
    }
    text_note_4_->setVisible(true);
    
    if (current_level != 0 && current_level <= tourment_status.tournamentinfo().tournamentlevel().level().size())
    {
        if (current_level == tourment_status.tournamentinfo().tournamentlevel().level().size())
        {
            int32_t small_blind = tourment_status.tournamentinfo().tournamentlevel().level(current_level-1).smallblind();
            int32_t big_blind = tourment_status.tournamentinfo().tournamentlevel().level(current_level-1).bigblind();
            int32_t current_ante = tourment_status.tournamentinfo().tournamentlevel().level(current_level-1).ante();
            
            text_note_5_->setString(std::string(tools::local_string("current_level", "当前级别：")) + tools::to_string(small_blind) + std::string("/") + tools::to_string(big_blind) + std::string("/") + tools::to_string(current_ante));
            text_note_6_->setVisible(false);
        }else
        {
            int32_t current_small_blind = tourment_status.tournamentinfo().tournamentlevel().level(current_level-1).smallblind();
            int32_t current_big_blind = tourment_status.tournamentinfo().tournamentlevel().level(current_level-1).bigblind();
            int32_t current_ante = tourment_status.tournamentinfo().tournamentlevel().level(current_level-1).ante();
            float current_last_time = 1.0 * tourment_status.lasttime() / (1000.0 * 60.0);
            
            int32_t next_small_blind = tourment_status.tournamentinfo().tournamentlevel().level(current_level).smallblind();
            int32_t next_big_blind = tourment_status.tournamentinfo().tournamentlevel().level(current_level).bigblind();
            int32_t next_ante = tourment_status.tournamentinfo().tournamentlevel().level(current_level).ante();
            
            std::string tourment_status_text;
            switch (tourment_status.status()) {
                case msg::TournamentStatus::TOURNAMENT_NORMAL :{
                    tourment_status_text = "";
                    break;
                }
                case msg::TournamentStatus::TOURNAMENT_RESTING :{
                    tourment_status_text = " 休息中";
                    break;
                }
                case msg::TournamentStatus::TOURNAMENT_WAITINGTOSTART :{
                    tourment_status_text = " 等待开始";
                    break;
                }
                default:
                    break;
            }
            std::string current_last_time_str = "";
            if (current_last_time < 1)
            {
                current_last_time_str += tools::to_string((int32_t)(current_last_time * 60));
                current_last_time_str += "秒";
            }else
            {
                current_last_time_str += tools::to_string((int32_t)(current_last_time));
                current_last_time_str += "分钟";
            }
            text_note_5_->setString(std::string("当前级别：") + tools::to_string(current_small_blind) + std::string("/") + tools::to_string(current_big_blind) + std::string("/") + tools::to_string(current_ante) + std::string("  剩余") + current_last_time_str + tourment_status_text);
            
            text_note_6_->setString(std::string("下一级别：") + tools::to_string(next_small_blind) + std::string("/") + tools::to_string(next_big_blind) + std::string("/") + tools::to_string(next_ante));
            
            text_note_5_->setVisible(true);
            text_note_6_->setVisible(true);
        }
    }else
    {
        cocos2d::log("WARNING : TourmentLevel size error!");
        text_note_5_->setVisible(false);
        text_note_6_->setVisible(false);
    }
    text_note_7_->setVisible(false);
}