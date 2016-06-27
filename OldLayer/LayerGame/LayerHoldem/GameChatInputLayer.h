#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

using namespace cocos2d;
using namespace cocos2d::ui;

class GameChatInputLayer : public Layer, public EditBoxDelegate
{
public:
    CREATE_FUNC(GameChatInputLayer);
    virtual bool init();
    
    void click_image_back(Ref* sender);
    void click_image_clear(Ref* sender);
    void click_image_send(Ref* sender);
    
#pragma mark EditBox delegate
    virtual void editBoxReturn(EditBox* editBox);
    
    void keyBoardWillShow(const float duration, const float distance);
    void keyBoardWillHide(const float duration, const float distance);
    
private:
    uint32_t groupid;
    Node* root_;
    Node* image_container_;
    EditBox* tf_inputbox_;
};