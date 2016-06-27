//
//  NodeBlindCell.h
//  pokerclient
//
//  Created by ll on 15/11/26.
//
//

#ifndef __pokerclient__NodeBlindCell__
#define __pokerclient__NodeBlindCell__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "cocos-ext.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;

class NodeBlindCellData{
public:
    std::string blind_rank;
    std::string blind_blind;
    std::string blind_ante;
    std::string blind_lasttime;
    std::string blind_resttime;
};

class NodeBlindCell : public TableViewCell {
public:
    
    CREATE_FUNC(NodeBlindCell);
    virtual bool init();
    
    void update_data(const std::shared_ptr<NodeBlindCellData> & blind_cell_data);
    void set_is_select(bool is_touch);
private:
    std::weak_ptr< NodeBlindCellData> blind_cell_data_;
    
private:
    Node* root_;
    
    Text* text_rank_;
    Text* text_blind_;
    Text* text_ante_;
    Text* text_last_time_;
    Text* text_rest_time_;
    
    ImageView* image_bg_;
};

#endif /* defined(__pokerclient__NodeBlindCell__) */
