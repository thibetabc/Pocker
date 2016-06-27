//
//  NodeMahjongReminder.hpp
//  pokerclient
//
//  Created by zhongyu on 16/6/12.
//
//

#ifndef NodeMahjongReminder_hpp
#define NodeMahjongReminder_hpp
#include "SceneManager.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "cocos-ext.h"
#include "msg.pb.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;

class NodeMahjongReminder : public Node
{
public:
    CREATE_FUNC(NodeMahjongReminder);
    virtual bool init();
private:
    Node* root_;
    
    Node* node_card_;
    Text* text_fan_;
    Text* text_fan_desc_;
    Text* text_zhang_;
    Text* text_zhang_desc_;
public:
    void set_data(const msg::MahjongTing &ting);
    
};

#endif /* NodeMahjongReminder_hpp */
