//
//  HudLayer.h
//  pokerclient
//
//  Created by duanmt on 15/5/3.
//
//

#ifndef __pokerclient__HudLayer__
#define __pokerclient__HudLayer__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "BaseLayer.h"
#include "events.h"
#include "LayerManager.h"
#include "ReconnectView.h"
#include "GroupFriend.h"
#include "LayerComment.h"

using namespace cocos2d;
using namespace cocos2d::ui;

#define GroupFriendLayerTag 100
#define GroupFriendCommentTag 101

class HudLayer : public BaseLayer
{
public:
    enum LAYERTAG
    {
        TAG_MYGROUP = 1,
        TAG_FRIENDS = 2,
        TAG_ROOMLIST = 3,
        TAG_COMMETNS = 4,
        TAG_SETTING = 5
    };
    
    //CREATE_FUNC(HudLayer);
    static HudLayer * create(LAYERTAG Tag = LAYERTAG::TAG_ROOMLIST);
    virtual bool init();
    
    void click_btn_1(Ref* sender);
    void click_btn_2(Ref* sender);
    void click_btn_3(Ref* sender);
    void click_btn_4(Ref* sender);
    void click_btn_5(Ref* sender);
    
    
    void ShowTutorial(int button);  // by ronadlz. 
    
    void click_btn_add();
    void click_btn_search();
    
    virtual void onEnterTransitionDidFinish();
    void ShowDisconnect(bool IsDisconnect);
    void goto_room_list();
    GroupFriend* get_groupfriendlayer();
    LayerComment* get_groupfriendcommentlayer();
    void goto_chat_layer();
private:
    void reset_btns();
    void set_btn_image_clicked(Ref* sender);
    void receive_group_message(cmd_data_pointer);
    void receive_friend_message(cmd_data_pointer);
    void update_ui(cmd_data_pointer);
private:
    Button* btn_1_;
    Button* btn_2_;
    Button* btn_3_;
    Button* btn_4_;
    Button* btn_5_;
    
    static LAYERTAG m_nTag;
    
    Text* text_1_;
    Text* text_2_;
    Text* text_3_;
    Text* text_4_;
    Text* text_5_;
    
    ImageView* image_red_group_;
    ImageView* image_red_friend_;
    
    struct ButtonImage
    {
        ButtonImage(const std::string& normal, const std::string& select) :
        image_normal(normal),
        image_select(select)
        {}
        std::string image_normal;
        std::string image_select;
    };
    
    std::map<Button*, ButtonImage> btn_images_;
    
    BaseLayer* child_layer_;
    
    events::event_dispatch dispatch_;
    
};

#endif /* defined(__pokerclient__HudLayer__) */
