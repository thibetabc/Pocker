//
//  MatchBaseInfo.hpp
//  pokerclient
//
//  Created by qqronaldz on 15/11/26.
//
//

#ifndef MatchBaseInfo_h
#define MatchBaseInfo_h

#include <stdio.h>
#include "ui/CocosGUI.h"
#include "cocos-ext.h"
#include "cocos2d.h"
#include "BaseLayer.h"
#include "GameTools.h"
using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;

class MatchBaseInfo : public BaseLayer ,public EditBoxDelegate
{
    
public:
    
    CREATE_FUNC(MatchBaseInfo);
    static MatchBaseInfo *create(bool isCreate,bool type = false)
    {
        m_bIsCreate = isCreate;
        m_bType = type;
        return create();
    }
    bool init();
    void set_start_time();
    void click_check_box(Ref *sender);
    void refresh();
    void set_isCreate(bool isCreate){this->m_bIsCreate = isCreate;}
#pragma mark EditBox delegate
    virtual void editBoxEditingDidBegin(EditBox* editBox);
    virtual void editBoxEditingDidEnd(EditBox* editBox);
    virtual void editBoxTextChanged(EditBox* editBox, const std::string& text);
    virtual void editBoxReturn(EditBox* editBox);
    
    
    virtual bool onTouchBegan(Touch *touch, Event *unused_event);
    /** Callback function for touch moved.
     *
     * @param touch Touch infomation.
     * @param unused_event Event information.
     * @js NA
     */
    virtual void onTouchMoved(Touch *touch, Event *unused_event){}
    /** Callback function for touch ended.
     *
     * @param touch Touch infomation.
     * @param unused_event Event information.
     * @js NA
     */
    virtual void onTouchEnded(Touch *touch, Event *unused_event){}
private:
    static bool m_bIsCreate;
    static bool m_bType;
    Node *root;
    Text *text;
    std::string year;
    std::string month;
    std::string day;
    std::string hour;
    std::string min;
    std::string sec = "1";
    int select_model_type;
    
    
    
};

#endif /* MatchBaseInfo_hpp */
