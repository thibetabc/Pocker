//
//  LayerGroupMatchScore.h
//  pokerclient
//
//  Created by lvzhangle on 16/4/5.
//
//

#ifndef __pokerclient__LayerGroupMatchScore__
#define __pokerclient__LayerGroupMatchScore__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "BaseLayer.h"
#include "cocos-ext.h"

#include "GameDataManager.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;

class LayerGroupMatchScore : public BaseLayer
{
public:
    
    CREATE_FUNC(LayerGroupMatchScore);
    virtual bool init();
    
};

#endif /* defined(__pokerclient__LayerGroupMatchScore__) */

