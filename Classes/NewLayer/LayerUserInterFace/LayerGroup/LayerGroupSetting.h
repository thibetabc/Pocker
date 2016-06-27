//
//  LayerGroupSetting.h
//  pokerclient
//
//  Created by qqronaldz on 16/4/20.
//
//

#ifndef LayerGroupSetting_h
#define LayerGroupSetting_h

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "SceneManager.h"
#include "cocos-ext.h"
#include "msg.pb.h"
#include "ShaderSprite.h"
#include "NodePropertyCell.h"


using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;

class LayerGroupSetting : public BaseLayer, public EditBoxDelegate
{
public:
//    CREATE_FUNC(LayerGroupSetting);
    static LayerGroupSetting *create(const uint32_t groupId);
    LayerGroupSetting();
    ~LayerGroupSetting();
private:
    bool init();
    void initManagerNode();
    void createMember();
    void updateMember();
    void updateCommonSetting();
    void createRemoveButton();
    void removeRemoveButton();
    void createPermissionSetting(Vec2 position);
    void updataPermissionSetting();
    void register_events();
    void updateGroupInfo();
    void cleanGroupChatData();
    void pickerImage();
    void layerUserInfo(msg::UserDetailInfo info);
    void layerFriendList();
    void setChectboxTexture(CheckBox *checkbox);
    void setGroupName(const std::string name);
    void checkboxPerssiomEvent(Ref *ref);
    void managerCell(bool isShow);
    void managerEventListener(Ref *ref, CheckBox::EventType type);
    void headIconOnTouchEnded(Touch *touch, Event *event);
    void deductSlide(Ref *ref);
    void deductSlideEventListener(Ref *ref, Slider::EventType type);
    bool isManagerOrMaster();
    bool isMaster();
    void setLanguage(Node *node);
    void createCellCreditValue(ssize_t index);
    void setManagerColor(bool isSelected);
    void RefreshLayer();
    
#pragma mark - EditBoxDetegate
    void editBoxEditingDidBegin(EditBox* editBox);
    void editBoxEditingDidEnd(EditBox* editBox);
    void editBoxTextChanged(EditBox* editBox, const std::string& text);
    void editBoxReturn(EditBox* editBox);
#pragma mark - protobuf common 共用
    void requestGroupInfo();
    void requestEditGroupInfo(int type, std::string content);
    void requestIgnoreNotifications(bool ignore);
    void requestDeleteLeave();
    
    void receiveGroupInfo(cmd_data_pointer data);
    void receiveEditGroupInfo(cmd_data_pointer data);
    void receiveIgonreNotifications(cmd_data_pointer data);
    void receiveDeleteLeave(cmd_data_pointer data);
    void receiveUploadFile(cmd_data_pointer data);
    void receiveAddMember(cmd_data_pointer data);
    
#pragma mark - protobuf adm 管理员/俱乐部主
    void requestManager(CheckBox *checkbox);
    void requestManagerRakePercent(int percent);
    void requestPublicRank(bool isPublic);
    void requestAddMember(std::vector<msg::UserDetailInfo> userDatailInfo);
    void requestDelMember(uint32_t userId);
    void requestBeAdministrator(uint32_t userId, bool isAdmin);
    void requestUserPermission(uint32_t userId);
    void requestCreditValue(uint32_t userId, uint32_t value);
    
    
    void receiverManager(cmd_data_pointer data);
    void receiveBeAdministrator(cmd_data_pointer data);
    void receiveGroupPermission(cmd_data_pointer data);
    void receiveUserPermission(cmd_data_pointer data);
    
private:
    //固定2个按钮[+] [-]
    const int BUTTON_NUM = 2;
    //列
    const int MAX_COL = 4;
    //间隔宽高
    const int SPACE_WIDTH = 0;
    const int SPACE_HEIGHT = 50;
    //俱乐部主、管理员设置界面的高度，控制移动位置，避免listview靠下显示不全
    const int MANAGER_SETTING_HEIGHT = 600;
    const Size mBgOpacitySize = Size(60, 40); //减去背景框透明度的宽高
    
    //模板头像图片size
    Size mImageHeadSize;
    //固定的俱乐部头像LayoutSize
    Size mGroupHeadSize;
    //行数
    int mRow;
    //平均宽高
    int mPreWidth, mPreHeight;
    //插入头像的设置界面高度
    int mInsertHeight;
     //插入头像的行
    int mInsertRow;
    
    //俱乐部人数
    size_t mGroupNum;
    
    //头像的点击事件
    EventListenerTouchOneByOne *mListener;

    //删除成员状态
    bool isRemoving;
    //俱乐部主/管理员设置
    //layout头像
    Layout *mLayoutMember;
    //layout头像高度
    Size mLayoutMemberSize;
    //layout头像+俱乐部头像
    Layout *mLayoutAllHead;
    //listview头像
    ListView *mListviewAllHead;
    //整个listview界面
    ListView *mListviewAll;
    
    
    //公用设置
    Layout *mLayoutCommonSetting;
    ListView *mListviewCommonSetting;
    
    //弹出的Layer吞噬事件 listview变动位置
    Layer *mLayerPermissionSetting;
    ListView *mListviewHeadSetting;
    
    //俱乐部
    uint32_t mGroupId;
    Node *mRoot;
    
    //Layout俱乐部管理
    Layout *mLayoutManager;
    //Size俱乐部管理
    Size mSizeLayoutManager;
    //俱乐部管理
    CheckBox *mCheckboxManager;
    //信用值返还
    CheckBox *mCheckboxCreditValueBack;
    //允许玩家查看群成员
    CheckBox *mCheckboxClickViewPlayer;
    //禁言
    CheckBox *mCheckboxSilenced;
    //Layout公开玩家排行
    Layout *mLayoutRank;
    //公开玩家排行
    CheckBox *mCheckboxRank;
    //Layout信用值
    Layout *mLayoutCreditValue;
    //抽水比例
    Layout *mLayoutDeduct;
    Slider *mSliderManager;
    Text *mTextDeduct;
    bool isCreateManagerCell;
    Size mGroupNameCellSize;
    Text *mTitleGroupName;
    ShaderSprite *mImageHead;
    Text *mTextGroupName;
    EditBox *mEditBoxGroupName;
    Button *mBtnShare;
    Text *mTextRemark;
    EditBox *mEditBoxRemark;
    CheckBox *mCheckboxNotification;
    Button *mButtonCleanChatHistory;
    Button *mButtonDeleteLeave;
    Button *mButtonSetupNew;
    
    //设置俱乐部成员权限
    Node *mPremissionRoot;
    EditBox *mEditboxCreditValue;
    std::string mStrUploadPickerImage;
    bool isDeleteGroup;
    int mSelectTag;
    uint32_t mMemberId;
    
    //成员权限、新手模板
    enum PremissionSetting
    {
        MEMBER,     //成员 - 管理员和俱乐部主
        NEW_MEMBER, //模板 - 俱乐部主
    };
    PremissionSetting mPremissionSetting;
    bool isPremissionTemplateModify;
    bool isScroll;
    Vec2 mTouchPosition;
    ImageView *mImageCreditBg;
    bool isNotifications; //146下发GroupSetting消息免打扰
    const std::string CHECKBOX_SELECT = "login_res/creategameroom_res/checkbox_cell_selected.png";
    const std::string CHECKBOX_UNSELECT = "login_res/creategameroom_res/checkbox_cell_unselected.png";
    const int MAX_MEMBER_NUM = 10000; //叉和头像tag差值，用于创建和删除
    bool isExitLayer;
    const int MAX_DEDUCT_PERCENT = 10;
    bool isRefreshGroupInfo;
    
#pragma mark - Group 俱乐部数据
    msg::GroupUserStatus mGroupUserStatus; //俱乐部身份
    msg::GroupInfo mGroupInfo; //俱乐部信息
    std::vector<msg::GroupUserInfo> mGroupUserInfo; //俱乐部成员信息
    msg::GroupPermission mPermissionMember; //成员权限，单独请求
    msg::GroupPermission mPermissionOwn; //自己-信用额
    msg::GroupManageSetting mGroupManageSetting;//管理
   
    
};


#endif /* LayerGroupSetting_h */
