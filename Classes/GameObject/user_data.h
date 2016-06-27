#ifndef __pokerclient__user_data__
#define __pokerclient__user_data__

#include <map>
#include <memory>
#include <vector>
#include <string>
#include <iostream>
#include "cocos2d.h"
#include "msg.pb.h"
class user_base
{
public:
    const msg::SittingStatus get_status() {return get_common_info()->sittingstatus();}
    const int get_seat_num() {return get_common_info()->sitnum();}
    void set_seat_num(const int value) { get_common_info()->set_sitnum(value);}
    void set_status(const msg::SittingStatus value) { get_common_info()->set_sittingstatus(value);}
    virtual void clear() = 0;
    virtual void set_common_info(const msg::CommonPlayerInfo& value) = 0;    const msg::Sex get_sex() { return get_common_info()->userinfo().sex();}
    const uint32_t get_id() { return get_common_info()->userinfo().userid();}
    const std::string& get_nickname() { return get_common_info()->userinfo().nickname();}
    const std::string get_head_pic_name();
    void set_userinfo(const msg::UserDetailInfo& value) {get_common_info()->mutable_userinfo()
        ->CopyFrom(value);}
protected:
    void set_userid(int userid) {get_common_info()->mutable_userinfo()->set_userid(userid);}
    virtual msg::CommonPlayerInfo* get_common_info() = 0;
};

class user_texas : public user_base
{
public:
    user_texas(int userid) {set_userid(userid);}
    
    msg::PokerPlayerInfo* get_properties() { return &properties_;}
    void set_common_info(const msg::CommonPlayerInfo& value){properties_.mutable_playerinfo()->CopyFrom(value);}
    void clear();
protected:
    msg::CommonPlayerInfo* get_common_info() {return properties_.mutable_playerinfo();}
    msg::PokerPlayerInfo properties_;
};

class user_ofc : public user_base
{
private:
    class card_is_location
    {
    public:
        uint32_t is_card;
        uint32_t card_location;
    };
public:
    user_ofc(int userid) {set_userid(userid);}
    
    void clear();
    msg::OFCPlayerInfo* get_properties() { return &properties_;}
    msg::UserDetailInfo* get_user_info() {return properties_.mutable_playerinfo()->mutable_userinfo();}
    void set_common_info(const msg::CommonPlayerInfo& value){properties_.mutable_playerinfo()->CopyFrom(value);}
    
    void set_ofc_playerinfo(const msg::OFCPlayerInfo& playerinfo);
    
    bool get_is_ofc_hand_card_change(){return is_ofc_hand_card_change_;};
    void set_is_ofc_hand_card_change(bool is_ofc_hand_card_change){is_ofc_hand_card_change_ = is_ofc_hand_card_change;};
    
    const msg::OFCActionStatus get_ofc_action_status() {return properties_.actionstatus();};
    
    const int get_all_score(){return properties_.playerscore();};
    //Fantasy; //0 代表没有，剩下多少数字代表有多少张牌
    const int get_fantasy() { return properties_.has_fantasycards() ? properties_.fantasycards() : 0;}
    const bool get_refantasy() {return properties_.refantasy();}
    
    const uint32_t get_first_row_score() { return properties_.has_firstrowscore() ? properties_.firstrowscore() : 0;}
    const uint32_t get_middle_row_score() { return properties_.has_middlerowscore() ? properties_.middlerowscore() : 0;}
    const uint32_t get_last_row_score() { return properties_.has_lastrowscore() ? properties_.lastrowscore() : 0;}
    
    //牌型
    msg::OFCCardType get_first_row() { return properties_.has_firstcardtype() ? properties_.firstcardtype() : msg::OFCCardType::NO_TYPE;}
    msg::OFCCardType get_middle_row() { return properties_.has_middlecardtype() ? properties_.middlecardtype() : msg::OFCCardType::NO_TYPE;}
    msg::OFCCardType get_last_row() { return properties_.has_lastcardtype() ? properties_.lastcardtype() : msg::OFCCardType::NO_TYPE;}
    
    //step_score; //每局得分
    void set_step_score(const int32_t step_score){step_score_ = step_score;}
    const int32_t get_step_score(){return step_score_;}
    
    //time_operation;
    const msg::ActionRequest* get_action_request() { return properties_.mutable_actionrequest();}
    const std::vector<uint32_t> get_all_cards();
    void clean_all_cards() {
        properties_.clear_card();
    }
    
    const std::vector<uint32_t> get_first_row_cards();
    const std::vector<uint32_t> get_middle_row_cards();
    const std::vector<uint32_t> get_last_row_cards();
    const std::vector<uint32_t> get_dis_cards();
    const std::vector<uint32_t> get_all_hand_cards();
    
    enum HAND_SORT_TYPE{
        NONE = 0x00,
        FUNCTION1 = 0x01, //按花色
        FUNCTION2 = 0x02, //按大小
        FUNCTION3 = 0x03  //按对子
    };
    
    const std::vector<uint32_t> get_hand_cards(uint8_t hand_sort_type = HAND_SORT_TYPE::NONE);
    
    void set_hand_cards(const std::vector<uint32_t>& hand_cards){
        properties_.clear_holecard();
        for (auto card: hand_cards)
            properties_.add_holecard(card);
    }
    
    void set_hand_cards_location(const std::vector<msg::OFCCardLocation>& hand_cards_location){
        if (hand_cards_location.size() > 0)
            hand_card_location_ = hand_cards_location;
        else
            cocos2d::log("66 set_hand_cards error:hand_cards size is %lu", hand_cards_location.size());
    }
    
    bool find_hand_card(const uint32_t keycard){
        for (auto card: properties_.holecard()){
            if (keycard == card)
                return true;
        }
        return false;
    }
    
    void reset_hand_cards(){
        hand_card_location_.clear();
        for (int i=0; i< properties_.holecard_size(); i++)
            hand_card_location_.push_back(msg::OFCCardLocation::DISCARD);
    }
    
    void reset_hand_cards_fantansy_first()
    {
        for (int i = 0; i < hand_card_location_.size(); i++)
        {
            if (hand_card_location_[i] == msg::OFCCardLocation::FIRST)
                hand_card_location_[i] = msg::OFCCardLocation::DISCARD;
        }
    }
    
    void reset_hand_cards_fantansy_middle()
    {
        for (int i = 0; i < hand_card_location_.size(); i++)
        {
            if (hand_card_location_[i] == msg::OFCCardLocation::MIDDLE)
                hand_card_location_[i] = msg::OFCCardLocation::DISCARD;
        }
    }
    
    void reset_hand_cards_fantansy_last()
    {
        for (int i = 0; i < hand_card_location_.size(); i++)
        {
            if (hand_card_location_[i] == msg::OFCCardLocation::LAST)
                hand_card_location_[i] = msg::OFCCardLocation::DISCARD;
        }
    }
    
    void clear_hand_cards(){
        properties_.clear_holecard();
        reset_hand_cards();
    }
    bool swap(uint32_t StartCard, uint32_t EndCard);
    bool putcard(uint32_t Card, msg::OFCCardLocation Location);
    const std::vector<msg::OFCCardLocation>& get_hand_card_location() { return hand_card_location_;}
    
    void set_hand_card_location(std::vector<msg::OFCCardLocation> hand_card_location) {hand_card_location_ = hand_card_location;}
    static std::vector<std::string> get_score_image_name(uint32_t score, bool isOwner);
    static std::string get_pattern_image_name(msg::OFCCardType row_type, bool isOwner);
protected:
    msg::CommonPlayerInfo* get_common_info() {return properties_.mutable_playerinfo();}
    int32_t step_score_;// 当前一局
    std::vector<msg::OFCCardLocation> hand_card_location_;//正在放牌的位置
    bool is_ofc_hand_card_change_;
    msg::OFCPlayerInfo properties_;
};

const std::vector<std::string> row_type_dic_keys_={"gaopai","yidui","liangdui",
    "santiao","tonghua","shunzi","hulu",
    "sitiao","tonghuashun","huangtong"
};

class user_mahjong : public user_base
{
public:
    user_mahjong(int userid) {set_userid(userid);}
    
    void clear();
    msg::MahjongPlayerInfo* get_properties() { return &properties_;}
    void set_common_info(const msg::CommonPlayerInfo& value){properties_.mutable_playerinfo()->CopyFrom(value);}
protected:
    msg::CommonPlayerInfo* get_common_info() {return properties_.mutable_playerinfo();}
    msg::MahjongPlayerInfo properties_;
};

#endif /* defined(__pokerclient__user_data__) */
