#pragma once

#include "cocos2d.h"

class EmptyLayer : public cocos2d::Layer {
public:
    static cocos2d::Scene* createSecene(const bool landscape, const std::function<void()>& call);
    
    static EmptyLayer* create(const bool landscape, const std::function<void()>& call);
    virtual bool init(const bool landscape, const std::function<void()>& call);
    
    
    virtual void onEnterTransitionDidFinish();
    
    bool landscap;
    std::function<void()> callback;
};

