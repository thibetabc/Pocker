//
//  NodeChooseGameTypeCell.h
//  pokerclient
//
//  Created by lvzhangle on 16/3/27.
//
//
#ifndef __pokerclient__NodeChooseGameTypeCell__
#define __pokerclient__NodeChooseGameTypeCell__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "cocos-ext.h"

#include "LayerChooseGameType.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;

class NodeChooseGameTypeCell : public TableViewCell {
public:
    CREATE_FUNC(NodeChooseGameTypeCell);
    virtual bool init();
    
    void update_data(LayerChooseGameType::CHOOSE_ROOM_TYPE game_type);
    LayerChooseGameType::CHOOSE_ROOM_TYPE get_data(){return cell_game_type_;}
    LayerChooseGameType::CHOOSE_ROOM_TYPE cell_game_type_;
private:
    
    ImageView* image_gametype_bg_;
    ImageView* image_gametype_icon_;
    Text* text_gametype_name_;
    
    Node* root_;
};
#endif /* defined(__pokerclient__NodeChooseGameTypeCell__) */