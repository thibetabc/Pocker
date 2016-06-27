//
//  NodeSelectGameType.hpp
//  pokerclient
//
//  Created by zhongyu on 16/4/21.
//
//

#ifndef NodeSelectGameType_hpp
#define NodeSelectGameType_hpp
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "cocos-ext.h"
#include "msg.pb.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;
class NodeSelectGameType:public ImageView {
public:
    CREATE_FUNC(NodeSelectGameType);
    virtual bool init();
private:
    Node* root_;
    ImageView* image_gametype_icon_;
    Button* btn_choose_gametype_;
    Text* text_gametype_name_;
    std::function<void()> callback_{};
    ImageView* image_select_type_game_bg_;
public:
    void refresh_data(msg::RoomType type);
    void set_click_callback(std::function<void()> callback){callback_=callback;};
    void click_image_bg(Ref* sender);
};

#endif /* NodeSelectGameType_hpp */
