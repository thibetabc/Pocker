//
//  LayerUserInfo.hpp
//  pokerclient
//  好友资料
//  Created by zhongyukeji on 16/4/11.
//
//

#ifndef LayerUserInfo_hpp
#define LayerUserInfo_hpp

#include <stdio.h>
#include "SceneManager.h"
class LayerUserInfo : public BaseLayer, public cocos2d::ui::EditBoxDelegate
{
    
public:
    LayerUserInfo();
    //    CREATE_FUNC(LayerUserInfo);
    virtual bool init();
public:
    static LayerUserInfo* create(msg::UserDetailInfo info);
protected:
    
    void RequestDeleteFriend();
    void register_events();
    void handle_add_friend(cmd_data_pointer data);
    void handle_delete_friend(cmd_data_pointer data);
    void handle_block_user(cmd_data_pointer data);
    void handle_deal_request(cmd_data_pointer data);
    //获取已屏蔽的朋友圈列表
    void handle_all_block_user_list(cmd_data_pointer data);
    //获取好友列表
    void handle_get_friends_list(cmd_data_pointer data);
    
    void sendAddFriend();
    void sendDelFriend();
    //同意/拒绝好友申请
    void sendInvitationFriend(bool isAgree);
    //请求好友列表
    void sendFriendList();
    void setCheckBoxTextures(cocos2d::ui::CheckBox *checkbox);
    void requestDiamond();
    void receiverDiamond(cmd_data_pointer data);
    
#pragma mark - EditboxDelegate
    void editBoxEditingDidEnd(cocos2d::ui::EditBox* editBox);
    void editBoxReturn(cocos2d::ui::EditBox* editBox);
    
    void RefreshLayer();
    
    
private:
    void InitNormal();
    msg::UserDetailInfo mInfo;
    bool isBlockMoment;
    bool IsInvitation;
    cocos2d::ui::CheckBox *mCheckboxHiteMoments;
    cocos2d::ui::Button *mBtnSendMsg;
    cocos2d::ui::Button *mBtnDeleteFriend;
    bool mIsFriend;
    cocos2d::ui::EditBox *mEditboxtDiamond;
    cocos2d::ui::Button *mBtnConfirm;
};

#endif /* LayerUserInfo_hpp */
