//
//  NodeFriendCircleComment.cpp
//  pokerclient
//
//  Created by zhongyu on 16/6/12.
//
//

#include "NodeFriendCircleComment.h"
#include "events.h"
#include "SceneManager.h"
#include "ShaderSprite.h"


#define headiconroot "Default/image_defualt_user_icon_large.png"


NodeFriendCircleComment* NodeFriendCircleComment::create()
{
    NodeFriendCircleComment* layerCircleInfo=new NodeFriendCircleComment;
    if (layerCircleInfo->init())
    {
        layerCircleInfo->autorelease();
        return layerCircleInfo;
    }
    return nullptr;
    
    delete layerCircleInfo;
    layerCircleInfo = nullptr;
    return nullptr;
}

bool NodeFriendCircleComment::init(){
    if(!Widget::init())
    {
        return false;
    }
    ifDelete_=false;
    root = CSLoader::createNode("NodeFriendsCircleRewardcell.csb");//朋友圈评论UI
    headImage = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root, "Image_Friend_Icon"));
    textReplay = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "Text_name_0"));
    textDate = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "Text_date"));
    texthour = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "Text_hour"));
    textComment = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "Text_reply_info"));
    Panel_comment = dynamic_cast<Layout*>(CSLoader::seekNodeByName(root, "Panel_comment"));
    this->addChild(root);
    Panel_comment->setTouchEnabled(true);
    Panel_comment->setSwallowTouches(false);
    this->setSwallowTouches(false);
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(false);
    listener->onTouchBegan = CC_CALLBACK_2(NodeFriendCircleComment::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(NodeFriendCircleComment::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(NodeFriendCircleComment::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, Panel_comment);
    
    return true;
}

int NodeFriendCircleComment::getCommentIdx()
{
    return idx_;
}
void NodeFriendCircleComment::setCommentIdx(int idx)
{
    idx_=idx;
}

void NodeFriendCircleComment::setIfDelete(bool ifDelete)
{
    ifDelete_=ifDelete;
}

bool NodeFriendCircleComment::getIfDelete()
{
    return ifDelete_;
}

std::string NodeFriendCircleComment::getStringFromInt(int64_t number)
{
    std::stringstream ss;
    std::string str;
    ss<<number;
    ss>>str;
    return str;
}

void NodeFriendCircleComment::setCommentInfoShow(std::vector<msg::CommentInfo> commentInfo ,int index )
{
    int i=index;
    auto allInfo=commentInfo;
    auto userinfo=commentInfo[i].userinfo();
    userID=userinfo.userid();
    auto nickname =userinfo.nickname();
    auto infoma=commentInfo.at(i);
    auto type =infoma.type();
    this->setCommentID(infoma.commentid());
    this->setParentID(infoma.parentcomment());
    this->setIfDelete(infoma.isdelete());
    
    std::map<int,std::string> alluserinfo;
    for(auto itor:commentInfo)
    {
        if(itor.type()!=msg::CommentType::LIKE)
        {
            alluserinfo[itor.commentid()]=itor.userinfo().nickname();
        }
    }
    if(type==1)
    {
        textReplay->setString(nickname+"：");
    }
    else if(type ==3)
    {
        std::string replyName=alluserinfo[infoma.parentcomment()];
    textReplay->setString(nickname+" "+tools::local_string("reply","回复")+" "+replyName+" :");
    }
    auto info = commentInfo[i];
    textDate->setString(getStringFromInt(commentInfo[i].time()));
    textComment->setString(commentInfo[i].content());
    textComment->setContentSize(Size(117,38));
    std::string time = getStringFromInt(commentInfo[i].time());
    std::string text = "";
    const std::string& text1=time;
    double time2 = tools::stod(text1);
    time2 /= 1000;
    text = tools::time_display(time2);
    texthour->setString(text);
    textDate->setVisible(false);
    
    auto imgName=userinfo.picname();
    
    if(imgName == "")
    {
        imgName = "Default/image_defualt_user_icon_small48X48.png";
    }
    ShaderSprite *imgIcon = ShaderSprite::createAndDownload(imgName, "Default/image_defualt_user_icon_small48X48.png", "Default/image_defualt_user_icon_small48X48.png");
    imgIcon->setAnchorPoint(Vec2(0,0.5));
    imgIcon->setPosition(headImage->getPosition());
    headImage->setVisible(false);
    this->addChild(imgIcon);
    
    
}

void NodeFriendCircleComment::setLayerHeight(float height)
{
    height_=height;
}

float NodeFriendCircleComment::getLayerHeight()
{
    return height_;
}

void NodeFriendCircleComment::setMomentID(int momentID)
{
    momentID_=momentID;
}
int NodeFriendCircleComment::getMomentID(int momentID)
{
    return momentID_;
}

void NodeFriendCircleComment::setParentID(int parentID)
{
    parentID_=parentID;
}

int NodeFriendCircleComment::getParentID()
{
    return parentID_;
}

void NodeFriendCircleComment::setCommentID(int commentID)
{
    commentID_=commentID;
}

int NodeFriendCircleComment::getCommentID(int commentID)
{
    return commentID_;
}


#pragma mark touch
bool NodeFriendCircleComment::onTouchBegan(Touch* touch, Event* event)
{   touchBeginPosY= touch->getLocation().y;
    return true;
};
void NodeFriendCircleComment::onTouchMoved(Touch* touch, Event* event){
    
};
void NodeFriendCircleComment::onTouchEnded(Touch* touch, Event* event)
{
    
    auto target = event->getCurrentTarget();
    
    float touchPosY =touch->getLocation().y;
    float dis =touchPosY-touchBeginPosY;
    log("touch end event %f",dis);
    if(abs(dis)<10)
    {
        isScrolling=false;
        
    }
    else
    {
        isScrolling=true;
    }
};

void NodeFriendCircleComment::setCommentEvent(LayerFriendList* friendList )
{
    Panel_comment->setTouchEnabled(true);
    if(userID!=GDM->get_user_id())//回复别人的评论
    {
    Panel_comment->addClickEventListener([&, friendList](Ref *ref)
                                         {
                                             if(isScrolling) return ;
                                             log("getCommentID %d",commentID_);
                                             int comid=this->getCommentID(0);
                                             log("prensent comID is %d,momID is %d",commentID_,momentID_);
                                             friendList->setComment(momentID_,commentID_,2,commentID_);
                                         });
    }
    else //删除自己的评论
    {
        Panel_comment->addClickEventListener([&, friendList](Ref *ref)
                                             {
                                                 if(isScrolling) return ;
                                                 int comid=this->getCommentID(0);
                                                 friendList->setDeleteFriendCircleInfo(2,momentID_,commentID_);
                                             });
    }
    
}