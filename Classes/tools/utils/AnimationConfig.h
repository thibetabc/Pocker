#pragma once

#include "cocos2d.h"

using namespace cocos2d;

namespace AnimationConfig {
    Animation *createAnimationWithName(const std::string& plistName,float delay);
    Animate *createAnimWithName(const std::string& plistName,float delay);
    Sprite* getFirstSpriteByName(const std::string& plistName);
    
    
    
    
    
    void addSpriteFrameToCache(const std::string& plistName);
    void clear();
    
    
    
    
    
    const std::string getFirstFrameNameByName(const std::string& plistName);
    static std::map<std::string, std::string> _firstFrameName;
}