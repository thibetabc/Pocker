//
//  LayerMenu.h
//  pokerclient
//
//  Created by lvzhangle on 16/4/19.
//
//

#ifndef __pokerclient__LayerMenu__
#define __pokerclient__LayerMenu__


#include "cocos2d.h"
#include "ui/CocosGUI.h"

#include "msg.pb.h"

using namespace cocos2d;
using namespace cocos2d::ui;

class LayerMenu : public Layer
{
public:
    static LayerMenu* create(Point posWorld, cocos2d::Size sizeElement, const msg::Property& property);
    
     void set_property_callback(std::function<void(const msg::Property&)> property_callback) {property_callback_ = property_callback;}
    
private:
    bool init(Point posWorld, cocos2d::Size sizeElement, const msg::Property& property);
    void click_menu(Ref *sender);
    
    std::function<void(const msg::Property&)> property_callback_;

    msg::Property property_;
private:
    
    cocos2d::Point posWorld_;
    cocos2d::Size sizeElement_;
    
    
};

#endif /* defined(__pokerclient__LayerMenu__) */