//
//  GameLayer.cpp
//  pokerclient
//
//  Created by admin on 15-12-23.
//
//

#include "GameLayer.h"
#include "MatchScroeNode.h"
#include "PokerRoomLayer.h"
#include "newOfcPokerRoom.h"
#include "MahjongRoomLayer.h"
#include "SceneManager.h"
#include "net_manager.h"
#include "PokerDataManager.h"
#include "utils.h"

#define MATCHLAYERTAG 1000
#define ZORDER_UI  1002
#define RECONNECTLAYERTAG 2000

void GameLayer::ShowInGameScore(const msg::Processor_420_PokerGetScore_DOWN& processor)
{
    auto msn = MatchScroeNode::create();
    msn->handle_get_score(processor);
    addChild(msn, ZORDER_UI);
    msn->setTag(MATCHLAYERTAG);
}

const std::string GameLayer::GetName(){
    return NAME_LAYERGAMEROOM;
}

MatchScroeNode* GameLayer::GetScoreLayer()
{
    return static_cast<MatchScroeNode*>(getChildByTag(MATCHLAYERTAG));
}


void GameLayer::handle_connect_break(const std::shared_ptr<server_status_data>& data)
{
    if (data || data->serverid != net_manager::SERVER_ROOM) return;
    if(!getChildByTag(RECONNECTLAYERTAG))
    {
        ReconnectRoomView* reconnectlayer = ReconnectRoomView::create();
        addChild(reconnectlayer, INT_MAX, RECONNECTLAYERTAG);
    }
    static_cast<ReconnectRoomView*>(getChildByTag(RECONNECTLAYERTAG))->update_note(tools::local_string("reconnect","重连"));
    static_cast<ReconnectRoomView*>(getChildByTag(RECONNECTLAYERTAG))->showbutton(false);
}

void GameLayer::handle_connect_success(const std::shared_ptr<server_status_data>& data)
{
    if (data->serverid != net_manager::SERVER_ROOM) return;
    if(getChildByTag(RECONNECTLAYERTAG))
    {
        getChildByTag(RECONNECTLAYERTAG)->removeFromParent();
    }
}

void GameChildNode::onEnterTransitionDidFinish()
{
    Node::onEnterTransitionDidFinish();
    Refresh();
}

void GameChildLayer::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();
    Refresh();
}

const std::shared_ptr<PokerRoom> PokerChildNode::GetRoom()
{
    Node* node = dynamic_cast<Node*>(this);
    while (node != nullptr && dynamic_cast<PokerRoomLayer*>(node) == nullptr){
        node = node->getParent();
    }
    assert(node != nullptr);
    return dynamic_cast<PokerRoomLayer*>(node)->GetRoom();
}

const std::shared_ptr<PokerRoom> PokerChildLayer::GetRoom()
{
    Node* node = dynamic_cast<Node*>(this);
    while (node != nullptr && dynamic_cast<PokerRoomLayer*>(node) == nullptr){
        node = node->getParent();
    }
    assert(node != nullptr);
    return dynamic_cast<PokerRoomLayer*>(node)->GetRoom();
}

const std::shared_ptr<OFCRoom> OFCChildNode::GetRoom()
{
    Node* node = dynamic_cast<Node*>(this);
    while (node != nullptr && dynamic_cast<newOfcPokerRoom*>(node) == nullptr){
        node = node->getParent();
    }
    assert(node != nullptr);
    return dynamic_cast<newOfcPokerRoom*>(node)->GetRoom();
}

const std::shared_ptr<OFCRoom> OFCChildLayer::GetRoom()
{
    Node* node = dynamic_cast<Node*>(this);
    while (node != nullptr && dynamic_cast<newOfcPokerRoom*>(node) == nullptr){
        node = node->getParent();
    }
    assert(node != nullptr);
    return dynamic_cast<newOfcPokerRoom*>(node)->GetRoom();
}
const std::shared_ptr<MahjongRoom> MahjongChildLayer::GetRoom()
{
    Node* node = dynamic_cast<Node*>(this);
    while (node != nullptr && dynamic_cast<MahjongRoomLayer*>(node) == nullptr){
        node = node->getParent();
    }
    assert(node != nullptr);
    return dynamic_cast<MahjongRoomLayer*>(node)->GetRoom();
}

void GameLayer::onExit()
{
    BaseLayer::onExit();
//    log("退出游戏---删除图片资源前: %s", Director::getInstance()->getTextureCache()->getCachedTextureInfo().c_str());
    Director::getInstance()->purgeCachedData();
    Director::getInstance()->getTextureCache()->removeAllTextures();
//    log("退出游戏---删除图片资源后: %s", Director::getInstance()->getTextureCache()->getCachedTextureInfo().c_str());
}