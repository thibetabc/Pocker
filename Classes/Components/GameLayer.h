//
//  GameLayer.h
//  pokerclient
//
//  Created by admin on 15-12-23.
//
//

#ifndef __pokerclient__GameLayer__
#define __pokerclient__GameLayer__

#include <stdio.h>
#include "msg.pb.h"
#include "ReconnectRoomView.h"
#include "MatchScroeNode.h"
#include "SceneManager.h"


using namespace cocos2d;

class PokerRoom;
class OFCRoom;
class MahjongRoom;

class GameLayer : public BaseLayer
{
public:
    virtual void PositionUpdate() = 0;
    virtual void UpdatePlayerAction(int userid, int countdown, bool is_using_timebank = false) = 0;
    virtual void receive_game_start() = 0;
    virtual void receive_game_end() = 0;
    virtual void receive_table_note(std::string note) = 0;
    virtual void show_history_layer() = 0;
    virtual void update_game_status(const msg::GameStatus game_status) = 0;
    void ShowInGameScore(const msg::Processor_420_PokerGetScore_DOWN& processor);
    virtual void handle_connect_break(const std::shared_ptr<server_status_data>& data);
    virtual void handle_connect_success(const std::shared_ptr<server_status_data>& data);
    const std::string GetName();
    const uint32_t get_roomid(){return RoomID;}
    const bool get_isrecord(){return isRecord;}
    MatchScroeNode* GetScoreLayer();
    virtual void onExit();
protected:
    int GetServerID() {return net_manager::SERVER_ROOM;}
    uint32_t RoomID;
    bool isRecord;
};

class GameChildNode : public Node
{
public:
    virtual void Refresh() {}
    void onEnterTransitionDidFinish();
};

class GameChildLayer : public BaseLayer
{
public:
    virtual void Refresh() {}
    void onEnterTransitionDidFinish();
protected:
    int GetServerID() {return net_manager::SERVER_ROOM;}
};

class PokerChildNode : public GameChildNode
{
public:
    const std::shared_ptr<PokerRoom> GetRoom();
};

class PokerChildLayer : public GameChildLayer
{
public:
    const std::shared_ptr<PokerRoom> GetRoom();
};

class OFCChildNode : public GameChildNode
{
public:
    const std::shared_ptr<OFCRoom> GetRoom();
};

class OFCChildLayer : public GameChildLayer
{
public:
    const std::shared_ptr<OFCRoom> GetRoom();
};

class MahjongChildLayer : public GameChildLayer
{
public:
    const std::shared_ptr<MahjongRoom> GetRoom();
};

#endif /* defined(__pokerclient__GameLayer__) */
