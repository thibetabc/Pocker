//
//  NodeComInfoPlayerCell.h
//  pokerclient
//
//  Created by ll on 15/11/26.
//
//

#ifndef __pokerclient__NodeComInfoPlayerCell__
#define __pokerclient__NodeComInfoPlayerCell__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "cocos-ext.h"

#include "utils.h"

#include "msg.pb.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;

class NodeComInfoPlayerCell : public TableViewCell {
public:
    
    CREATE_FUNC(NodeComInfoPlayerCell);
    virtual bool init();
    
    void update_data(const std::shared_ptr<msg::TourmentTableInfo> & player_cell_data);
    std::weak_ptr<msg::TourmentTableInfo> get_player_cell_data(){return player_cell_data_;};
    void set_is_select(bool is_touch);
private:
    std::weak_ptr<msg::TourmentTableInfo> player_cell_data_;
    
private:
    Node* root_;
    ImageView* image_bg_;
    Text* text_table_id_;

};

#endif /* defined(__pokerclient__NodeComInfoPlayerCell__) */
