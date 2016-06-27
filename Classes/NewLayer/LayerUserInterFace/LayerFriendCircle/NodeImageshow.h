//
//  NodeImageshow.hpp
//  pokerclient
//
//  Created by zhongyu on 16/6/22.
//
//

#ifndef NodeImageshow_h
#define NodeImageshow_h

#include <stdio.h>
#include "SceneManager.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "cocos-ext.h"

#define BIGIMG "friend_circle/bg2.png"
#define SMALLIMG "friend_circle/bg1.png"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;
class NodeImageshow:public Layout
{
public:
    static NodeImageshow* create();
    virtual bool init();
   
    void setUserid(msg::UserDetailInfo user_info);
    int getUserid();

    
    void setImageType(ImageView* image , int type ,std::string imageName);//type=1 大图 ，type=2 小图
    void setImageHeadType(msg::UserDetailInfo user_info);
    msg::UserDetailInfo user_info_;
    Layout* imageLayout;
private:
    int userId_;
    
};

#endif /* NodeImageshow_hpp */
