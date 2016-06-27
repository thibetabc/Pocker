#include "PlayerInfoNode.h"
#include "user_data.h"
#include "PokerDataManager.h"
#include "game_net_data.h"
#include "cmd_def.h"
#include "utils.h"
#include "GameTools.h"
#include "GameDataManager.h"
#include "FileManager.h"
#include "AudioManager.h"
#include "Room.h"
#include "TipView.h"

bool PlayerInfoNode::init()
{
    if (!Node::init()) {
        return false;
    }
    root_ = nullptr;
    progerss_ = nullptr;
    sprite_icon_ = nullptr;

    
    recreate();
    
    return true;
}

void PlayerInfoNode::download_head_image_finished(cmd_data_pointer)
{
    update_data(user_);
}

void PlayerInfoNode::update_data(const std::shared_ptr<user_texas>& user)
{
    log("user_head_image:%s", user->get_head_pic_name().c_str());
    user_ = user;
    //image_head_->setVisible(true);
    //text_nickname_->setVisible(true);
    sprite_name_bg_->setVisible(true);
    sprite_chip_bg_->setVisible(true);
    sprite_chip_->setVisible(true);
    text_chip_amount_->setVisible(true);
    text_status_bg_->setVisible(true);
    text_status_->setVisible(true);
    
    text_chip_amount_->setString(tools::to_string(user_->get_chip_amount()));
    text_chip_amount_->setFontSize(20);
    sprite_chip_->setTexture(GameTools::get_chip_image_name(user_->get_chip_amount()));
    
    //image_gray_->setOpacity(255);
    image_head_->loadTexture(user->get_head_pic_name());
    
    if(!user->get_head_pic_name().empty())
    {
        image_head_->setVisible(false);
        
        if(GDM->is_file_exist(user->get_head_pic_name()))
        {
            if (sprite_icon_ && sprite_icon_->getParent())
            {
                sprite_icon_->removeFromParent();
                sprite_icon_ = nullptr;
            }
            
            
            //ShaderSprite * tmp_sprite_icon_ = ShaderSprite::create(user->get_head_pic_name(), "login_res/game_res/avatar_mask_circle.png");
            
            sprite_icon_ = ShaderSprite::create(user->get_head_pic_name(), "login_res/game_res/avatar_mask_circle.png");
            //sprite_icon_ = Sprite::createWithTexture(tmp_sprite_icon_->getTexture());
            
            
            sprite_icon_->setScale(image_head_->getContentSize().width / sprite_icon_->getContentSize().width,
                                   image_head_->getContentSize().height / sprite_icon_->getContentSize().height);
            sprite_icon_->setPosition(image_head_->getPosition());
            root_->addChild(sprite_icon_, image_head_->getZOrder());
        }
        else {
            FM.download_file(user->get_head_pic_name());
            
            dispatch_.register_event(EVENT_DOWLOAD_FILE,
                                     BIND_FUNC(this, PlayerInfoNode::download_head_image_finished));
        }
        
    }
    text_nickname_->setString(user->get_nickname());
    //sprite_box_->setTexture("login_res/game_res/lv_05.png");

    
    std::string card_type = user->get_properties()->hihandtype();
    std::string low_card_type =  user->get_properties()->lohandtype();
    card_type = tools::trim(card_type);
    low_card_type = tools::trim(low_card_type);
    std::string result;
    if(!card_type.empty() && !low_card_type.empty()) {
        result = card_type + "/" + low_card_type;
    }
    if (result.empty()) {
        result = card_type.empty() ? low_card_type : card_type;
    }
    
    if (user->get_properties()->actionstatus() ==  msg::ActionStatus::FOLD)
        result = "";
    
    show_card_type(result);
    
    switch (user->get_status()) {
        case msg::SittingStatus::PLAYING:
            set_description("");
            break;
        case msg::SittingStatus::OBSERVER:
            set_description("站起");
            break;
        case msg::SittingStatus::SITOUT:
            set_description("暂离");
            break;
        case msg::SittingStatus::SITDOWN:
            set_description("等待");
            break;
        default:
            set_description("");
            break;
    }
    image_gray_->setVisible(false);
    

//    if ((GetRoom()->get_owner_id() == GDM->get_user_id())&&(user_->get_id() != GDM->get_user_id()))
//    {
//        btn_kick_->setVisible(true);
//    }
//    else
//    {
//        btn_kick_->setVisible(false);
//    }
    
    
}

void PlayerInfoNode::setWholeOpacity(int opacity){
    this->setCascadeOpacityEnabled(true);
    this->setOpacity(opacity);
    
    
    /*
    image_head_->setOpacity(opacity);
    text_nickname_->setOpacity(opacity);
    sprite_chip_->setOpacity(opacity);
    text_chip_amount_->setOpacity(opacity);
    text_status_bg_->setOpacity(opacity);
    text_status_->setOpacity(opacity);
    image_player_bg_->setOpacity(opacity);
    image_gray_->setOpacity(opacity);
    sprite_box_->setOpacity(opacity);
    if ( player_info_last)
        player_info_last->setOpacity(opacity);
     if(sprite_icon_) {
         sprite_icon_->setOpacity(opacity);
     }*/
    
}

void PlayerInfoNode::update_data()
{
    image_head_->setVisible(false);
    //text_nickname_->setVisible(false);
    sprite_name_bg_->setVisible(false);
    sprite_chip_bg_->setVisible(false);
    sprite_chip_->setVisible(false);
    text_chip_amount_->setVisible(false);
    text_status_bg_->setVisible(false);
    text_status_->setVisible(false);
    sprite_dealer_->setVisible(false);
    if(player_info_last)
    {
        player_info_last->setVisible(false);
    }
    
    image_gray_->setOpacity(100);  // from 0->255 by ronaldz.
    auto &poker_room_data = this->GetRoom();
    if (poker_room_data == nullptr)
    {
        cocos2d::log("PlayerInfoNode::update_data poker_room_data == nullptr");
        return;
    }
    if (!poker_room_data->get_is_record() && !poker_room_data->get_is_tournament()){
//        if(this->getPosition().x < 375) {   //左边
//            image_gray_->loadTexture("login_res/game_res/left_empty_seat.png");  // from sprite_box to image_gray . this is a big bug.
//        }
//        else {
//            image_gray_->loadTexture("login_res/game_res/right_empty_seat.png"); // from sprite_box to image_gray . this is a big bug.
//        }
        image_gray_->setVisible(true);
        image_gray_->setTouchEnabled(true);
        //image_player_bg_->setTouchEnabled(true);
        //sprite_box_->setVisible(true);
        sprite_box_->setVisible(false);
        
    }
    else
    {
        image_gray_->setOpacity(0);  // from 0->255 by ronaldz.  // make it invisible.
        image_gray_->setTouchEnabled(false);
        //image_player_bg_->setTouchEnabled(false);
        sprite_box_->setVisible(false);
    }
    
    if(sprite_icon_) {
        sprite_icon_->removeFromParent();
        sprite_icon_ = nullptr;
    }
    
    btn_kick_->setVisible(false);
    
    setWholeOpacity(255);
}

void PlayerInfoNode::click_image_box(Ref* sender)
{
    auto &poker_room_data = this->GetRoom();
    if (poker_room_data == nullptr)
    {
        cocos2d::log("PlayerInfoNode::click_image_box poker_room_data == nullptr");
        return;
    }
    uint32_t roomid = poker_room_data->get_room_id();
        bool is_tournament = poker_room_data->get_is_tournament();
    if (user_ != nullptr)
    {
        if ((poker_room_data->get_owner_id() == GDM->get_user_id()) &&
            (user_->get_id() != GDM->get_user_id()) &&
            !is_tournament)
        {
            btn_kick_->setVisible(true);
        }
        else
        {
            btn_kick_->setVisible(false);
        }
    }else
    {
        btn_kick_->setVisible(false);
    }
    
    if (!poker_room_data->get_is_record()){
        auto& users = poker_room_data->get_users();
        for (auto& iter : users) {
            if(iter.second->get_seat_num() == seat_id_) {
                return;
            }
        }
        PDM->send_seat_info(roomid, msg::SittingStatus::SITDOWN, seat_id_);
    }
}

void PlayerInfoNode::set_dealer(const bool dealer)
{
    if (dealer) {
        if(user_.get()&&(user_->get_status() == msg::SittingStatus::PLAYING))
        {
            sprite_dealer_->setVisible(dealer);
        }
    }
    else
    {
        sprite_dealer_->setVisible(dealer);
    }

}

void PlayerInfoNode::show_progress_box(const float time, const float pastTime, bool is_using_timebank)
{
    if (time <= 0) {
        return;
    }
    
    if (is_using_timebank)
    {
        sprite_box_->setTexture("login_res/game_res/player_timer_timebank.png");
    }else
    {
        sprite_box_->setTexture("login_res/game_res/player_timer.png");
    }
    
    sprite_box_->setVisible(false);
    //image_box_bg_->setVisible(true);
    
    auto progress = ProgressTimer::create(sprite_box_);
    progress->setType(cocos2d::ProgressTimer::Type::RADIAL);
    progress->setMidpoint(Vec2(0.5, 0.5));
    if (pastTime == 0) {
        progress->setPercentage(100);
    }
    else{
        progress->setPercentage(100 * ( 1- pastTime/time));
    }
    
    progress->setScaleX(-1);
    //progress->setAnchorPoint(Point::ZERO);
    progress->setPosition(sprite_box_->getPosition());
    root_->addChild(progress);
    
    float progressTime = time - pastTime;
    auto action = ProgressTo::create(progressTime, 0);
    
    Sequence* seq = Sequence::create(action, CallFunc::create([=]()
    {
        //sprite_box_->setVisible(true);
        //image_box_bg_->setVisible(false);
        progress->removeFromParent();
        progerss_ = nullptr;
        
        sprite_timer_fire->stopAllActions();
        sprite_timer_fire->setVisible(false);
        sprite_timer_fire->setPosition(sprite_timer_fire_pos);
        
//        if(giveup_callback_ && isMySelf)
//        {
//            giveup_callback_();
//        }
        
    }), NULL);
    
    progress->runAction(seq);
    progerss_ = progress;
    
    Animation* animation = Animation::create();
    animation->setDelayPerUnit(0.06f);
    
    for (int i = 1; i <= 4; i++) {
        std::string filename = "player_timer_fire_" + tools::to_string(i) + ".png";
        //animation->addSpriteFrameWithFile(filename);
        SpriteFrame * frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(filename);
        animation->addSpriteFrame(frame);
    }
    Animate* animate = Animate::create(animation);
    sprite_timer_fire->runAction(RepeatForever::create(animate));
    sprite_timer_fire->setVisible(true);
    
    //by lvzhangle
    //Here I have write a CircleBy action in CCActionInterval.h
    auto fire_action = CircleBy::create(progressTime, image_box_bg_->getPosition(), image_box_bg_->getContentSize().width / 2, 100.0 - progerss_->getPercentage(), 100.0);
    
    if (fire_action)
        sprite_timer_fire->runAction(fire_action);
    
//    Size size = sprite_box_->getContentSize();
//    size = Size(size.width - 4, size.height - 5);
//    float r = 20;
//    
//    auto move1 = MoveTo::create(time*0.09f, sprite_timer_fire_pos + Point(size.width*0.5f-r,0));
//    
//    ccBezierConfig bezier1;
//    bezier1.controlPoint_1 = sprite_timer_fire_pos + Point(size.width*0.5f,0);
//    bezier1.controlPoint_2 = sprite_timer_fire_pos + Point(size.width*0.5f,0);
//    bezier1.endPosition = sprite_timer_fire_pos + Point(size.width*0.5f,-r);
//    auto bezierTo1 = BezierTo::create(time*0.07f, bezier1);
//    
//    auto move2 = MoveTo::create(time*0.18f, sprite_timer_fire_pos + Point(size.width*0.5f,-(size.height-r)));
//    
//    ccBezierConfig bezier2;
//    bezier2.controlPoint_1 = sprite_timer_fire_pos + Point(size.width*0.5f,-size.height);
//    bezier2.controlPoint_2 = sprite_timer_fire_pos + Point(size.width*0.5f,-size.height);
//    bezier2.endPosition = sprite_timer_fire_pos + Point(size.width*0.5f-r,-size.height);
//    auto bezierTo2 = BezierTo::create(time*0.07f, bezier2);
//    
//    auto move3 = MoveTo::create(time*0.18f, sprite_timer_fire_pos + Point(-(size.width*0.5f-r),-size.height));
//    
//    ccBezierConfig bezier3;
//    bezier3.controlPoint_1 = sprite_timer_fire_pos + Point(-size.width*0.5f,-size.height);
//    bezier3.controlPoint_2 = sprite_timer_fire_pos + Point(-size.width*0.5f,-size.height);
//    bezier3.endPosition = sprite_timer_fire_pos + Point(-size.width*0.5f,-(size.height-r));
//    auto bezierTo3 = BezierTo::create(time*0.07f, bezier3);
//    
//    auto move4 = MoveTo::create(time*0.18f, sprite_timer_fire_pos + Point(-size.width*0.5f,-r));
//    
//    ccBezierConfig bezier4;
//    bezier4.controlPoint_1 = sprite_timer_fire_pos + Point(-size.width*0.5f,0);
//    bezier4.controlPoint_2 = sprite_timer_fire_pos + Point(-size.width*0.5f,0);
//    bezier4.endPosition = sprite_timer_fire_pos + Point(-(size.width*0.5f-r),0);
//    auto bezierTo4 = BezierTo::create(time*0.07f, bezier4);
//    
//    auto move5 = MoveTo::create(time*0.09f, sprite_timer_fire_pos);
//    
//    
//    Sequence * path = nullptr;
//    
//    path = Sequence::create(move1,bezierTo1,move2, bezierTo2,move3,bezierTo3,move4,bezierTo4,move5,  NULL);
//    
//    
//    if (pastTime == 0) {
//        sprite_timer_fire->runAction(path);
//    }
//    else
//    {
//        float curTime = pastTime;
//        
//        float t0 = 0;
//        float t1 = t0 + time * 0.125f;
//        float t2 = t1 + time * 0.25f;
//        float t3 = t2 + time * 0.25f;
//        float t4 = t3 + time * 0.25f;
//        float t5 = time;
//        
//        Vec2 startPoint = sprite_timer_fire_pos;
//        Vec2 curPoint;
//        if ( t0 < curTime && curTime < t1) {
//            curPoint = startPoint + Point(size.width*0.5f * ((curTime - t0)/(t1 - t0)),0);
//            if (curPoint.x < sprite_timer_fire_pos.x + size.width*0.5f-r) {
//                move1 = MoveTo::create(time*0.09f - curTime, sprite_timer_fire_pos + Point(size.width*0.5f-r,0));
//                
//                path = Sequence::create(move1,bezierTo1,move2, bezierTo2,move3,bezierTo3,move4,bezierTo4,move5,  NULL);
//            }
//            else{
//                path = Sequence::create(bezierTo1,move2, bezierTo2,move3,bezierTo3,move4,bezierTo4,move5,  NULL);
//            }
//            
//            
//        }
//        else if ( t1 < curTime && curTime < t2) {
//            startPoint = sprite_timer_fire_pos + Point(size.width*0.5f,0);
//            curPoint = startPoint + Point(0,-size.height * ((curTime - t1)/(t2 - t1)));
//            
//            if (curPoint.y > sprite_timer_fire_pos.y -(size.height-r)) {
//                
//                move2 = MoveTo::create(time*0.18f- (curTime - t1) + time*0.07f*0.5f, sprite_timer_fire_pos + Point(size.width*0.5f,-(size.height-r)));
//                
//                
//                path = Sequence::create(move2, bezierTo2,move3,bezierTo3,move4,bezierTo4,move5,  NULL);
//            }
//            else{
//                path = Sequence::create(bezierTo2,move3,bezierTo3,move4,bezierTo4,move5,  NULL);
//            }
//            
//            
//        }
//        else if ( t2 < curTime && curTime < t3) {
//            startPoint = sprite_timer_fire_pos + Point(size.width*0.5f,-size.height);
//            curPoint = startPoint + Point(-size.width * ((curTime - t2)/(t3 - t2)),0);
//            
//            if (curPoint.x > sprite_timer_fire_pos.x -(size.width*0.5f-r)) {
//                
//                move3 = MoveTo::create(time*0.18f- (curTime - t2) + time*0.07f*0.5f, sprite_timer_fire_pos + Point(-(size.width*0.5f-r),-size.height));
//                
//                
//                path = Sequence::create(move3,bezierTo3,move4,bezierTo4,move5,  NULL);
//            }
//            else{
//                path = Sequence::create(bezierTo3,move4,bezierTo4,move5,  NULL);
//            }
//            
//        }
//        else if ( t3 < curTime && curTime < t4) {
//            startPoint = sprite_timer_fire_pos + Point(-size.width*0.5f,-size.height);
//            curPoint = startPoint + Point(0,size.width * ((curTime - t3)/(t4 - t3)));
//            
//            if (curPoint.y < sprite_timer_fire_pos.y -r) {
//                
//                
//                move4 = MoveTo::create(time*0.18f- (curTime - t3) + time*0.07f*0.5f, sprite_timer_fire_pos + Point(-size.width*0.5f,-r));
//                
//                path = Sequence::create(move4,bezierTo4,move5,  NULL);
//            }
//            else{
//                path = Sequence::create(bezierTo4,move5,  NULL);
//            }
//            
//            
//        }
//        else if ( t4 < curTime && curTime < t5) {
//            startPoint = sprite_timer_fire_pos + Point(-size.width*0.5f,0);
//            curPoint = startPoint + Point(size.width*0.5f * ((curTime - t4)/(t5 - t4)),0);
//            
//            move5 = MoveTo::create(time*0.09f - curTime - t4 + time*0.07f*0.5f, sprite_timer_fire_pos);
//            path = Sequence::create(move5,  NULL);
//        }
//        sprite_timer_fire->setPosition(curPoint);
//        sprite_timer_fire->runAction(path);
//        
//    }
}

void PlayerInfoNode::show_green_box(const bool visible)
{
    sprite_box_->setVisible(visible);  // modified by ronaldz, from true to visible.
    if(progerss_) {
        progerss_->stopAllActions();
        progerss_->removeFromParent();
        progerss_ = nullptr;
        sprite_timer_fire->stopAllActions();
        sprite_timer_fire->setVisible(false);
        sprite_timer_fire->setPosition(sprite_timer_fire_pos);
        
        //image_box_bg_->setVisible(false);
    }
}
void PlayerInfoNode::hideWinAnimation()
{
    if (sprite_winner_) {
        sprite_winner_->stopAllActions();
        sprite_winner_->setVisible(false);
    }
}
void PlayerInfoNode::set_description(const std::string& description)
{
    
    log("status:%s",description.c_str());
    if (description.empty() && user_ != nullptr)
    {
        if(user_->get_status() == msg::PLAYING)
        {
            
            text_status_->setString(description);
        }
    }
    else
    {
        text_status_->setString(description);
    }
    
   
    
    if (getPositionY() <200)
    { // ronaldz. 最下面的需要吧夹住这些内容上移一下。
       player_info_fold->setPositionY(70+player_info_fold->getContentSize().height*1.5);
        player_info_check->setPositionY(70+player_info_check->getContentSize().height*1.5);
        player_info_bet->setPositionY(70+player_info_bet->getContentSize().height*1.5);
        player_info_raise->setPositionY(70+player_info_raise->getContentSize().height*1.5);
        player_info_allin->setPositionY(70+player_info_allin->getContentSize().height*1.5);
    }
    
    if (getPositionY() > 1100) {
        player_info_fold->setPosition(-136, 70);
        player_info_check->setPosition(-136, 70);
        player_info_bet->setPosition(-136, 70);
        player_info_raise->setPosition(-136, 70);
       
            player_info_allin->setPosition(-136, 70);
    }
    
    if(getPositionY() > -1)// 原来是200， 这个是有很大问题的， 也就是说谁坐在最下面谁就不变灰。 这是不对的。 我改成－1. 也就是， 无论谁都要改。  ronaldz 20160220
    {
        if (player_info_last) {
            player_info_last->setVisible(false);
        }
        
        if (description == "弃牌") {
            if (getPositionX() >= 375) {
                player_info_fold->setTexture("login_res/game_res/player_info_fold_1.png");
            }
            
            player_info_fold->setVisible(false);
            player_info_last = player_info_fold;
            this->setWholeOpacity(100);
            

        }
        else if (description == "让牌")
        {
            if (getPositionX() >= 375) {
                player_info_check->setTexture("login_res/game_res/player_info_check_1.png");
            }
            
            player_info_check->setVisible(true);
            player_info_last = player_info_check;
            this->setWholeOpacity(255);

        }
        else if (description == "下注")
        {
            if (getPositionX() >= 375) {
                player_info_bet->setTexture("login_res/game_res/player_info_bet_1.png");
            }
            player_info_bet->setVisible(true);
            player_info_last = player_info_bet;
            this->setWholeOpacity(255);

        }
        else if (description == "加注")
        {
            if (getPositionX() >= 375) {
                player_info_raise->setTexture("login_res/game_res/player_info_raise_1.png");
            }
           
            player_info_raise->setVisible(true);
            player_info_last = player_info_raise;
            this->setWholeOpacity(255);

        }
        else if (description == "全下")
        {
            if (getPositionX() >= 375) {
                player_info_allin->setTexture("login_res/game_res/player_info_allin_1.png");
            }
            
            player_info_allin->setVisible(true);
            player_info_last = player_info_allin;
            this->setWholeOpacity(255);

        }
        else if (description == "暂离" || description =="等待")
        {
            if (sprite_icon_) {
                sprite_icon_->setColor(Color3B::GRAY);
            }
            this->setWholeOpacity(100);
        }else if (description =="" || description =="站起")
        {
            this->setWholeOpacity(255);
        }

    }
    
    if (description.empty()) {
        text_status_bg_->setVisible(false);
        this->setWholeOpacity(255);
    }
    else
    {
        text_status_bg_->setVisible(true);
    }
    
    
}

void PlayerInfoNode::show_win_amount(const uint32_t amount)
{
//    AM->playEffect("sound/oh.mp3");
    sprite_winner_->setVisible(true);
    
    Animation* animation = Animation::create();
    animation->setDelayPerUnit(0.1f);
    for (int i = 1; i <= 8; i++) {
        std::string filename = "player_win_" + tools::to_string(i) + ".png";
        animation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(filename));
    }
    Animate* animate = Animate::create(animation);
    sprite_winner_->runAction(RepeatForever::create(animate));
    
    text_win_type_bg_->setVisible(true);
    text_win_type_->setVisible(true);
    text_win_amount_->setVisible(true);
    FiniteTimeAction * mb = MoveBy::create(1, Vec2(0,160));
    FiniteTimeAction * fo = FadeOut::create(1);
//    Action * action = Spawn::create(mb,fo, NULL);
//    Sequence::create(action, CCCallFunc::create(<#const std::function<void ()> &func#>),NULL)
    text_win_amount_->runAction(Sequence::create(mb,fo,NULL));
    
    
    text_win_amount_->setPosition(Point(text_chip_amount_->getPosition().x + (1 - text_chip_amount_->getAnchorPoint().x) * text_chip_amount_->getContentSize().width, text_chip_amount_->getPosition().y));
    text_win_amount_->setString("+" + tools::to_string(amount));
}

void PlayerInfoNode::show_card_type(const std::string& card_type)
{
    if (card_type.empty()) {
        text_win_type_bg_->setVisible(false);
    }
    else
    {
        text_win_type_bg_->setVisible(true);
    }
    
    text_win_type_->setVisible(true);
    text_win_type_->setString(card_type);
    
    cocos2d::log("PlayerInfoNode::show_card_type %s", card_type.c_str());
}

void PlayerInfoNode::clear()
{
    sprite_winner_->stopAllActions();
    sprite_winner_->setVisible(false);
    text_win_type_bg_->setVisible(false);
    text_win_type_->setVisible(false);
    text_win_amount_->setVisible(false);
    text_win_amount_->setOpacity(255);
//    text_win_amount_->setPositionY(113);
    sprite_box_->setVisible(false);
    sprite_dealer_->setVisible(false);
    sprite_timer_fire->stopAllActions();
    sprite_timer_fire->setVisible(false);
    sprite_timer_fire->setPosition(sprite_timer_fire_pos);
    if (sprite_icon_) {
        sprite_icon_->setColor(Color3B::WHITE);
    }
    if(player_info_last)
    {
        player_info_last->setVisible(false);
    }

    btn_kick_->setVisible(false);
    
}
void PlayerInfoNode::recreate()
{
    if(root_) {
        root_->removeFromParent();
        root_ = nullptr;
        sprite_icon_ = nullptr;
    }
    auto root = CSLoader::createNode("NodePlayerInfo.csb");
    this->addChild(root);
    //image_player_bg_ = dynamic_cast<ImageView* >(CSLoader::seekNodeByName(root, "image_player_bg"));
    //image_player_bg_->addClickEventListener(std::bind(&PlayerInfoNode::click_image_box, this, std::placeholders::_1));
    
    image_gray_ = dynamic_cast<ImageView* >(CSLoader::seekNodeByName(root, "image_gray"));
    image_gray_->addClickEventListener(std::bind(&PlayerInfoNode::click_image_box, this, std::placeholders::_1));
    image_gray_->setLocalZOrder(200);
    //    image_gray_->setOpacity(0);
    
    sprite_name_bg_ = dynamic_cast<Sprite* >(CSLoader::seekNodeByName(root, "sprite_name_bg"));
    sprite_chip_bg_ = dynamic_cast<Sprite* >(CSLoader::seekNodeByName(root, "sprite_chip_bg"));
    
    sprite_name_bg_->setVisible(false);
    sprite_chip_bg_->setVisible(false);
    
    image_head_ = dynamic_cast<ImageView* >(CSLoader::seekNodeByName(root, "image_head"));
    sprite_chip_ = dynamic_cast<Sprite* >(CSLoader::seekNodeByName(root, "sprite_chip"));
    text_nickname_ = dynamic_cast<Text* >(CSLoader::seekNodeByName(root, "text_nickname"));
    text_status_bg_ = dynamic_cast<Sprite* >(CSLoader::seekNodeByName(root, "text_status_bg"));
    text_status_bg_->setLocalZOrder(1);
    text_status_bg_->setVisible(false);
    
    text_status_ = dynamic_cast<Text* >(CSLoader::seekNodeByName(root, "text_status"));
    text_status_->setString("");
    text_status_->setLocalZOrder(1);
    
    text_chip_amount_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_chip_amount"));
    
    image_box_bg_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root, "image_box_bg"));
    //image_box_bg_->setVisible(false);
    image_box_bg_->addClickEventListener(std::bind(&PlayerInfoNode::click_image_box, this, std::placeholders::_1));
    
    sprite_box_ = dynamic_cast<Sprite* >(CSLoader::seekNodeByName(root, "sprite_box"));
    sprite_box_->setVisible(false);

    sprite_dealer_ = dynamic_cast<Sprite* >(CSLoader::seekNodeByName(root, "sprite_dealer"));
    sprite_dealer_->setVisible(false);
    
    sprite_winner_ = dynamic_cast<Sprite* >(CSLoader::seekNodeByName(root, "sprite_winner"));
    //sprite_winner_->setPosition(sprite_winner_->getPosition() + Point(-32,37));
    sprite_winner_->setLocalZOrder(100);
    sprite_winner_->setGlobalZOrder(100);
    //int y = sprite_winner_->getPositionY();
    sprite_winner_->setPositionY(-40);   // by ronaldz. 我们往下放一点， 让牌露出来。

    text_win_type_bg_ = dynamic_cast<Sprite* >(CSLoader::seekNodeByName(root, "text_win_type_bg"));
    text_win_type_bg_->setLocalZOrder(1);
    text_win_type_bg_->setVisible(false);
    
    text_win_type_ = dynamic_cast<Text* >(CSLoader::seekNodeByName(root, "text_win_type"));
    text_win_type_->setLocalZOrder(2);
    text_win_amount_ = dynamic_cast<Text* >(CSLoader::seekNodeByName(root, "text_win_amount"));
    text_win_amount_->setVisible(false);
    text_win_amount_->setFontSize(32);
    text_win_amount_->setLocalZOrder(2);
    
    
    text_win_type_bg_->setVisible(false);
    text_win_type_->setVisible(false);
    
    player_info_last = nullptr;
    
    player_info_bet = dynamic_cast<Sprite* >(CSLoader::seekNodeByName(root, "player_info_bet"));
    player_info_bet->setVisible(false);
    player_info_check = dynamic_cast<Sprite* >(CSLoader::seekNodeByName(root, "player_info_check"));
    player_info_check->setVisible(false);
    
    player_info_fold = dynamic_cast<Sprite* >(CSLoader::seekNodeByName(root, "player_info_fold"));
    player_info_fold->setVisible(false);
    
    player_info_raise = dynamic_cast<Sprite* >(CSLoader::seekNodeByName(root, "player_info_raise"));
    player_info_raise->setVisible(false);
    
    player_info_allin = dynamic_cast<Sprite* >(CSLoader::seekNodeByName(root, "player_info_allin"));
    player_info_allin->setVisible(false);

    btn_kick_ =  dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "btn_kick"));
    btn_kick_->addClickEventListener(std::bind(&PlayerInfoNode::click_btn_kick, this, std::placeholders::_1));
    
    btn_kick_->setVisible(false);
    
    set_dealer(false);
    
    show_green_box(false);
    
    sprite_winner_->setVisible(false);
    root_ = root;
    
    sprite_timer_fire = dynamic_cast<Sprite* >(CSLoader::seekNodeByName(root, "sprite_timer_fire"));
    sprite_timer_fire->setLocalZOrder(1);
    sprite_timer_fire->setVisible(false);
    sprite_timer_fire_pos = sprite_timer_fire->getPosition();
}

void PlayerInfoNode::show_voice_animation(const float duration)
{
    Sprite* s = Sprite::create("login_res/record_voice/user_voice_1.png");
    s->setPosition(image_head_->getPosition().x, image_head_->getPosition().y + image_head_->getContentSize().height);
    this->addChild(s, 10);
    
    auto animation = Animation::create();
    for (int i = 1; i < 4; i++) {
        animation->addSpriteFrameWithFile("login_res/record_voice/user_voice_" + tools::to_string(i) + ".png");
    }
    animation->setDelayPerUnit(0.1f);
    
    s->runAction(RepeatForever::create(Animate::create(animation)));
    
    static int idx = 0;
    std::string remove_key = "remove_voice" + tools::to_string(idx++);
    scheduleOnce([=](float){
        s->removeFromParent();
    }, duration, remove_key);
}

void PlayerInfoNode::setVisible(bool vis)
{
    assert(0);
    Node::setVisible(vis);
}

void PlayerInfoNode::setVisible1(bool vis)
{
    if(!vis) {
        cocos2d::log("setVisible1 false");
    }
    Node::setVisible(vis);
}
void PlayerInfoNode::removeFromParentAndCleanup(bool cleanup)
{
    Node::removeFromParentAndCleanup(cleanup);
}

void PlayerInfoNode::removeFromParent()
{
    Node::removeFromParent();
}

void PlayerInfoNode::click_btn_kick(Ref* sender)
{
    btn_kick_->setVisible(false);
    auto &poker_room_data = this->GetRoom();
    if (poker_room_data == nullptr)
    {
        cocos2d::log("PlayerInfoNode::click_btn_kick poker_room_data == nullptr");
        return;
    }
    bool is_tournament = poker_room_data->get_is_tournament();
    if (user_)
    {
        if ((poker_room_data->get_owner_id() == GDM->get_user_id()) &&
            (user_->get_id() != GDM->get_user_id()) &&
            !is_tournament)
        {
            
            auto tip = TipView::showAlertView("确定要踢出这个人吗？");
            tip->show_cancel(true);
            tip->set_tipview_close_callback([this](){
                PDM->send_kick_player(GetRoom()->get_room_id(), user_->get_id());
                
            });
        }
    }
}

void PlayerInfoNode::set_dismiss()
{
    if (btn_kick_->isVisible())
    {
        btn_kick_->setVisible(false);
    }
}