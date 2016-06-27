//
//  NodeMatchInfoLabelCell.h
//  pokerclient
//
//  Created by ll on 15/12/2.
//
//

#ifndef __pokerclient__NodeMatchInfoLabelCell__
#define __pokerclient__NodeMatchInfoLabelCell__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "cocos-ext.h"
#include "msg.pb.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;


class NodeMatchInfoLabelCell : public Widget {
public:
    CREATE_FUNC(NodeMatchInfoLabelCell);
    
    virtual bool init();
    
    void update_data(const std::string label_word);
    
    void click_checkbox(Ref* sender);
    void set_click_label(const std::function<void(std::string)> func_click_label) {func_click_label_ = func_click_label;}
    
private:
    std::function<void(std::string)> func_click_label_;
    std::string textlabel_;
    
    Node* root_;
    Text* text_note_;
    ImageView* image_note_bg_;
};

#endif /* defined(__pokerclient__NodeMatchInfoLabelCell__) */
