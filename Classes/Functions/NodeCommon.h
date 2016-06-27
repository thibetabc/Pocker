//
//  NodeCommon.hpp
//  pokerclient
//
//  Created by zhongyukeji on 16/6/16.
//
//

#ifndef NodeCommon_hpp
#define NodeCommon_hpp

#include <global_header.h>

using namespace cocos2d;
using namespace cocos2d::ui;

enum ALIGN_MODE{
    TOPLEFT,
    TOPRIGHT,
    BOTTOMLEFT,
    BOTTOMRIGHT,
    LEFT,
    RIGHT,
    TOP,
    BOTTOM,
    MIDDLE,
    NONE
};

void AlignNode(Node* node, ALIGN_MODE PositionMode, int32_t XOffset, int32_t YOffset, ALIGN_MODE AnchorMode = NONE);

#endif /* NodeCommon_hpp */
