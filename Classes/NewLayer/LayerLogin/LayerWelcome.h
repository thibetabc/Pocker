//
//  LayerWelcome.hpp
//  pokerclient
//
//  Created by zhongyukeji on 16/4/6.
//
//

#ifndef LayerWelcome_h
#define LayerWelcome_h

#include <stdio.h>
#include "SceneManager.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"

using namespace cocos2d;
using namespace cocos2d::ui;

class LayerWelcome : public BaseLayer
{
public:
    CREATE_FUNC(LayerWelcome);
    virtual bool init();
};

#endif /* LayerWelcome_h */
