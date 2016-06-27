//
//  MahjongLayer.hpp
//  pokerclient
//
//  Created by zhongyukeji on 16/5/30.
//
//

#ifndef MahjongLayer_hpp
#define MahjongLayer_hpp

#include <stdio.h>
#include "Room.h"
#include "NodeMahjong.h"


using namespace cocos2d;
using namespace cocos2d::ui;
class MahjongRoom;

class MahjongRoomLayer : public GameLayer{
public:
    MahjongRoomLayer();
    ~MahjongRoomLayer();
    static MahjongRoomLayer* Create(uint32_t ARoomID, bool AIsRecord)
    {
        MahjongRoomLayer *pRet = new(std::nothrow) MahjongRoomLayer();
        pRet->RoomID = ARoomID;
        pRet->isRecord = AIsRecord;
        if (pRet && pRet->init())
        {
            pRet->autorelease();
            return pRet;
        }
        else
        {
            delete pRet;
            pRet = NULL;
            return NULL;
        }
    }
    virtual bool init();
    const std::shared_ptr<MahjongRoom> GetRoom();
    
    void PositionUpdate();
    void UpdatePlayerAction(int userid, int countdown, bool is_using_timebank = false) {}
    void receive_game_start();
    void receive_game_end();
    void receive_table_note(std::string note);
    void show_history_layer() {}
    void update_game_status(const msg::GameStatus game_status);
    void init_auto_buttons(const msg::MahjongPlayerSetting& setting);
    void receive_player_action(const msg::MahjongAction& action);
    void receive_switch_type(const msg::MahjongCardSwitchType& switchtype);
    void prop_update();
    void sitdownAddClickListener(Ref *ref);
    void beginGameAddClickListener(Ref *ref);
    void handCardAddClickListener(Ref *ref);
    void updateMahjongPlayerInfo(msg::MahjongPlayerInfo *mahjongPlayerInfo, NodeMahjong::NODE_DIR sitDir);
    void update_poker_menu_status();
    void add_player_node(NodeMahjong::NODE_DIR sitDir);
    void SetSwitchEnabled(bool value);
    void click_hand_ting(msg::MahjongCard card);
    void remove_ting_layer();
    void show_choose_switch_card();
    void remove_layers();
private:
    
    //Direction
    NodeDirection* mNodeMahjongDirection;

    Node* root_;
    Button* btn_pull_down_;
    
    Button* btn_menu_;
    Button* btn_voice_;
    Button* btn_on_the_water_;
    Button* btn_reminder_;
    Button* btn_auto_card_;
    Button* btn_auto_play_;
    Text *mTextRoomId;
    const int CARD_HEIGHT = 12;
    const int MAHJONG_TAG = 20160614; //麻将tag
    ImageView* image_mahjong_mark_;
    Text* text_mahjong_num_;
    std::string record_filename_;

    Text* text_switch_tip_ = nullptr;
    ImageView* image_huanpai_tip_ = nullptr;
    ImageView* image_huanpai_bg_ = nullptr;
    
    Button* btn_start_;
    bool is_auto_card_ = false;
    bool is_auto_discard = false;
    msg::MahjongPlayerSetting setting_;
    std::vector<msg::MahjongTing> ting_;
    std::vector<msg::MahjongPossibleTing> possible_ting_;
    std::vector<Button*> sit_btn_;
    std::vector<NodeMahjong*> user_mahjong_node_;
    Button *mButtonHu;
    Button *mButtonPeng;
    Button *mButtonGang;
    Button *mButtonPass;
    Layout *mLayoutSuit;
    Button *mButtonTiao;
    Button *mButtonTong;
    Button *mButtonWan;
    Button *mButtonSwichCard;
    TextAtlas *mTextAtlasSwitchCard;
    Sprite *mSpriteSwichCard;
    Sprite *mSpriteLastDiscardTag;
    Button *mShowTrusteeship;
public:
    void click_btn_pull_down(Ref* sender);
    void click_btn_voice(Ref* sender, Widget::TouchEventType event);
    void click_btn_on_the_water(Ref* sender);
    void click_btn_reminder(Ref* sender);
    void click_btn_auto_card(Ref* sender);
    void click_btn_auto_play(Ref* sender);
    void click_btn_hu(Ref *sender);
    void click_btn_peng(Ref *sender);
    void click_btn_gang(Ref *sender);
    void click_btn_pass(Ref *sender);
    void click_btn_suit(Ref *sender);
    void click_btn_switch_card(Ref *sender);
    void click_btn_trusteeship(Ref *sender);
    void setSwitchCard(int selectCount);
#pragma mark touch
    bool onTouchBegan(Touch* touch, Event* event){ return true; };
    void onTouchMoved(Touch* touch, Event* event){};
    void onTouchEnded(Touch* touch, Event* event);
private:
#pragma mark - 控件操作
    void ShowAction(bool HasHu, bool HasPeng, bool HasGang);
    void DoAction(msg::MahjongActionChoice action);
    void ShowChooseSuit(bool IsShow, msg::MahjongSuit suit);
    void ChooseSuit();
    void ShowSwitchCard(bool IsShow);
    void SendSwitchCard(std::vector<msg::MahjongCard> Cards);
    void ShowTrusteeship(bool isShow);
    int MAX_PLAYER = 4;
    NodeMahjong *getNode(uint32_t userId);
    void showDiscardTag(Node *node);
    void initData();
    NodeMahjong::NODE_DIR getPlayerDir(int sitNum);
   
};

#endif /* MahjongLayer_hpp */
