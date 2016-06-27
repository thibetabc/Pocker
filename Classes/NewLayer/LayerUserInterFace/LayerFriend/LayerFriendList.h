//
//  LayerFriendList.hpp
//  pokerclient
//
//  Created by zhongyukeji on 16/4/11.
//
//

#ifndef LayerFriendList_h
#define LayerFriendList_h

#include <stdio.h>
#include "SceneManager.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "cocos-ext.h"
#include "LayerFriendsCircleDetail.h"
#include "ProcessorManager.h"
#include "NET_CMD.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;

class LayerFriendList : public BaseLayer, public TableViewDataSource, public TableViewDelegate ,public EditBoxDelegate
{
public:
    enum Type
    {
        NONE = -1,
        SELECT_SINGLE,
        SELECT_MULTIPLE,
    };
    
public:
    CREATE_FUNC(LayerFriendList);
    static LayerFriendList *create(Type, std::vector<uint32_t> vectorInvalid = std::vector<uint32_t>());
    virtual bool init();
    void send_msg();
    void setCircleImageShow(std::string imageName);
    void set_select_callback(const std::function<void(const std::vector<msg::UserDetailInfo>)> &callback)
    {
        select_callback_ = callback;
    }
    void update_unread_message_count();
    void update_unread_invitation_count();
    void receive_chat_message();
    void RefreshMessage();
    void handle_get_friends_circle_info(cmd_data_pointer data);
    void getOnlineData(bool ifResetTableviewData);
    void send_circle_info();//发送朋友圈信息
    void handle_get_publish_circle_info(cmd_data_pointer data);
    void setButtonState();
    void shareInfo(const std::string& info);//分享界面
    void setComment(int commentID,int MomentId ,int sendType ,int parentID);
    void deleteMomment(int momentID);
    void deleteComment(int MomentID,int CommentID);
    void setDeleteFriendCircleInfo(int type,int momentID,int commentID);
    bool is_show_edit;
    bool ifShowFriendCircle;
    EditBox* ed_property_value_;
    TextField* tf_input;
    ImageView* image_chat_bar_bg;
    std::string commentIfo;//评论信息
    bool circle_tableview_change;//朋友圈数据跟改
    float circle_posY;
    void jumpToFriendInfoPage(msg::UserDetailInfo user_info_);
    
    
protected:
    void RefreshLayer();
    void handle_get_friends_list(cmd_data_pointer data);
    //    void handle_add_friend(cmd_data_pointer data);
    void handle_get_comment_req(cmd_data_pointer data);
    void handle_get_commentinfo_req(cmd_data_pointer data);
    void handle_delete_comment_info(cmd_data_pointer data);
    void register_events();
private:
    LayerFriendList();
    void send_comment_req();//发送评论
    bool isInValidList(uint32_t userId);
    void setShowFriendCircle(bool showOrNot);
    std::function<void(const std::vector<msg::UserDetailInfo>)> select_callback_;
    void sendComment(int commentID);
    
#pragma mark - tableviewDatasource
    virtual Size cellSizeForTable(TableView *table)
    {
        return Size(DESIGN_WIDTH, GroupCellHeight);
    };
    TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
    ssize_t numberOfCellsInTableView(TableView *table);
    
#pragma mark - tableviewDelegate
    void tableCellTouched(TableView* table, TableViewCell* cell);
    void tableCellHighlight(TableView* table, TableViewCell* cell);
    void tableCellUnhighlight(TableView* table, TableViewCell* cell);
    
#pragma mark EditBox delegate
    virtual void editBoxEditingDidBegin(EditBox* editBox);
    virtual void editBoxEditingDidEnd(EditBox* editBox);
    virtual void editBoxTextChanged(EditBox* editBox, const std::string& text);
    virtual void editBoxReturn(EditBox* editBox);

    
private:
    Sprite* imgNewFriendMsgIcon;
    Text* textNewFriendMsgUnreadCount;
    Button* btnNewFriend;
    TableView *tableview_;
    TableView *tableviewcircle;
    LayerFriendsCircleDetail* layerfriendinfo;
    Button* btnFriendCircle ;
    Button* btnAddFriends;
    Button* btnSearchFriend;
    Text *title_;
    Text *text_btn_circle;
    Text *text_btn_friends;
    Layout *layout_friend;
    Layout *layout_circle;
    
    Button* btn_send;
    ImageView* image_input_bg;
    Button* btn_keyboard;
    Button* btn_send_comment;
    ImageView* image_tipview_bg;
    Button* btn_confirm;
    Button* btn_cancle;
    Text* text_tip_title;
    std::string inputCommentInfo;
    bool is_edit_;
    int commentID_;
    int momentID_;
    int sendType_;
    int parentID_;
    int deleteMoment;
    int deleteComentID_;
    
    //    std::vector<msg::FriendInfo> mFriendInfo;
    struct showLength
    {
        float totalHeight;//总高度
        float wordHeight;//字体高度
        float ImageHeight;//图片高度
        float zanHeight;//赞高度
        int commentCount;
        int zanCount;
        float commentHeight;//评论高度
    };
    
    
    struct CustomData
    {
        bool isValid; //是否可以选
        bool isSelect; //是否选中
        bool isOnline;
        msg::UserDetailInfo userDetailInfo;
    };
    std::vector<struct CustomData> mCustomData;
    std::vector<uint32_t> mInvalidId; //无效id，不可选
    ssize_t mLastIndex;
    Type mType;
    Size sizeOffset;
    ssize_t countFriendList;
    msg::MomentDisplayType momentType_;
    
    
};
#endif /* LayerFriendList_hpp */
