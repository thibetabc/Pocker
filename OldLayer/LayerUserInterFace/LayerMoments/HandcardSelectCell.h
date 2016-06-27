//
//  HandcardSelectCell.h
//  pokerclient
//
//  Created by admin on 15-9-7.
//
//

#ifndef __pokerclient__HandcardSelectCell__
#define __pokerclient__HandcardSelectCell__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "BaseLayer.h"
#include "cocos-ext.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;


class HandcardSelectCell : public TableViewCell {
public:
    CREATE_FUNC(HandcardSelectCell);
    
    virtual bool init();
    
    void click_btn_replay(Ref* sender);
    
private:
    Text* text_group_name_;
    Text* text_room_name_;
    Text* text_player_name_;
    Text* text_pool_chip_;
    Text* text_ju_;
    

};


#endif /* defined(__pokerclient__HandcardSelectCell__) */
