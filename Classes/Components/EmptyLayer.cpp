#include "EmptyLayer.h"
#include "Landscape.h"

using namespace cocos2d;

cocos2d::Scene* EmptyLayer::createSecene(const bool landscape, const std::function<void()>& call)
{
    Scene* s = Scene::create();
    EmptyLayer* el = EmptyLayer::create(landscape, call);
    s->addChild(el);
    return s;
}

EmptyLayer* EmptyLayer::create(const bool landscape, const std::function<void()>& call)
{
    EmptyLayer* el = new EmptyLayer();
    if(el->init(landscape, call)) {
        el->autorelease();
    }
    else {
        delete el;
        el = nullptr;
    }
    return el;
}


bool EmptyLayer::init(const bool land, const std::function<void()>& call)
{
    if(!Layer::init()) {
        return false;
    }
    this->landscap = land;
 
    
    callback = call;
    
    return true;
}



void EmptyLayer::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();
    
    
//    if(landscap) {
//        landscape();
//    }
//    else {
//        portrait();
//    }
    
    scheduleOnce([=](float){
        if(callback) {
            this->unschedule("EmptyLayer::callback");
            log("EmptyLayer::onEnterTransitionDidFinish()");
            
           
            callback();
        }
    }, 3.0f, "EmptyLayer::callback");
}