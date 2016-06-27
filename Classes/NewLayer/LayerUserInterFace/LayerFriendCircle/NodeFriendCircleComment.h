//
//  NodeFriendCircleComment.hpp
//  pokerclient
//
//  Created by zhongyu on 16/6/12.
//
//

#ifndef NodeFriendCircleComment_h
#define NodeFriendCircleComment_h

#include <stdio.h>
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "cocos-ext.h"
#include "FileManager.h"
#include "ProcessorManager.h"
#include "LayerFriendList.h"
using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;

class NodeFriendCircleComment:public Widget
{
public:
    static NodeFriendCircleComment* create();
    virtual bool init();
    void setMomentID(int momentID);
    int getMomentID(int momentID);
    void setParentID(int parentID);
    int getParentID();
    void setCommentID(int commentID);
    int getCommentID(int commentID);
    void setCommentInfoShow(std::vector<msg::CommentInfo> commentInfo,int index);
    std::string getStringFromInt(int64_t number);
    void setLayerHeight(float height);
    float getLayerHeight();
    void setCommentEvent(LayerFriendList* friendList );
    bool ifDelete_;
    void setIfDelete(bool ifDelete);
    bool getIfDelete();
    int getCommentIdx();
    void setCommentIdx(int idx);
    
    int userID;
    float height_;
    Node* root;
    ImageView* headImage;
    Text* textReplay;
    Text* textDate;
    Text* texthour;
    Text* textComment;
    Layout* Panel_comment;
    int momentID_;
    int commentID_;
    int parentID_;
    bool isScrolling;
    float touchBeginPosY;
    int idx_;
private:
    
#pragma mark touch
    bool onTouchBegan(Touch* touch, Event* event);
    void onTouchMoved(Touch* touch, Event* event);
    void onTouchEnded(Touch* touch, Event* event);
    
    
};


#endif /* NodeFriendCircleComment_hpp */
