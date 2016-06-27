//
//  LayerMahjongFinalResult.hpp
//  pokerclient
//
//  Created by zhongyu on 16/6/3.
//
//

#ifndef LayerMahjongFinalResult_hpp
#define LayerMahjongFinalResult_hpp

#include "SceneManager.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "cocos-ext.h"
#include "msg.pb.h"
#include "cmd_data.h"
#include "GameLayer.h"
//#include ""

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;

class LayerMahjongFinalResult : public MahjongChildLayer
{
public:
    CREATE_FUNC(LayerMahjongFinalResult);
    virtual bool init();
    void refresh_data();
private:
    Node* root_;
    
    ListView* list_view_result_;
    Button* btn_close_;
    TableView* tableview_;
    
    std::unordered_map<int, int> user_ids_;
    std::vector<Point>  cell_pos_ = {Point(79,857), Point(79,677),Point(79,497),Point(79,317) };
};


#endif /* LayerMahjongFinalResult_hpp */
