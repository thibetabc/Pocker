//
//  LayerShowImage.cpp
//  pokerclient
//
//  Created by zhongyu on 16/6/12.
//
//

#include "LayerShowImage.h"
#include "ProcessorManager.h"
#include "GameDataManager.h"
#include "NET_CMD.h"
#include "FileManager.h"

LayerShowImage *LayerShowImage::create()
{
    LayerShowImage* layerCircleInfo=new LayerShowImage;
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
bool LayerShowImage::init()
{
    if(!BaseLayer::init())
    {
        return false;
    }
    
    auto root = CSLoader::createNode("LayerShowImage.csb");
    this->addChild(root);
    imageShow = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root, "Image_show"));
    btnBack = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "Button_close"));
    btnBack->addClickEventListener([&](Ref *ref)
                                   {
                                       this->removeFromParent();
                                   });
    
    return true;
}

void LayerShowImage::showImage(std::string image)
{
    printf("%s",image.c_str());
    printf("root is %s",(FM.get_file_name(image)).c_str());
    imageShow->loadTexture(FM.get_file_name(image));
    
    imageShow->ignoreContentAdaptWithSize(true);
    float Yscale=1334.00/imageShow->getContentSize().height;
    float Xscale=750.00/imageShow->getContentSize().width;
    float showScale;
    showScale=getMindata(Xscale, Yscale);
    showScale=getMindata(1, showScale);
    imageShow->setScale(showScale);
}

float LayerShowImage::getMindata(float x, float y)
{
    if(x<=y)
    {
        return x;
    }
    else return y;
}
