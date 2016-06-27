//
//  LayerShowImage.hpp
//  pokerclient
//
//  Created by zhongyu on 16/6/12.
//
//

#ifndef LayerShowImage_h
#define LayerShowImage_h

#include <stdio.h>
#include "SceneManager.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "cocos-ext.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;

class LayerShowImage : public BaseLayer
{
public:
    static LayerShowImage *create();
    virtual bool init();
    void showImage(std::string image);
    float getMindata(float x, float y);
    ImageView* imageShow;
    Button* btnBack;
    
};
#endif /* LayerShowImage_hpp */
