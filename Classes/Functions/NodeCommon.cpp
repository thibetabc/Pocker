//
//  NodeCommon.cpp
//  pokerclient
//
//  Created by zhongyukeji on 16/6/16.
//
//

#include "NodeCommon.h"

void AlignNode(Node* node, ALIGN_MODE PositionMode, int32_t XOffset, int32_t YOffset, ALIGN_MODE AnchorMode)
{
    auto parentnode = node->getParent();
    if (parentnode){
        ALIGN_MODE anchor = AnchorMode;
        if (anchor == NONE)
            anchor = PositionMode;
        auto size = parentnode->getContentSize();
        switch (anchor) {
            case TOPLEFT:
                node->setAnchorPoint(Vec2(0, 1));
                break;
            case TOPRIGHT:
                node->setAnchorPoint(Vec2(1, 1));
                break;
            case BOTTOMLEFT:
                node->setAnchorPoint(Vec2(0, 0));
                break;
            case BOTTOMRIGHT:
                node->setAnchorPoint(Vec2(1, 0));
                break;
            case LEFT:
                node->setAnchorPoint(Vec2(0, 0.5));
                break;
            case RIGHT:
                node->setAnchorPoint(Vec2(1, 0.5));
                break;
            case TOP:
                node->setAnchorPoint(Vec2(0.5, 1));
                break;
            case BOTTOM:
                node->setAnchorPoint(Vec2(0.5, 0));
                break;
            case MIDDLE:
                node->setAnchorPoint(Vec2(0.5, 0.5));
                break;
            default:
                break;
        }
        switch (PositionMode) {
            case TOPLEFT:
                node->setPosition(Vec2(XOffset, size.height - YOffset));
                break;
            case TOPRIGHT:
                node->setPosition(Vec2(size.width - XOffset, size.height - YOffset));
                break;
            case BOTTOMLEFT:
                node->setPosition(Vec2(XOffset, YOffset));
                break;
            case BOTTOMRIGHT:
                node->setPosition(Vec2(size.width - XOffset, YOffset));
                break;
            case LEFT:
                node->setPosition(Vec2(XOffset, size.height/2 + YOffset));
                break;
            case RIGHT:
                node->setPosition(Vec2(size.width - XOffset, size.height/2 + YOffset));
                break;
            case TOP:
                node->setPosition(Vec2(size.width / 2 + XOffset, size.height - YOffset));
                break;
            case BOTTOM:
                node->setPosition(Vec2(size.width / 2 + XOffset, YOffset));
                break;
            case MIDDLE:
                node->setPosition(Vec2(size.width / 2 + XOffset, size.height/2 + YOffset));
                break;
            default:
                break;
        }
    }
}