#include "ChipPoolNode.h"
#include "utils.h"
#include "PokerDataManager.h"
#include "Room.h"
#include "GameDataManager.h"
#include "user_data.h"
#include "utils.h"
#include "PokerRoomLayer.h"
#include "msg.pb.h"
#include "net_data_recv.h"

bool ChipPoolNode::init()
{
    if(!Node::init()) {
        return false;
    }

    auto root = CSLoader::createNode("NodeChipAmount.csb");
    this->addChild(root);
    
    text_chip_number_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_chip_number"));
    text_chip_number_->setFontSize(30);
    
    text_half_pool_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_half_pool"));
    text_half_pool_->setString(tools::local_string("half_pot",  "1/2池"));
    text_half_pool_amount = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_half_pool_amount"));
    
    text_two_third_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_two_third_pool"));
    text_two_third_->setString(tools::local_string("twothirds_pot", "2/3池"));
    text_two_third_pool_amount = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_two_third_pool_amount"));
    
    
    text_full_pool_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_full_pool"));
    text_full_pool_->setString(tools::local_string("full_pot", "满池"));
    text_full_pool_amount = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_full_pool_amount"));
    
    text_all_in_amount = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_all_in_amount"));
    
    
    slider_text = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "slider_text"));
    slider_text_Y = slider_text->getPositionY();
    slider_chip_ = dynamic_cast<Slider*>(CSLoader::seekNodeByName(root, "slider_chip"));
    slider_chip_->setCapInsetProgressBarRebderer(Rect(10, 10, 10, 10));
    slider_chip_->setCapInsets(Rect(10, 10, 10, 10));
    sliderRender = dynamic_cast<Scale9Sprite*>(slider_chip_->getVirtualRenderer());
    sliderRender->setVisible(false);
    chip_add_sub_bg = dynamic_cast<Sprite*>(CSLoader::seekNodeByName(root, "chip_add_sub_bg_10"));
    chip_add_sub_bg->setVisible(false);
    barLength = slider_chip_->getContentSize().width;
    
    button_half_pool = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "button_half_pool"));
    
    button_two_third_pool = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "button_two_third_pool"));
    
    button_full_pool = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "button_full_pool"));
    
    button_all_in = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "button_all_in"));
    

    
    button_half_pool->addClickEventListener(std::bind(&ChipPoolNode::click_image_half_pool, this, std::placeholders::_1));
    

    button_two_third_pool->addClickEventListener(std::bind(&ChipPoolNode::click_image_two_third, this, std::placeholders::_1));
    button_full_pool->addClickEventListener(std::bind(&ChipPoolNode::click_image_full_pool, this, std::placeholders::_1));
    
    button_all_in->addClickEventListener(std::bind(&ChipPoolNode::click_image_allin, this, std::placeholders::_1));
    
    text_half_pool_->setString(tools::local_string("one_third", "1/2池"));
    text_two_third_->setString(tools::local_string("half_pool", "2/3池"));
    text_full_pool_->setString(tools::local_string("full_pool", "满池"));
    slider_text->setString(tools::local_string("raise_free","自由加注"));
    
    slider_chip_->addTouchEventListener(std::bind(&ChipPoolNode::clickTouchEventListener, this, std::placeholders::_1, std::placeholders::_2));
                                        
    slider_chip_->addEventListener(std::bind(&ChipPoolNode::click_slider, this, std::placeholders::_1, std::placeholders::_2));
    slider_chip_->setPercent(0);

    amount_ = 0;
    
    pokerRoomLayer = nullptr;
    
    return true;
}

void ChipPoolNode::click_btn_sub_chip(Ref* sender)
{
    uint32_t min_chip = player_action_info_.betmin();
    uint32_t max_chip = player_action_info_.betmax();
    uint32_t unit = player_action_info_.unit();
    
    
    uint32_t sub = max_chip - min_chip;
    int32_t value = amount_ - unit;
    if(value >= min_chip && value <= max_chip) {
        float percent = (value - min_chip) * 100.0f / sub;
        slider_chip_->setPercent(percent);
        amount_ = value;
        text_chip_number_->setString(tools::to_string(amount_));
    }
}
void ChipPoolNode::click_btn_add_chip(Ref* sender)
{
    uint32_t min_chip = player_action_info_.betmin();
    uint32_t max_chip = player_action_info_.betmax();
    uint32_t unit = player_action_info_.unit();
    uint32_t sub = max_chip - min_chip;
    int32_t value = amount_ + unit;
    if(value >= min_chip && value <= max_chip) {
        float percent = (value - min_chip) * 100.0f / sub;
        slider_chip_->setPercent(percent);
        amount_ = value;
        text_chip_number_->setString(tools::to_string(amount_));
    }
}
void ChipPoolNode::clickTouchEventListener(Ref *sender, Widget::TouchEventType controlEvent)
{
    if (controlEvent == Widget::TouchEventType::BEGAN) {
        sliderRender->setVisible(true);
        chip_add_sub_bg->setVisible(true);
        button_half_pool->setVisible(false);
        button_two_third_pool->setVisible(false);
        button_full_pool->setVisible(false);
        button_all_in->setVisible(false);
        
    }
    else if (controlEvent == Widget::TouchEventType::ENDED || controlEvent == Widget::TouchEventType::CANCELED)
    {
        sliderRender->setVisible(false);
        chip_add_sub_bg->setVisible(false);
        slider_text->setPositionY(slider_text_Y);
        if (pokerRoomLayer) {
            pokerRoomLayer->click_btn_add(nullptr);
        }
    
    }
}
void ChipPoolNode::click_slider(Ref* sender, Slider::EventType)
{
    uint32_t min_chip = player_action_info_.betmin();
    uint32_t max_chip = player_action_info_.betmax();
    cocos2d::log("ChipPoolNode:click_slider min_chip:%u, max_chip:%u", min_chip, max_chip);
    uint32_t sub = max_chip - min_chip;
    uint32_t count = sub * slider_chip_->getPercent() / 100 + min_chip;
    log("%f",slider_chip_->getPercent());
    
    float res = slider_chip_->getPercent() / 100.0f;
    float dis = barLength * res;
    slider_text->setPositionY(slider_text_Y + dis);
    
    if(count % player_action_info_.unit() != 0) {
        count = count / player_action_info_.unit() * player_action_info_.unit() + player_action_info_.unit();
    }
    
    auto user = static_pointer_cast<user_texas>(GetRoom()->get_user(GDM->get_user_id()));
    if(count > max_chip) {
        count -= player_action_info_.unit();
    }
    
    count = std::max(min_chip, count);
    log("count:%d", count);
    if(count <= min_chip) {
        count = min_chip;
    }
    if(count >= max_chip) {
        count = max_chip;
    }
    amount_ = count;
    text_chip_number_->setString(tools::to_string(count));
}
void ChipPoolNode::click_image_allin(Ref* sender)
{
    if (pokerRoomLayer) {
        pokerRoomLayer->click_btn_all(nullptr);
    }
}
void ChipPoolNode::click_image_min(Ref* sender)
{
    slider_chip_->setPercent(0);
    
    click_slider(slider_chip_, Slider::EventType::ON_PERCENTAGE_CHANGED);
}

uint32_t ChipPoolNode::getBetSizeByPot(float APotRatio)
{
    uint32_t min_chip = player_action_info_.betmin();
    uint32_t max_chip = player_action_info_.betmax();
    
    int amount = player_action_info_.betpot() * APotRatio;
    if(amount % player_action_info_.unit() != 0) {
        amount = amount / player_action_info_.unit() * player_action_info_.unit() + player_action_info_.unit();
    }
    amount += GetRoom()->get_bet_max();
    
    if(amount > max_chip || amount < min_chip)
        amount = 0;
    
    return amount;
}

uint32_t ChipPoolNode::getAllinSize()
{
    uint32_t min_chip = player_action_info_.betmin();
    uint32_t max_chip = player_action_info_.betmax();
    auto user = static_pointer_cast<user_texas>(GetRoom()->get_user(GDM->get_user_id()));
    int amount = user->get_properties()->chips() + user->get_properties()->betchips();
    if(amount > max_chip || amount < min_chip)
        amount = 0;
    
    return amount;
}

void ChipPoolNode::click_image_two_third(Ref* sender)
{
    amount_ = getBetSizeByPot(0.6666666667f);
    
    if (pokerRoomLayer) {
        pokerRoomLayer->click_btn_add(nullptr);
    }
}

void ChipPoolNode::click_image_half_pool(Ref* sender)
{
    amount_ = getBetSizeByPot(0.5f);
    
    if (pokerRoomLayer) {
        pokerRoomLayer->click_btn_add(nullptr);
    }
}

void ChipPoolNode::click_image_full_pool(Ref* sender)
{
    amount_ = getBetSizeByPot(1);
    
    if (pokerRoomLayer) {
        pokerRoomLayer->click_btn_add(nullptr);
    }
}


void ChipPoolNode::update_data(const msg::PokerActionRequest & data)
{
    player_action_info_.CopyFrom(data);
    if (player_action_info_.actionchoice() == msg::ActionChoice::FOLD_CHECK_BET)
    {
        slider_text->setString(tools::local_string("bet_free", "自由下注"));
    }else
    {
        slider_text->setString(tools::local_string("raise_free", "自由加注"));
    }
    amount_ = data.betmin();
    text_chip_number_->setString(tools::to_string(amount_));
    update_Chip_Amount();
}

void ChipPoolNode::update_Chip_Amount()
{
    uint32_t half_pool_amount = getBetSizeByPot(0.5f);
    if (half_pool_amount == 0) {
        button_half_pool->setVisible(false);
    }
    else
    {
        button_half_pool->setVisible(true);
        text_half_pool_amount->setString(tools::to_string(half_pool_amount));
    }
    
    uint32_t two_third_pool_amount = getBetSizeByPot(0.6666666667f);
    if (two_third_pool_amount == 0) {
        button_two_third_pool->setVisible(false);
    }
    else
    {
        button_two_third_pool->setVisible(true);
        text_two_third_pool_amount->setString(tools::to_string(two_third_pool_amount));
    }
    
    uint32_t full_pool_amount = getBetSizeByPot(1);
    if (full_pool_amount == 0) {
        button_full_pool->setVisible(false);
    }
    else
    {
        button_full_pool->setVisible(true);
        text_full_pool_amount->setString(tools::to_string(full_pool_amount));
    }
    
    uint32_t allin_amount = getAllinSize();
    if (allin_amount == 0) {
        button_all_in->setVisible(false);
    }
    else
    {
        button_all_in->setVisible(true);
        text_all_in_amount->setString(tools::to_string(allin_amount));
    }
}
const uint32_t ChipPoolNode::get_amount()
{
    return amount_;
}

void ChipPoolNode::setVisible(bool vis)
{
    Node::setVisible(vis);
//    button_half_pool->setVisible(vis);
//    button_two_third_pool->setVisible(vis);
//    button_full_pool->setVisible(vis);
    button_all_in->setVisible(vis);
    
    slider_chip_->setPercent(0);
}