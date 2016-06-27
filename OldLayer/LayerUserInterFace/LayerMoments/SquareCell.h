//
//  SquareCell.h
//  pokerclient
//
//  Created by admin on 15-8-28.
//
//

#ifndef __pokerclient__SquareCell__
#define __pokerclient__SquareCell__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "BaseLayer.h"
#include "cocos-ext.h"
#include "msg.pb.h"
#include "ShaderSprite.h"
#include "events.h"

void shareCallback(int platform,int stCode,std::string& errorMsg);


using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;

class SquareCell : public TableViewCell , public TableViewDataSource, public TableViewDelegate{
public:
    CREATE_FUNC(SquareCell);

    virtual bool init();
    void bind_csb(std::string str_name);
    void update_property(msg::Moment* moment, int pic_number);
    void update_ui();
    void update_data(msg::Moment* fri_data, const float listview_width);
    const msg::Moment* get_moment_data() { return moment_data_;}
    
    void click_image_like(Ref* sender);
    void click_image_comment(Ref* sender);
    void click_btn_replay(Ref* sender);
    void click_btn_del(Ref* sender);
    void click_btn_shield(Ref * sender); // 举报并屏蔽朋友圈
    
    void adjustPos(int i, int lineOfPics);
    ListView* listview;
    
#pragma mark tableview datasource --
    virtual cocos2d::Size tableCellSizeForIndex(TableView *table, ssize_t idx);
    virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(TableView *table);

#pragma mark tableview delegate --
    virtual void tableCellTouched(TableView* table, TableViewCell* cell);
    virtual void tableCellHighlight(TableView* table, TableViewCell* cell){};
    virtual void tableCellUnhighlight(TableView* table, TableViewCell* cell){};
    virtual void tableCellWillRecycle(TableView* table, TableViewCell* cell){};
    
    
    const int get_contentY() { return contentY; }
    const int get_likeY() { return likeY; }
    const int get_commentY() { return commentY; }
    
    
    
    void handle_download_events(cmd_data_pointer);

private:
    
    /*ImageView* image_friend_;
    Text* text_issue_;
    Button* btn_like_;
    Button* btn_comment_;*/
    ImageView* image_bg;
    ImageView* image_nickname;
    Text *text_nickname_;
    ImageView *image_square_head_pic;
    TextField *text_publish_content;
    Text *text_content_type;
    Text *text_need_points;
    Text *text_reward_points;
    Text *tt_need_point;
    Text *tt_reward_point;
    Text *text_review;
    ImageView *image_review;
    Text *text_like;
    ImageView *image_like;
    Text *text_content_time;
    Button* btn_handcard;
    Text* text_del_;
    cocos2d::ShaderSprite* sprite_icon_;
    Text* text_shield; // 举报并屏蔽朋友圈
    
    msg::Moment* moment_data_;
    
    Node* root_;
    bool m_bIsAdded;
    
    events::event_dispatch dispatch_;
    
    
    //y
    int contentY;
    int likeY;
    int commentY;
    
    // 举报并屏蔽的朋友圈ID
    uint32_t shield_id;
    
    // by ronaldz
    Vector<ImageView*> photos;
    Vector<TextField*> comments;
};

#endif /* defined(__pokerclient__SquareCell__) */
