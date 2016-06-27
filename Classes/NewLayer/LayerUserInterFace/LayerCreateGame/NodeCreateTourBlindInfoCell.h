//
//  NodeCreateTourBlindInfoCell.h
//  pokerclient
//
//  Created by lvzhangle on 16/5/26.
//
//
#ifndef __pokerclient__NodeCreateTourBlindInfoCell__
#define __pokerclient__NodeCreateTourBlindInfoCell__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "cocos-ext.h"
#include "msg.pb.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;


class NodeCreateTourBlindInfoCell : public Widget, public EditBoxDelegate {
public:
    
    static NodeCreateTourBlindInfoCell* create(uint32_t idx, msg::BlindLevel blindLevel, bool isCreate);
    
    virtual bool init(uint32_t idx, msg::BlindLevel blindLevel, bool isCreate);
    
    uint32_t get_idx() {return cell_idx_;}
    
    bool get_editting() {return is_editting_;}
    
    void set_editting(bool isEditting);
    
    msg::BlindLevel get_blind_info() {return blind_level_;}
    
    void set_cell_callback(std::function<void(const int &)> blind_cell_callback) {blind_cell_callback_ = blind_cell_callback;}
    void set_change_callback(std::function<void(const int &, msg::BlindLevel blindLevel)> change_blind_callback) {change_blind_callback_ = change_blind_callback;}
    void set_add_callback(std::function<void(const int &)> blind_add_callback) {blind_add_callback_ = blind_add_callback;}
    void set_delete_callback(std::function<void(const int &)> blind_delete_callback) {blind_delete_callback_ = blind_delete_callback;}
private:
    void click_cell(Ref * sender);
    void click_add_blind_cell(Ref * sender);
    void click_delete_blind_cell(Ref * sender);
#pragma mark EditBox delegate
    virtual void editBoxEditingDidBegin(EditBox* editBox);
    virtual void editBoxEditingDidEnd(EditBox* editBox);
    virtual void editBoxTextChanged(EditBox* editBox, const std::string& text);
    virtual void editBoxReturn(EditBox* editBox);
    
    uint32_t cell_idx_;
    bool is_create_;
    msg::BlindLevel blind_level_;
    bool is_editting_ = false;
    std::function<void(const int &)> blind_cell_callback_;
    std::function<void(const int &, msg::BlindLevel blindLevel)> change_blind_callback_;
    std::function<void(const int &)> blind_add_callback_;
    std::function<void(const int &)> blind_delete_callback_;
#pragma mark UI
    Button* btn_delete_blind_cell_;
    Button* btn_add_blind_cell_;
    
    EditBox* editbox_blind_info_blind_;
    EditBox* editbox_blind_info_ante_;
    EditBox* editbox_blind_info_lasttime_;
    EditBox* editbox_blind_info_resttime_;
    
    TextField* tf_blind_info_blind_;
    TextField* tf_blind_info_ante_;
    TextField* tf_blind_info_lasttime_;
    TextField* tf_blind_info_resttime_;
    
    Text* text_blind_info_rank_;
    Text* text_blind_info_blind_;
    Text* text_blind_info_ante_;
    Text* text_blind_info_lasttime_;
    Text* text_blind_info_resttime_;
    
    ImageView* image_property_input_bg_;
    
    Node* root_;
};

#endif /* defined(__pokerclient__NodeCreateTourBlindInfoCell__) */


