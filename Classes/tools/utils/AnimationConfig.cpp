//
//  AnimationHelper.cpp
//  DK2.0
//
//  Created by mingt on 9/23/14.
//
//

#include "AnimationConfig.h"
#include "utils.h"

Animation *AnimationConfig::createAnimationWithName(const std::string& plistName,float delay)
{
    if (plistName.empty()) return nullptr;
    /// 优化, 从动画缓存中获取数据
    AnimationCache* cache = CCAnimationCache::getInstance();
    if (cache->getAnimation(plistName.c_str()) != nullptr) {
        Animation *animation = cache->getAnimation(plistName.c_str());
        animation->setDelayPerUnit(delay);
        return animation;
    }
    /// 优化结束
    std::vector<std::string> plist_array = tools::split(plistName, ",");
    Vector<SpriteFrame *> frames_array;
    for (const std::string& path : plist_array){
        addSpriteFrameToCache(path);
        ValueMap dictionary = FileUtils::getInstance()->getValueMapFromFile(path);
        CCAssert(!dictionary.empty(), ("error, SharedApi getAnimation:dictionary is nil,the plist:" + path).c_str());
        ValueMap& frames = dictionary.at("frames").asValueMap();
        std::vector<std::string> allkeys;
        for (auto& key : frames) {
            allkeys.push_back(key.first);
        }
        std::sort(allkeys.begin(), allkeys.end(), [](const std::string& obj1, const std::string& obj2){
            return obj1 < obj2;
        });
        _firstFrameName[plistName] = allkeys.front();
        SpriteFrameCache::getInstance()->addSpriteFramesWithFile(path.c_str());
        for (auto& key : allkeys){
            auto& frameName = key;
            SpriteFrame* frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName);
            frames_array.pushBack(frame);
        }
    }
    Animation *animation = Animation::createWithSpriteFrames(frames_array, delay);
    cache->addAnimation(animation, plistName.c_str());
    return animation;

}

Animate *AnimationConfig::createAnimWithName(const std::string& plistName,float delay)
{
    Animation *animation = createAnimationWithName(plistName, delay);
    Animate *aniamte =  Animate::create(animation);
    return aniamte;
}

const std::string AnimationConfig::getFirstFrameNameByName(const std::string& plistName)
{
    auto iter = _firstFrameName.find(plistName);
    if(iter != _firstFrameName.end()) {
        return iter->second;
    }
    return "";
}
Sprite* AnimationConfig::getFirstSpriteByName(const std::string& plistName)
{
    std::string name = getFirstFrameNameByName(plistName);
    if (name.empty()) {
        addSpriteFrameToCache(plistName);
        name = getFirstFrameNameByName(plistName);
    }
    auto sprite = Sprite::createWithSpriteFrameName(name);
    if(!sprite) {
        sprite = Sprite::create(name);
    }
    return sprite;
}

void AnimationConfig::addSpriteFrameToCache(const std::string& plistName)
{
    auto str = tools::split(plistName, ",");
    for(auto& iter : str) {
        SpriteFrameCache::getInstance()->addSpriteFramesWithFile(iter);
    }
}

void AnimationConfig::clear()
{
    _firstFrameName.clear();
}