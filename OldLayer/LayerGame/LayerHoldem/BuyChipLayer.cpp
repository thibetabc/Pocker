#include "BuyChipLayer.h"
#include "utils.h"
#include "PokerDataManager.h"
#include "Room.h"
#include "GameDataManager.h"
#include "user_data.h"
#include "PokerRoomLayer.h"
#include "TipView.h"
#include "ProcessorManager.h"

bool BuyChipLayer::init()
{
    if(!Layer::init()) {
        return false;
    }
    LayerColor *layer = LayerColor::create(Color4B(100,100,100,100));
    this->addChild(layer);
    this->setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    this->setTouchEnabled(true);
    this->setSwallowsTouches(true);
    
    auto& size = Director::getInstance()->getWinSize();
    auto layout = ui::Layout::create();
    layout->setContentSize(size);
    layout->setTouchEnabled(true);
    layout->setAnchorPoint(Point::ZERO);
    this->addChild(layout, -10);
    
    auto root = CSLoader::createNode("LayerBuyChip1.csb");
    this->addChild(root);
    Size winSize = Director::getInstance()->getWinSize();
    LayerColor *layerBlack = LayerColor::create(Color4B(0, 0, 0, 100),winSize.width,winSize.height * 2);
    layerBlack->setPositionY(-winSize.height);
    this->addChild(layerBlack,-1);
    auto text_max_add = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_max_add"));
    auto text_min_add = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_min_add"));
    text_max_add->setString(tools::local_string("max_buy", "最多可添加"));
    text_min_add->setString(tools::local_string("min_buy", "最少可添加"));
    
    text_min_number_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_min_number"));
    text_max_number_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_max_number"));
    text_buy_number_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_buy_number"));
    
    auto btn_cancel = dynamic_cast<Button *>(CSLoader::seekNodeByName(root, "btn_cancel"));
    auto btn_confirm = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "btn_confirm"));
    btn_cancel->addClickEventListener(std::bind(&BuyChipLayer::click_btn_cancel, this, std::placeholders::_1));
    btn_confirm->addClickEventListener(std::bind(&BuyChipLayer::click_btn_confirm, this, std::placeholders::_1));
    
    auto btn_add = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "btn_add"));
    auto btn_sub = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "btn_sub"));
    
    btn_add->addClickEventListener(std::bind(&BuyChipLayer::click_btn_add, this, std::placeholders::_1));
    btn_sub->addClickEventListener(std::bind(&BuyChipLayer::click_btn_sub, this, std::placeholders::_1));
    
    slider_chip_ = dynamic_cast<Slider*>(CSLoader::seekNodeByName(root, "slider_chip"));
    slider_chip_->addEventListener(std::bind(&BuyChipLayer::slider_slider_chip, this, std::placeholders::_1, std::placeholders::_2));
    
    auto processor = PM->GetUserPermission_up(GetRoom()->get_group_id(), GDM->get_user_id());
    send_data(processor,REQ_GET_PERMISSION);
    return true;
}

void BuyChipLayer::register_events()
{
    dispatch_.register_event(REQ_GET_PERMISSION,
                             BIND_FUNC(this, BuyChipLayer::handle_get_permission));
}

void BuyChipLayer::handle_get_permission(cmd_data_pointer data)
{
    std::shared_ptr<net_data_recv_package> rec = dynamic_pointer_cast<net_data_recv_package>(data);
    if (!check_request(rec)) return;
    auto processor = PM->GetUserPermission_down(rec);
    permission_.CopyFrom(processor);
}

bool BuyChipLayer::onTouchBegan(Touch *touch, Event *unused_event)
{
    removeFromParent();
    return true;
}

void BuyChipLayer::onTouchMoved(Touch *touch, Event *unused_event)
{
    
}
void BuyChipLayer::onTouchEnded(Touch *touch, Event *unused_event)
{
    
}

void BuyChipLayer::click_btn_cancel(Ref* sender)
{
    removeFromParent();
}
void BuyChipLayer::click_btn_confirm(Ref* sender)
{
    //    int amount = PDM->get_room()->get_buyin_max() - PDM->get_room()->get_buyin_min();
    //    int buy_count = PDM->get_room()->get_buyin_min() + amount * slider_chip_->getPercent() / 100.0f;
    PDM->send_poker_add_chips(amount_, GetRoom()->get_room_id());
    
    auto user = static_pointer_cast<user_texas>(GetRoom()->get_user( GDM->get_user_id()));
    if(user->get_status() == msg::SittingStatus::PLAYING)
    {
        PokerRoomLayer * prl = dynamic_cast<PokerRoomLayer *>(getParent()->getParent());
        if(prl&&prl->get_is_playing())
        {
            TipView::showAlertView("这手牌打完后会给筹码。");
        }
        
    }
    removeFromParent();
}

void BuyChipLayer::Refresh()
{
    auto user = static_pointer_cast<user_texas>(GetRoom()->get_user( GDM->get_user_id()));
    if(user) {
        int buyin_min = GetRoom()->get_buyin_min();
        int buyin_max = GetRoom()->get_buyin_max();
        
        bool need_credit = permission_.needcredit();
        if (need_credit)
        {
            uint32_t credit = permission_.credit();
            if (credit < buyin_max)
                buyin_max = credit;
        }
        
        int min_chip = buyin_min - user->get_chip_amount();
        int max_chip = buyin_max - user->get_chip_amount();
        min_chip = std::max(0, min_chip);
        
        text_min_number_->setString(tools::to_string(min_chip));
        text_max_number_->setString(tools::to_string(max_chip));
        text_buy_number_->setString(tools::to_string(min_chip));
    }
    slider_slider_chip(slider_chip_, Slider::EventType::ON_PERCENTAGE_CHANGED);
}

void BuyChipLayer::slider_slider_chip(Ref* sender, Slider::EventType event)
{
    
    auto user = static_pointer_cast<user_texas>(GetRoom()->get_user(GDM->get_user_id()));
    //    int min = PDM->get_room()->get_buyin_min() - user->get_chip_amount();
    //    int max = PDM->get_room()->get_buyin_max() - user->get_chip_amount();
    //    int amount = max - min;
    //    int buy_count = min + amount * slider_chip_->getPercent() / 100.0f;
    //    text_buy_number_->setString(tools::to_string(buy_count));
    
    int buyin_min = GetRoom()->get_buyin_min();
    int buyin_max = GetRoom()->get_buyin_max();
    
    bool need_credit = permission_.needcredit();
    if (need_credit)
    {
        uint32_t credit = permission_.credit();
        if (credit < buyin_max)
            buyin_max = credit;
    }
    
    int min_chip = buyin_min - user->get_chip_amount();
    int max_chip = buyin_max - user->get_chip_amount();
    
    
    if (min_chip < 0) {
        min_chip = 0;
    }
    
    int count = (max_chip - min_chip) * slider_chip_->getPercent() / 100.0f + min_chip;
    
    if(count % GetRoom()->get_buy_min_unit() != 0) {
        count = count / GetRoom()->get_buy_min_unit() * GetRoom()->get_buy_min_unit() + GetRoom()->get_buy_min_unit();
    }
    
    if(count <= min_chip) {
        count = min_chip;
    }
    if(count >= max_chip) {
        count = max_chip;
    }
    amount_ = count;
    text_buy_number_->setString(tools::to_string(count));
    //cocos2d::log("BuyChipLayer:slider_chip min:%d,max:%d,count:%d", min_chip, max_chip, count);
}

void BuyChipLayer::click_btn_add(Ref* sender)
{
    //    slider_chip_->setPercent(slider_chip_->getPercent());
    //    slider_slider_chip(nullptr, Slider::EventType::ON_PERCENTAGE_CHANGED);
    
    auto user = static_pointer_cast<user_texas>( GetRoom()->get_user(GDM->get_user_id()));
    
    int buyin_min = GetRoom()->get_buyin_min();
    int buyin_max = GetRoom()->get_buyin_max();
    
    bool need_credit = permission_.needcredit();
    if (need_credit)
    {
        uint32_t credit = permission_.credit();
        if (credit < buyin_max)
            buyin_max = credit;
    }
    
    int min_chip = buyin_min - user->get_chip_amount();
    int max_chip = buyin_max - user->get_chip_amount();
    
    uint32_t unit = GetRoom()->get_buy_min_unit();
    uint32_t sub = max_chip - std::max(0, min_chip);
    
    int32_t value = amount_ + unit;
    if(value >= min_chip && value <= max_chip) {
        float percent = (value - std::max(0, min_chip)) * 100.0f / sub;
        slider_chip_->setPercent(percent);
        amount_ = value;
        text_buy_number_->setString(tools::to_string(amount_));
    }
}
void BuyChipLayer::click_btn_sub(Ref* sender)
{
    if(amount_ <= 0){
        return;
    }
    auto user = static_pointer_cast<user_texas>( GetRoom()->get_user(GDM->get_user_id()));
    
    int buyin_min = GetRoom()->get_buyin_min();
    int buyin_max = GetRoom()->get_buyin_max();
    
    bool need_credit = permission_.needcredit();
    if (need_credit)
    {
        uint32_t credit = permission_.credit();
        if (credit < buyin_max)
            buyin_max = credit;
    }
    
    int min_chip = buyin_min - user->get_chip_amount();
    int max_chip = buyin_max - user->get_chip_amount();
    
    uint32_t unit = GetRoom()->get_buy_min_unit();
    
    uint32_t sub = max_chip - std::max(min_chip, 0);
    int32_t value = amount_ - unit;
    if(value >= min_chip && value <= max_chip) {
        float percent = (value - std::max(0, min_chip)) * 100.0f / sub;
        slider_chip_->setPercent(percent);
        amount_ = value;
        text_buy_number_->setString(tools::to_string(amount_));
    }
}