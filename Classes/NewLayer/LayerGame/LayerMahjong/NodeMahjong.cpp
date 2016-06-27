//
//  NodeMahjong.cpp
//  pokerclient
//
//  Created by HuangFeng on 16/6/3.
//
//

#include "NodeMahjong.h"
#include "PokerDataManager.h"
#include "GameLayer.h"
#include "utils.h"
#include "GameDataManager.h"
#include "FileManager.h"
#include "NET_CMD.h"
#include "cmd_def.h"
#include "NodeCommon.h"
#include "MahjongRoomLayer.h"

bool NodeMahjong::init()
{
    if(!Node::init())
    {
        return false;
    }
    handcard = NodeHandCard::create();
    addChild(handcard);
    
    discards = NodeDiscard::create();
    addChild(discards);
    
    Fu = NodeFu::create();
    addChild(Fu);
    
    mHuCard = NodeHu::create();
    addChild(mHuCard);
    dispatch_.register_event(EVENT_DOWLOAD_FILE,
                             BIND_FUNC(this, NodeMahjong::handle_download_event));
    
    btn_sit_ = Button::create();
    btn_sit_->loadTextures("image/Mahjong_game_res/mahjong_menu_res/btn_mahjong_site_normal.png", "image/Mahjong_game_res/mahjong_menu_res/btn_mahjong_site_pressed.png");
    //    btn_sit_->setRotation(180);
    addChild(btn_sit_);
    btn_sit_->setVisible(false);
    btn_sit_->addClickEventListener([&](Ref* sender){
        if ( call_back_ )
        {
            call_back_();
        }
    });
    return true;
}

void NodeMahjong::update(msg::MahjongPlayerInfo *mahjongPlayerInfo, bool IsPlaying, bool is_own_exist)
{
    if ( !mahjongPlayerInfo )
    {
        btn_sit_->setVisible(true);
        if ( is_own_exist )
        {
            btn_sit_->setVisible(false);
        }
        switch (Direction_)
        {
            case NODE_UP:
                AlignNode(btn_sit_, BOTTOMLEFT,336,224);
                break;
            case NODE_LEFT:
                AlignNode(btn_sit_, TOPLEFT, 10, 200);
                break;
            case NODE_RIGHT:
                AlignNode(btn_sit_,TOPLEFT, 165, 226);
                break;
            case NODE_DOWN:
                AlignNode(btn_sit_, TOPLEFT, 336,228);
                break;
            default:
                break;
        }
        clear();
        return;
    }
    
    switch (Direction_) {
        case NODE_LEFT:
        {
            AlignNode(handcard, TOPLEFT, 28, 108);
            AlignNode(Fu, BOTTOMLEFT,14, -20);
            AlignNode(discards, TOPRIGHT, 0, 182);
            AlignNode(mHuCard, BOTTOMLEFT,  84, 8);
            discards->setContentSize(Size(HORZ_OUT_WIDTH*discards->GetTotalRow(), HORZ_OUT_HEIGHT*discards->GetCardPerRow()));
            mHuCard->setContentSize(Size(VERT_OUT_HEIGHT*mHuCard->GetTotalRow(), VERT_OUT_WIDTH*mHuCard->GetCardPerRow()));
            break;
        }
        case NODE_UP:
        {
            AlignNode(handcard, TOPRIGHT, 50, 158);
            AlignNode(Fu, TOPLEFT, 4, 176);
            AlignNode(discards, BOTTOMRIGHT, 273, 0);
            AlignNode(mHuCard, TOPLEFT, 100, 246);
            discards->setContentSize(Size(VERT_OUT_WIDTH*discards->GetCardPerRow(), VERT_OUT_HEIGHT*discards->GetTotalRow()));
            mHuCard->setContentSize(Size(VERT_OUT_WIDTH*mHuCard->GetCardPerRow(), VERT_OUT_HEIGHT*mHuCard->GetTotalRow()));
            break;
        }
        case NODE_RIGHT:
        {
            AlignNode(handcard, BOTTOMRIGHT, 28, 140);
            AlignNode(Fu, TOPRIGHT, 14, -18);
            AlignNode(discards, BOTTOMLEFT, 0, 234);
            AlignNode(mHuCard, TOPRIGHT, 110, 16);
            discards->setContentSize(Size(HORZ_OUT_WIDTH*discards->GetTotalRow(), HORZ_OUT_HEIGHT*discards->GetCardPerRow()));
            mHuCard->setContentSize(Size(VERT_OUT_HEIGHT*mHuCard->GetTotalRow(), VERT_OUT_WIDTH*mHuCard->GetCardPerRow()));
            break;
        }
        case NODE_DOWN:
        {
            AlignNode(handcard, BOTTOMLEFT, 0, 0);
            AlignNode(Fu, BOTTOMRIGHT, 4, 0);
            AlignNode(discards, TOPLEFT, 273, 0);
            AlignNode(mHuCard, BOTTOMRIGHT, 8, 126);
            discards->setContentSize(Size(VERT_OUT_WIDTH*discards->GetCardPerRow(), VERT_OUT_HEIGHT*discards->GetTotalRow()));
            mHuCard->setContentSize(Size(VERT_OUT_WIDTH*mHuCard->GetCardPerRow(), VERT_OUT_HEIGHT*mHuCard->GetTotalRow()));
            break;
        }
    }
    
    btn_sit_->setVisible(false);
    auto commonPlayerInfo = mahjongPlayerInfo->playerinfo(); //玩家信息
    user_id_ = commonPlayerInfo.userinfo().userid();
    auto userDetailInfo = commonPlayerInfo.userinfo();
    
    msg::MahjongCard drawcard = msg::MahjongCard::INVISIBLE;
    if (mahjongPlayerInfo->has_drawcard())
        drawcard = mahjongPlayerInfo->drawcard();
    
    std::vector<msg::MahjongCard> discard;
    for (auto iter: mahjongPlayerInfo->discard())
        discard.push_back((msg::MahjongCard)iter);
    discards->update(discard);
    std::vector<msg::MahjongCard> cards;
    for (int i=0; i<mahjongPlayerInfo->card_size(); i++)
        cards.push_back(mahjongPlayerInfo->card(i));
    
    ShowHeadPic(userDetailInfo.picname(), userDetailInfo.nickname(), mahjongPlayerInfo->playerscore(), mahjongPlayerInfo->suit(), IsPlaying);
    refresh_user_status(commonPlayerInfo.sittingstatus());
    
    
    std::vector<msg::MahjongCard> gangcard;
    std::vector<msg::MahjongPossibleTing> possibleting;
    NodeHandCard::SELECT_MODE selectmode = NodeHandCard::SELECT_NONE;
    if(userDetailInfo.userid() == GDM->get_user_id() && mahjongPlayerInfo->has_actionrequest())
    {
        for (auto ting: mahjongPlayerInfo->actionrequest().possibleting())
            possibleting.push_back(ting);
        switch (mahjongPlayerInfo->actionrequest().type()){
            case msg::MahjongRequestType::MahjongChooseDiscard:
                selectmode = NodeHandCard::SELECT_DISCARD;
                break;
            case msg::MahjongRequestType::MahjongChooseSwitchCard:
                selectmode = NodeHandCard::SELECT_SWITCHCARD;
                break;
            default:
                selectmode = NodeHandCard::SELECT_NONE;
                break;
        }
        for (auto iter: mahjongPlayerInfo->actionrequest().gangcard())
            gangcard.push_back((msg::MahjongCard)iter);
    }
    
    if (userDetailInfo.userid() != GDM->get_user_id() && mahjongPlayerInfo->actionrequest().type() == msg::MahjongRequestType::MahjongChooseSwitchCard )
    {
        if ( choose_switch_card_ == nullptr )
        {
            choose_switch_card_ = Sprite::create("image/Mahjong_game_res/image_action/image_xuanpai.png");
            this->addChild(choose_switch_card_,1000);
        }
        choose_switch_card_->setVisible(true);
        switch (Direction_)
        {
            case NODE_UP:
                AlignNode(choose_switch_card_, TOP, 0, 240);
                break;
            case NODE_LEFT:
                AlignNode(choose_switch_card_, TOPLEFT, 71, 274);
                break;
            case NODE_RIGHT:
                AlignNode(choose_switch_card_, TOPLEFT, 37, 304);
                break;
            case NODE_DOWN:
                AlignNode(choose_switch_card_, TOP, 0, 340);
                break;
            default:
                break;
        }
    }
    else if (userDetailInfo.userid() == GDM->get_user_id() && mahjongPlayerInfo->actionrequest().type() == msg::MahjongRequestType::MahjongChooseSwitchCard )
    {
        if ( switch_callback_ )
        {
            switch_callback_();
        }
    }
    else
    {
        if ( choose_switch_card_ )
        {
            choose_switch_card_->setVisible(false);
        }
    }
    
    handcard->update(cards, drawcard, possibleting, mahjongPlayerInfo->suit(), selectmode, gangcard);
    
    Fu->update(mahjongPlayerInfo->fu());
    
    mHuCard->update(mahjongPlayerInfo->scorechange());//胡牌
    
    ShowHeadPic(userDetailInfo.picname(), userDetailInfo.nickname(), mahjongPlayerInfo->playerscore(), mahjongPlayerInfo->suit(), IsPlaying);
}

void NodeMahjong::refresh_user_status(msg::SittingStatus status)
{
    if ( text_player_status_ )
    {
        switch (status)
        {
            case msg::SittingStatus::PLAYING:
                text_player_status_->setString("");
                break;
            case msg::SittingStatus::SITDOWN:
                text_player_status_->setString("等待游戏开始");
                break;
            case msg::SittingStatus::WANTTOLEAVE:
                text_player_status_->setString("即将离开");
                break;
            default:
                text_player_status_->setString("");
                break;
        }
    }
}

void NodeMahjong::handle_download_event(cmd_data_pointer data)
{
    auto s = std::dynamic_pointer_cast<cmd_data_string>(data);
    if (s->get_value() != pic_name_) return;
    if ( sprite_photo_ && sprite_photo_->getParent() )
    {
        sprite_photo_->removeFromParent();
        sprite_photo_ = nullptr;
    }
    sprite_photo_ = ShaderSprite::create(pic_name_, "Default/image_mask_mahjong.png");
    player_bg_->addChild(sprite_photo_);
    sprite_photo_->setPosition(player_bg_->getContentSize().width/2, player_bg_->getContentSize().height/2+20);
    sprite_photo_->setScale(42/sprite_photo_->getContentSize().width, 46/sprite_photo_->getContentSize().height);
    
}

void NodeMahjong::SetLastCard(bool HasLastCard, msg::MahjongCardZone zone)
{
    if (HasLastCard)
        switch (zone){
            case msg::MahjongCardZone::DiscardZone:
                discards->SetLastDiscard(true);
                Fu->SetLastDiscard(false);
                mHuCard->SetLastDiscard(false);
                break;
            case msg::MahjongCardZone::FuZone:
                discards->SetLastDiscard(false);
                Fu->SetLastDiscard(true);
                mHuCard->SetLastDiscard(false);
                break;
            case msg::MahjongCardZone::HuZone:
                discards->SetLastDiscard(false);
                Fu->SetLastDiscard(false);
                mHuCard->SetLastDiscard(true);
                break;
        }
    else{
        discards->SetLastDiscard(false);
        Fu->SetLastDiscard(false);
        mHuCard->SetLastDiscard(false);
    }
}

#pragma mark - 控件操作
void NodeMahjong::ShowHeadPic(const std::string& Picname, const std::string& NickName, int score, msg::MahjongSuit BlankSuit, bool IsPlaying){
    
    if ( player_bg_ == nullptr )
    {
        player_bg_ = ImageView::create("image/Mahjong_game_res/image_mahjong_photo_bg.png");
        sprite_photo_ = ShaderSprite::create("Default/image_defualt_group_icon_small.png", "Default/image_mask_mahjong.png");
        
        text_player_score_ = Text::create();
        text_player_score_->setTextColor(Color4B(254, 250, 226, 100));
        text_player_score_->setFontSize(20);
        
        text_player_status_ = Text::create();
        text_player_status_->setFontSize(24);
        
        text_player_nickname_ = Text::create();
        text_player_nickname_->setTextColor(Color4B(164, 255, 119, 100));
        text_player_nickname_->setFontSize(20);
        
        sprite_lack_ = Sprite::create("image/Mahjong_game_res/image_circle.png");
        text_lack_ = Text::create();
        text_lack_->setTextColor(Color4B(254, 250, 226, 100));
        text_lack_->setFontSize(28);
        this->addChild(player_bg_);
        player_bg_->addChild(sprite_photo_);
        sprite_photo_->setPosition(player_bg_->getContentSize().width/2, player_bg_->getContentSize().height/2+20);
        player_bg_->addChild(text_player_score_);
        text_player_nickname_->setPosition(Vec2(player_bg_->getContentSize().width/2, player_bg_->getContentSize().height/2-16));
        player_bg_->addChild(text_player_status_);
        player_bg_->addChild(text_player_nickname_);
        text_player_score_->setPosition(Vec2(player_bg_->getContentSize().width/2, player_bg_->getContentSize().height/2-34));
        player_bg_->addChild(sprite_lack_);
        sprite_lack_->addChild(text_lack_);
        text_lack_->cocos2d::Node::setPosition(sprite_lack_->getContentSize().width/2, sprite_lack_->getContentSize().height/2);
    }
    text_player_nickname_->setString(NickName);
    //    text_player_status_->setString("等待下次开始");
    text_player_score_->setString(tools::to_string(score));
    sprite_lack_->setVisible(true);
    
    switch (BlankSuit)
    {
        case msg::MahjongSuit::TIAO:
            text_lack_->setString("条");
            break;
        case msg::MahjongSuit::BING:
            text_lack_->setString("筒");
            break;
        case msg::MahjongSuit::WAN:
            text_lack_->setString("万");
            break;
        default:
            text_lack_->setString("");
            sprite_lack_->setVisible(false);
            break;
    }
    pic_name_ = Picname;
    if ( sprite_photo_ && sprite_photo_->getParent() )
    {
        sprite_photo_->removeFromParent();
        sprite_photo_ = nullptr;
    }
    if ( GDM->is_file_exist(Picname) )
    {
        sprite_photo_ = ShaderSprite::create(Picname,  "Default/image_mask_mahjong.png");
        player_bg_->addChild(sprite_photo_);
        sprite_photo_->setPosition(player_bg_->getContentSize().width/2, player_bg_->getContentSize().height/2+20);
        sprite_photo_->setScale(42/sprite_photo_->getContentSize().width, 46/sprite_photo_->getContentSize().height);
    }
    else
    {
        FM.download_file(Picname);
    }
    
    switch (getDirection()) {
        case NODE_UP:
            sprite_lack_->setPosition(0,0);
            text_player_status_->setAnchorPoint(Vec2(1, 0.5));
            text_player_status_->setPosition(Vec2(0,35));
            if ( IsPlaying )
            {
                AlignNode(player_bg_, TOPRIGHT,6,30);
            }
            else
            {
                AlignNode(player_bg_, BOTTOMLEFT,336,224);
            }
            
            break;
        case NODE_LEFT:
            sprite_lack_->setPosition(player_bg_->getContentSize().width,0);
            text_player_status_->setAnchorPoint(Vec2(0, 0.5));
            text_player_status_->setPosition(Vec2(player_bg_->getContentSize().width,35));
            if ( IsPlaying )
            {
                AlignNode(player_bg_, TOPLEFT, 6, 6);
            }
            else
            {
                AlignNode(player_bg_, TOPLEFT, 10, 238);
            }
            
            break;
        case NODE_RIGHT:
            sprite_lack_->setPosition(0,0);
            
            text_player_status_->setAnchorPoint(Vec2(1, 0.5));
            text_player_status_->setPosition(Vec2(0,35));
            if ( IsPlaying )
            {
                AlignNode(player_bg_, BOTTOMRIGHT, 6, 30);
            }
            else
            {
                AlignNode(player_bg_,TOPLEFT, 180, 226);
            }
            
            break;
        case NODE_DOWN:
            sprite_lack_->setPosition(player_bg_->getContentSize().width,0);
            text_player_status_->setAnchorPoint(Vec2(0, 0.5));
            text_player_status_->setPosition(Vec2(player_bg_->getContentSize().width,35));
            if ( IsPlaying )
            {
                AlignNode(player_bg_, TOPLEFT, 6, 248+50);
            }
            else
            {
                AlignNode(player_bg_, TOPLEFT, 336,228+50);
            }
            
            break;
        default:
            break;
    }
}

void NodeMahjong::ShowAction(msg::MahjongActionChoice Action){
    if ( player_action_ == nullptr )
    {
        player_action_ = Sprite::create();
        this->addChild(player_action_);
        player_action_->setAnchorPoint(Vec2(0.5, 0.5));
    }
    std::string action_image = "";
    switch (Action) {
        case msg::MahjongActionChoice::HU:
            action_image = "image/Mahjong_game_res/image_action/image_hu.png";
            break;
        case msg::MahjongActionChoice::CHI:
            action_image = "image/Mahjong_game_res/image_action/image_chi.png";
            break;
        case msg::MahjongActionChoice::PENG:
            action_image = "image/Mahjong_game_res/image_action/image_peng.png";
            break;
        case msg::MahjongActionChoice::GANG:
            action_image = "image/Mahjong_game_res/image_action/image_gang.png";
            break;
        case msg::MahjongActionChoice::PASS:
            action_image = "image/Mahjong_game_res/image_action/image_guo.png";
            break;
        case msg::MahjongActionChoice::HUJIAOZHUANYI:
            action_image = "image/Mahjong_game_res/image_action/image_hujiaozhuanyi.png";
            break;
        default:
            break;
    }
    player_action_->setTexture(action_image);
    switch (getDirection()) {
        case NODE_UP:
            player_action_->setPosition(Vec2(this->getContentSize().width/2, this->getContentSize().height/4));
            break;
        case NODE_LEFT:
            player_action_->setPosition(Vec2(this->getContentSize().width*3/4, this->getContentSize().height/2));
            break;
        case NODE_RIGHT:
            player_action_->setPosition(Vec2(this->getContentSize().width/4, this->getContentSize().height/2));
            break;
        case NODE_DOWN:
            player_action_->setPosition(Vec2(this->getContentSize().width/2, this->getContentSize().height*3/4));
            break;
        default:
            break;
    }
    player_action_->setScale(0.1);
    player_action_->setVisible(true);
    auto action_scale_1 = ScaleTo::create(2 , 0.8);
    auto action_scale_2 = ScaleTo::create(0.1 , 0.8);
    auto call_func_1 = CallFunc::create([&](){
        player_action_->setVisible(false);
    });
    
    player_action_->runAction(cocos2d::Sequence::create(action_scale_1, DelayTime::create(0.8), action_scale_2, call_func_1, NULL));
}

void NodeMahjong::ShowAction(int ScoreChange){
    if ( text_change_score_ == nullptr )
    {
        if ( ScoreChange > 0 )
        {
            text_change_score_ = TextAtlas::create("","image/Mahjong_game_res/image_action/reward_score.png", 40, 42,".");
            score_mark = Sprite::create("image/Mahjong_game_res/image_action/reward_mark.png");
        }
        else
        {
            text_change_score_ = TextAtlas::create("","image/Mahjong_game_res/image_action/reduce_score.png", 40, 42,".");
            score_mark = Sprite::create("image/Mahjong_game_res/image_action/reducr_mark.png");
        }
        this->addChild(score_mark);
        score_mark->addChild(text_change_score_);
    }
    text_change_score_->setString(tools::to_string(ScoreChange));
    switch (getDirection()) {
        case NODE_UP:
            AlignNode(score_mark, TOP, 0, 240);
            break;
        case NODE_LEFT:
            AlignNode(score_mark, TOPLEFT, 71, 274);
            break;
        case NODE_RIGHT:
            AlignNode(score_mark, TOPLEFT, 37, 304);
            break;
        case NODE_DOWN:
            AlignNode(score_mark, TOP, 0, 340);
            break;
        default:
            break;
    }
    AlignNode(text_change_score_, LEFT, score_mark->getContentSize().width + 10, 0);
    
    auto call_func_1 = CallFunc::create([&](){
        score_mark->setVisible(false);
        text_change_score_->removeFromParent();
        score_mark->removeFromParent();
        text_change_score_ = nullptr;
        score_mark = nullptr;
    });
    
    score_mark->runAction(cocos2d::Sequence::create(DelayTime::create(2), call_func_1, nullptr));
}
void NodeMahjong::clear()
{
    handcard->clear();
    discards->clear();
    Fu->clear();
    mHuCard->clear();
    if ( player_bg_ )
    {
        if (sprite_photo_)
            sprite_photo_->removeFromParent();
        text_lack_->removeFromParent();
        sprite_lack_->removeFromParent();
        text_player_status_->removeFromParent();
        text_player_nickname_->removeFromParent();
        text_player_score_->removeFromParent();
        player_bg_->removeFromParent();
        
        player_bg_ = nullptr;
        text_player_status_ = nullptr;
        text_player_nickname_ = nullptr;
        text_player_score_ = nullptr;
        text_lack_ = nullptr;
        sprite_lack_ = nullptr;
        sprite_photo_ = nullptr;
        
    }
}
bool NodeDirection::init()
{
    if(!Node::init())
    {
        return false;
    }
    
    auto nodeRoot = CSLoader::createNode("NodeMahjongDirection.csb");
    auto imageBg = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(nodeRoot, "image_bg"));
    mSpriteLeft = dynamic_cast<Sprite*>(CSLoader::seekNodeByName(imageBg, "sprite_left"));
    mSpriteDown = dynamic_cast<Sprite*>(CSLoader::seekNodeByName(imageBg, "sprite_down"));
    mSpriteRight = dynamic_cast<Sprite*>(CSLoader::seekNodeByName(imageBg, "sprite_right"));
    mSpriteUp = dynamic_cast<Sprite*>(CSLoader::seekNodeByName(imageBg, "sprite_up"));
    
    mSpriteTagLeft = dynamic_cast<Sprite*>(CSLoader::seekNodeByName(imageBg, "sprite_tag_left"));
    mSpriteTagDown = dynamic_cast<Sprite*>(CSLoader::seekNodeByName(imageBg, "sprite_tag_down"));
    mSpriteTagRight = dynamic_cast<Sprite*>(CSLoader::seekNodeByName(imageBg, "sprite_tag_right"));
    mSpriteTagUp = dynamic_cast<Sprite*>(CSLoader::seekNodeByName(imageBg, "sprite_tag_up"));
    mTextAtlasCountdown = dynamic_cast<TextAtlas*>(CSLoader::seekNodeByName(imageBg, "text_atlas_countdown"));
    resetCountdown();
    addChild(nodeRoot);
    return true;
}


void NodeDirection::setEastDirection(NodeMahjong::NODE_DIR dir)
{
    if (eastdir_ == dir) return;
    eastdir_ = dir;
    switch(dir)
    {
        case NodeMahjong::NODE_DIR::NODE_DOWN:
            mSpriteDown->setTexture("dir_east.png");
            mSpriteRight->setTexture("dir_south.png");
            mSpriteUp->setTexture("dir_west.png");
            mSpriteLeft->setTexture("dir_north.png");
            break;
        case NodeMahjong::NODE_DIR::NODE_RIGHT:
            mSpriteRight->setTexture("dir_east.png");
            mSpriteUp->setTexture("dir_south.png");
            mSpriteLeft->setTexture("dir_west.png");
            mSpriteDown->setTexture("dir_north.png");
            break;
        case NodeMahjong::NODE_DIR::NODE_UP:
            mSpriteUp->setTexture("dir_east.png");
            mSpriteLeft->setTexture("dir_south.png");
            mSpriteDown->setTexture("dir_west.png");
            mSpriteRight->setTexture("dir_north.png");
            break;
        case NodeMahjong::NODE_DIR::NODE_LEFT:
            mSpriteLeft->setTexture("dir_east.png");
            mSpriteDown->setTexture("dir_south.png");
            mSpriteRight->setTexture("dir_west.png");
            mSpriteUp->setTexture("dir_north.png");
            break;
        default:
            break;
    }
}

void NodeDirection::resetCountdown()
{
    mCountdown = 0;
    mTextAtlasCountdown->setString(tools::anything2string(mCountdown));
    mTextAtlasCountdown->setColor(Color3B(116, 225, 253));
    mSpriteTagUp->setVisible(false);
    mSpriteTagUp->stopAllActions();
    mSpriteTagLeft->setVisible(false);
    mSpriteTagLeft->stopAllActions();
    mSpriteTagDown->setVisible(false);
    mSpriteTagDown->stopAllActions();
    mSpriteTagRight->setVisible(false);
    mSpriteTagRight->stopAllActions();
    unschedule(CC_SCHEDULE_SELECTOR(NodeDirection::scheduleCountdown));
}

void NodeDirection::setCountdown(NodeMahjong::NODE_DIR dir, int time, bool IsTimeBank)
{
    resetCountdown();
    auto action = RepeatForever::create(Sequence::createWithTwoActions(FadeTo::create(0.8f, 125), FadeTo::create(0.4f, 255)));
    switch(dir)
    {
        case NodeMahjong::NODE_DIR::NODE_RIGHT:
            mSpriteTagRight->setVisible(true);
            mSpriteTagRight->runAction(action);
            break;
        case NodeMahjong::NODE_DIR::NODE_DOWN:
            mSpriteTagDown->setVisible(true);
            mSpriteTagDown->runAction(action);
            break;
        case NodeMahjong::NODE_DIR::NODE_LEFT:
            mSpriteTagLeft->setVisible(true);
            mSpriteTagLeft->runAction(action);
            break;
        case NodeMahjong::NODE_DIR::NODE_UP:
            mSpriteTagUp->setVisible(true);
            mSpriteTagUp->runAction(action);
            break;
        default:
            break;
    }
    
    
    mCountdown = time;
    mTextAtlasCountdown->setString(tools::anything2string(mCountdown));
    mTextAtlasCountdown->setColor(IsTimeBank ? Color3B(235, 74, 74) : Color3B(116, 225, 253));
    schedule(CC_SCHEDULE_SELECTOR(NodeDirection::scheduleCountdown), 1.0f);
    
}
void NodeDirection::scheduleCountdown(float dt)
{
    if(--mCountdown >= 0)
    {
        mTextAtlasCountdown->setString(tools::anything2string(mCountdown));
    }
    else
    {
        mCountdown = -1;
        unschedule(CC_SCHEDULE_SELECTOR(::NodeDirection::scheduleCountdown));
        log("time over");
    }
}

void NodeDirection::setCountdownVisible(bool isVisible)
{
    mTextAtlasCountdown->setVisible(isVisible);
}


CLASS_CARD MahjongChildNode::GetBackPosition(){
    switch(GetDirection()){
        case NodeMahjong::NODE_DIR::NODE_RIGHT: return CLASS_CARD::EAST_HAND_AN_GANG;
        case NodeMahjong::NODE_DIR::NODE_LEFT: return CLASS_CARD::WEST_HAND_AN_GANG;
        case NodeMahjong::NODE_DIR::NODE_DOWN: return CLASS_CARD::SOUTH_HAND_AN_GANG;
        case NodeMahjong::NODE_DIR::NODE_UP: return CLASS_CARD::NORTH_HAND_AN_GANG;
        default: return CLASS_CARD::NORTH_DI;
    }
}

CLASS_CARD MahjongChildNode::GetHandPosition(){
    switch(GetDirection()){
        case NodeMahjong::NODE_DIR::NODE_RIGHT: return CLASS_CARD::EAST_HAND;
        case NodeMahjong::NODE_DIR::NODE_LEFT: return CLASS_CARD::WEST_HAND;
        case NodeMahjong::NODE_DIR::NODE_DOWN:
            if (IsSelf())
                return CLASS_CARD::SOUTH_HAND;
            else
                return CLASS_CARD::SOUTH_INVISIBLE_HAND;
        case NodeMahjong::NODE_DIR::NODE_UP: return CLASS_CARD::NORTH_HAND;
        default: return CLASS_CARD::NORTH_DI;
    }
}

CLASS_CARD MahjongChildNode::GetOutPosition(){
    switch(GetDirection()){
        case NodeMahjong::NODE_DIR::NODE_RIGHT: return CLASS_CARD::EAST_OUT;
        case NodeMahjong::NODE_DIR::NODE_LEFT: return CLASS_CARD::WEST_OUT;
        case NodeMahjong::NODE_DIR::NODE_DOWN: return CLASS_CARD::SOUTH_OUT;
        case NodeMahjong::NODE_DIR::NODE_UP: return CLASS_CARD::NORTH_OUT;
        default: return CLASS_CARD::NORTH_DI;
    }
}

CLASS_CARD MahjongChildNode::GetDoubleOutPosition(){
    switch(GetDirection()){
        case NodeMahjong::NODE_DIR::NODE_RIGHT: return CLASS_CARD::EAST_DOUBLE_OUT;
        case NodeMahjong::NODE_DIR::NODE_LEFT: return CLASS_CARD::WEST_DOUBLE_OUT;
        case NodeMahjong::NODE_DIR::NODE_DOWN: return CLASS_CARD::SOUTH_DOUBLE_OUT;
        case NodeMahjong::NODE_DIR::NODE_UP: return CLASS_CARD::NORTH_DOUBLE_OUT;
        default: return CLASS_CARD::NORTH_DI;
    }
}

CLASS_CARD MahjongChildNode::GetPengPosition(){
    switch(GetDirection()){
        case NodeMahjong::NODE_DIR::NODE_RIGHT: return CLASS_CARD::EAST_HAND_PENG;
        case NodeMahjong::NODE_DIR::NODE_LEFT: return CLASS_CARD::WEST_HAND_PENG;
        case NodeMahjong::NODE_DIR::NODE_DOWN: return CLASS_CARD::SOUTH_HAND_PENG;
        case NodeMahjong::NODE_DIR::NODE_UP: return CLASS_CARD::NORTH_HAND_PENG;
        default: return CLASS_CARD::NORTH_DI;
    }
}

CLASS_CARD MahjongChildNode::GetPengHorzPosition(){
    switch(GetDirection()){
        case NodeMahjong::NODE_DIR::NODE_RIGHT: return CLASS_CARD::EAST_HAND_PENG_POSITION;
        case NodeMahjong::NODE_DIR::NODE_LEFT: return CLASS_CARD::WEST_HAND_PENG_POSITION;
        case NodeMahjong::NODE_DIR::NODE_DOWN: return CLASS_CARD::SOUTH_HAND_PENG_POSITION;
        case NodeMahjong::NODE_DIR::NODE_UP: return CLASS_CARD::NORTH_HAND_PENG_POSITION;
        default: return CLASS_CARD::NORTH_DI;
    }
}

int32_t MahjongChildNode::GetHandCardWidth()
{
    switch(GetDirection()){
        case NodeMahjong::NODE_DIR::NODE_RIGHT: return EAST_HAND_WIDTH;
        case NodeMahjong::NODE_DIR::NODE_LEFT: return WEST_HAND_WIDTH;
        case NodeMahjong::NODE_DIR::NODE_DOWN: return SOUTH_HAND_WIDTH;
        case NodeMahjong::NODE_DIR::NODE_UP: return NORTH_HAND_WIDTH;
        default: return CLASS_CARD::NORTH_DI;
    }
}

int32_t MahjongChildNode::GetHandCardHeight()
{
    switch(GetDirection()){
        case NodeMahjong::NODE_DIR::NODE_RIGHT: return EAST_HAND_HEIGHT;
        case NodeMahjong::NODE_DIR::NODE_LEFT: return WEST_HAND_HEIGHT;
        case NodeMahjong::NODE_DIR::NODE_DOWN: return SOUTH_HAND_HEIGHT;
        case NodeMahjong::NODE_DIR::NODE_UP: return NORTH_HAND_HEIGHT;
        default: return CLASS_CARD::NORTH_DI;
    }
}

std::vector<msg::MahjongCard> NodeHandCard::GetSelectedCards()
{
    std::vector<msg::MahjongCard> result;
    for (auto child: getChildren()){
        auto Card = dynamic_cast<MahjongCard*>(child);
        if (Card && Card->GetSelected())
            result.push_back(Card->GetCardNumber());
    }
    return result;
}

void NodeHandCard::SetAllCardsUnselected()
{
    for (auto child: getChildren()){
        auto Card = dynamic_cast<MahjongCard*>(child);
        if (Card)
            Card->SetSelected(false);
    }
}

void NodeHandCard::setZoomSuit(msg::MahjongSuit Value)
{
    if (zoomsuit_ != Value){
        zoomsuit_ = Value;
        //RefreshCards();
    }
}

void NodeHandCard::setZoomIndex(int Value)
{
    if (zoomindex_ != Value){
        zoomindex_ = Value;
        //RefreshCards();
    }
}

#pragma mark - handCardOnClick
void NodeHandCard::handCardOnClick(Ref *ref)
{
    MahjongRoomLayer* mahjong_layer = dynamic_cast<MahjongRoomLayer*>(SceneManager::GetRoomLayer());
    if(mahjong_layer)
    {
        mahjong_layer->remove_ting_layer();
    }
    auto SelectCard = dynamic_cast<MahjongCard*>(ref);
    switch (selectmode_){
        case SELECT_DISCARD:
            if (SelectCard->GetSelected()){
                SelectCard->SetSelected(false);
                setZoomIndex(-1);
                msg::MahjongCard disCard = SelectCard->GetCardNumber();
                GameLayer* layer = dynamic_cast<GameLayer*>(SceneManager::GetRoomLayer());
                if(layer)
                    PDM->send_mahjong_discard(layer->get_roomid(), disCard);
                
            }
            else{
                SetAllCardsUnselected();
                SelectCard->SetSelected(true);
                setZoomIndex(SelectCard->getTag());
                if (SelectCard->get_ting()){
                    MahjongRoomLayer* layer = dynamic_cast<MahjongRoomLayer*>(SceneManager::GetRoomLayer());
                    if(layer)
                        layer->click_hand_ting(SelectCard->GetCardNumber());
                }
            }
            break;
        case SELECT_SWITCHCARD:
            if (SelectCard->GetSelected()){
                auto size = GetSelectedCards().size();
                SelectCard->SetSelected(false);
                if (size == 3)
                    NotifySwitchEnabled(false);
                else if (size == 1)
                    setZoomSuit(msg::MahjongSuit::SUITNONE);
            }
            else {
                auto SelectCards = GetSelectedCards();
                if (SelectCards.size() < 3){
                    bool bSameSuit = true;
                    msg::MahjongCard selectcard = SelectCard->GetCardNumber();
                    for (auto card: SelectCards)
                    {
                        if (GetSuit(card) != GetSuit(selectcard))
                        {
                            bSameSuit = false;
                            break;
                        }
                    }
                    if (bSameSuit)
                    {
                        SelectCard->SetSelected(true);
                        if (SelectCards.size() == 2)
                            NotifySwitchEnabled(true);
                        else if (SelectCards.size() == 0)
                            setZoomSuit(GetSuit(selectcard));
                    }
                }
            }
            break;
        case SELECT_GANG:
        {
            msg::MahjongCard selectcard = SelectCard->GetCardNumber();
            for (auto card: gangcard_){
                if (card == selectcard){
                    GameLayer* layer = dynamic_cast<GameLayer*>(SceneManager::GetRoomLayer());
                    if(layer)
                        PDM->send_mahjong_choice(layer->get_roomid(), msg::MahjongActionChoice::GANG, selectcard);
                    break;
                }
            }
        }
            break;
        default:
            break;
    }
}

void NodeHandCard::SetSelectedCard(const std::vector<msg::MahjongCard>& card)
{
    selectedindex_.clear();
    std::vector<msg::MahjongCard> tempcard;
    tempcard.assign(card.begin(), card.end());
    for (int i=0; i<card_.size(); i++){
        auto iter = std::find(tempcard.begin(), tempcard.end(), card_.at(i));
        if (iter != tempcard.end() && std::find(selectedindex_.begin(), selectedindex_.end(), i) == selectedindex_.end()){
            selectedindex_.push_back(i);
            tempcard.erase(iter);
        }
        if (tempcard.size() == 0)
            break;
    }
    if (std::find(tempcard.begin(), tempcard.end(), drawcard_) != tempcard.end())
        selectedindex_.push_back(card_.size());
    RefreshCardSelected();
}


void NodeHandCard::RefreshCardSelected()
{
    for (auto child: getChildren()){
        auto button = dynamic_cast<MahjongCard*>(child);
        if (button && std::find(selectedindex_.begin(), selectedindex_.end(), button->getTag()) != selectedindex_.end()){
            button->SetSelected(true);
        }
    }
}

void NodeHandCard::SetZoom()
{
    if (selectmode_ != SELECT_DISCARD)
        setZoomIndex(-1);
    if (selectmode_ != SELECT_SWITCHCARD)
        setZoomSuit(msg::MahjongSuit::SUITNONE);
}

void NodeHandCard::SetCardEnabled()
{
    if (!IsSelf()) return;
    for (auto child: getChildren()){
        auto button = dynamic_cast<MahjongCard*>(child);
        if (button){
            msg::MahjongCard card = button->GetCardNumber();
            if (selectmode_ == SELECT_GANG)
                button->SetEnabled(std::find(gangcard_.begin(), gangcard_.end(), card) != gangcard_.end());
            else
                button->SetEnabled(GetSuit(card) != blanksuit_);
        }
    }
}

void NodeHandCard::setSelectMode(SELECT_MODE value)
{
    if (selectmode_ == value)
        return;
    selectmode_ = value;
    SetZoom();
    SetCardEnabled();
}

void NodeHandCard::NotifySwitchEnabled(bool value)
{
    MahjongRoomLayer* layer = dynamic_cast<MahjongRoomLayer*>(SceneManager::GetRoomLayer());
    if(layer)
        layer->SetSwitchEnabled(value);
}

void NodeHandCard::AddCard(msg::MahjongCard card, int32_t Index, bool IsDrawCard, bool isTing )
{
    auto button = createCard(GetHandPosition(), card);
    button->setTag(Index);
    button->set_ting(isTing);
    addChild(button);
    switch(GetDirection()){
        case NodeMahjong::NODE_DIR::NODE_RIGHT:
            if (IsDrawCard)
                AlignNode(button, TOPRIGHT, 0, 0);
            else
                AlignNode(button, BOTTOMRIGHT, 0, GetHandCardHeight() * Index / 2);
            button->setLocalZOrder(-Index);
            break;
        case NodeMahjong::NODE_DIR::NODE_LEFT:
            if (IsDrawCard)
                AlignNode(button, BOTTOMLEFT, 0, 0);
            else
                AlignNode(button, TOPLEFT, 0, GetHandCardHeight() * Index / 2);
            break;
        case NodeMahjong::NODE_DIR::NODE_DOWN:
            button->addClickEventListener(std::bind(&NodeHandCard::handCardOnClick, this, std::placeholders::_1));
            if (IsDrawCard)
                AlignNode(button, BOTTOMRIGHT, 0, 0);
            else
                AlignNode(button, BOTTOMLEFT, GetHandCardWidth() * Index, 0);
            
            if (isTing)
            {
                auto sprite = Sprite::create("select_card_tag.png");
                auto sizeCard = button->getContentSize();
                sprite->setAnchorPoint(Vec2(0, 0));
                sprite->setPosition(Vec2(14,90));
                sprite->setName("sprite_ting_tag");
                button->addChild(sprite);
            }
            break;
        case NodeMahjong::NODE_DIR::NODE_UP:
            if (IsDrawCard)
                AlignNode(button, TOPLEFT, 0, 0);
            else
                AlignNode(button, TOPRIGHT, GetHandCardWidth() * Index, 0);
            break;
        default:
            break;
    }
    
}

bool NodeHandCard::is_possible_ting(msg::MahjongCard card)
{
    bool ting = false;
    for ( int i = 0;i < possibleting_.size() ;i++ )
    {
        if ( possibleting_.at(i).has_card() && possibleting_.at(i).card() == card )
        {
            ting = true;
            break;
        }
    }
    return ting;
}


void NodeHandCard::update(const std::vector<msg::MahjongCard>& card, msg::MahjongCard DrawCard, const std::vector<msg::MahjongPossibleTing>& PossibleTing, msg::MahjongSuit blanksuit, SELECT_MODE selectmode, const std::vector<msg::MahjongCard>& gangcard)
{
    bool bNeedUpdate = false;
    bNeedUpdate = drawcard_ != DrawCard || card_.size() != card.size();
    if (!bNeedUpdate)
        for (int i=0; i < card.size(); i++)
            if (card.at(i) != card_.at(i)){
                bNeedUpdate = true;
                break;
            }
    selectmode_ = selectmode;
    gangcard_.clear();
    blanksuit_ = blanksuit;
    for (auto iter: gangcard)
        gangcard_.push_back(iter);
    SetZoom();
    if (bNeedUpdate){
        card_.clear();
        possibleting_.clear();
        drawcard_ = DrawCard;
        selectedindex_.clear();
        for (auto iter: card)
            card_.push_back(iter);
        for (auto iter: PossibleTing)
            possibleting_.push_back(iter);
        RefreshCards();
    }
    SetCardEnabled();
}

void NodeHandCard::RefreshCards()
{
    removeAllChildren();
    Size size;
    switch(GetDirection()){
        case NodeMahjong::NODE_DIR::NODE_RIGHT:
        case NodeMahjong::NODE_DIR::NODE_LEFT:
            size.width = GetHandCardWidth();
            size.height = ((card_.size() + 1) / 2 + 1) * GetHandCardHeight() + CARD_HAND_SPACE;
            break;
        case NodeMahjong::NODE_DIR::NODE_DOWN:
        case NodeMahjong::NODE_DIR::NODE_UP:
            size.height = GetHandCardHeight();
            size.width = (card_.size() + 1) * GetHandCardWidth() + CARD_HAND_SPACE;
            break;
        default:
            break;
    }
    setContentSize(size);
    for(int i=0; i<card_.size(); i++)
    {
        AddCard(card_.at(i), i, false , is_possible_ting(card_.at(i)));
    }
    if (drawcard_ != msg::MahjongCard::INVISIBLE)
    {
        AddCard(drawcard_, (int)card_.size(), true, is_possible_ting(drawcard_));
    }
    
    RefreshCardSelected();
}

void NodeHandCard::clear()
{
    removeAllChildren();
}
//弃牌标记
void NodeDiscard::SetLastDiscard(bool value)
{
    if (HasLastDiscard_ != value){
        HasLastDiscard_  = value;
        DoSetLastDiscard();
    }
}

void NodeDiscard::DoSetLastDiscard()
{
    if (GetTotalSize() == 0) return;
    int tag = (int) GetTotalSize() - 1;
    auto button = getChildByTag(tag);
    if (button)
        dynamic_cast<MahjongCard*>(button)->SetHasMark(HasLastDiscard_);
}

void NodeDiscard::AddCard(msg::MahjongCard card, int index, bool enabled)
{
    int BaseZOrder = 10;
    int rowIndex = index / GetCardPerRow();
    int columnIndex = index % GetCardPerRow();
    int DoubleSize = GetTotalSize() - GetCardPerRow() * GetTotalRow() - 1;
    int offset = 0;
    
    auto button = createCard(index < DoubleSize ? GetDoubleOutPosition() : GetOutPosition(), card);
    if (index <= GetTotalSize() - GetCardPerRow() * GetTotalRow())
        button->setTag(index + GetCardPerRow() * GetTotalRow());
    else
        button->setTag(index);
    addChild(button);
    
    switch (GetDirection())
    {
        case NodeMahjong::NODE_DIR::NODE_LEFT:
            if (DoubleSize >= 0 && DoubleSize >= rowIndex * GetCardPerRow() && index <= DoubleSize)
                offset = (columnIndex + 1) * HORZ_OUT_HEIGHT - HORZ_DOUBLE_OUT_HEIGHT;
            else
                offset = columnIndex * HORZ_OUT_HEIGHT;
            
            AlignNode(button, TOPRIGHT, HORZ_OUT_WIDTH * rowIndex, offset);
            button->setLocalZOrder(BaseZOrder + columnIndex);
            break;
        case NodeMahjong::NODE_DIR::NODE_UP:
            //if (DoubleSize >= 0 && DoubleSize >= columnIndex)
            //    offset = (rowIndex + 1) * VERT_OUT_HEIGHT - VERT_DOUBLE_OUT_HEIGHT;
            //else
            offset = rowIndex * VERT_OUT_HEIGHT;
            
            AlignNode(button, BOTTOMRIGHT, VERT_OUT_WIDTH * columnIndex, offset);
            button->setLocalZOrder(BaseZOrder - rowIndex);
            break;
        case NodeMahjong::NODE_DIR::NODE_RIGHT:
            //if (DoubleSize >= 0 && DoubleSize >= rowIndex * GetCardPerRow())
            //    offset = (columnIndex + 1) * HORZ_OUT_HEIGHT - HORZ_DOUBLE_OUT_HEIGHT;
            //else
            offset = columnIndex * HORZ_OUT_HEIGHT;
            
            AlignNode(button, BOTTOMLEFT, HORZ_OUT_WIDTH * rowIndex, offset);
            button->setLocalZOrder(BaseZOrder - columnIndex);
            break;
        case NodeMahjong::NODE_DIR::NODE_DOWN:
            if (DoubleSize >= 0 && DoubleSize >= columnIndex && index <= DoubleSize)
                offset = (rowIndex + 1) * VERT_OUT_HEIGHT - VERT_DOUBLE_OUT_HEIGHT;
            else
                offset = rowIndex * VERT_OUT_HEIGHT;
            
            AlignNode(button, TOPLEFT, VERT_OUT_WIDTH* columnIndex, offset);
            button->setLocalZOrder(BaseZOrder + rowIndex);
            break;
        default:
            break;
    }
    button->setEnabled(enabled);
}

void NodeDiscard::update(const std::vector<msg::MahjongCard>& card){
    bool bNeedUpdate = false;
    bNeedUpdate = dis_cards_.size() != card.size();
    if (!bNeedUpdate)
        for (int i=0; i < card.size(); i++)
            if (card.at(i) != dis_cards_.at(i)){
                bNeedUpdate = true;
                break;
            }
    if (!bNeedUpdate)
        return;
    
    dis_cards_.clear();
    for (int i=0; i<card.size(); i++)
        dis_cards_.push_back(card[i]);
    removeAllChildren();
    for (int i=0; i<dis_cards_.size(); i++){
        if (i >= GetTotalRow() * GetCardPerRow())
            break;
        if (dis_cards_.size() > i + GetTotalRow() * GetCardPerRow())
            AddCard(dis_cards_.at(i + GetTotalRow() * GetCardPerRow()), i);
        else
            AddCard(dis_cards_.at(i), i);
    }
    DoSetLastDiscard();
}

void NodeDiscard::clear()
{
    removeAllChildren();
}

void NodeFu::SetLastDiscard(bool value)
{
    if (HasLastDiscard_ != value){
        HasLastDiscard_  = value;
        DoSetLastDiscard();
    }
}

void NodeFu::DoSetLastDiscard()
{
    auto button = getChildByTag(LastCardTag);
    if (button)
        dynamic_cast<MahjongCard*>(button)->SetHasMark(HasLastDiscard_);
}

void NodeFu::update(const ::google_public::protobuf::RepeatedPtrField<msg::MahjongFu>& fu){
    
    bool bNeedUpdate = false;
    bNeedUpdate = data.size() != fu.size();
    if (!bNeedUpdate)
    {
        for (int i = 0; i < data.size(); i++)
        {
            auto iterFuOld = data.Get(i);
            auto iterFuNew = fu.Get(i);
            
            if (iterFuOld.card().size() != iterFuNew.card().size())
            {
                bNeedUpdate = true;
            }else
            {
                for (int j = 0; j < iterFuOld.card().size(); j++)
                {
                    if (iterFuOld.card(j) != iterFuNew.card(j))
                    {
                        bNeedUpdate = true;
                        break;
                    }
                }
            }
            if (bNeedUpdate)
                break;
        }
    }
    if (!bNeedUpdate)
    {
        return;
    }
    
    removeAllChildren();
    data.CopyFrom(fu);
    
    int HorzNum = 0;
    int VertNum = 0;
    bool hasDoubleHorz = false;
    for(auto iterFu : data)
    {
        switch(iterFu.type())
        {
            case msg::MahjongFuType::SHUN:
            case msg::MahjongFuType::KE:
                HorzNum++;
                VertNum+=2;
                break;
            case msg::MahjongFuType::MINGGANG:
                HorzNum++;
                VertNum+=3;
                break;
            case msg::MahjongFuType::ANGANG:
                VertNum+=4;
                break;
            case msg::MahjongFuType::BUGANG:
                HorzNum++;
                VertNum+=2;
                hasDoubleHorz = true;
                break;
            default:
                break;
        }
    }
    
    Size SizeAll;
    switch(GetDirection()){
        case NodeMahjong::NODE_DIR::NODE_RIGHT:
        case NodeMahjong::NODE_DIR::NODE_LEFT:
            SizeAll.height += HORZ_OUT_HEIGHT * VertNum + VERT_OUT_HEIGHT * HorzNum + MAX_FU_SPACE * (data.size() - 1);
            SizeAll.width = MAX(HORZ_OUT_WIDTH, VERT_OUT_WIDTH * (hasDoubleHorz ? 2 : 1));
            break;
        case NodeMahjong::NODE_DIR::NODE_DOWN:
        case NodeMahjong::NODE_DIR::NODE_UP:
            SizeAll.width += VERT_OUT_WIDTH * VertNum + HORZ_OUT_WIDTH * HorzNum + MAX_FU_SPACE * (data.size() - 1);
            SizeAll.height = MAX(VERT_OUT_HEIGHT, HORZ_OUT_HEIGHT * (hasDoubleHorz ? 2 : 1));
            break;
        default:
            break;
    }
    setContentSize(SizeAll);
    
    CurrHorzNum = 0;
    CurrVertNum = 0;
    CurrFuNum = 0;
    for(int i=0; i<data.size(); i++)
    {
        auto iterFu = data.Get(i);
        msg::MahjongCard cardIndex = iterFu.getcard();
        msg::MahjongFuType cardType = iterFu.type();
        int cardPosition = iterFu.source();
        switch(cardType)
        {
            case msg::MahjongFuType::KE: //碰
            {
                for(int j = MAX_FU_PENG - 1; j>=0; j--)
                {
                    if((j+1) == cardPosition){
                        CLASS_CARD classCard = GetPengHorzPosition();
                        AddCard(classCard, cardIndex, i == data.size() - 1);
                    }
                    else{
                        CLASS_CARD classCard = GetPengPosition();
                        AddCard(classCard, cardIndex, false);
                    }
                }
                break;
            }
            case msg::MahjongFuType::BUGANG://补杠
            {
                for(int j = MAX_FU_PENG - 1; j>=0; j--)
                {
                    CLASS_CARD classCard = GetPengPosition();
                    if((j+1) == cardPosition)
                    {
                        classCard = GetPengHorzPosition();
                        AddCard(classCard, cardIndex, false, true);
                        AddCard(classCard, cardIndex, false);
                    }
                    else
                        AddCard(classCard, cardIndex, false);
                }
                break;
            }
            case msg::MahjongFuType::MINGGANG://明杠
            {
                //杠别人家
                if(cardPosition == 3)
                {
                    cardPosition = 4;//杠上家
                }
                for(int j = MAX_FU_GANG - 1; j>=0; j--)
                {
                    if((j+1) == cardPosition){
                        CLASS_CARD classCard = GetPengHorzPosition();
                        AddCard(classCard, cardIndex, i == data.size() - 1);
                    }
                    else{
                        CLASS_CARD classCard = GetPengPosition();
                        AddCard(classCard, cardIndex, false);
                    }
                }
                break;
            }
            case msg::MahjongFuType::ANGANG: //暗杠
            {
                for(int j = MAX_FU_GANG - 1; j>=0; j--)
                {
                    CLASS_CARD classCard = GetBackPosition();
                    if(j == 0 )
                        classCard = GetPengPosition();
                    AddCard(classCard, cardIndex, false);
                }
                break;
            }
            default:
                break;
        }
        CurrFuNum++;
    }
    DoSetLastDiscard();
}

void NodeFu::clear()
{
    removeAllChildren();
}

void NodeFu::AddCard(CLASS_CARD type, msg::MahjongCard card, bool IsLast, bool IsBuGang){
    int BaseZOrder = 100;
    bool IsHorz = type == CLASS_CARD::EAST_HAND_PENG_POSITION ||
    type == CLASS_CARD::WEST_HAND_PENG_POSITION ||
    type == CLASS_CARD::NORTH_HAND_PENG_POSITION ||
    type == CLASS_CARD::SOUTH_HAND_PENG_POSITION;
    
    auto Card = createCard(type, card);
    addChild(Card);
    if (IsLast)
        Card->setTag(LastCardTag);
    Card->setLocalZOrder(BaseZOrder);
    switch (GetDirection()){
        case NodeMahjong::NODE_DIR::NODE_LEFT:{
            int offset = VERT_OUT_HEIGHT * CurrHorzNum + HORZ_OUT_HEIGHT * CurrVertNum + MAX_FU_SPACE * CurrFuNum;
            AlignNode(Card, BOTTOMLEFT, IsBuGang ? VERT_OUT_WIDTH : 0, offset);
            Card->setLocalZOrder(BaseZOrder - CurrVertNum - CurrHorzNum);
            break;
        }
        case NodeMahjong::NODE_DIR::NODE_UP:{
            int offset = VERT_OUT_WIDTH * CurrVertNum + HORZ_OUT_WIDTH * CurrHorzNum + MAX_FU_SPACE * CurrFuNum;
            AlignNode(Card, TOPLEFT, offset, IsBuGang ? HORZ_OUT_HEIGHT : 0);
            if (IsBuGang)
                Card->setLocalZOrder(BaseZOrder + 10);
            break;
        }
        case NodeMahjong::NODE_DIR::NODE_RIGHT:{
            int offset = VERT_OUT_HEIGHT * CurrHorzNum + HORZ_OUT_HEIGHT * CurrVertNum + MAX_FU_SPACE * CurrFuNum;
            AlignNode(Card, TOPRIGHT, IsBuGang ? VERT_OUT_WIDTH : 0, offset);
            break;
        }
        case NodeMahjong::NODE_DIR::NODE_DOWN:{
            int offset = VERT_OUT_WIDTH * CurrVertNum + HORZ_OUT_WIDTH * CurrHorzNum + MAX_FU_SPACE * CurrFuNum;
            AlignNode(Card, BOTTOMRIGHT, offset, IsBuGang ? HORZ_OUT_HEIGHT : 0);
            break;
        }
        default:
            break;
    }
    if (!IsBuGang){
        if (IsHorz)
            CurrHorzNum++;
        else
            CurrVertNum++;
    }
}

void NodeDeck::update(int num)
{
    removeAllChildren();
    for(int i = 0; i < num; ++i){
        switch (GetDirection()){
            case NodeMahjong::NODE_DIR::NODE_LEFT:
            {
                auto card = createCard(CLASS_CARD::WEST_DI, msg::MahjongCard::INVISIBLE);
                auto sizeCard= card->getBoundingBox().size;
                card->setTag(i);
                card->setPosition(Vec2(0, (sizeCard.height - 26) * i));
                addChild(card);
                break;
            }
            case NodeMahjong::NODE_DIR::NODE_UP:
            {
                auto card = createCard(CLASS_CARD::NORTH_DI, msg::MahjongCard::INVISIBLE);
                auto sizeCard = card->getBoundingBox().size;
                card->setTag(i);
                card->setPosition(Vec2((sizeCard.width - 45) * i, 0));
                addChild(card);
                break;
            }
            case NodeMahjong::NODE_DIR::NODE_RIGHT:
            {
                auto card = createCard(CLASS_CARD::EAST_DI, msg::MahjongCard::INVISIBLE);
                auto sizeCard = card->getBoundingBox().size;
                card->setTag(i);
                card->setPosition(Vec2(0, (sizeCard.height - 26) * i));
                addChild(card);
                break;
            }
            case NodeMahjong::NODE_DIR::NODE_DOWN:
            {
                auto card = createCard(CLASS_CARD::SOUTH_DI, msg::MahjongCard::INVISIBLE);
                auto sizeCard = card->getBoundingBox().size;
                card->setTag(i);
                card->setPosition(Vec2((sizeCard.width - 45) * i, 0));
                addChild(card);
                break;
            }
            default:
                break;
        }
    }
}

void NodeHu::update(const ::google_public::protobuf::RepeatedPtrField<msg::MahjongScoreChange>& scorechange)
{
    std::vector<msg::MahjongScoreChange> scorechangeWithHu;
    for (auto iter: scorechange)
        if (iter.has_hucard())
            scorechangeWithHu.push_back(iter);
    bool bNeedUpdate = false;
    bNeedUpdate = data.size() != scorechangeWithHu.size();
    if (!bNeedUpdate)
    {
        for (int i = 0; i < data.size(); i++)
        {
            if (data.at(i).hucard() != scorechangeWithHu.at(i).hucard())
                bNeedUpdate = true;
            break;
        }
    }
    if (!bNeedUpdate)
    {
        return;
    }
    
    removeAllChildren();
    data.assign(scorechangeWithHu.begin(), scorechangeWithHu.end());
    
    for (int i=0; i< scorechangeWithHu.size(); i++){
        if (i >= GetTotalRow() * GetCardPerRow())
            break;
        if (scorechangeWithHu.size() > i + GetTotalRow() * GetCardPerRow()){
            AddCard(scorechangeWithHu.at(i + GetTotalRow() * GetCardPerRow()).hucard(), i, !scorechangeWithHu.at(i + GetTotalRow() * GetCardPerRow()).multiwin());
        }
        else{
            AddCard(scorechangeWithHu.at(i).hucard(), i, !scorechangeWithHu.at(i ).multiwin());
        }
    }
    
    if (scorechangeWithHu.size() > 0){
        auto size = getContentSize();
        if(size.width > 0 && size.height > 0)
        {
            auto imageBg = ImageView::create("image_hu_card_bg.png");
            imageBg->setScale9Enabled(true);
            imageBg->setCapInsets(Rect(22, 24, 25, 25));
            imageBg->setContentSize(Size(size.width + 8, size.height + 8));
            addChild(imageBg, -1);
            AlignNode(imageBg, MIDDLE, 0, 0);
        }
    }
    
}
void NodeHu::clear()
{
    removeAllChildren();
}

