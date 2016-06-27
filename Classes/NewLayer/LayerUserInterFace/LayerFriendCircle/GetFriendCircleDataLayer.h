//
//  GetFriendCircleDataLayer.hpp
//  pokerclient
//
//  Created by zhongyu on 16/6/11.
//
//

#ifndef GetFriendCircleDataLayer_h
#define GetFriendCircleDataLayer_h

#include <stdio.h>
#include "SceneManager.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "cocos-ext.h"
#include "utils.h"
#include "ProcessorManager.h"
#include "GameDataManager.h"
#include "NET_CMD.h"
#include "GameTools.h"
#include "TipView.h"
#include "LayerSearchUser.h"
#include "LayerInvitationList.h"
#include "LayerUserInfo.h"
#include "InvitationManager.h"
#include "ChatMessageManager.h"
#include "ShaderSprite.h"
#include "LayerChoosePhoto.h"
#include "Toast.h"
#include "RecordLayer.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;

class  GetFriendCircleDataLayer:public BaseLayer
{
public:
    static GetFriendCircleDataLayer* create();
    virtual bool init();
    void downloadFile(std::string imgName,ImageView* showImageView,int picPosType);
    void register_events();
    void handle_get_publish_circle_info(cmd_data_pointer data);
    void downloadFileEvent(cmd_data_pointer data);
    void sendZanData(msg::Processor_203_PublishComment processor);
    void handle_dian_zan_info(cmd_data_pointer data);
    void getComment(int commentID);
    void handle_get_comment_info(cmd_data_pointer data);
    void deleteComment(int commentID);//删除评论
    void handle_delete_comment_info(cmd_data_pointer data);
    float getMindata(float x, float y);
    void setImageShow(std::string imgName , ImageView* showImageView , int picPosType);
    events::event_dispatch dispatch_;
    ImageView* showImage;
    std::string mCurrentDownloadFile;

    
};

#endif /* GetFriendCircleDataLayer_hpp */
