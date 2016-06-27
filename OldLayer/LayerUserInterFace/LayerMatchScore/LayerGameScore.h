//
//  LayerGameScore.h
//  pokerclient
//
//  Created by lvzhangle on 16/4/5.
//
//

#ifndef __pokerclient__LayerGameScore__
#define __pokerclient__LayerGameScore__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "BaseLayer.h"
#include "cocos-ext.h"

#include "GameDataManager.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;

class LayerGameScore : public BaseLayer
{
public:
    
    CREATE_FUNC(LayerGameScore);
    virtual bool init();
    
};

#endif /* defined(__pokerclient__LayerGameScore__) */

