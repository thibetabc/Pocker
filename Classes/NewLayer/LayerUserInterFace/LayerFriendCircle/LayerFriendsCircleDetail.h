//
//  LayerFriendsCircleDetail.hpp
//  pokerclient
//
//  Created by zhongyu on 16/6/3.
//
//

#ifndef LayerFriendsCircleDetail_h
#define LayerFriendsCircleDetail_h
#include <stdio.h>
#include "SceneManager.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "cocos-ext.h"
#include "LayerPublichPriendsCircle.h"
#include "FileManager.h"
#include "events.h"
#include "LayerShare.h"
#include "NET_CMD.h"
#include "ProcessorManager.h"
#include "GameDataManager.h"
#include "GetFriendCircleDataLayer.h"
#define headiconroot "Default/image_defualt_user_icon_large.png"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;
class LayerFriendsCircleDetail:public Widget, public TableViewDataSource, public TableViewDelegate
{
private:
    struct imageDate
    {
        std::string  imageName; //人物头像名字
        std::string userid; //人物ID
    };
    
    struct showLength
    {
        float totalHeight;//总高度
        float wordHeight;//字体高度
        float ImageHeight;//图片高度
        float zanHeight;//赞高度
        bool ifHasHandid;//有手牌分享
        int commentCount;
        int zanCount;
        int imageCount;//图片数量
        float commentHeight;//评论高度
    };
    float touchBeginPosY;
    Touch* listener;
    float circlePos_; //tableview位置
#pragma mark touch
    bool onTouchBegan(Touch* touch, Event* event);
    void onTouchMoved(Touch* touch, Event* event);
    void onTouchEnded(Touch* touch, Event* event);
    
public:
    bool isScrolling;
    bool *ifscroll;
    Node* root;
    TableView* tableview_;
   // bool ifShowFriendCircle;
    bool layerShow_;
    int handid;
    
    static LayerFriendsCircleDetail* create();
    virtual bool init();
    void RefreshMessage();
    
    void setHeadImageShow(TableViewCell*cell, std::string headImage);
    Button *btnBack;
    void jumpToPublishPage();
    void setTableviewDetailShow(TableViewCell*cell,ssize_t idx);
    void setShowData(std::vector<msg::Moment> mVectorMoment,bool layerShow  ,float circlePos);
    std::vector<msg::Moment> momentData;
    std::vector<struct showLength> detaHeight;
    float getHeight(ssize_t idx,bool havaWords, bool haveImage,bool havehandID,int commentCount,bool havezan);
    bool haveData;
    void setTableviewOnlineDataShow(TableViewCell* cell, ssize_t idx , std::vector<msg::Moment> moment );
    void setDownloadPicShow(std::string imgName ,ImageView* image , TableViewCell *cell,int type ,int picPosType);
    void register_events();
    events::event_dispatch dispatch_;
    TableViewCell* presentCell;
    
    void click_replay(int hand_id_number);//回放
    void imageClickEvent(std::string imgName);
    void click_btn_share(Ref* sender);
    void click_btn_zan(int momentid);//点赞
    void saveCallback(bool succeed, const std::string& info);
    std::string getStringFromInt(int64_t number);
    
    void handle_dian_zan_info(cmd_data_pointer data);
    void showDianZanStatus(msg::Status status);
    void deleteComment(msg::Status status);
    void sendCommentInfo();
    void showCommentInfo(std::vector<msg::CommentInfo> mVectorCommentInfo);
    void jumpToCommentPage(int momentID,int commentID ,int index);
    void jumpToDeleteCommentPage(int momentID);
    
    GetFriendCircleDataLayer* getFriendDataManagerLayer;
    
#pragma mark - tableviewDatasource
    virtual Size cellSizeForTable(TableView *table)
    {
        return Size(DESIGN_WIDTH, GroupCellHeight);
    };
    TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
    ssize_t numberOfCellsInTableView(TableView *table);
    Size tableCellSizeForIndex(TableView *table, ssize_t idx);
    
#pragma mark - tableviewDelegate
    void tableCellTouched(TableView* table, TableViewCell* cell);
//    void tableCellHighlight(TableView* table, TableViewCell* cell);
//    void tableCellUnhighlight(TableView* table, TableViewCell* cell);
    
    
};

#endif /* LayerFriendsCircleDetail_hpp */
