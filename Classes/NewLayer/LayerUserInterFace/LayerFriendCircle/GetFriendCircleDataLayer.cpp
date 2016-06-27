//
//  GetFriendCircleDataLayer.cpp
//  pokerclient
//
//  Created by zhongyu on 16/6/11.
//
//

#include "GetFriendCircleDataLayer.h"
#include "LayerFriendsCircleDetail.h"
#include "Filemanager.h"
#define ZORDER_NEW_LAYER 9
#define RECORDLAYERTAG 1

GetFriendCircleDataLayer* GetFriendCircleDataLayer::create()
{
    GetFriendCircleDataLayer* layerCircleInfo=new GetFriendCircleDataLayer;
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

bool GetFriendCircleDataLayer::init(){
    if(!BaseLayer::init())
    {
        return false;
    }
    
    return true;
}

void GetFriendCircleDataLayer::sendZanData(msg::Processor_203_PublishComment processor)
{
//    msg::CommentInfo moment;
//   // msg::Processor_203_PublishComment processor; //发布评论
//    msg::Request request;
//    //msg::NormalMomentInfo *nmInfo=processor.mutable_moment()->mutable_momentinfo()->mutable_normalmomentinfo();
//    msg::CommentInfo* commentinfos=processor.mutable_commentinfo();
//    processor.mutable_commentinfo()->set_momentid(0);
//    processor.mutable_commentinfo()->set_commentid(0);
//    processor.mutable_commentinfo()->set_type(::msg::CommentType::LIKE);
//    processor.mutable_commentinfo()->set_content("1234567");
    send_data(processor, REQ_PUBLISH_COMMENT);
}

void GetFriendCircleDataLayer::getComment(int commentID)//请求评论
{
    auto processor = PM->GetComment_UP(commentID);
    send_data(processor, REQ_GET_COMMENT);
}

void GetFriendCircleDataLayer::deleteComment(int commentID)//删除评论
{
    auto processor = PM->GetComment_UP(commentID);
    send_data(processor, REQ_DELETE_MOMENT);
}


void GetFriendCircleDataLayer::register_events()
{
    dispatch_.register_event(EVENT_DOWLOAD_FILE,
                             BIND_FUNC(this, GetFriendCircleDataLayer::downloadFileEvent));
    dispatch_.register_event(REQ_PUBLISH_COMMENT,BIND_FUNC(this,GetFriendCircleDataLayer::handle_dian_zan_info));//点赞
    dispatch_.register_event(REQ_GET_COMMENT,BIND_FUNC(this,GetFriendCircleDataLayer::handle_get_comment_info));//请求评论
    dispatch_.register_event(REQ_DELETE_MOMENT,BIND_FUNC(this,GetFriendCircleDataLayer::handle_delete_comment_info));//删除朋友圈

}

void GetFriendCircleDataLayer::handle_get_publish_circle_info(cmd_data_pointer data)
{
    printf("13124");
    
}

void GetFriendCircleDataLayer::handle_delete_comment_info(cmd_data_pointer data)
{
    ShowTipMessage(false);
    std::shared_ptr<net_data_recv_package> rec=dynamic_pointer_cast<net_data_recv_package>(data);
    if(!check_request(rec)) return;
    auto processor = PM->response_down(rec);
    dynamic_cast<LayerFriendsCircleDetail*>(this->getParent())->deleteComment(processor.status());
}

void GetFriendCircleDataLayer::handle_get_comment_info(cmd_data_pointer data)//获取评论返回
{
    ShowTipMessage(false);
    std::shared_ptr<net_data_recv_package> rec=dynamic_pointer_cast<net_data_recv_package>(data);
    if(!check_request(rec)) return;
    auto processor=PM->GetComment_Down(rec);
    auto commentInfo = processor.commentinfo();
    std::vector<msg::CommentInfo> mVectorCommentInfo;
    for(auto iter : commentInfo)
    {
        mVectorCommentInfo.push_back(iter);
    }
    dynamic_cast<LayerFriendsCircleDetail*>(this->getParent())->showCommentInfo(mVectorCommentInfo);
    
}

void GetFriendCircleDataLayer::downloadFileEvent(cmd_data_pointer data)
{
    auto s = std::dynamic_pointer_cast<cmd_data_string>(data);
    auto downloadFile = s->get_value();
    if(mCurrentDownloadFile.empty() || downloadFile.empty())
    {
        return;
    }
    if(downloadFile == mCurrentDownloadFile)
    {
        auto fileName = FM.get_file_name(downloadFile);
        showImage->loadTexture(fileName);
        this->setImageShow(fileName,showImage ,0);
    }
}

void GetFriendCircleDataLayer::downloadFile(std::string imgName , ImageView* showImageView , int picPosType)
{
    mCurrentDownloadFile = imgName;
    showImage=showImageView;
    if(GDM->is_file_exist(imgName))
    {
        this->setImageShow(imgName,showImageView , picPosType);
        return;
    }
    FM.download_file(imgName);
}

void GetFriendCircleDataLayer::setImageShow(std::string imgName , ImageView* showImageView ,int picPosType)
{
    showImageView->loadTexture(FM.get_file_name(imgName));
    if(picPosType==0)
    {
        showImageView->ignoreContentAdaptWithSize(true);
        float Yscale=326.00/showImageView->getContentSize().height;
        float Xscale=628.00/showImageView->getContentSize().width;
        log("scal is %f,%f",Xscale,Yscale);
        showImageView->setVisible(true);
        float showScale;
        showScale=getMindata(Xscale, Yscale);
        showScale=getMindata(1, showScale);
        log("show scale is %f",showScale);
        showImageView->setScale((Xscale,Yscale));
    }
    /*
    if(picPosType==10000) return;//不需要调整图片大小
    showImageView->ignoreContentAdaptWithSize(true);
    float Yscale=326.00/showImageView->getContentSize().height;
    float Xscale=628.00/showImageView->getContentSize().width;
    log("scal is %f,%f",Xscale,Yscale);
    showImageView->setVisible(true);
    float showScale;    
    showScale=getMindata(Xscale, Yscale);
    showScale=getMindata(1, showScale);
    log("show scale is %f",showScale);
    showImageView->setScale((Xscale,Yscale));
     */
}

float GetFriendCircleDataLayer::getMindata(float x, float y)
{
    if(x<=y)
    {
        return x;
    }
    else
        return y;
}

void GetFriendCircleDataLayer::handle_dian_zan_info(cmd_data_pointer data)
{
    ShowTipMessage(false);
    std::shared_ptr<net_data_recv_package> rec=dynamic_pointer_cast<net_data_recv_package>(data);
    if(!check_request(rec)) return;
    auto processor = PM->response_down(rec);
    dynamic_cast<LayerFriendsCircleDetail*>(this->getParent())->showDianZanStatus(processor.status());
}

