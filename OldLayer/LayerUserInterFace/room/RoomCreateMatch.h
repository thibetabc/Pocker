//
//  RoomCreateMatch.hpp
//  pokerclient
//
//  Created by qqronaldz on 15/11/26.
//
//

#ifndef RoomCreateMatch_h
#define RoomCreateMatch_h

#include <stdio.h>
#include "cocos2d.h"
#include "BaseLayer.h"
#include "msg.pb.h"
#include "cmd_data.h"
#include "events.h"
using namespace cocos2d;

class RoomCreateMatch : public BaseLayer
{
public:
    CREATE_FUNC(RoomCreateMatch);
    static RoomCreateMatch* create(bool isCreate,bool SitAndGO)
    {
        m_bIsCreate = isCreate;
        isSitAndGO = SitAndGO;
        return create();
    }
    void refresh(cmd_data_pointer data);
    bool init();
    void remove_self(cmd_data_pointer data);
    void set_isCreate(bool isCreate){this->m_bIsCreate = isCreate;}
    void click_btn_back(Ref *sender);
    void click_btn_create(Ref *sender);
    
    void click_btn_base_info(Ref *sender);
    void click_btn_chip_type(Ref *sender);
    void click_btn_reward_type(Ref *sender);
    void click_btn_join_list(Ref *sender);
    void set_room_type(msg::RoomType roomtype);
    void set_room_id(uint32_t room_id){this->room_id = room_id;}
    void click_join();
    void send_get_jion_list(cmd_data_pointer);
public:
    static bool m_bIsFirstOpen ;
    static int  m_nChoosedSizeModel ;     //0自定义
    static int  m_nChoosedSpeedModel ;
private:
    bool hasJoin;
    cocos2d::ui::Text *btn_create;
    cocos2d::ui::Button *image_view;
    uint32_t room_id;
    uint32_t group_id_;
    Node *root;
    static bool m_bIsCreate;
    static bool isSitAndGO;
    events::event_dispatch dispatch_;
};
#endif /* RoomCreateMatch_hpp */
