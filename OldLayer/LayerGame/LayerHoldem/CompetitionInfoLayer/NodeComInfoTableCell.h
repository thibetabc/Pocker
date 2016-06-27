//
//  NodeComInfoTableCell.h
//  pokerclient
//
//  Created by ll on 15/11/30.
//
//

#ifndef __pokerclient__NodeComInfoTableCell__
#define __pokerclient__NodeComInfoTableCell__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "cocos-ext.h"

#include "utils.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;


class NodeComInfoTableCellData
{
public:
    int32_t player_seat_num;
    std::string player_nickname;
    int32_t player_chips;
};

class NodeComInfoTableCell : public TableViewCell {
public:
    
    CREATE_FUNC(NodeComInfoTableCell);
    virtual bool init();
    
    void update_data(const std::shared_ptr<NodeComInfoTableCellData> & table_cell_data);
private:
    std::weak_ptr<NodeComInfoTableCellData> table_cell_data_;
    
private:
    Node* root_;
    
    Text* text_player_seat_num_;
    Text* text_player_nickname_;
    Text* text_player_chips_;
};

#endif /* defined(__pokerclient__NodeComInfoTableCell__) */
