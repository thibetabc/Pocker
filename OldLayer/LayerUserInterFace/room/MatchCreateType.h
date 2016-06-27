//
//  MatchCreateType.hpp
//  pokerclient
//
//  Created by qqronaldz on 15/12/17.
//
//

#ifndef MatchCreateType_hpp
#define MatchCreateType_hpp

#include <stdio.h>
#include "cocos2d.h"
using namespace cocos2d;
class RoomLayer;
class MatchCreateType : public Layer
{
public:
    CREATE_FUNC(MatchCreateType);
    virtual bool init();
    void click_btn_sit_and_go();
    void click_btn_time_match();
    RoomLayer *roomLayer_;
    virtual bool onTouchBegan(Touch *touch, Event *unused_event);
    /** Callback function for touch moved.
     *
     * @param touch Touch infomation.
     * @param unused_event Event information.
     * @js NA
     */
    virtual void onTouchMoved(Touch *touch, Event *unused_event);
    /** Callback function for touch ended.
     *
     * @param touch Touch infomation.
     * @param unused_event Event information.
     * @js NA
     */
    virtual void onTouchEnded(Touch *touch, Event *unused_event);
};
#endif /* MatchCreateType_hpp */
