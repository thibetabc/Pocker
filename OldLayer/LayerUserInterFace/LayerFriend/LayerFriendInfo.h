//
//  LayerFriendInfo.h
//  pokerclient
//
//  Created by lvzhangle on 16/4/5.
//
//

#ifndef __pokerclient__LayerFriendInfo__
#define __pokerclient__LayerFriendInfo__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "BaseLayer.h"
#include "cocos-ext.h"

#include "GameDataManager.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;

class LayerFriendInfo : public BaseLayer
{
public:
    
    CREATE_FUNC(LayerFriendInfo);
    virtual bool init();
    
};

#endif /* defined(__pokerclient__LayerFriendInfo__) */

