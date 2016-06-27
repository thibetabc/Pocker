//
//  LayerTourJoinUserList.h
//  pokerclient
//
//  Created by lvzhangle on 16/5/09.
//
//

#ifndef __pokerclient__LayerTourJoinUserList__
#define __pokerclient__LayerTourJoinUserList__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "SceneManager.h"
#include "cocos-ext.h"
#include "msg.pb.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;

class LayerTourJoinUserList : public BaseLayer
{
public:
    static LayerTourJoinUserList* create(uint32_t roomID, std::vector<msg::UserDetailInfo> vecUserInfos);
    virtual bool init();
    
    void update_data(uint32_t roomID, std::vector<msg::UserDetailInfo> vecUserInfos);

private:
    void click_btn_back(Ref * sender);
    void click_btn_join(Ref * sender);
    
    void handle_join(cmd_data_pointer data);
    
    void register_events();
    
    uint32_t room_id_;
    std::vector<msg::UserDetailInfo> vec_user_infos_;
    bool has_join_;
#pragma mark UI
    Text* text_title_;
    
    Button* btn_back_;
    
    ImageView * image_listview_bg_;
    ImageView * image_join_user_count_bg_;
    Text* text_join_user_count_;
    
    Button * btn_join_;
    
    ListView* listview_;
    
    Node* root_;
};

#endif /* defined(__pokerclient__LayerTourJoinUserList__) */