#include "user_data.h"
#include "cmd_def.h"
#include "PokerDataManager.h"
#include "cocos2d.h"
#include <math.h>
#include "utils.h"
#include "GameTools.h"
#include "PlatformSdk.h"

const std::string user_base::get_head_pic_name() {
    std::string head_image_name = get_common_info()->userinfo().picname();
    std::string::size_type idx = head_image_name.rfind(".");
    if (idx == std::string::npos) {
        head_image_name += ".png";
    }
    return head_image_name;
}

std::vector<std::string> user_ofc::get_score_image_name(uint32_t score, bool isOwner)
{
    std::vector<std::string> name_vector;
    uint32_t abs_score = score;
    if (abs_score > 99)
    {
        uint32_t hundred_num = abs_score/100;
        uint32_t ten_num = (abs_score - hundred_num * 100)/10;
        uint32_t single_num = abs_score - hundred_num * 100 - ten_num * 10;
        if (score>0)
        {
            if (isOwner)
            {
                name_vector.push_back("image/ofc_game_new_res/player_score/jia_own.png");
                name_vector.push_back("image/ofc_game_new_res/player_score/jia_own_" + tools::to_string(hundred_num) + ".png");
                name_vector.push_back("image/ofc_game_new_res/player_score/jia_own_" + tools::to_string(ten_num) + ".png");
                name_vector.push_back("image/ofc_game_new_res/player_score/jia_own_" + tools::to_string(single_num) + ".png");
            }
            else
            {
                name_vector.push_back("image/ofc_game_new_res/player_score/jia_other.png");
                name_vector.push_back("image/ofc_game_new_res/player_score/jia_other_" + tools::to_string(hundred_num) + ".png");
                name_vector.push_back("image/ofc_game_new_res/player_score/jia_other_" + tools::to_string(ten_num) + ".png");
                name_vector.push_back("image/ofc_game_new_res/player_score/jia_other_" + tools::to_string(single_num) + ".png");
            }
        }
    }
    else if (abs_score > 9 && abs_score < 100 )
    {
        uint32_t ten_num = abs_score/10;
        uint32_t single_num = abs_score - ten_num * 10;
        if (score>0)
        {
            if (isOwner)
            {
                name_vector.push_back("image/ofc_game_new_res/player_score/jia_own.png");
                name_vector.push_back("image/ofc_game_new_res/player_score/jia_own_" + tools::to_string(ten_num) + ".png");
                name_vector.push_back("image/ofc_game_new_res/player_score/jia_own_" + tools::to_string(single_num) + ".png");
            }
            else
            {
                name_vector.push_back("image/ofc_game_new_res/player_score/jia_other.png");
                name_vector.push_back("image/ofc_game_new_res/player_score/jia_other_" + tools::to_string(ten_num) + ".png");
                name_vector.push_back("image/ofc_game_new_res/player_score/jia_other_" + tools::to_string(single_num) + ".png");
            }
        }
    }
    else
    {
        if (score>0)
        {
            if (isOwner)
            {
                name_vector.push_back("image/ofc_game_new_res/player_score/jia_own.png");
                name_vector.push_back("image/ofc_game_new_res/player_score/jia_own_" + tools::to_string(abs_score) + ".png");
            }
            else
            {
                name_vector.push_back("image/ofc_game_new_res/player_score/jia_other.png");
                name_vector.push_back("image/ofc_game_new_res/player_score/jia_other_" + tools::to_string(abs_score) + ".png");
            }
        }
    }
    
    return name_vector;
}
std::string user_ofc::get_pattern_image_name(msg::OFCCardType row_type, bool isOwner)
{
    int index = 0;
    switch (row_type)
    {
        case msg::OFCCardType::HIGH_CARDS:
            index = 0;
            break;
        case msg::OFCCardType::ONE_PAIR:
            index = 1;
            break;
        case msg::OFCCardType::TWO_PAIRS:
            index = 2;
            break;
        case msg::OFCCardType::THREE_OF_A_KIND:
            index = 3;
            break;
        case msg::OFCCardType::FLUSH:
            index = 4;
            break;
        case msg::OFCCardType::STRAIGHT:
            index = 5;
            break;
        case msg::OFCCardType::FULL_HOUSE:
            index = 6;
            break;
        case msg::OFCCardType::FOUR_OF_A_KIND:
            index = 7;
            break;
        case msg::OFCCardType::STRAIGHT_FLUSH:
            index = 8;
            break;
        case msg::OFCCardType::ROYAL_STRAIGHT_FLUSH:
            index = 9;
            break;
        case msg::OFCCardType::TYPE_NORMAL:
            index = 2;
            break;
        case msg::OFCCardType::TYPE_INVALID:
            index = 3;
            break;
        case msg::OFCCardType::TYPE_FOLD:
            index = 4;
            break;
        default:
            break;
    }
    std::string type;
    int prenset_language_type = GameTools::getLanguage();
    if (isOwner)
    {
        std::string dic_key = row_type_dic_keys_.at(index)+"_2.png";
        type = GameTools::get_languages_pic_key(dic_key);
        
    }
    else
    {
        std::string dic_key = row_type_dic_keys_.at(index)+"_1.png";
        type = GameTools::get_languages_pic_key(dic_key);
    }
    printf("card type is %s",type.c_str());
    return type;
}

const std::vector<uint32_t> user_ofc::get_first_row_cards() {
    std::vector<uint32_t> result;
    for (auto card: properties_.card())
        if (card.cardlocation() == msg::OFCCardLocation::FIRST)
        result.push_back(card.card());
    return result;
}

const std::vector<uint32_t> user_ofc::get_middle_row_cards() {
    std::vector<uint32_t> result;
    for (auto card: properties_.card())
        if (card.cardlocation() == msg::OFCCardLocation::MIDDLE)
            result.push_back(card.card());
    return result;
}

const std::vector<uint32_t> user_ofc::get_last_row_cards() {
    std::vector<uint32_t> result;
    for (auto card: properties_.card())
        if (card.cardlocation() == msg::OFCCardLocation::LAST)
            result.push_back(card.card());
    return result;
}

const std::vector<uint32_t> user_ofc::get_dis_cards(){
    std::vector<uint32_t> result;
    for (auto card: properties_.card())
        if (card.cardlocation() == msg::OFCCardLocation::DISCARD)
            result.push_back(card.card());
    return result;
}

const std::vector<uint32_t> user_ofc::get_all_hand_cards() {
    std::vector<uint32_t> result;
    for (auto card: properties_.holecard())
        result.push_back(card);
    return result;
}

const std::vector<uint32_t> user_ofc::get_all_cards() {
    std::vector<uint32_t> first_row_cards_ = get_first_row_cards();
    std::vector<uint32_t> middle_row_cards_ = get_middle_row_cards();
    std::vector<uint32_t> last_row_cards_ = get_last_row_cards();
    int nIndex = 0;
    std::vector<uint32_t> result;
    for (int i=0; i<first_row_cards_.size(); i++){
        result.push_back(first_row_cards_[i]);
        nIndex++;
    }
    for (int i=0; i<properties_.holecard_size(); i++)
        if (hand_card_location_[i] == msg::OFCCardLocation::FIRST){
            result.push_back(properties_.holecard(i));
            nIndex++;
        }
    for (int i=nIndex; i<3; i++){
        result.push_back(100);
        nIndex++;
    }
    for (int i=0; i<middle_row_cards_.size(); i++){
        result.push_back(middle_row_cards_[i]);
        nIndex++;
    }
    for (int i=0; i<properties_.holecard_size(); i++)
        if (hand_card_location_[i] == msg::OFCCardLocation::MIDDLE){
            result.push_back(properties_.holecard(i));
            nIndex++;
        }
    for (int i=nIndex; i<8; i++){
        result.push_back(100);
        nIndex++;
    }
    for (int i=0; i<last_row_cards_.size(); i++){
        result.push_back(last_row_cards_[i]);
        nIndex++;
    }
    for (int i=0; i<properties_.holecard_size(); i++)
        if (hand_card_location_[i] == msg::OFCCardLocation::LAST){
            result.push_back(properties_.holecard(i));
            nIndex++;
        }
    for (int i=nIndex; i<13; i++){
        result.push_back(100);
        nIndex++;
    }
    return result;
}

const std::vector<uint32_t> user_ofc::get_hand_cards(uint8_t hand_sort_type) {
    
    std::function<bool(const int, const int)> greaternumber = [](const int n1, const int n2){
        return (n1-1) % 13 > (n2-1) % 13;
    };
    
    std::vector<uint32_t> result;
    for (int i=0; i<properties_.holecard_size(); i++)
        if (hand_card_location_[i] == 0)
            result.push_back(properties_.holecard(i));
    if (hand_sort_type == FUNCTION1)
        std::sort(result.begin(), result.end(), std::greater<int>());
    else if (hand_sort_type == FUNCTION2)
        std::sort(result.begin(), result.end(), greaternumber);
    else if (hand_sort_type == FUNCTION3){
        std::vector<uint32_t> sortedresult;
        int number[13] = {0,0,0,0,0,0,0,0,0,0,0,0,0};
        for (auto card: properties_.holecard())
            number[(card - 1) % 13]++;
        for (int i=4; i>0; i--)
            for (int j=0; j<13; j++)
                if (number[j] == i){
                    for (int k=0; k<result.size(); k++)
                        if((result[k] - 1) % 13 == j)
                            sortedresult.push_back(result[k]);
                }
        result = sortedresult;
    }
    return result;
}

bool user_ofc::swap(uint32_t StartCard, uint32_t EndCard){
    int startIndex = -1;
    int endIndex = -1;
    for (int i=0; i<properties_.holecard_size(); i++){
        if (properties_.holecard(i) == StartCard)
            startIndex = i;
        else if (properties_.holecard(i) == EndCard)
            endIndex = i;
    }
    if ((startIndex != -1) && (endIndex != -1)){
        msg::OFCCardLocation temp = hand_card_location_[startIndex];
        hand_card_location_[startIndex] = hand_card_location_[endIndex];
        hand_card_location_[endIndex] = temp;
        return true;
    }
    else
        return false;
}

bool user_ofc::putcard(uint32_t Card, msg::OFCCardLocation Location){
    bool result;
    std::vector<uint32_t> first_row_cards_ = get_first_row_cards();
    std::vector<uint32_t> middle_row_cards_ = get_middle_row_cards();
    std::vector<uint32_t> last_row_cards_ = get_last_row_cards();
    int Index = -1;
    for (int i=0; i<properties_.holecard_size(); i++){
        if (properties_.holecard(i) == Card)
            Index = i;
    }
    
    int first_hand_size = 0;
    int middle_hand_size = 0;
    int last_hand_size = 0;
    for (int i = 0; i < hand_card_location_.size(); i++)
    {
        if (hand_card_location_[i] == 1)
            first_hand_size++;
        else if (hand_card_location_[i] == 2)
            middle_hand_size++;
        else if (hand_card_location_[i] == 3)
            last_hand_size++;
    }
    
    switch (Location){
        case 1:
            result = first_row_cards_.size()<(3-first_hand_size);
            break;
        case 2:
            result = middle_row_cards_.size()<(5-middle_hand_size);
            break;
        case 3:
            result = last_row_cards_.size()<(5-last_hand_size);
            break;
        default: result = true;
    }
    if (result && (Index != -1)){
        hand_card_location_[Index] = Location;
        return true;
    }
    else
        return false;
}

void user_ofc::set_ofc_playerinfo(const msg::OFCPlayerInfo& playerinfo)
{
    is_ofc_hand_card_change_ = false;
    if (properties_.holecard_size() != playerinfo.holecard_size())
        is_ofc_hand_card_change_ =  true;
    for (auto card: properties_.holecard())
    {
        bool isfind = false;
        for (auto comparecard: playerinfo.holecard())
        {
            if (card == comparecard)
            {
                isfind = true;
                break;
            }
        }
        if (!isfind)
            is_ofc_hand_card_change_ =  true;
    }
    
    if (is_ofc_hand_card_change_)
    {
        hand_card_location_.clear();
        for (int i=0; i<playerinfo.holecard_size(); i++)
            hand_card_location_.push_back(msg::OFCCardLocation::DISCARD);
    }
    
    properties_.Clear();
    properties_.CopyFrom(playerinfo);
}

void user_ofc::clear(){
    msg::OFCPlayerInfo tempinfo;
    tempinfo.CopyFrom(properties_);
    properties_.mutable_playerinfo()->CopyFrom(tempinfo.playerinfo());
    properties_.set_playerscore(tempinfo.playerscore());
}

void user_texas::clear(){
    msg::PokerPlayerInfo tempinfo;
    tempinfo.CopyFrom(properties_);
    properties_.mutable_playerinfo()->CopyFrom(tempinfo.playerinfo());
    properties_.set_chips(tempinfo.chips());
}

void user_mahjong::clear(){
    msg::MahjongPlayerInfo tempinfo;
    tempinfo.CopyFrom(properties_);
    properties_.mutable_playerinfo()->CopyFrom(tempinfo.playerinfo());
    properties_.set_playerscore(tempinfo.playerscore());
}
