//
//  NodeMahjong.h
//  pokerclient
//
//  Created by HuangFeng on 16/6/3.
//
//

#ifndef NodeMahjong_h
#define NodeMahjong_h

#include "global_header.h"
#include "MahjongCommon.h"
#include "GameDataManager.h"

using namespace cocos2d;
using namespace cocos2d::ui;

class NodeDiscard;
class NodeHandCard;
class NodeFu;
class NodeHu;

class NodeMahjong : public Node
{
public:
    CREATE_FUNC(NodeMahjong);
    virtual bool init();
    enum NODE_DIR
    {
        NODE_RIGHT,
        NODE_LEFT,
        NODE_DOWN,
        NODE_UP
    };
    
    void update(msg::MahjongPlayerInfo* mahjongPlayerInfo, bool IsPlaying , bool is_own_exist = false);

    void setDirectoion(NODE_DIR dir) {Direction_ = dir;}
    NODE_DIR getDirection() {return Direction_;}
    uint32_t getUserId(){return user_id_;};
    void ShowAction(msg::MahjongActionChoice Action);
    void ShowAction(int ScoreChange);
    void SetLastCard(bool HasLastCard, msg::MahjongCardZone zone);
    void refresh_user_status(msg::SittingStatus status);
    void set_click_btn_sit_callback(std::function<void()> callback){ call_back_ = callback; };
    void clear();
    NodeHandCard* GetHandNode() {return handcard;}
    void set_switch_callback(std::function<void()> callback){ switch_callback_ =  callback;};
private:
#pragma mark - 控件操作
    void ShowHeadPic(const std::string& Picname, const std::string& NickName, int score, msg::MahjongSuit BlankSuit, bool IsPlaying);
private:
    void handle_download_event(cmd_data_pointer data);
    NODE_DIR Direction_;
    std::function<void()> call_back_;
    std::function<void()> switch_callback_;
    NodeHandCard* handcard;
    NodeDiscard* discards;
    NodeFu* Fu;
    NodeHu* mHuCard;
    Button* btn_sit_ = nullptr;
    //头像
    ImageView* player_bg_ = nullptr;
    ShaderSprite* sprite_photo_ = nullptr;
    Text* text_player_nickname_ = nullptr;
    Text* text_player_status_ = nullptr;
    Text* text_player_score_ = nullptr;
    Sprite* sprite_lack_ = nullptr;
    Text* text_lack_ = nullptr;
    events::event_dispatch dispatch_;
    std::string pic_name_;
    uint32_t user_id_ = 0;
    
    //飘过的信息
    Sprite* player_action_ = nullptr ;
    TextAtlas *text_change_score_ = nullptr ;
    Sprite* score_mark = nullptr;
    Sprite* choose_switch_card_ = nullptr;
};

class NodeDirection: public Node{
public:
    CREATE_FUNC(NodeDirection);
    void resetCountdown();
    void setCountdown(NodeMahjong::NODE_DIR dir, int time, bool IsTimeBank);
    int getCountDown(){ return mCountdown; }
    void scheduleCountdown(float dt);
    void setEastDirection(NodeMahjong::NODE_DIR dir);
    void setCountdownVisible(bool isVisible);
    virtual bool init();
private:
    //底座方向
    Sprite *mSpriteUp;
    Sprite *mSpriteDown;
    Sprite *mSpriteLeft;
    Sprite *mSpriteRight;
    NodeMahjong::NODE_DIR eastdir_ = NodeMahjong::NODE_DIR::NODE_DOWN;
    Sprite *mSpriteTagRight;
    Sprite *mSpriteTagLeft;
    Sprite *mSpriteTagUp;
    Sprite *mSpriteTagDown;
    TextAtlas *mTextAtlasCountdown;
    int mCountdown;
};

class MahjongChildNode: public Node{
protected:
    CLASS_CARD GetHandPosition();
    CLASS_CARD GetBackPosition();
    CLASS_CARD GetOutPosition();
    CLASS_CARD GetPengPosition();
    CLASS_CARD GetDoubleOutPosition();
    CLASS_CARD GetPengHorzPosition();
    int32_t GetHandCardWidth();
    int32_t GetHandCardHeight();
    NodeMahjong::NODE_DIR GetDirection(){ return static_cast<NodeMahjong*>(getParent())->getDirection();}
    bool IsSelf() {return static_cast<NodeMahjong*>(getParent())->getUserId() == GDM->get_user_id();}
};

class NodeHandCard: public MahjongChildNode{
public:
    enum SELECT_MODE{
        SELECT_NULL,
        SELECT_NONE,
        SELECT_DISCARD,
        SELECT_SWITCHCARD,
        SELECT_GANG
    };
    CREATE_FUNC(NodeHandCard);
    void update(const std::vector<msg::MahjongCard>& card, msg::MahjongCard DrawCard, const std::vector<msg::MahjongPossibleTing>& PossibleTing, msg::MahjongSuit blanksuit, SELECT_MODE selectmode, const std::vector<msg::MahjongCard>& gangcard);
    void setSelectMode(SELECT_MODE value);
    SELECT_MODE getSelectMode() {return selectmode_;}
    std::vector<msg::MahjongCard>& GetGangCard() {return gangcard_;}
    void SetSelectedCard(const std::vector<msg::MahjongCard>& card);
    std::vector<msg::MahjongCard> GetSelectedCards();
    void clear();
    bool is_possible_ting(msg::MahjongCard card);
private:
    void setZoomSuit(msg::MahjongSuit Value);
    void setZoomIndex(int Value);
    void SetAllCardsUnselected();
    void NotifySwitchEnabled(bool value);
    void AddCard(msg::MahjongCard card, int32_t Index, bool IsDrawCard, bool isTing = false);
    void SetZoom();
    void RefreshCards();
    void RefreshCardSelected();
    void SetCardEnabled();
    std::vector<msg::MahjongCard> card_;
    std::vector<msg::MahjongCard> gangcard_;
    std::vector<u_long> selectedindex_;
    std::vector<msg::MahjongPossibleTing> possibleting_;
    msg::MahjongCard drawcard_;
    msg::MahjongSuit blanksuit_;
    msg::MahjongSuit zoomsuit_ = msg::MahjongSuit::SUITNONE;
    int zoomindex_ = -1;
    void handCardOnClick(Ref *ref);
    SELECT_MODE selectmode_ = SELECT_NULL;
};

class NodeDiscard: public MahjongChildNode{
public:
    CREATE_FUNC(NodeDiscard);
    void update(const std::vector<msg::MahjongCard>& card);
    virtual int GetCardPerRow() {return 6;}
    virtual int GetTotalRow() {return 4;}
    void clear();
    void SetLastDiscard(bool value);
protected:
    virtual int GetTotalSize() {return (int)dis_cards_.size();}
    void AddCard(msg::MahjongCard card, int Index, bool enabled = true);
    void DoSetLastDiscard();
private:
    bool HasLastDiscard_ = false;
    std::vector<msg::MahjongCard> dis_cards_;
};

class NodeFu: public MahjongChildNode{
public:
    CREATE_FUNC(NodeFu);
    void update(const ::google_public::protobuf::RepeatedPtrField<msg::MahjongFu>& fu);
    void clear();
    void SetLastDiscard(bool value);
private:
    static const int LastCardTag = 100;
    void DoSetLastDiscard();
    bool HasLastDiscard_ = false;
    void AddCard(CLASS_CARD type, msg::MahjongCard card, bool IsLast, bool IsBuGang = false);
    ::google_public::protobuf::RepeatedPtrField<msg::MahjongFu> data;
    int CurrFuNum = 0;
    int CurrHorzNum = 0;
    int CurrVertNum = 0;
};

class NodeDeck: public MahjongChildNode{
public:
    CREATE_FUNC(NodeDeck);
    void update(int num);
private:
    int num_;
};

class NodeHu: public NodeDiscard{
public:
    CREATE_FUNC(NodeHu);
    void clear();
    void update(const ::google_public::protobuf::RepeatedPtrField<msg::MahjongScoreChange>& card);
    int GetCardPerRow() {return 4;}
    int GetTotalRow() {return 3;}
protected:
    int GetTotalSize() {return (int)data.size();}
private:
    std::vector<msg::MahjongScoreChange> data;
};

#endif /* NodeMahjong_h */



