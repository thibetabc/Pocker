//
//  NodeSelectedCircleButton.hpp
//  pokerclient
//
//  Created by zhongyu on 16/4/14.
//
//

#ifndef NodeCircleChoose_hpp
#define NodeCircleChoose_hpp

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "cocos-ext.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;

class NodeCircleChoose:public ImageView {
public:
    CREATE_FUNC(NodeCircleChoose);
    virtual bool init();
    
private:
    Node* root_;
    Button* btn_choose_status_;
    Button* btn_not_choose_status_;
    std::function<void(std::string)> function_callback_;
public:
    
    void set_click_callback(std::function<void(std::string)> function_callback){
        btn_not_choose_status_->setTouchEnabled(true);
        function_callback_=function_callback;};
    void click_btn_event(Ref* sender);
    void set_node_unchoose_status(bool isChoose=false);
};


#endif /* NodeCircleChoose_hpp */
