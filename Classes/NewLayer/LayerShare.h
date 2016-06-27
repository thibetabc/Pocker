//
//  LayerShare.h
//  pokerclient
//
//  Created by HuangFeng on 16/5/21.
//
//

#ifndef LayerShare_h
#define LayerShare_h
#include "SceneManager.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "cocos-ext.h"
#include "PlatformSdk.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;

class LayerShare : public BaseLayer
{
public:
    LayerShare();
    ~LayerShare();
    CREATE_FUNC(LayerShare);
    virtual bool init();
    
    void setShareUserInfo(uint32_t userId);
    void setShareGroupInfo(uint32_t userId, uint32_t groupId);
    void setShareRoomInfo(uint32_t userId, uint32_t groupId, uint32_t roomId);
    void setShareGamePokerRoomInfo(uint32_t userId, uint32_t groupId, uint32_t roomId);
    void setShareGameOfcRoomInfo(uint32_t userId, uint32_t groupId, uint32_t roomId);
    void setShareRecordDetail();
    void setShareRecordDetail(std::string imgPath);
    
private:
    void weixinWeChatClickEvent(Ref *ref);
    void weixinCircleClickEvent(Ref *ref);
private:
    std::string title;
    std::string description;
    std::string imgName;
    std::string linkUrl;
    PlatformSdk::ShareType mShareType;
};

#endif /* LayerShare_h */
