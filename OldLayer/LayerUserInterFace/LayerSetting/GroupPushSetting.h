//
//  GroupPushSetting.h
//  pokerclient
//
//  Created by ronaldz on 15/10/29.
//
//

#ifndef __pokerclient__GroupPushSetting__
#define __pokerclient__GroupPushSetting__

#include <stdio.h>
#include "BaseLayer.h"
#include "cocos2d.h"
USING_NS_CC;
using namespace cocos2d::ui;
class GroupPushSetting : public BaseLayer
{
public:
    CREATE_FUNC(GroupPushSetting);
    bool init();
    void UpdateCellData();
    void click_btn_back(Ref *sender);
    void click_btn_save_(Ref *sender);
private:
    ListView* listview_property_;
};
#endif /* defined(__pokerclient__GroupPushSetting__) */
