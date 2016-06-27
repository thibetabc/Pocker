//
//  LayerMahjongSetting.hpp
//  pokerclient
//
//  Created by zhongyu on 16/6/3.
//
//

#ifndef LayerMahjongSetting_hpp
#define LayerMahjongSetting_hpp

#include "SceneManager.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

#include "NodeMahjongSettingCell.h"

using namespace cocos2d;
using namespace cocos2d::ui;

class LayerMahjongSetting : public BaseLayer
{
public:
    CREATE_FUNC(LayerMahjongSetting);
    virtual bool init();
private:
#pragma mark ui
    Node* root_;
    
    Node* node_first_;
    Node* node_second_;
    Node* node_third_;
    Node* node_fourth_;
    Node* node_fifth_;
    Node* node_sixth_;
    
    NodeMahjongSettingCell* cell_first_;
    NodeMahjongSettingCell* cell_second_;
    NodeMahjongSettingCell* cell_third_;
    NodeMahjongSettingCell* cell_fourth_;
    NodeMahjongSettingCell* cell_fifth_;
    NodeMahjongSettingCell* cell_sixth_;
    
    ImageView* image_title_;
    Button* btn_close_;
    Text* text_first_part_;
    Text* text_second_part_;
#pragma mark value
public:
#pragma mark func
    
#pragma mark click_event
    void click_btn_close(Ref* sender)
    {
        this->removeFromParent();
    }
};

#endif /* LayerMahjongSetting_hpp */
