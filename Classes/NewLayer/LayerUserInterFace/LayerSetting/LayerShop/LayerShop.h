//
//  LayerShop.hpp
//  pokerclient
//
//  Created by zhongyukeji on 16/4/11.
//
//

#ifndef LayerShop_hpp
#define LayerShop_hpp

#include <stdio.h>
#include "SceneManager.h"
#include "cocos-ext.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;

class LayerShop : public BaseLayer ,public TableViewDataSource ,public TableViewDelegate
{
public:
    static LayerShop* create(bool isShop=true);
    virtual bool init(bool isShop);
    void handle_get_id(int uId);
#pragma mark ui
private:
    Node* root_;
    Button* btn_back_;
    Button* btn_order_tip_;
    Text* text_tip_num_;
    Text* text_title_;
    ImageView* image_tip_bg_;
    
    ListView* listview_shop_;
    TableView* tableview_;
    
#pragma mark click_events
protected:
    void click_btn_back(Ref* sender);
    void click_btn_order_tip(Ref* sender);
    void click_tableview_cell();
    void addLayerCharge();
    
protected:
    void register_events();
    void HandleGetGoodsList(cmd_data_pointer data);
    void HandleBuyGoods(cmd_data_pointer data);
    void HandleAndroidCharge(cmd_data_pointer data);
    void SendBuyGood();
    void RefreshLayer();
    void ios_pay_list_refresh(cmd_data_pointer data);
    
#pragma mark tableview_datascource
    virtual cocos2d::Size tableCellSizeForIndex(TableView *table, ssize_t idx){return Size(750,400);};
    virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(TableView *table);
#pragma mark tableview delegate --
    virtual void tableCellTouched(TableView* table, TableViewCell* cell);
    virtual void tableCellHighlight(TableView* table, TableViewCell* cell);
    virtual void tableCellUnhighlight(TableView* table, TableViewCell* cell);
    virtual void tableCellWillRecycle(TableView* table, TableViewCell* cell){};
    
private:
    std::vector<msg::GoodInfo> shop_goods;
    msg::GoodInfo current_selected_good_;
    std::vector<std::vector<msg::GoodInfo>> shop_good_vec_;
    void SetTransing(bool isTransing);
    void SendPay(std::string productID,uint32_t userID);
    bool is_shop_;
    std::string PID_="com.chinarichinc.haopai.diamond_1";

};
#endif /* LayerShop_hpp */
