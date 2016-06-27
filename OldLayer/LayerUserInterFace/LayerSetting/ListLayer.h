//
//  ListLayer.h
//  pokerclient
//
//  Created by ronaldz on 15/10/30.
//
//

#ifndef __pokerclient__ListLayer__
#define __pokerclient__ListLayer__

#include <stdio.h>
#include "cocos2d.h"
#include "msg.pb.h"
#include "RoomPropertyCell.h"
using namespace cocos2d;
#endif /* defined(__pokerclient__ListLayer__) */
class ListLayer : public Layer
{
public:
    static ListLayer* create(int AViewTag, int ACellIndex);
    bool init();
    void menu_click(Ref *sender);
    void update(Point bg_base, Point base,int distance,msg::Property *property_);
    virtual bool onTouchBegan(Touch *touch, Event *unused_event);
    virtual void onTouchEnded(Touch *touch, Event *unused_event);
private:
    CREATE_FUNC(ListLayer);
    msg::Property *property_;
    int viewTag, cellIndex;
};