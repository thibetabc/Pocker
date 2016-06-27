//
//  LayerFriendsCircleDetail.cpp
//  pokerclient
//
//  Created by zhongyu on 16/6/3.
//
//

#include "LayerFriendsCircleDetail.h"
#include "utils.h"
#include "NodeFriendImage.h"
#include "LayerFriendList.h"
#include "NodeFriendCircleComment.h"
#include "GameDataManager.h"
#include "TipView.h"
#include "NodeImageshow.h"

#define IMAGESHOWNUMBER 5000
#define IMAGEHEADSHOWNUMBER 1000
#define COMMENTNUMBER 500

LayerFriendsCircleDetail* LayerFriendsCircleDetail::create()
{
    LayerFriendsCircleDetail* layerCircleInfo=new LayerFriendsCircleDetail;
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

bool LayerFriendsCircleDetail::init(){
    if(!Widget::init())
    {
        return false;
    }
    haveData=false;
    
    root=CSLoader::createNode("LayerFriendCircleDetail.csb");
    auto listview =root->getChildByName("ListView_1");
    circlePos_=0;
    Size contsize =listview->getContentSize();
    tableview_ = TableView::create(this,listview->getContentSize());
    tableview_->setAnchorPoint(listview->getAnchorPoint());
    tableview_->setContentOffset(Vec2(0, 0));
    tableview_->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    tableview_->setPosition(listview->getPosition());
    tableview_->setDelegate(this);
    tableview_->setClippingToBounds(true);
    tableview_->setTag(200);
    tableview_->setDataSource(this);
    tableview_->setVisible(false);
    root->addChild(tableview_);
    this->addChild(root);
    
    getFriendDataManagerLayer=GetFriendCircleDataLayer::create();
    getFriendDataManagerLayer->setVisible(false);
    this->addChild(getFriendDataManagerLayer);
    
    auto Button_2=dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "Button_2"));
    Button_2->addClickEventListener([&](Ref *ref)
                                    {
                                        this->jumpToPublishPage();
                                    });
    Button_2->setVisible(false);
    auto Button_1=dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "Button_1"));
    Button_1->setVisible(false);
    this->setVisible(false);
    return true;
}

#pragma mark touch
bool LayerFriendsCircleDetail::onTouchBegan(Touch* touch, Event* event)
{   touchBeginPosY= touch->getLocation().y;
    return true;
};
void LayerFriendsCircleDetail::onTouchMoved(Touch* touch, Event* event){
    
};
void LayerFriendsCircleDetail::onTouchEnded(Touch* touch, Event* event)
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

void LayerFriendsCircleDetail::jumpToPublishPage()
{
    auto layer = dynamic_cast<LayerFriendList*>(SceneManager::get_layer_by_name(typeid(LayerFriendList).name()));
    layer->addChild(LayerPublichPriendsCircle::create());
}

TableViewCell* LayerFriendsCircleDetail::tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx)
{
    TableViewCell *cell = table->dequeueCell();
    Text* textPlayerName;
    if(!cell)
    {
        auto root = CSLoader::createNode("NodeFriendsInfo.csb");
        auto imageBg = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root, "Image_head_icon"));
        // 各种控件
        auto Panel_1=dynamic_cast<Layout*>(CSLoader::seekNodeByName(root, "Panel_1"));
        auto showImage_bg=dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root, "Image_bg"));
        auto Image_handid=dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root, "Image_handid"));
        auto Panel_state=dynamic_cast<Layout*>(CSLoader::seekNodeByName(root, "Panel_state"));
        auto Panel_friendHead=dynamic_cast<Layout*>(CSLoader::seekNodeByName(root, "Panel_friendHead"));
        auto Panel_comment=dynamic_cast<Layout*>(CSLoader::seekNodeByName(root, "Panel_comment_0"));
        auto Text_detail=dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "Text_detail"));
        imageBg->setVisible(false);
        Panel_1->setSwallowTouches(false);
        showImage_bg->setSwallowTouches(false);
        Image_handid->setSwallowTouches(false);
        Panel_state->setSwallowTouches(false);
        Panel_friendHead->setSwallowTouches(false);
        Panel_comment->setSwallowTouches(false);
        
        
        for(int i=1;i<=4;i++)
        {
            std::stringstream ss;
            std::string str;
            ss<<i;
            ss>>str;
            std::string imgroot="Image_pic_bg_"+str;
            auto imgshow=dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root, imgroot));
            imgshow->setVisible(false);
        }
        cell = new TableViewCell();
        cell->autorelease();
        cell->addChild(root);
    }
    if(haveData)
    {
        this->setTableviewOnlineDataShow(cell,idx,momentData);
    }
//    auto lar = dynamic_cast<LayerFriendList*>(SceneManager::get_layer_by_name(typeid(LayerFriendList).name()));
//    bool isshow=lar->ifShowFriendCircle;
//    if(isshow)
//    {
        this->setVisible(true);
//    }
 
    
    return cell;
    
}

void LayerFriendsCircleDetail::showDianZanStatus(msg::Status status)
{
    switch (status){
        case msg::Status::SUCCESS:
            Toast::ToastShow(tools::local_string("operate_successed","操作成功"));
            break;
        case msg::Status::FAILED:
            Toast::ToastShow(tools::local_string("operate_failed","操作失败"));
            break;
        default:
            break;
    }
    dynamic_cast<LayerFriendList*>(SceneManager::get_layer_by_name(typeid(LayerFriendList).name()))->getOnlineData(true);
    tableview_->reloadData();
}

void LayerFriendsCircleDetail::deleteComment(msg::Status status)//删除评论
{
    switch (status){
        case msg::Status::SUCCESS:
            Toast::ToastShow(tools::local_string("operate_successed","操作成功"));
            break;
        case msg::Status::FAILED:
            Toast::ToastShow(tools::local_string("operate_failed","操作失败"));
            break;
        default:
            break;
    }
    tableview_->reloadData();
}

void LayerFriendsCircleDetail::register_events()
{
    dispatch_.register_event(REQ_PUBLISH_MOMENT,BIND_FUNC(this,LayerFriendsCircleDetail::handle_dian_zan_info));//点赞
    
}

void LayerFriendsCircleDetail::handle_dian_zan_info(cmd_data_pointer data)
{
    
}

void LayerFriendsCircleDetail::setDownloadPicShow(std::string imgName ,ImageView* image , TableViewCell *cell ,int type ,int picPosType)
{
    auto Panel_image=dynamic_cast<Layout*>(CSLoader::seekNodeByName(cell, "Panel_image"));
    Panel_image->setSwallowTouches(false);
    if(imgName == "")
    {
        imgName = "Default/image_defualt_user_icon_small.png";
    }
    if(type==1)
    {

        ShaderSprite *imgIcon = ShaderSprite::createAndDownload(imgName, "Default/image_defualt_user_icon_small.png", "Default/image_defualt_user_icon_small.png");
        imgIcon->setPosition(image->getPosition());
        if(picPosType==99)//如果是人物右上角头像
        {
            if(cell->getChildByTag(12345+picPosType))
            {
                cell->removeChildByTag(12345+picPosType);
            }
               imgIcon->setTag(12345+picPosType);
            cell->addChild(imgIcon);
        }
        else
        {
            auto Panel_image =dynamic_cast<Layout*>(CSLoader::seekNodeByName(cell, "Panel_friendHead"));
            if(Panel_image->getChildByTag(12345+picPosType))
            {
                Panel_image->removeChildByTag(12345+picPosType);
            }
            imgIcon->setTag(12345+picPosType);
            Panel_image->addChild(imgIcon);
        }
        image->setVisible(false);
    }
    else {
    /*  //圆矩形显示框
    else if(type==2)
    {
        auto imageLayout = Layout::create();
        imageLayout->setContentSize(Size(300,210));
        imageLayout->setTouchEnabled(true);
        imageLayout->setAnchorPoint(Point(0,0));
        imageLayout->setPosition(image->getPosition());
        ShaderSprite *imgIcon= ShaderSprite::createAndDownload(imgName, "friend_circle/bg1.png", "friend_circle/bg1.png");
        imgIcon->setTag(100);
        imgIcon->setAnchorPoint(Point(0,0));
        imageLayout->addChild(imgIcon);
        Panel_image->addChild(imageLayout);
        imageLayout->setTag(IMAGESHOWNUMBER+picPosType);
        imageLayout->addClickEventListener([&](Ref *ref)
                                           {
                                               std::string name= dynamic_cast<ShaderSprite*>(dynamic_cast<Layout*>(ref)->getChildByTag(100))->getFileName();
                                               this->imageClickEvent(name);
                                           });
        image->setVisible(false);
        image->setSwallowTouches(false);
        imageLayout->setSwallowTouches(false);
        
    }
    else if(type==3)
    {
        auto imageLayout = Layout::create();
        imageLayout->setContentSize(Size(594,302));
        imageLayout->setTouchEnabled(true);
        imageLayout->setAnchorPoint(Point(0,0));
        imageLayout->setPosition(image->getPosition());
        ShaderSprite *imgIcon= ShaderSprite::createAndDownload(imgName, "friend_circle/bg2.png", "friend_circle/bg2.png");
        imgIcon->setTag(100);
        imageLayout->addChild(imgIcon);
        Panel_image->addChild(imageLayout);
        imageLayout->setTag(IMAGESHOWNUMBER+picPosType);
        imageLayout->addClickEventListener([&](Ref *ref)
                                           {
                                               std::string name= dynamic_cast<ShaderSprite*>(dynamic_cast<Layout*>(ref)->getChildByTag(100))->getFileName();
                                               this->imageClickEvent(name);
                                           });
        image->setVisible(false);
        return;
    }
     */
    //矩形显示框
    //GetFriendCircleDataLayer *getData=new GetFriendCircleDataLayer;
    getFriendDataManagerLayer->downloadFile(imgName , image , picPosType);
    image->setTouchEnabled(true);
    image->addClickEventListener([&](Ref *ref)
                                 {
                                     bool isisScrolling=isScrolling;
                                     if(isScrolling)
                                     {
                                         return;
                                     }
                                     std::string name= dynamic_cast<ImageView*>(ref)->getImageName();
                                     this->imageClickEvent(name);
                                 });
    image->setVisible(true);
    image->setSwallowTouches(false);
    }
}

void LayerFriendsCircleDetail::imageClickEvent(std::string imgName)
{
    printf("saklda %s",imgName.c_str());
    dynamic_cast<LayerFriendList*>(SceneManager::get_layer_by_name(typeid(LayerFriendList).name()))->setCircleImageShow(imgName);
}

std::string LayerFriendsCircleDetail::getStringFromInt(int64_t number)
{
    std::stringstream ss;
    std::string str;
    ss<<number;
    ss>>str;
    return str;
}

void LayerFriendsCircleDetail::click_btn_share(Ref* sender)
{
    utils::captureScreen(CC_CALLBACK_2(LayerFriendsCircleDetail::saveCallback, this),"share_record.png");
}
void LayerFriendsCircleDetail::saveCallback(bool succeed, const std::string& info)
{
    if(succeed)
    {
        log("RecordDetailLayer::saveCallback 微信分享-图片保存路径 = %s", info.c_str());
        Director::getInstance()->getScheduler()->performFunctionInCocosThread([&, info](){
        auto layer =dynamic_cast<LayerFriendList*>(SceneManager::get_layer_by_name(typeid(LayerFriendList).name()));
        layer->shareInfo(info);
        });
    }
    else
    {
        log("Capture screen failed.");
    }
    
}

void LayerFriendsCircleDetail::click_btn_zan(int momentid)
{
    msg::CommentInfo moment;
    msg::Processor_203_PublishComment processor; //发布评论
    msg::Request request;
    msg::CommentInfo* commentinfos=processor.mutable_commentinfo();
    processor.mutable_commentinfo()->set_momentid(momentid);
    processor.mutable_commentinfo()->set_commentid(1);
    processor.mutable_commentinfo()->set_type(::msg::CommentType::LIKE);
    getFriendDataManagerLayer->sendZanData(processor);
    
}

void LayerFriendsCircleDetail::sendCommentInfo()//请求评论
{
    int MomentId=0;
    getFriendDataManagerLayer->getComment(MomentId);

}

void LayerFriendsCircleDetail::showCommentInfo(std::vector<msg::CommentInfo> mVectorCommentInfo)
{
    int size = mVectorCommentInfo.size();
}

void LayerFriendsCircleDetail::jumpToCommentPage(int momentID,int commentID ,int index)
{
    auto comment = momentData[index];
    auto commentInfo =momentData[index].commentinfos();
    dynamic_cast<LayerFriendList*>(SceneManager::get_layer_by_name(typeid(LayerFriendList).name()))->setComment(momentID, commentID,1,0);
   
}

void LayerFriendsCircleDetail::setTableviewOnlineDataShow(TableViewCell* cell, ssize_t idx , std::vector<msg::Moment> moment)
{
    auto playerName =  dynamic_cast<Text*>(CSLoader::seekNodeByName(cell, "Text_name"));
    auto Image_head_icon =dynamic_cast<ImageView*>(CSLoader::seekNodeByName(cell, "Image_head_icon"));
    auto Tess=dynamic_cast<Text*>(CSLoader::seekNodeByName(cell, "Text_detail"));
    auto dateTime = dynamic_cast<Text*>(CSLoader::seekNodeByName(cell, "Text_time"));
    auto Image_bground= dynamic_cast<ImageView*>(CSLoader::seekNodeByName(cell, "Image_bg"));
    auto textDelete = dynamic_cast<Button*>(CSLoader::seekNodeByName(cell, "Button_delete"));
    auto Button_zan = dynamic_cast<Button*>(CSLoader::seekNodeByName(cell, "Button_zan"));
    auto Button_judge= dynamic_cast<Button*>(CSLoader::seekNodeByName(cell, "Button_judge"));
    auto Button_share= dynamic_cast<Button*>(CSLoader::seekNodeByName(cell, "Button_share"));
    auto Panel_friendHead= dynamic_cast<Layout*>(CSLoader::seekNodeByName(cell, "Panel_friendHead"));//点赞面板
    auto Image_show_line= dynamic_cast<ImageView*>(CSLoader::seekNodeByName(cell, "Image_show_line"));
    auto Image_show_zan= dynamic_cast<ImageView*>(CSLoader::seekNodeByName(cell, "Image_show_zan"));
    auto Panel_state=dynamic_cast<Layout*>(CSLoader::seekNodeByName(cell, "Panel_state"));
    auto Panel_comment1= dynamic_cast<Layout*>(CSLoader::seekNodeByName(cell, "Panel_comment_0"));//评论面板
    auto Panel_image =dynamic_cast<Layout*>(CSLoader::seekNodeByName(cell, "Panel_image"));
    auto Image_handid = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(cell, "Image_handid"));
    auto Image_show_line_0= dynamic_cast<ImageView*>(CSLoader::seekNodeByName(cell, "Image_show_line_0"));
    auto Image_pic_bg= dynamic_cast<ImageView*>(CSLoader::seekNodeByName(cell, "Image_pic_bg"));
    auto Image_pic_bg1= dynamic_cast<ImageView*>(CSLoader::seekNodeByName(cell, "Image_pic_bg_1"));
    auto Image_pic_bg2= dynamic_cast<ImageView*>(CSLoader::seekNodeByName(cell, "Image_pic_bg_2"));
    auto Image_pic_bg3= dynamic_cast<ImageView*>(CSLoader::seekNodeByName(cell, "Image_pic_bg_3"));
    auto Image_pic_bg4= dynamic_cast<ImageView*>(CSLoader::seekNodeByName(cell, "Image_pic_bg_4"));
    Image_pic_bg->setSwallowTouches(false);
    Image_pic_bg1->setSwallowTouches(false);
    Image_pic_bg2->setSwallowTouches(false);
    Image_pic_bg3->setSwallowTouches(false);
    Image_pic_bg4->setSwallowTouches(false);
    Image_handid->setSwallowTouches(false);
    
    Image_show_line_0->setVisible(false);
    textDelete->setTitleText(tools::local_string("delete","删除"));
    Panel_image->setVisible(false);
    Image_head_icon->setVisible(false);
    
    float headIconPositionY =detaHeight.at(idx).totalHeight+200-70-5;
    Image_head_icon->setPositionY(headIconPositionY);
    playerName->setPositionY(headIconPositionY);
    textDelete->setPositionY(headIconPositionY);
    Panel_image->setSwallowTouches(false);
    
    for(int i=0;i<=4;i++)
    {
        if(Panel_image->getChildByTag(IMAGESHOWNUMBER+i))
        {
        Panel_image->removeChildByTag(IMAGESHOWNUMBER+i);
        }
    }
    
    Size size;
    size.width=Image_bground->getContentSize().width;
    size.height=detaHeight.at(idx).totalHeight+100;
    Image_bground->setContentSize(size);
    Image_bground->setPositionY(0);
    Size wordSize;
    wordSize.width=Tess->getContentSize().width;
    wordSize.height=detaHeight.at(idx).wordHeight;
    printf("word height is  %f",wordSize.height);
    Tess->setContentSize(Size(Tess->getContentSize().width,detaHeight.at(idx).wordHeight));
    Tess->setPositionY(headIconPositionY - 35-detaHeight.at(idx).wordHeight - 5-10);
    float wordPositionY=headIconPositionY - 35 - 5-10;
    Panel_state->setPositionY(wordPositionY-detaHeight.at(idx).wordHeight-detaHeight.at(idx).ImageHeight-57-5);
    float statePosY=Panel_state->getPositionY();
    Panel_friendHead->removeAllChildren();
    Panel_friendHead->setLocalZOrder(99);
    if(detaHeight.at(idx).zanHeight>0)
    {
        Panel_friendHead->setVisible(true);
        Panel_friendHead->setContentSize(Size(Panel_friendHead->getContentSize().width,detaHeight.at(idx).zanHeight));
        Panel_friendHead->setPositionY(statePosY-detaHeight.at(idx).zanHeight);
    }
    else//没有赞
    {
        Panel_friendHead->setVisible(false);
        Image_show_zan->setVisible(false);
        Image_show_line->setVisible(false);
    }
    Panel_comment1->setContentSize(Size(Panel_comment1->getContentSize().width,detaHeight.at(idx).commentHeight));
    Panel_comment1->setPositionY(statePosY-detaHeight.at(idx).zanHeight-detaHeight.at(idx).commentHeight-10);
    
    Panel_image->setContentSize(Size(Panel_image->getContentSize().width,detaHeight.at(idx).ImageHeight));
    Panel_image->setPositionY(wordPositionY-detaHeight.at(idx).wordHeight-detaHeight.at(idx).ImageHeight);
    if(detaHeight.at(idx).ifHasHandid)
    {
        Image_handid->setVisible(true);
        Image_handid->setPositionY(wordPositionY-detaHeight.at(idx).wordHeight-detaHeight.at(idx).ImageHeight/2);
    }
    Image_handid->setVisible(false);
    Button_share->addClickEventListener(std::bind(&LayerFriendsCircleDetail::click_btn_share, this,std::placeholders::_1));//分享功能

    std::string time =getStringFromInt(moment[idx].time());
    std::string text = "";
    double time2 = tools::stod(time);
    time2 /= 1000;
    text = tools::time_display(time2);
    dateTime->setString(text);
    
    Panel_comment1->removeAllChildren();
    
    bool havezan=false;
    bool havealReadyZan=false;//已经赞过了
    int haveDianZanCommentMomentID=0;//点赞了的MomentID
    auto userinfo= moment[idx].userinfo();
    auto momentid=moment[idx].momentid();
    playerName->setString(userinfo.nickname());
    int userid=userinfo.userid();
    int myuserid = GDM->get_user_id();
    textDelete->setVisible(false);
    if(myuserid==userid)//是自己发的朋友圈
    {
        textDelete->setVisible(true);
        textDelete->setTag(momentData[idx].momentid());
        textDelete->addClickEventListener([&](Ref *ref)
                                          {
                                              int textID =dynamic_cast<Button*>(ref)->getTag();
                                              this->jumpToDeleteCommentPage(textID);
                                          });
    }
    this->setDownloadPicShow(userinfo.picname(), Image_head_icon , cell , 1 ,99);
    auto normalmomentinfo=moment[idx].momentinfo().normalmomentinfo();
    auto commentInfo =moment[idx].commentinfos();
    std::vector<msg::CommentInfo> commentInfoVec ;
    for(auto iter:commentInfo.commentinfo())
    {
        commentInfoVec.push_back(iter);
    }
    Button_judge->setTag(idx);
    Button_judge->addClickEventListener([&](Ref *ref)
                                        {
                                            int index =dynamic_cast<Button*>(ref)->getTag();
                                            printf("idex is %d",index);
                                            this->jumpToCommentPage(momentData[index].momentid(),1,index);
                                        });
    int commentCount=0;
    if(commentInfoVec.size()==0)//没有评论
    {
        
        Image_show_line->setVisible(false);
        Image_show_zan->setVisible(false);
    }
    else if(commentInfoVec.size()>0)//有评论
    {
        Panel_state->setTag(commentInfoVec[0].momentid()+5000);//存储对应的momentid
        std::vector<std::string>headImage;
        std::vector<msg::UserDetailInfo>headInfo;
        std::map<std::string ,bool >useridDic;
        for(int i=0;i<commentInfoVec.size();i++)
        {
            if(commentInfoVec[i].type()==msg::CommentType::LIKE && commentInfoVec[i].isdelete()!=true)//有点赞的并且没被删除
            {
                Image_show_line->setVisible(true);
                Image_show_zan->setVisible(true);
                havezan=true;
                if(commentInfoVec[i].userinfo().userid()==myuserid)
                {
                    havealReadyZan=true;
                    haveDianZanCommentMomentID=commentInfoVec[i].commentid();
                    
                };
                headImage.push_back(commentInfoVec[i].userinfo().picname());
                headInfo.push_back(commentInfoVec[i].userinfo());
             continue;
            }
            
            if(commentInfoVec[i].type()!=msg::CommentType::LIKE && commentInfoVec[i].isdelete()!=true)//有评论并且没删除
            {
                commentCount=commentCount+1;
            }
            
            if(commentInfoVec[i].isdelete()!=true)//评论没删除的时候才能显示
            {
            auto commentLayer = NodeFriendCircleComment::create();
            commentLayer->setAnchorPoint(Point(0,0));
            commentLayer->setPosition(Vec2(0,Panel_comment1->getContentSize().height-commentCount*72));
            Panel_comment1->addChild(commentLayer);
            commentLayer->setCommentInfoShow(commentInfoVec,i);

            auto mominfo = momentData[idx];
            int moid=mominfo.momentid();
            commentLayer->setMomentID(moid);
            ifscroll = &isScrolling;
            commentLayer->setCommentEvent(dynamic_cast<LayerFriendList*>(SceneManager::get_layer_by_name(typeid(LayerFriendList).name())));
            }
        }
        if(havezan==true)//有点赞头像,显示点赞头像
        {
            Panel_friendHead->removeAllChildren();
            for(int j=0;j<headImage.size();j++)
            {
                std::string imgName=headImage[j];
                msg::UserDetailInfo headInfoDetail=headInfo[j];
                if(imgName == "")
                {
                    imgName = "Default/image_defualt_user_icon_small48X48.png";
                }
                
                NodeImageshow* nodeImage = NodeImageshow::create();
                nodeImage->setImageHeadType(headInfoDetail);
                nodeImage->setAnchorPoint(Vec2(0,0));
                nodeImage->setPosition(Vec2(42+j*48+j*22,36-24));
                Panel_friendHead->addChild(nodeImage);
               // Panel_friendHead->setColor(Color3B(125,125,125));
//                ShaderSprite *imgIcon = ShaderSprite::createAndDownload(imgName, "Default/image_defualt_user_icon_small48X48.png", "Default/image_defualt_user_icon_small48X48.png");
//                imgIcon->setAnchorPoint(Vec2(0,0.5));
//                imgIcon->setPosition(Vec2(42+j*48+j*22,36));
//                Panel_friendHead->addChild(imgIcon);
                
            }
        }
    }

    //设置点赞按钮触控和显示
    if(havealReadyZan==true)
        {
            Button_zan->setTouchEnabled(true);
            Button_zan->loadTextures("friend_circle/icon-dianzan-p.png", "friend_circle/icon-dianzan-p.png");//红色点赞按钮
            Button_zan->setTag(haveDianZanCommentMomentID+1);
            Button_zan->addClickEventListener([&](Ref *ref){
            int momentid =dynamic_cast<Layout*>(dynamic_cast<Button*>(ref)->getParent())->getTag()-5000;
            int comentid =dynamic_cast<Button*>(ref)->getTag()-1;
            dynamic_cast<LayerFriendList*>(dynamic_cast<LayerFriendList*>(SceneManager::get_layer_by_name(typeid(LayerFriendList).name())))->setDeleteFriendCircleInfo(3,momentid,comentid);
            });
        }
    else
    {
        Button_zan->loadTextures("friend_circle/btn_praise_n.png", "friend_circle/btn_praise_n.png");//显示无色点赞按钮
        Button_zan->setTag(momentData[idx].momentid()+1);
        Button_zan->addClickEventListener([&](Ref *ref)
                     {
                         int momentid =dynamic_cast<Button*>(ref)->getTag()-1;
                         this->click_btn_zan(momentid);
                     });
    }

    Tess->setString(normalmomentinfo.wordcontent());
    std::vector<std::string> picnameArray;
    for(auto iter : normalmomentinfo.picname())
    {
        picnameArray.push_back(iter);
    }

    if(picnameArray.size()>1)
    {
        Image_pic_bg->setVisible(false);
        Panel_image->setVisible(true);
        Image_handid->setVisible(false);
        for(int k=0;k<picnameArray.size();k++)
        {
            int j=k+1;
            std::stringstream ss;
            std::string str;
            ss<<j;
            ss>>str;
            std::string imgroot="Image_pic_bg_"+str;
            auto imgshow=dynamic_cast<ImageView*>(CSLoader::seekNodeByName(cell, imgroot));
            
            if(j<=2)
            {
                int kk=(int)((picnameArray.size()+1)/2);
                int posy=12+217*(kk-1);
                printf(" height is %d",posy);
                imgshow->setPositionY(posy);
            }
            else
            {
                imgshow->setPositionY(12);
            }
            this->setDownloadPicShow(picnameArray[k], imgshow , cell,2,j);
            
            auto listener = EventListenerTouchOneByOne::create();
            listener->setSwallowTouches(false);
            listener->onTouchBegan = CC_CALLBACK_2(LayerFriendsCircleDetail::onTouchBegan, this);
            listener->onTouchMoved = CC_CALLBACK_2(LayerFriendsCircleDetail::onTouchMoved, this);
            listener->onTouchEnded = CC_CALLBACK_2(LayerFriendsCircleDetail::onTouchEnded, this);
            _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, imgshow);
            
            }
        
        if(picnameArray.size()<4)
        {
            for(int j =picnameArray.size()+1;j<=4;j++)
            {
                std::string imgroot="Image_pic_bg_"+getStringFromInt(j);
                auto imgshow=dynamic_cast<ImageView*>(CSLoader::seekNodeByName(cell, imgroot));
                imgshow->setVisible(false);
            }
        }
    }
    else if(picnameArray.size()==1)//只有一张图片
    {
        Image_pic_bg->setVisible(false);
       
        auto Panel_image = dynamic_cast<Layout*>(CSLoader::seekNodeByName(cell, "Panel_image"));
        log("sahjkdsajd==%f",detaHeight.at(idx).wordHeight);
        Panel_image->setVisible(true);
        Image_pic_bg->setVisible(true);
        Panel_image->setPositionY(wordPositionY-detaHeight.at(idx).wordHeight-detaHeight.at(idx).ImageHeight);
        std::string imgName;
        imgName=picnameArray[0];
        printf("%s",imgName.c_str());
        
        this->setDownloadPicShow(imgName, Image_pic_bg , cell,3,0);
        auto listener = EventListenerTouchOneByOne::create();
        listener->setSwallowTouches(false);
        listener->onTouchBegan = CC_CALLBACK_2(LayerFriendsCircleDetail::onTouchBegan, this);
        listener->onTouchMoved = CC_CALLBACK_2(LayerFriendsCircleDetail::onTouchMoved, this);
        listener->onTouchEnded = CC_CALLBACK_2(LayerFriendsCircleDetail::onTouchEnded, this);
        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, Image_pic_bg);
        
        for(int i=1;i<=4;i++)
        {
            std::string imgroot="Image_pic_bg_"+getStringFromInt(i);
            auto imgshow=dynamic_cast<ImageView*>(CSLoader::seekNodeByName(cell, imgroot));
            imgshow->setVisible(false);
            
        }
    }
    else if(normalmomentinfo.handid()!=0)
    {
        Image_handid->setVisible(true);
        auto Button_handid = dynamic_cast<Button*>(CSLoader::seekNodeByName(cell, "Button_handid"));
        for(int i=1;i<=4;i++)
        {
            std::stringstream ss;
            std::string str;
            ss<<i;
            ss>>str;
            std::string imgroot="Image_pic_bg_"+str;
            auto imgshow=dynamic_cast<ImageView*>(CSLoader::seekNodeByName(cell, imgroot));
            imgshow->setVisible(false);
        }
        handid=normalmomentinfo.handid();
        Button_handid->setTag(handid);
        Button_handid->addClickEventListener([&](Ref *ref)
                                             {
                                                 auto handidnumber=dynamic_cast<Button *>(ref)->getTag();
                                                 this->click_replay(handidnumber);
                                             });
    }
    else if(normalmomentinfo.handid()==0)
    {
        auto Image_handid = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(cell, "Image_handid"));
        Image_handid->setVisible(false);
        
    }
}

void LayerFriendsCircleDetail::click_replay(int hand_id_number)
{
    PDM->setIsDetail(false);
    PDM->send_poker_get_hand_history(hand_id_number, true);
}

void LayerFriendsCircleDetail::setShowData(std::vector<msg::Moment> mVectorMoment ,bool layerShow ,float circlePos)
{
    layerShow_=layerShow;
    momentData.clear();
    momentData = mVectorMoment;
    detaHeight.clear();
    circlePos_=circlePos;
    int dateSize =mVectorMoment.size();
    for (auto iter : momentData)
    {
        bool haveImage=false;
        bool haveHandid=false;
        bool havaZan=false;
        
        int imgeheadCount=0;
        int commentCount = 0;
        int headImageCount=0;
        int imageCount=0;
        
        float imageheight=0;
        float Wordsdetailheight=0;
        float commentheight=0;
        float zanHeadHeight=0;
        float handIDHeight=0;
        
        auto userinfo =iter.userinfo();
        auto momentinfo = iter.momentinfo();
        auto normalinfo =momentinfo.normalmomentinfo();
        auto commentdetail = normalinfo.wordcontent();
        float commentdetaillength= commentdetail.length();//发表说明长度
        std::string temp="我";
        float d_lenght=temp.length();
        int count =commentdetaillength/d_lenght/22+1;
        Wordsdetailheight=33*count;
        if (normalinfo.handid()!=0){
            haveHandid=true;
        };
        
        for(auto iter : normalinfo.picname())
        {
            imageCount++;
        }
        
        auto commentInfos=iter.commentinfos().commentinfo();
        std::vector<std::string>headImage;
        for(auto info:commentInfos)
        {
            if(info.type()==msg::CommentType::LIKE &&info.isdelete()!=true)
            {
                haveImage=true;
                headImageCount++;
            }
            if(info.type()!=msg::CommentType::LIKE&&info.isdelete()!=true)
            {
                commentCount++;
            }
        };
        
        if(imageCount>1)//分享图片高度
        {
            imageheight=220*(int(imageCount/3)+1);
        }
        else if(imageCount==1)
        {
            imageheight=326;
        }
        
        if(haveHandid)//handid高度
        {
            imageheight=326;
            handIDHeight=0;
        }
        
        if(headImageCount>=1)//赞头像高度
        {
            int heightCount=headImageCount/8+1;
            zanHeadHeight=72*heightCount;
        }
        
        if(commentCount>=1)//评论高度
        {
            commentheight=commentCount*72;
        }
        
        float totalHeight = Wordsdetailheight+imageheight+handIDHeight+zanHeadHeight+commentheight+40;
        struct showLength showLengthTemp;
        showLengthTemp.totalHeight=totalHeight;
        showLengthTemp.wordHeight=Wordsdetailheight;
        showLengthTemp.ImageHeight=imageheight;
        showLengthTemp.zanHeight=zanHeadHeight;
        showLengthTemp.commentHeight= commentheight;
        showLengthTemp.commentCount=commentCount;
        showLengthTemp.zanCount=headImageCount;
        showLengthTemp.imageCount=imageCount;
        showLengthTemp.ifHasHandid=haveHandid;
        detaHeight.push_back(showLengthTemp);
    }
    haveData=true;
    tableview_->setVisible(false);
    tableview_->reloadData();
    log("pos is %f",circlePos_);
    if(circlePos_!=0)
    {
    tableview_->setContentOffset(Vec2(0,circlePos_));
   
    }
    tableview_->setVisible(true);
    tableview_->setVisible(layerShow_);
    
}

void LayerFriendsCircleDetail::jumpToDeleteCommentPage(int momentID)
{

    dynamic_cast<LayerFriendList*>(SceneManager::get_layer_by_name(typeid(LayerFriendList).name()))->setDeleteFriendCircleInfo(1,momentID,0);
}

void LayerFriendsCircleDetail::setHeadImageShow(TableViewCell*cell, std::string headImage)
{
    auto imageBg = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(cell, "Image_head_icon"));
    imageBg->loadTexture(headImage);
}

ssize_t LayerFriendsCircleDetail::numberOfCellsInTableView(cocos2d::extension::TableView *table)
{
    if(!haveData)
    {
    return 1;
    }
    else
    {
        return momentData.size();
    }
}

void LayerFriendsCircleDetail::tableCellTouched(TableView* table, TableViewCell* cell)
{
    auto frienlistlayer = dynamic_cast<LayerFriendList*>(SceneManager::get_layer_by_name(typeid(LayerFriendList).name()));
//    int idx =cell->getIdx();
//    if(idx==momentData.size()-1) //向下拉数据
//    {
//        
//    }
}

Size LayerFriendsCircleDetail::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
    Size size;
    size.width=750;
    if(haveData==false)
    {
        size.height=1000;
    }
    else
    {
    size.height=detaHeight.at(idx).totalHeight+200;
    
    }
    
    return size;
}
