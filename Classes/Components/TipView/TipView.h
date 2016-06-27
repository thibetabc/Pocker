#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "events.h"
#include "MultiEditBox.h"
#include "SceneManager.h"
class user_texas;

using namespace cocos2d;
using namespace cocos2d::ui;

class TipView : public BaseLayer
{
    
public:
    static TipView* showAlertView(const std::string& message, const bool cancel = false);
    
    void set_tipview_close_callback(const std::function<void()>& callback) { tipview_close_callback_ = callback;}
    void set_tipview_cancel_callback(const std::function<void()>& callback) { tipview_cancel_callback_ = callback;}
    
    CREATE_FUNC(TipView);
    virtual bool init();
    void update_note(const std::string& message);
    
    Node* root_;
    Text* text_tip_title_;
    Button* btn_confirm_;
    Button* btn_cancle_;
    Button* btn_confirm_single_;
    
    void set_language();
    void click_btn_confirm(Ref* sender);
    void click_btn_cancle(Ref* sender);
    void show_cancel(bool isShowCancle);
    virtual void onExit();
    
    std::function<void()> tipview_close_callback_ = nullptr;
    std::function<void()> tipview_cancel_callback_ = nullptr;

    
    class EditBoxSet {
    public:
        cocos2d::ui::MultiEditBox* box;
        bool enable;
    };
    
    static std::list<EditBoxSet*> edit_boxes;
    
//    static void get_all_boxes(Node* node, std::list<EditBoxSet*>& boxes);
};


