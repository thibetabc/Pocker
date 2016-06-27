//
//  LayerGroupMenu.h
//  pokerclient
//
//  Created by ll on 15/10/13.
//
//

#ifndef __pokerclient__LayerGroupMenu__
#define __pokerclient__LayerGroupMenu__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "events.h"
#include "LayerManager.h"

using namespace cocos2d;
using namespace cocos2d::ui;

class LayerGroupMenu : public LayerPop
{
public:
    
    CREATE_FUNC(LayerGroupMenu);
    virtual bool init();
    void set_click_group_create(const std::function<void()>& callback) { click_group_create_ = callback;}
    void set_click_group_search(const std::function<void()>& callback) { click_group_search_ = callback;}
    
    void set_dismiss_callback(const std::function<void()>& callback) { dismiss_callback_ = callback;}
    
    void click_group_create(Ref* sender);
    void click_group_search(Ref* sender);
    
    virtual void onExit();
    
private:
    std::function<void()> click_group_create_;
    std::function<void()> click_group_search_;
    
    std::function<void()> dismiss_callback_;

};

#endif /* defined(__pokerclient__LayerGroupMenu__) */
