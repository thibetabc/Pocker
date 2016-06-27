//
//  GroupPushSettingCell.h
//  pokerclient
//
//  Created by ronaldz on 15/10/29.
//
//

#ifndef __pokerclient__GroupPushSettingCell__
#define __pokerclient__GroupPushSettingCell__

#include <stdio.h>
#include "msg.pb.h"
#include "cocos-ext.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
using namespace cocos2d;
using namespace extension;
using namespace cocos2d::ui;
class GroupPushSettingCell : public Widget
{
public:
    CREATE_FUNC(GroupPushSettingCell);
    bool init();
    void update(msg::Property *property);
    ListView * m_pListView;
private:
    cocos2d::ui::Text * m_pText;
    
};

#endif /* defined(__pokerclient__GroupPushSettingCell__) */
