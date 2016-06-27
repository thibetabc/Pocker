//
//  LayerFriendContactsList.hpp
//  pokerclient
//
//  Created by HuangFeng on 16/4/30.
//
//

#ifndef LayerFriendContactsList_h
#define LayerFriendContactsList_h

#include <stdio.h>
#include "SceneManager.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "cocos-ext.h"
#include "ConnectingView.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;

class LayerFriendContactsList : public BaseLayer, public TableViewDataSource, public TableViewDelegate
{
public:
    CREATE_FUNC(LayerFriendContactsList);
    virtual bool init();
protected:
    LayerFriendContactsList();
    ~LayerFriendContactsList();
    void RefreshLayer();
  
private:
    void accessContants(float dt);
#pragma mark - tableviewDatasource
    virtual cocos2d::Size cellSizeForTable(TableView *table)
    {
        return cocos2d::Size(DESIGN_WIDTH, GroupCellHeight);
    };
    TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
    ssize_t numberOfCellsInTableView(TableView *table);
    
#pragma mark - tableviewDelegate
    void tableCellTouched(TableView* table, TableViewCell* cell);
    void tableCellHighlight(TableView* table, TableViewCell* cell);
    void tableCellUnhighlight(TableView* table, TableViewCell* cell);
    
    
    
#pragma mark - protobuf
    void register_events();
    void requestFriendContactsList();
    void receiveFriendContactsList(cmd_data_pointer data);
    void receiveAddFriend(cmd_data_pointer data);
    
    
private:
    TableView *mTableview;
    ConnectingView *connectingView;
    
public:
    
    
    enum class USER_STATUS
    {
        FRIEND,       //好友
        NON_FRIEND,   //非好友
        NON_APP_USER, //非app用户可以邀请来玩（备选field）
    };
    
    //手机联系人
    typedef struct _Contacts
    {
        std::string name;
        std::vector<std::string> allTelephone;
        std::vector<std::string> allEmail;
    }Contacts;
    
    //过滤手机联系人
    typedef struct _AppOfPhoneUsers
    {
        std::string name;
        std::string telephone;
        std::string email;
        USER_STATUS userStatus;
        msg::UserDetailInfo appUsers;
    }AppOfPhoneUsers;
    //请求列表消息数据
    std::vector<msg::Invitation> mInvitations;
    //使用app的手机联系人数据
    //    std::vector<msg::FriendByPhoneNumber> mAppPhoneContacts;
    //手机联系人数据
    std::vector<Contacts> mContacts;
    void setContacts(std::vector<Contacts> contacts);
//    void filterContacts();
    //手机联系人电话
    const std::vector<std::string> getAllPhones();
    //按号码找联系人名字
    Contacts getContactsByPhone(std::string phone);
    //接收app手机联系人
    void handle_get_phone_contacts_list(cmd_data_pointer data);
    //最终显示在UI的手机联系人
    CC_SYNTHESIZE(std::vector<AppOfPhoneUsers>, mAppOfPhoneUsers, AppOfPhoneUsers);
    cocos2d::Size sizeOffset;
    
    
};
#endif /* LayerFriendContactsList_h */
