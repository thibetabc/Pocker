//
//  ShopLayer.h
//  pokerclient
//
//  Created by ronaldz on 15/10/28.
//
//

#ifndef __pokerclient__ShopLayer__
#define __pokerclient__ShopLayer__

#include <stdio.h>

#include "BaseLayer.h"
#include "msg.pb.h"

class ShopLayer : public BaseLayer
{
public:
    CREATE_FUNC(ShopLayer);
    bool init();
    void click_back(Ref *sender);
    void update_data(std::vector<std::shared_ptr<msg::GoodInfo>> data , bool isDiamond = false);
private:
    cocos2d::ui::ListView *listview;
    
};
#endif /* defined(__pokerclient__ShopLayer__) */
