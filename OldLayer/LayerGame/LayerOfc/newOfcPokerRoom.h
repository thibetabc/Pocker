//
//  newOfcPokerRoom.h
//  pokerclient
//
//  Created by ll on 15/10/29.
//
//

#ifndef __pokerclient__newOfcPokerRoom__
#define __pokerclient__newOfcPokerRoom__

#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "cmd_def.h"
#include "user_data.h"
#include "BaseLayer.h"
#include "cocos-ext.h"
#include "events.h"
#include "Room.h"
#include "GameLayer.h"
#include "NodeOfcPlayerOwn.h"

#include "newOfcPokerRoomFantasy.h"
#include "OfcBackGroundLayer.h"

using namespace cocos2d;
using namespace cocos2d::ui;
class NodeOfcPlayer;
class OFCRoom;



class newOfcPokerRoom : public GameLayer
{
public:
    static newOfcPokerRoom* Create(uint32_t ARoomID, bool AIsRecord)
    {
        newOfcPokerRoom *pRet = new(std::nothrow) newOfcPokerRoom();
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
    void handle_android_back()
    {
        OfcBackGroundLayer* layer_background_ = dynamic_cast<OfcBackGroundLayer*>(root_->getChildByName("BACKGROUND_LAYER_TAG"));
        layer_background_->click_btn_quit(nullptr);
    }
    virtual bool init();
    void PositionUpdate();
    void PositionUpdate(bool isRefreshAll);
    const std::shared_ptr<OFCRoom> GetRoom();
    
    void click_btn_start(Ref * sender);
    
    void update_record_status();
    void update_game_status(const msg::GameStatus game_status);
    void UpdatePlayerAction(int userid, int countdown, bool is_using_timebank = false);
    void handle_update_dealer();
    void receive_game_start();

    void receive_game_end();
    void update_ofc_send_player_cards_success(bool isSuccess);
    void receive_ofc_prop_update();
    void receive_table_note(std::string note);

    void receive_ofc_result_score();
    void handle_ofc_receive_result_type();
    void handle_ofc_receive_result_compare();
    void show_history_layer();
  
    void init_auto_buttons(msg::OFCPlayerSetting& setting);
private:
    
    void register_events();

    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
private:
    void setViewOffset(bool isOn, bool isRefreshAll);
    void update_viewoffset();
 
    void OpenTestFantasy(bool isOpen = false);
    Node* root_;
    
    events::event_dispatch dispatch_;
    
    std::vector<NodeOfcPlayer*> player_vec_;
    NodeOfcPlayerOwn* player_own_;
    Button* btn_start_;
    bool isGameOver_;
    int32_t isFantasy_;

    std::vector<cocos2d::Point> player_node_normal_ = {cocos2d::Point(80, 1000), cocos2d::Point(80, 700), cocos2d::Point(80, 400), cocos2d::Point(80, 100)};
    std::vector<cocos2d::Point> player_node_in_game_ = {cocos2d::Point(80, 1030),cocos2d::Point(80, 775),cocos2d::Point(80, 520), cocos2d::Point(80, 260)};
};

#endif /* defined(__pokerclient__newOfcPokerRoom__) */