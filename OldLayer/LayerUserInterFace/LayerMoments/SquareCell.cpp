//
//  SquareCell.cpp
//  pokerclient
//
//  Created by admin on 15-8-28.
//
//

#include "SquareCell.h"
#include "GameDataManager.h"
#include "utils.h"
#include "LayerComment.h"
#include "events.h"
#include "cmd_def.h"
#include "FileManager.h"
#include "GroupFriend.h"
#include "PokerDataManager.h"
#include "TipView.h"
#include "net_manager.h"
#include "CCUMSocialSDK.h"
#include "ProcessorManager.h"
#include "UMShareButton.h"
// 使用友盟命令空间
USING_NS_UM_SOCIAL;
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#include "PayManager.h"
// 引入相关的头文件


#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

#endif

class GroupFriend;
class user_base;
class user_data;


bool SquareCell::init()
{
    m_bIsAdded = false;
    photos.clear();
    comments.clear();
    if(!TableViewCell::init()) {
        return false;
    }
    listview = nullptr;
    
    // register events;
    dispatch_.register_event(EVENT_DOWLOAD_FILE,
                             BIND_FUNC(this, SquareCell::handle_download_events));
    
    return true;
}


void SquareCell::handle_download_events(cmd_data_pointer)
{
    update_ui();
    
}

void SquareCell::bind_csb(std::string str_name)
{
    if(m_bIsAdded)
        return;
    reset();
    cleanup();
    m_bIsAdded = true;
    root_ = CSLoader::createNode(str_name.c_str());
    this->addChild(root_);
    cocos2d::log("%f",root_->getContentSize().height);
    image_bg = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "Image_bg"));
    image_bg->setContentSize(Size(image_bg->getContentSize().width, 250)); // the next time may change.
    int size = image_bg->getContentSize().height;
    
    image_bg->setSwallowTouches(true);
    cocos2d::log("%f",image_bg->getContentSize().height);
    image_nickname = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_square_head_pic"));
    text_nickname_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_nickname"));
    text_publish_content = dynamic_cast<TextField*>(CSLoader::seekNodeByName(root_, "TextField_content"));
    text_publish_content->setTouchEnabled(false);
    text_publish_content->setString(tools::local_string("first_moment", "这是你的第一条约友圈，对这个新鲜的圈子有什么想说的，快来表达一下吧！"));
    text_publish_content->setContentSize(Size(text_publish_content->getContentSize().width, 110)); // the next time may change.
    
    
    text_content_type = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_content_type"));
    
    size = text_publish_content->getContentSize().height;
    
    text_need_points = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_need_points"));
    text_need_points->setString(tools::local_string("need_point", "所需积分"));
    
    text_reward_points = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_reward_points"));
    text_reward_points->setString(tools::local_string("reward_point", "悬赏积分"));
    tt_need_point = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "Text_need_points"));
    tt_reward_point = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "Text_reward_points"));
    
    text_review = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_review"));
    image_review = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_review"));
    //    text_review->setString(tools::local_string("content","评论"));
    text_review->setString(tools::local_string("comment", "评论"));
    text_review->setSwallowTouches(false);
    
    text_like = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_like"));
    text_like->setTouchEnabled(true);
    text_like->setSwallowTouches(false);
    
    image_like = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_like"));
    //image_like->loadTexture("login_res/comments/dianzan-1_05.png");
    text_like->setString(tools::local_string("sharemoment", "分享"));
    text_del_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_del"));
    text_del_->addClickEventListener(std::bind(&SquareCell::click_btn_del, this, std::placeholders::_1));
    text_del_->setVisible(false);
    
    // 举报并屏蔽朋友圈
    text_shield = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_shield"));
    text_shield->addClickEventListener(std::bind(&SquareCell::click_btn_shield, this, std::placeholders::_1));
    text_shield->setSwallowTouches(false);
    
    
    
    text_need_points->setVisible(false);
    text_reward_points->setVisible(false);
    tt_need_point->setVisible(false);
    tt_reward_point->setVisible(false);
    text_content_type->setVisible(false);
    
    text_content_time = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_content_time"));
    
    image_like->setTouchEnabled(true);
    image_like->setVisible(false);
    text_like->setTouchEnabled(true);
    text_like->setSwallowTouches(false);
    
    image_review->setTouchEnabled(true);
    image_review->setVisible(false);
    text_review->setTouchEnabled(true);
    text_review->setSwallowTouches(false);
    
    //listview
    /*auto listview = dynamic_cast<ListView*>(CSLoader::seekNodeByName(root_, "listview"));
     listview_ = listview;
     
     listview_->setZOrder(-2);
     
     tableview_ = TableView::create(this, listview->getContentSize());
     tableview_->setAnchorPoint(listview->getAnchorPoint());
     tableview_->setContentOffset(Vec2(0, 0));
     tableview_->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
     tableview_->setPosition(listview->getPosition());
     tableview_->setDelegate(this);
     //    tableview_->setZOrder(-3);
     
     root_->addChild(tableview_);*/
    
    image_like->addClickEventListener(std::bind(&SquareCell::click_image_like, this, std::placeholders::_1));
    text_like->addClickEventListener(std::bind(&SquareCell::click_image_like, this, std::placeholders::_1));
    
    /*
    image_review->addClickEventListener(std::bind(&SquareCell::click_image_comment, this, std::placeholders::_1));
    text_review->addClickEventListener(std::bind(&SquareCell::click_image_comment, this, std::placeholders::_1));
    */
   
    
    sprite_icon_ = nullptr;
    
}

void SquareCell::update_property(msg::Moment* moment_data, int pic_number)
{
    this->moment_data_ = moment_data;
    update_ui();

}

void SquareCell::update_ui(){
    
    int pic_number = moment_data_->mutable_momentinfo()->mutable_normalmomentinfo()->picname().size();
    
    
    // 获取举报并屏蔽的朋友圈ID
    shield_id = moment_data_->mutable_userinfo()->userid();
    
    
    int height_change  = 0; // the height change of the cell.
    
    auto user_pic_number = moment_data_->mutable_momentinfo()->mutable_normalmomentinfo()->picname().size();
    std::vector<std::string> urls;
    std::string contentstr = std::string(moment_data_->mutable_momentinfo()->mutable_normalmomentinfo()->wordcontent());
    std::vector<std::string> strings = tools::split(moment_data_->mutable_momentinfo()->mutable_normalmomentinfo()->wordcontent(), " ");
    for (std::string str : strings) {
        if (str.find("http://")==0 || str.find("HTTP://")== 0 || str.find("Http://") ==0 ||
            str.find("https://")==0 || str.find("HTTPS://")== 0 || str.find("Https://") ==0 ||
            str.find("www")==0 || str.find("WWW")== 0){
            pic_number++;
            long index = contentstr.find(str);
            contentstr.replace(index, std::string("[网页链接]").length(), "[网页链接]");
            
            if ( str.find("www")==0 || str.find("WWW")== 0){
                str = "http://"+str;
            }
            urls.push_back(str);
        }
    }
    int type_pic_number = pic_number / 3;
    if (pic_number % 3 > 0)
    {
        type_pic_number ++;
    }
    
    bind_csb("Node_square_date.csb");
    
    if(shield_id == GDM->get_user_id()){
        text_shield->setVisible(false);
    }
    else{
        text_shield->setVisible(true);
    }

    
    // by ronaldz.  we have it automatically rendor.
    text_publish_content->setString(contentstr);
    text_publish_content->setFontSize(30);
    text_publish_content->setFontName("Thonburi");  // 这个字体必须和前面的一致， 否则就会出现算错的现象。
    
    auto render_size = text_publish_content->getAutoRenderSize();
    int char_count = text_publish_content->getCharCount();
//    if (render_size.height == 0 ){
//        render_size.height = 500; // 硬patch 遇到特殊字符好像处理不了，返回为0
//    }
    auto size = text_publish_content->getContentSize();
    auto orisize = Size(580, 110);
    
    int line_count = ceilf(render_size.width / size.width);
    if (line_count == 0)
    {
        line_count = ceilf((float)char_count/18.5);
    }
    size.height = 40 * line_count;
    
    
    text_publish_content->setContentSize(size);   // ronaldz. here to adjust to the content size.
    
    
    height_change = size.height-orisize.height;
    //size = root_->();
    //size.height += height_change;
    root_->setContentSize(root_->getContentSize() + Size(0,height_change));
    
    // 这块是为了重用这个cell， 因此需要吧上一次的都隐藏。
    for(auto sp : photos)
    {
        log("sprite tag = %d", sp->getTag());
        sp->setVisible(false);
        sp->cleanup();
        //sp->release();
    }
    photos.clear();
    // 这块是为了重用这个cell， 因此需要吧上一次的都隐藏。
    for(auto sp : comments)
    {
        log("sprite tag = %d", sp->getTag());
        sp->setVisible(false);
        sp->cleanup();
        //sp->release();
    }
    comments.clear();
    
    
    
    int CommentAreaHeight = 0;
    int likeY = 0;
    int commentY = 0;
    bool hasShared = false;
    for (int i = moment_data_->commentinfos().commentinfo_size()-1; i >=0 ; i--) {
        if(moment_data_->commentinfos().commentinfo(i).type() == 1 || moment_data_->commentinfos().commentinfo(i).type() == 3){//pinglun
            
            commentY ++;
            /*
            auto comment  = TextField::create();
            
            auto str =moment_data_->commentinfos().commentinfo(i).content();
            this->addChild(comment);
            comments.pushBack(comment);
            
            comment->setString(str.c_str());
            comment->setFontSize(20);
            comment->setColor(Color3B(0,0, 0));
            comment->setPlaceHolderColor(Color3B(200,200,200));
            
            comment->setFontName("Thonburi");
            auto render_size = comment->getAutoRenderSize();
            
            auto size = Size(380, 110);
            auto orisize = Size(380, render_size.height);  // one line.
            if(render_size.width > size.width) {
                int line_count = ceilf(render_size.width / size.width);
                size.height = render_size.height * line_count;
            }
            else {
                if (render_size.height>0){
                    size.height = render_size.height;
                }
            }
            comment->setAnchorPoint(Point(0.5,0.5));
            comment->setContentSize(size);   // ronaldz. here to adjust to the content size.
            
            comment->setPosition(Vec2(text_content_time->getPositionX() + 100+ comment->getContentSize().width/2, CommentAreaHeight +text_content_time->getContentSize().height));
            
            CommentAreaHeight += size.height;
            */
            
        }else if (moment_data_->commentinfos().commentinfo(i).type() == 2){
            likeY ++;
            uint32_t comment_like_user_id = 0;
            
            if (moment_data_->commentinfos().commentinfo(i).has_userinfo())
                comment_like_user_id = moment_data_->commentinfos().commentinfo(i).userinfo().userid();
            
            if (comment_like_user_id == GDM->get_user_id())
                hasShared = true;
        }
    }
    height_change += CommentAreaHeight;   // this is the height of the comment area.
    
    std::string str_like_result;
    if (hasShared)
    {
        str_like_result = tools::local_string("shared", "已分享") + tools::to_string(likeY);
    }else
    {
        str_like_result = tools::local_string("sharemoment", "分享") + tools::to_string(likeY);
    }
    text_like->setString(str_like_result);
    
    std::string str_review_result;
    str_review_result = tools::local_string("commentmoment", "评论") + tools::to_string(commentY);
    text_review->setString(str_review_result);
    text_review->addClickEventListener([=](Ref*){
        cocos2d::log("click_comment");
        cocos2d::log("%d,%d",moment_data_->momentid(),moment_data_->momentid());
        cmd_data_pointer data(new cmd_data_uint32(moment_data_->momentid()) );
        events::global_dispatch.raise_event(EVENT_TODO_COMMENT, data);
    });
    
    
    
        //********************
    if (moment_data_->mutable_momentinfo()->normalmomentinfo().has_handid()) {
        
        
        ImageView * temp_image = ImageView::create();
        
        this->addChild(temp_image);
        
        photos.pushBack(temp_image);
        temp_image->loadTexture("replay.png"); // to do , 把这个图换乘replay
        
        Size s(text_publish_content->getContentSize().width/3, text_publish_content->getContentSize().width/3);
        Size contentSize = temp_image->getContentSize();
        if(contentSize.width > s.width || contentSize.height > s.height) {
            temp_image->ignoreContentAdaptWithSize(true);
            temp_image->setContentSize(s);
        }
        // temp_image->setPositionY(temp_image->getPositionY() -height_change);
        
        temp_image->setAnchorPoint(Point(0.5, 0.5));
        temp_image->setContentSize(s);
        temp_image->setTouchEnabled(true);
        temp_image->setSwallowTouches(false);
        
        // int line = (i+1) / 3;
        
        temp_image->setPositionY(text_content_time->getPositionY()-text_content_time->getContentSize().height + temp_image->getContentSize().height +CommentAreaHeight);
        temp_image->setPositionX(text_publish_content->getPositionX() + temp_image->getContentSize().width /2 );
        
        temp_image->addClickEventListener([=](Ref*){
            PDM->setIsDetail(false);
            PDM->send_poker_get_hand_history(moment_data_->mutable_momentinfo()->mutable_normalmomentinfo()->handid(), true);
        });
        /*
         
         
         temp_image->addClickEventListener(std::bind(&SquareCell::click_btn_replay, this, std::placeholders::_1));
         temp_image->setTag(moment_data_->mutable_momentinfo()->mutable_normalmomentinfo()->handid());
         */
    }else{
        
        
        
        //综合内容
       for (int i = 0; i < pic_number; i++){
            std::string s = "im_";
            char c = (i+1)+'0';
            std::string str_picname = s + c;
            //ImageView * temp_image = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, str_picname.c_str()));
            
            ImageView * temp_image = ImageView::create();
            
            this->addChild(temp_image);
            
            photos.pushBack(temp_image);
            
            std::string PicName = "weblink.png";
            //TODO: 给temp_image赋值
            if (i<user_pic_number){
                PicName = moment_data_->mutable_momentinfo()->mutable_normalmomentinfo()->picname(i);
                if(GDM->is_file_exist(PicName)) {
                    temp_image->loadTexture(PicName);
                }
                else {
                    temp_image->loadTexture("msg_pic.png");
                    FM.download_file(PicName);
                }
            }else{
                temp_image->loadTexture(PicName);
            }
            
                Size size(text_publish_content->getContentSize().width/3, text_publish_content->getContentSize().width/3);
                Size contentSize = temp_image->getContentSize();
                if(contentSize.width > size.width || contentSize.height > size.height) {
                    temp_image->ignoreContentAdaptWithSize(false);
                    temp_image->setContentSize(size);
                }
               // temp_image->setPositionY(temp_image->getPositionY() -height_change);
                
                temp_image->setAnchorPoint(Point(0.5, 0.5));
                temp_image->setContentSize(size);
                temp_image->setTouchEnabled(true);
                temp_image->setSwallowTouches(false);
                
               // int line = (i+1) / 3;
           
               
                temp_image->setPositionY(text_content_time->getPositionY()-text_content_time->getContentSize().height+ (type_pic_number) * (2+temp_image->getContentSize().height)- (i - i%3) / 3 * (2+temp_image->getContentSize().height) +CommentAreaHeight);
           temp_image->setPositionX(text_publish_content->getPositionX() + temp_image->getContentSize().width /2  + i % 3 * temp_image->getContentSize().height +(i%3)* 2); // 加两个空格在图像中间。
           
           
           if ( i< user_pic_number){
                temp_image->addClickEventListener([=](Ref*){
                    
                    auto parent= static_cast<TableView*>(this->getParent()->getParent());
                    
                    if (parent!=NULL && !parent->isMoving())
                    {  //只有当父节点停止下来的时候才响应。
                        auto imageview = ImageView::create(PicName);
                        auto scene = Director::getInstance()->getRunningScene();
                        auto& size = Director::getInstance()->getWinSize();
                        auto layout = Layout::create();
                        layout->setContentSize(size);
                        float scale = 1.0f;
                        if(imageview->getContentSize().width > size.width) {
                            scale = std::min(scale, size.width / imageview->getContentSize().width);
                        }
                        if(imageview->getContentSize().height > size.height) {
                            scale = std::min(scale, size.height / imageview->getContentSize().height);
                        }
                        imageview->setScale(scale);
                        LayerColor* layer = LayerColor::create(Color4B(240, 239, 245, 255), size.width, size.height);
                        layout->addChild(layer, -2);
                        layout->setTouchEnabled(true);
                        layout->addClickEventListener([=](Ref*){
                            layout->removeFromParent();
                        });
                        layout->addChild(imageview, 0);
                        imageview->setPosition(Point(size) * 0.5);
                        scene->addChild(layout, 10);
                    }
                });
           }else{
               temp_image->addClickEventListener([=](Ref*){
                   auto parent= static_cast<TableView*>(this->getParent()->getParent());
                   
                   if (parent!=NULL && !parent->isMoving())

                   {  //只有当父节点停止下来的时候才响应。
                       Application::sharedApplication()->openURL(urls[i-user_pic_number]);
                   }
               });
           }
           
        }
    }
    
    
    int lines = pic_number / 3;
    if (pic_number % 3 > 0 || moment_data_->mutable_momentinfo()->normalmomentinfo().has_handid())
    {
        lines ++;
    }
   
    adjustPos(height_change, lines);
    //*****************
    
    
    std::string headPicName = moment_data_->mutable_userinfo()->picname();
    if(GDM->is_file_exist(headPicName)) {
        image_nickname->loadTexture(headPicName);
        
        
        image_nickname->setOpacity(0);
        image_nickname->loadTexture(headPicName);
        
        if (sprite_icon_ && sprite_icon_->getParent()) {
            sprite_icon_->removeFromParent();
        }
        sprite_icon_ = ShaderSprite::create(headPicName, "image/head_mask.png");
        sprite_icon_->setScale(image_nickname->getContentSize().width / sprite_icon_->getContentSize().width,
                               image_nickname->getContentSize().height / sprite_icon_->getContentSize().height);
        sprite_icon_->setPosition(image_nickname->getPosition());
        root_->addChild(sprite_icon_, image_nickname->getZOrder());
    }
    else {
        image_nickname->loadTexture("login_res/score_res/image_default_avatar.png");
        //image_nickname->loadTexture("login_res/score_res/image_default_avatar.png");
        FM.download_file(headPicName);
    }
    text_nickname_->setString(moment_data_->mutable_userinfo()->nickname());
    
    std::string text;
    uint64_t submit_time = moment_data_->time();
    
    double time = submit_time * 1.0;
    time /= 1000;
    text = tools::time_display(time);
    text_content_time->setString(text);
    
    
    text_review->setTag(moment_data_->momentid());
    image_review->setTag(moment_data_->momentid());
    image_like->setTag(moment_data_->momentid());
    text_like->setTag(moment_data_->momentid());
    
    if ( moment_data_->mutable_userinfo()->userid() == GDM->get_user_id()) {
        text_del_->setVisible(true);
    }else{
        text_del_->setVisible(false);
    }
    
    
    
    /*
    for (int i = 0; i < moment_data_->mutable_commentinfos()->commentinfo().size(); i++) {
        if (moment_data_->mutable_commentinfos()->commentinfo(i).userinfo().userid() ==
            GDM->get_user_data()->get_id()) {
            image_like->loadTexture("dianzan_05.png");
            text_like->setString("已分享");
        }
    }*/
}

void SquareCell::update_data(msg::Moment* fri_data, const float listview_width){
    
}

void SquareCell::adjustPos(int i, int lineOfPics){
    auto posi = image_bg->getContentSize().height;
    
    image_bg->setContentSize(Size(image_bg->getContentSize().width, 250/*design height of bg*/ + i +lineOfPics*text_publish_content->getContentSize().width/3));
    
    text_content_type->setPosition(Vec2(text_content_type->getPositionX(), text_content_type->getPositionY()+ 2 * i));
    posi = image_bg->getPositionY();
    
    posi = image_nickname->getPositionY();
    image_nickname->setPositionY(image_bg->getContentSize().height-image_nickname->getContentSize().height/2);
    posi = image_nickname->getPositionY();
    
    posi = text_nickname_->getPositionY();
    text_nickname_->setPositionY(image_bg->getContentSize().height-text_nickname_->getContentSize().height/2);
    posi = text_nickname_->getPositionY();
    
    text_del_->setPositionY(image_bg->getContentSize().height-text_del_->getContentSize().height/2);
    
   // image_nickname->setPositionY(text_nickname_->getPositionY());
    text_publish_content->setPositionY(text_nickname_->getPositionY()-text_nickname_->getContentSize().height/2);
    
    text_need_points->setPosition(Vec2(text_need_points->getPositionX(), text_need_points->getPositionY() +2 * i));
    text_reward_points->setPosition(Vec2(text_reward_points->getPositionX(), text_reward_points->getPositionY() + 2 * i));
    
    tt_need_point->setPosition(Vec2(tt_need_point->getPositionX(), tt_need_point->getPositionY() +2 * i));
    tt_reward_point->setPosition(Vec2(tt_reward_point->getPositionX(), tt_reward_point->getPositionY() +2 * i));

}

void SquareCell::click_image_like(Ref* sender)
{
    cocos2d::log("click_like");
    //todo获取朋友圈的id
    CCUMSocialSDK *sdk;
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    sdk = CCUMSocialSDK::create("556d5eaa67e58e5fcd001173");//友盟APP key
#else
    sdk = CCUMSocialSDK::create("557f7d5767e58e9cc9001d65");//友盟APP key
#endif
    sdk->setWeiXinAppInfo("wx659249d10a35848b","5b5b7b4b90c656a426d71c4f1e2bca32");//微信id，key（secret）
    /*
     vector<int>* platforms = new vector<int>();
     platforms->push_back(umeng::social::WEIXIN);
     platforms->push_back(umeng::social::WEIXIN_CIRCLE);
     
     sdk->setPlatforms(platforms);
     */
    
    //#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    //    sdk->openShare("要分享的文字内容", "/sdcard/image.png", share_selector(shareCallback));
    //#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    //    sdk->openShare("约吗约吧--尽情与朋友搓一把","icon108.png", share_selector(shareCallback));
    
    
    
    char  a[256];
    int i = text_like->getTag();
    sprintf(a,"http://%s:8080/poker/circle.jsp?momentid=%d", net_manager::shared_instence().FileServerIP.c_str(), i);
    sdk->setTargetUrl(a);
    std::string test =  "" + text_publish_content->getString();
    
    sdk->directShare(WEIXIN, test.c_str(), "icon108.png", share_selector(shareCallback));

    
    
    //todo发送点赞协议
    msg::Processor_203_PublishComment processor;
    processor.mutable_commentinfo()->set_momentid(image_like->getTag());
    processor.mutable_commentinfo()->set_type(msg::CommentType::LIKE);
    GDM->send_publish_comment(processor);
    image_like->loadTexture("dianzan_05.png");
    //text_like->setString(tools::local_string("shared", "已分享"));
    image_like->setTouchEnabled(false);
    //text_like->setTouchEnabled(false);

    
}



void SquareCell::click_image_comment(Ref* sender)
{
    cocos2d::log("click_comment");
    cocos2d::log("%d,%d",text_review->getTag(),image_review->getTag());
    
    cmd_data_pointer data(new cmd_data_uint32(image_review->getTag()) );
    
    events::global_dispatch.raise_event(EVENT_TODO_COMMENT, data);
    
}
void SquareCell::click_btn_replay(Ref* sender)
{
    //PDM->send_poker_get_hand_history(sender->getTag());
    
}
void SquareCell::click_btn_del(Ref* sender)
{
    auto tip = TipView::showAlertView("确定要删除这条消息吗？");
    tip->show_cancel(true);
    tip->set_tipview_close_callback([this](){
        GDM->send_delete_moment(image_review->getTag());
        
    });
}

// 举报并屏蔽朋友圈
void SquareCell::click_btn_shield(Ref* sender)
{
    auto processor = PM->BlockUser_up(shield_id, true);
    net_manager::shared_instence().send_data(processor,REQ_SHIELD_FRIEND);
}

#pragma mark tableview datasource --
Size SquareCell::tableCellSizeForIndex(TableView *table, ssize_t idx)
{}

TableViewCell* SquareCell::tableCellAtIndex(TableView *table, ssize_t idx)
{}
ssize_t SquareCell::numberOfCellsInTableView(TableView *table)
{}
#pragma mark tableview delegate --
void SquareCell::tableCellTouched(TableView* table, TableViewCell* cell)
{
    
}
void shareCallback(int platform,int stCode,std::string& errorMsg)
{
}


