//
//  LayerComInfoOverView.h
//  pokerclient
//
//  Created by ll on 15/11/26.
//
//

#ifndef __pokerclient__LayerComInfoOverView__
#define __pokerclient__LayerComInfoOverView__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "events.h"

#include "msg.pb.h"

using namespace cocos2d;
using namespace cocos2d::ui;

class LayerComInfoOverView : public Layer {
public:
    
    CREATE_FUNC(LayerComInfoOverView);
    virtual bool init();
    
public:
    void update_data(const msg::Processor_530_PokerGetTourmentStatus_DOWN& tourment_status);
private:
    Node* root_;

    Text* text_note_1_;
    Text* text_note_2_;
    Text* text_note_3_;
    Text* text_note_4_;
    Text* text_note_5_;
    Text* text_note_6_;
    Text* text_note_7_;
};


#endif /* defined(__pokerclient__LayerComInfoOverView__) */
