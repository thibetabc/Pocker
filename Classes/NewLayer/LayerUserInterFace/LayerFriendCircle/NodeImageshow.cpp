//
//  NodeImageshow.cpp
//  pokerclient
//
//  Created by zhongyu on 16/6/22.
//
//

#include "NodeImageshow.h"
#include "events.h"
#include "SceneManager.h"
#include "ShaderSprite.h"
#include "LayerUserInfo.h"
#include "LayerFriendList.h"

NodeImageshow* NodeImageshow::create()
{
    NodeImageshow* nodeImageInfo=new NodeImageshow;
    if (nodeImageInfo->init())
    {
        nodeImageInfo->autorelease();
        return nodeImageInfo;
    }
    return nullptr;
    
    delete nodeImageInfo;
    nodeImageInfo = nullptr;
    return nullptr;
}

bool NodeImageshow::init()
{
    if(!Layout::init())
    {
        return false;
    }
//    ShaderSprite *imgIcon;
//    if(type ==1)//点赞头像
//    {
//        imageLayout = Layout::create();
//        imageLayout->setContentSize(Size(48,48));
//        imageLayout->setTouchEnabled(true);
//        imageLayout->setAnchorPoint(Point(0,0));
//        imageLayout->setPosition(image->getPosition());
//        imgIcon= ShaderSprite::createAndDownload(imageName, "Default/image_defualt_user_icon_small48X48.png", "Default/image_defualt_user_icon_small48X48.png");
//        imgIcon->setTag(100);
//        imgIcon->setAnchorPoint(Point(0,0));
//        imageLayout->addChild(imgIcon);
//        imageLayout->setSwallowTouches(false);
//        imageLayout->setTouchEnabled(true);
//        this->setSwallowTouches(false);
//    }
//    if(type==2)
//    {
//        imageLayout = Layout::create();
//        imageLayout->setContentSize(Size(300,210));
//        imageLayout->setTouchEnabled(true);
//        imageLayout->setAnchorPoint(Point(0,0));
//        imageLayout->setPosition(image->getPosition());
//        imgIcon= ShaderSprite::createAndDownload(imageName, "friend_circle/bg1.png", "friend_circle/bg1.png");
//        imgIcon->setTag(100);
//        imgIcon->setAnchorPoint(Point(0,0));
//        imageLayout->addChild(imgIcon);
//       // Panel_image->addChild(imageLayout);
//        //imageLayout->setTag(IMAGESHOWNUMBER+picPosType);
//       /* imageLayout->addClickEventListener([&](Ref *ref)
//                                           {
//                                               std::string name= dynamic_cast<ShaderSprite*>(dynamic_cast<Layout*>(ref)->getChildByTag(100))->getFileName();
//                                               this->imageClickEvent(name);
//                                           });
//        image->setVisible(false);
//        */
//    }
//    else if(type==3)
//    {
//        imageLayout = Layout::create();
//        imageLayout->setContentSize(Size(594,302));
//        imageLayout->setTouchEnabled(true);
//        imageLayout->setAnchorPoint(Point(0,0));
//        imageLayout->setPosition(image->getPosition());
//        imgIcon= ShaderSprite::createAndDownload(imageName, "friend_circle/bg2.png", "friend_circle/bg2.png");
//        imgIcon->setTag(100);
//        imageLayout->addChild(imgIcon);
//       // Panel_image->addChild(imageLayout);
//        //imageLayout->setTag(IMAGESHOWNUMBER+picPosType);
//       /* imageLayout->addClickEventListener([&](Ref *ref)
//                                           {
//                                               std::string name= dynamic_cast<ShaderSprite*>(dynamic_cast<Layout*>(ref)->getChildByTag(100))->getFileName();
//                                               this->imageClickEvent(name);
//                                           });
//        image->setVisible(false);
//        */
//        
//    }
//    
//    this->addChild(imageLayout);
//    //imgIcon->setSwallowTouches(false);
//    imageLayout->setSwallowTouches(false);
//    image->setSwallowTouches(false);
    this->setSwallowTouches(false);
    
    return true;
}

void NodeImageshow::setImageType(ImageView* image , int type ,std::string imageName)//type=1 大图 ，type=2 小图
{
    ShaderSprite *imgIcon;
   if(type ==1)
   {
       imageLayout = Layout::create();
       imageLayout->setContentSize(Size(300,210));
       imageLayout->setTouchEnabled(true);
       imageLayout->setAnchorPoint(Point(0,0));
       imageLayout->setPosition(image->getPosition());
       imgIcon= ShaderSprite::createAndDownload(imageName, "friend_circle/bg1.png", "friend_circle/bg1.png");
       imgIcon->setTag(100);
       imgIcon->setAnchorPoint(Point(0,0));
       imageLayout->addChild(imgIcon);
   }
    else if(type ==2)
    {
        imageLayout = Layout::create();
        imageLayout->setContentSize(Size(594,302));
        imageLayout->setTouchEnabled(true);
        imageLayout->setAnchorPoint(Point(0,0));
        imageLayout->setPosition(image->getPosition());
        imgIcon= ShaderSprite::createAndDownload(imageName, "friend_circle/bg2.png", "friend_circle/bg2.png");
        imgIcon->setTag(100);
        imageLayout->addChild(imgIcon);

    }
    
    this->addChild(imageLayout);
    //imgIcon->setSwallowTouches(false);
    imageLayout->setSwallowTouches(false);
    image->setSwallowTouches(false);
    this->setSwallowTouches(false);
}

void NodeImageshow::setImageHeadType(msg::UserDetailInfo user_info)
{
    std::string imageName=user_info.picname();
    ShaderSprite *imgIcon;
    imageLayout = Layout::create();
    imageLayout->setContentSize(Size(48,48));
    imageLayout->setTouchEnabled(true);
    imageLayout->setAnchorPoint(Point(0,0));
    imgIcon= ShaderSprite::createAndDownload(imageName, "Default/image_defualt_user_icon_small48X48.png", "Default/image_defualt_user_icon_small48X48.png");
    imgIcon->setTag(100);
    imgIcon->setAnchorPoint(Point(0,0));
    imageLayout->addChild(imgIcon);
    imageLayout->setSwallowTouches(false);
    imageLayout->setTouchEnabled(true);
    this->setSwallowTouches(false);
    user_info_=user_info;
    imageLayout->addClickEventListener([&](Ref *ref)
                                       {
                                           auto friendlistlayer= dynamic_cast<LayerFriendList*>(SceneManager::get_layer_by_name(typeid(LayerFriendList).name()));
                                           friendlistlayer->jumpToFriendInfoPage(user_info_);
                                       });
    this->addChild(imageLayout);

}

void NodeImageshow::setUserid(msg::UserDetailInfo user_info )
{
    userId_=user_info.userid();
    user_info_=user_info;
    imageLayout->addClickEventListener([&](Ref *ref)
                                       {
                                           auto friendlistlayer= dynamic_cast<LayerFriendList*>(SceneManager::get_layer_by_name(typeid(LayerFriendList).name()));
                                           friendlistlayer->jumpToFriendInfoPage(user_info_);
                                       });

    
}

int NodeImageshow::getUserid()
{
    
}
