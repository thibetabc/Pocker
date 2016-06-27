//
//  NodeRewardCell.h
//  pokerclient
//
//  Created by ll on 15/11/26.
//
//

#ifndef __pokerclient__NodeRewardCell__
#define __pokerclient__NodeRewardCell__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "cocos-ext.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;

class NodeRewardCellData
{
public:
    std::string reward_rank;
    std::string reward_prize;
    std::string reward_other_prize;
};

class NodeRewardCell : public TableViewCell {
public:
    
    CREATE_FUNC(NodeRewardCell);
    virtual bool init();
    
    void update_data(const std::shared_ptr<NodeRewardCellData> & reward_cell_data);
    void set_is_select(bool is_touch);
private:
    std::weak_ptr< NodeRewardCellData> reward_cell_data_;
    
private:
    Node* root_;
    
    ImageView* image_bg_;
    
    Text* text_rank_;
    Text* text_prize_;
    Text* text_other_prize_;
};

#endif /* defined(__pokerclient__NodeRewardCell__) */
