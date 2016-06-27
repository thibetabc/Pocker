//
//  TournamentBuyChipLayer.h
//  pokerclient
//
//  Created by ll on 15/12/3.
//
//

#ifndef __pokerclient__TournamentBuyChipLayer__
#define __pokerclient__TournamentBuyChipLayer__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "BaseLayer.h"
#include "cocos-ext.h"
#include "GameLayer.h"
#include "events.h"
#include "LayerManager.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;

class PokerRoom;

class TournamentBuyChipLayer : public PokerChildLayer
{
public:
    CREATE_FUNC(TournamentBuyChipLayer);
    virtual bool init();
    void set_text_string(std::string text_string);
    
    void click_btn_cancel(Ref* sender);
    void click_btn_confirm(Ref* sender);
    
    void handle_android_back();

private:
    bool WillAddon = false;
    Text* text_buyin_;
};

#endif /* defined(__pokerclient__TournamentBuyChipLayer__) */
