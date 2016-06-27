//
//  LayerReportType.hpp
//  pokerclient
//
//  Created by zhongyu on 16/4/22.
//
//

#ifndef LayerReportType_hpp
#define LayerReportType_hpp

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "cocos-ext.h"
#include "msg.pb.h"
#include "SceneManager.h"
#include "cmd_data.h"
#include "MultiEditBox.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;

class LayerReportType:public BaseLayer{
public:
    CREATE_FUNC(LayerReportType);
    virtual bool init();
private:
    Node* root_;
    Button* btn_back_;
    Text* text_title_;
    Text* text_confirm_;
    Text* text_report_dec_;
    MultiEditBox* tf_input_report_;
public:
    void click_btn_back(Ref* sender);
    void click_text_confirm(Ref* sender);
    void handle_feed_back(cmd_data_pointer data);
    void register_events();
};

#endif /* LayerReportType_hpp */
