//
//  MatchChipLevelNode.hpp
//  pokerclient
//
//  Created by qqronaldz on 15/11/27.
//
//

#ifndef MatchChipLevelNode_hpp
#define MatchChipLevelNode_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "msg.pb.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
using namespace cocos2d;
using namespace extension;
using namespace cocos2d::ui;

class MatchChipLevelNode : public TableViewCell ,public cocos2d::ui::EditBoxDelegate 
{
public:
    CREATE_FUNC(MatchChipLevelNode);
    virtual bool init();
    void update_data(msg::BlindLevel *data,int idx,bool isCreate);
    void set_is_touch(bool is_touch);
    void set_edit(bool b);
    bool get_editing(){return m_bEditing;}
    void DeleteNode();
    void AddNode();
    void set_custom();
    void EditNode();
#pragma mark EditBox delegate
    virtual void editBoxEditingDidBegin(EditBox* editBox);
    virtual void editBoxEditingDidEnd(EditBox* editBox);
    virtual void editBoxTextChanged(EditBox* editBox, const std::string& text);
    virtual void editBoxReturn(EditBox* editBox);
    
//#pragma mark Layer delegate
//    virtual bool onTouchBegan(Touch *touch, Event *unused_event);
//    virtual void onTouchMoved(Touch *touch, Event *unused_event);
//    virtual void onTouchEnded(Touch *touch, Event *unused_event);

private:
    ImageView *bg;
    ImageView *delete_node;
    ImageView *add_node;
    Text *level;
    Text *blind_chip;
    Text *ante;
    Text *raise_chip_time;
    Text *rest_time;
    
    Node *root;
    bool m_bEditing;
    msg::BlindLevel *m_pData;
    int idx;
};

#endif /* MatchChipLevelNode_hpp */
