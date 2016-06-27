//
//  LayerShare.cpp
//  pokerclient
//
//  Created by HuangFeng on 16/5/21.
//
//

#include "LayerShare.h"
#include "utils.h"
#include "net_manager.h"

LayerShare::LayerShare():
title("好牌"),
imgName("icon_share.png"),
mShareType(PlatformSdk::ShareType::Web)
{
    
}

LayerShare::~LayerShare()
{
    
}


void LayerShare::setShareUserInfo(uint32_t userId)
{
    
    linkUrl = StringUtils::format("http://%s:8080/poker/share.jsp?userid=%d", net_manager::shared_instence().FileServerIPv4.c_str(), userId);
    description = "来看看这个朋友圈的约局神器，约德州大菠萝，比赛也可以";
}

void LayerShare::setShareGroupInfo(uint32_t userId, uint32_t groupId)
{
    
    linkUrl = StringUtils::format("http://%s:8080/poker/share.jsp?userid=%d&groupid=%d", net_manager::shared_instence().FileServerIPv4.c_str(), userId, groupId);
    description = "我在好牌里建了一个俱乐部，邀请你加入，快来啊！";
}

void LayerShare::setShareRoomInfo(uint32_t userId, uint32_t groupId, uint32_t roomId)
{
    
    linkUrl = StringUtils::format("http://%s:8080/poker/share.jsp?userid=%d&groupid=%d&roomid=%d", net_manager::shared_instence().FileServerIPv4.c_str(), userId, groupId, roomId);
    description = "我在好牌里创建了一个房间，快来一起玩啊！";
    
}

void LayerShare::setShareGamePokerRoomInfo(uint32_t userId, uint32_t groupId, uint32_t roomId)
{
    
    
    linkUrl = StringUtils::format("http://%s:8080/poker/share.jsp?userid=%d&groupid=%d&roomid=%d", net_manager::shared_instence().FileServerIPv4.c_str(), userId, groupId, roomId);
    if (groupId == 0)
    {
        description = "我正在房间里玩德州扑克，你不来吗？房间号：" + tools::to_string(roomId);
    }
    else
    {
        description = "我正在房间里玩德州扑克，你不来吗？";
    }
}

void LayerShare::setShareGameOfcRoomInfo(uint32_t userId, uint32_t groupId, uint32_t roomId)
{
    
    linkUrl = StringUtils::format("http://%s:8080/poker/share.jsp?userid=%d&groupid=%d&roomid=%d", net_manager::shared_instence().FileServerIPv4.c_str(), userId, groupId, roomId);
    if (groupId == 0)
    {
        description = "我正在房间里玩大菠萝，你不来吗？房间号：" + tools::to_string(roomId);
    }
    else
    {
       description = "我正在房间里玩大菠萝，你不来吗？";
    }
}

void LayerShare::setShareRecordDetail()
{
    description = "我的牌局记录";
    imgName = "share_record.png";
    
    mShareType = PlatformSdk::ShareType::Image;
}

void LayerShare::setShareRecordDetail(std::string imgPath)
{
    description = "我的牌局记录";
    imgName = imgPath;
    mShareType = PlatformSdk::ShareType::Image;
}




bool LayerShare::init()
{
    if(!BaseLayer::init())
    {
        return false;
    }
    
    auto root = CSLoader::createNode("LayerShare.csb");
    addChild(root);
    setTag(LayerShareTag);
    auto layoutBg = dynamic_cast<Layout*>(CSLoader::seekNodeByName(root, "layout_bg"));
    auto btnWeixinChat = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "btn_share_weixin_chat"));
    auto btnWeixinCircle = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "btn_share_weixin_circle"));
    auto layoutItemBg = dynamic_cast<Layout*>(CSLoader::seekNodeByName(root, "layout_content"));
    auto sizeLayoutItemBg = layoutItemBg->getContentSize();
    auto positionLayoutItemBg = layoutItemBg->getPosition();
    layoutItemBg->setPositionY(positionLayoutItemBg.y - sizeLayoutItemBg.height);
    layoutBg->setTouchEnabled(true);
    layoutBg->addClickEventListener([&,layoutItemBg, positionLayoutItemBg, sizeLayoutItemBg](Ref *ref)
    {
        auto sequeue = Sequence::createWithTwoActions(MoveTo::create(0.1f, Vec2(positionLayoutItemBg.x, positionLayoutItemBg.y - sizeLayoutItemBg.height)), CallFunc::create([&](){this->removeFromParent();}));
        layoutItemBg->runAction(sequeue);
    });
    btnWeixinChat->addClickEventListener(std::bind(&LayerShare::weixinWeChatClickEvent, this, std::placeholders::_1));
    btnWeixinCircle->addClickEventListener(std::bind(&LayerShare::weixinCircleClickEvent, this, std::placeholders::_1));
    layoutItemBg->runAction(MoveTo::create(0.2f, positionLayoutItemBg));
    return true;
}



void LayerShare::weixinWeChatClickEvent(Ref *ref)
{
 
    log("weixinWeChat");
    PlatformSdk::getInstance()->shareWeixin(PlatformSdk::TargetShare::WeiXinWeChat,
                                            mShareType,title, description, linkUrl, imgName);
    this->removeFromParent();
    
}


void LayerShare::weixinCircleClickEvent(Ref *ref)
{
    log("weixinCircle");
    PlatformSdk::getInstance()->shareWeixin(PlatformSdk::TargetShare::WeiXinCircle,
                                                  mShareType,title, description, linkUrl, imgName);
    this->removeFromParent();
}




