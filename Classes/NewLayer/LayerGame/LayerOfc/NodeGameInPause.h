//
//  NodeGameInPause.h
//  pokerclient
//
//  Created by zhongyu on 16/3/7.
//
//

#ifndef __pokerclient__NodeGameInPause__
#define __pokerclient__NodeGameInPause__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

using namespace cocos2d;
using namespace cocos2d::ui;

class NodeGameInPause : public Node {
public:
    CREATE_FUNC(NodeGameInPause);
    virtual bool init();
    
private:
    Text* text_in_pause_1_;
    Text* text_in_pause_2_;
};
#endif /* defined(__pokerclient__NodeGameInPause__) */
