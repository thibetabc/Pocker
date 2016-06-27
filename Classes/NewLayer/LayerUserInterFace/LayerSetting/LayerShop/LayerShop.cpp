//
//  LayerShop.cpp
//  pokerclient
//
//  Created by zhongyukeji on 16/4/11.
//
//

#include "LayerShop.h"
#include "NET_CMD.h"
#include "TipView.h"
#include "PayManager.h"
#include "ProcessorManager.h"
#include "NodeShopCell.h"
#include "TipView.h"
#include "utils.h"
#include "GameDataManager.h"
#include "LayerPayList.h"
#include "LayerChargeForOther.h"
#include "cmd_def.h"

LayerShop* LayerShop::create(bool isShop)
{
    LayerShop* shop = new LayerShop;
    if (shop->init(isShop))
    {
        shop->autorelease();
        return shop;
    }
    return nullptr;
}

bool LayerShop::init(bool isShop)
{
    if(!BaseLayer::init())
    {
        return false;
    }
    is_shop_ = isShop;
    root_ = CSLoader::createNode("LayerShop.csb");
    this->addChild(root_);
    
    
    btn_back_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_back"));
    btn_back_->addClickEventListener(std::bind(&LayerShop::click_btn_back,this,std::placeholders::_1));
    
    btn_order_tip_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root_, "btn_order_tip"));
    btn_order_tip_->addClickEventListener(std::bind(&LayerShop::click_btn_order_tip, this,std::placeholders::_1));
    if (!is_shop_ || CC_TARGET_PLATFORM != CC_PLATFORM_IOS)
    {
        btn_order_tip_->setVisible(false);
    }
    
    text_title_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_title"));
    text_title_->setString(tools::local_string("Mall","商城"));
    
    image_tip_bg_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_, "image_tip_bg"));
    image_tip_bg_->setVisible(false);
    
    text_tip_num_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_, "text_tip_num"));
    text_tip_num_->setString("0");
    
    listview_shop_ = dynamic_cast<ListView*>(CSLoader::seekNodeByName(root_, "listview_shop"));
    tableview_ = TableView::create(this, listview_shop_->getContentSize());
    tableview_->setAnchorPoint(listview_shop_->getAnchorPoint());
    tableview_->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    tableview_->setPosition(listview_shop_->getPosition());
    tableview_->setDelegate(this);

    root_->addChild(tableview_);
    listview_shop_->removeFromParent();
    
    
    if (btn_order_tip_->isVisible())
    {
        long tip_num = PayManager::getInstance()->getiOSPayList().size();
        if (tip_num>0)
        {
            image_tip_bg_->setVisible(true);
            text_tip_num_->setString(tools::to_string(tip_num));
        }
    }
    
    if (is_shop_)
    {
        shop_good_vec_.clear();
        std::vector<std::string> good_names = {"3280","600","6180","300","60"};
        std::vector<int> good_prices = {328,60,618,30,6};
        std::vector<std::string> images_name = {"login_res/shop_res/icon-diamond-3280.png","login_res/shop_res/icon-diamond-600.png","login_res/shop_res/icon-diamond-6180.png","login_res/shop_res/icon-diamond-300.png","login_res/shop_res/icon-diamond-60.png"};
        std::vector<std::string> product_id = {"com.chinarichinc.haopai.diamond_4_add","com.chinarichinc.haopai.diamond_3_add","com.chinarichinc.haopai.diamond_5_add","com.chinarichinc.haopai.diamond_2","com.chinarichinc.haopai.diamond_1"};
        for (int i = 0; i < 5 ; i += 2 )
        {
            std::vector<msg::GoodInfo> goods;
            for (int j = i ; j < i+2 && j < 5 ; j++) {
                msg::GoodInfo good;
                good.set_name(good_names[j]+tools::local_string("diamond", "钻石"));
                good.set_picname(images_name[j]);
                good.set_price(good_prices[j]);
                good.set_id(product_id[j]);
                goods.push_back(good);
            }
            shop_good_vec_.push_back(goods);
        }
        tableview_->reloadData();
    }
    else
    {
        auto processor = PM->null_up();
        send_data(processor,REQ_SEND_GET_GOODS_LIST);
    }

    return true;
}

#pragma mark click_events
void LayerShop::click_btn_back(cocos2d::Ref *sender)
{
    this->removeFromParent();
}

void LayerShop::click_btn_order_tip(cocos2d::Ref *sender)
{
    LayerPayList* pay_list = LayerPayList::create();
    this->addChild(pay_list,0,"LayerPayList");
}

#pragma mark tableview_datasource
TableViewCell* LayerShop::tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx)
{
    NodeShopCell* cell = dynamic_cast<NodeShopCell*>(table->dequeueCell());
    if(!cell) {
        cell = NodeShopCell::create();
    }
    cell->update_cell_data(shop_good_vec_.at(idx),is_shop_);
    return cell;
}

ssize_t LayerShop::numberOfCellsInTableView(cocos2d::extension::TableView *table)
{
    return shop_good_vec_.size();
}

void LayerShop::tableCellHighlight(TableView* table, TableViewCell* cell)
{
    NodeShopCell* shop_cell = dynamic_cast<NodeShopCell*>(cell);
    tableview_->touchBeganCallback_ = ([=](Touch *pTouch, Event *pEvent){
        Point touchLocation = this->convertTouchToNodeSpace(pTouch);
        if (shop_cell->is_touch_in_left(touchLocation))
        {
            shop_cell->get_left_cell()->cell_bg_load_image(false);
        }
        else if (shop_cell->is_touch_in_right(touchLocation))
        {
            shop_cell->get_right_cell()->cell_bg_load_image(false);
        }
        return true;
    });
}
void LayerShop::handle_get_id(int uId)
{
    if (uId==0)
    {
        TipView::showAlertView(tools::local_string("self_charge", "您没有好友，请为自己充值"));
    }
    else
    {
        this->SendPay(current_selected_good_.id(),uId);
    }
    this->removeChildByName("LayerChargeForOther");
    
}
void LayerShop::tableCellUnhighlight(TableView* table, TableViewCell* cell)
{
    NodeShopCell* shop_cell = dynamic_cast<NodeShopCell*>(cell);
    tableview_->touchMovedCallback_ = ([=](Touch *pTouch, Event *pEvent){
        Point touchLocation = this->convertTouchToNodeSpace(pTouch);
        shop_cell->get_left_cell()->cell_bg_load_image(true);
        shop_cell->get_right_cell()->cell_bg_load_image(true);
        return true;
    });
//    NodeShopCell* shop_cell = dynamic_cast<NodeShopCell*>(cell);
    tableview_->touchCancelledCallback_ = ([=](Touch *pTouch, Event *pEvent){
        Point touchLocation = this->convertTouchToNodeSpace(pTouch);
        shop_cell->get_left_cell()->cell_bg_load_image(true);
        shop_cell->get_right_cell()->cell_bg_load_image(true);
        return true;
    });
}

void LayerShop::tableCellTouched(TableView* table, TableViewCell* cell)
{
    NodeShopCell* shop_cell = dynamic_cast<NodeShopCell*>(cell);
    tableview_->touchEndedCallback_ = ([=](Touch *pTouch, Event *pEvent)
    {
        Point touchLocation = this->convertTouchToNodeSpace(pTouch);
        shop_cell->get_left_cell()->cell_bg_load_image(true);
        shop_cell->get_right_cell()->cell_bg_load_image(true);
        if (shop_cell->is_touch_in_left(touchLocation))
        {
            current_selected_good_.Clear();
            current_selected_good_.CopyFrom(shop_cell->get_cell_data().at(0));
            if (is_shop_)
            {
                this->addLayerCharge();
            }
            else
            {
                auto processor = PM->buygood_up(atoi( current_selected_good_.id().c_str()),GDM->get_user_id());
                send_data(processor,REQ_SEND_BUY_GOODS);
            }
            
        }
        else if (shop_cell->is_touch_in_right(touchLocation))
        {
            current_selected_good_.Clear();
            current_selected_good_.CopyFrom(shop_cell->get_cell_data().at(1));
            if (is_shop_)
            {
                this->addLayerCharge();
            }
            else
            {
                auto processor = PM->buygood_up(atoi( current_selected_good_.id().c_str()),GDM->get_user_id());
                send_data(processor,REQ_SEND_BUY_GOODS);
            }
        }
    });
    
}

void LayerShop::addLayerCharge()
{
    bool isCheckVersion = GDM->get_is_check_version();
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    isCheckVersion = false;
#endif
    LayerChargeForOther* layer = LayerChargeForOther::create(isCheckVersion);
    layer->set_callback(std::bind(&LayerShop::handle_get_id, this,std::placeholders::_1));
    this->addChild(layer,0,"LayerChargeForOther");
}


void LayerShop::click_tableview_cell()
{
    if (is_shop_)
    {
        this->SendPay(PID_, GDM->get_user_id());
    }
    else
    {
        
    }
}

void LayerShop::register_events()
{
    dispatch_.register_event(REQ_SEND_GET_GOODS_LIST,BIND_FUNC(this, LayerShop::HandleGetGoodsList));
    dispatch_.register_event(REQ_SEND_BUY_GOODS,BIND_FUNC(this,LayerShop::HandleBuyGoods));
    dispatch_.register_event(REQ_SEND_ANDROID_CHARGE, BIND_FUNC(this, LayerShop::HandleAndroidCharge));
    dispatch_.register_event(IOS_PAY_LIST_REFRESH, BIND_FUNC(this, LayerShop::ios_pay_list_refresh));
}
void LayerShop::ios_pay_list_refresh(cmd_data_pointer data)
{
    RefreshLayer();
}

void LayerShop::HandleGetGoodsList(cmd_data_pointer data)
{
    std::shared_ptr<net_data_recv_package> rec = dynamic_pointer_cast<net_data_recv_package>(data);
    if (!check_request(rec)) return;
    auto processor = PM->getGoodsInfo_down(rec);
    shop_good_vec_.clear();
    for (int i = 0 ; i < processor.goodsinfo_size() ; i += 2)
    {
        std::vector<msg::GoodInfo> goods;
        for (int j = i; j < i+2 && j < processor.goodsinfo_size() ; j++)
        {
            msg::GoodInfo good;
            good.CopyFrom(processor.goodsinfo(j));
            goods.push_back(good);
        }
        shop_good_vec_.push_back(goods);
    }
    tableview_->reloadData();
}

void LayerShop::HandleBuyGoods(cmd_data_pointer data)
{    std::shared_ptr<net_data_recv_package> rec = dynamic_pointer_cast<net_data_recv_package>(data);
    auto processor = PM->response_down(rec);
    switch (processor.status())
    {
        case msg::Status::SUCCESS:
        {
            TipView::showAlertView(tools::local_string("buy_ok", "购买成功"));
        }
            break;
        case msg::Status::FAILED:
        {
            TipView::showAlertView(processor.message());
        }
            break;
        default:
            break;
    }
    //TODO callback layerusersetting send getusersetting
}

void LayerShop::HandleAndroidCharge(cmd_data_pointer data)
{
    SetTransing(false);
    std::shared_ptr<net_data_recv_package> rec = dynamic_pointer_cast<net_data_recv_package>(data);
    auto processor = PM->androidpay_down(rec);
    
    log("充值返回 appid[%s], partnerId[%s], payId[%s], nonceStr[%s], timeStamp[%s],sign[%s]", processor.appid().c_str(), processor.partnerid().c_str(), processor.prepayid().c_str(), processor.noncestr().c_str(),processor.timestamp().c_str(), processor.sign().c_str());
    
    PayManager::getInstance()->SendWeChatPay(processor.appid(), processor.partnerid(), processor.prepayid(), processor.noncestr(),processor.timestamp(), processor.sign());
}

void LayerShop::SendPay(std::string productID,uint32_t userID)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    auto m = PayManager::getInstance();
    
    if(!m->GetTransing())
    {
        m->SetChargeForID(userID);
        m->SendBuy(productID);
        m->SetTransing(true);
    }
    else
    {
        TipView::showAlertView(tools::local_string("wait_for_previous_transaction", "请等待上次交易完成"));
    }
    
    
#else
    PayManager::getInstance()->SetTransing(true);
    auto processor = PM->androidpay_up(productID, userID);
    log("发起充值请求 productId = %s, userID = %d", productID.c_str(), userID);
    send_data(processor, REQ_SEND_ANDROID_CHARGE);
    
#endif
}

void LayerShop::RefreshLayer()
{
    if (btn_order_tip_->isVisible())
    {
        long tip_num = PayManager::getInstance()->getiOSPayList().size();
        if (tip_num>0)
        {
            image_tip_bg_->setVisible(true);
            text_tip_num_->setString(tools::to_string(tip_num));
        }
        else
        {
            image_tip_bg_->setVisible(false);
            text_tip_num_->setString("");
        }
    }
}

void LayerShop::SetTransing(bool isTransing)
{
    Node *setting_layer = Director::getInstance()->getRunningScene();
    if(isTransing)
    {
        //show
        Layer *tip_layer = Layer::create();
        tip_layer->setTouchEnabled(true);
        tip_layer->setSwallowsTouches(true);
        tip_layer->setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
        ImageView *image = ImageView::create("res/login_res/shop_05.png");
        image->setPosition(Vec2(375,720));
        
        Text *text = Text::create(tools::local_string("wait_for_charge", "正在发起购买，请稍候"), "", 26);
        text->setPosition(Vec2(0, 0));
        
        tip_layer->addChild(image);
        
        if(setting_layer)
        {
            tip_layer->setName("TipLayer");
            setting_layer->addChild(tip_layer,255);
        }
    }
    else
    {
        if(setting_layer)
        {
            Node *tip_layer = setting_layer->getChildByName("TipLayer");
            if(tip_layer)
            {
                tip_layer->removeFromParent();
            }
        }
    }
}