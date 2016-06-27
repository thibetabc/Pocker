//
//  ShopCell.h
//  pokerclient
//
//  Created by qqronaldz on 16/1/4.
//
//

#ifndef ShopCell_hpp
#define ShopCell_hpp

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "BaseLayer.h"
#include "cocos-ext.h"
#include "events.h"
#include "msg.pb.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;

class chat_data;

class ShopCell : public Widget
{
public:
    CREATE_FUNC(ShopCell);
    
    virtual bool init();
    
    void update_data(const std::shared_ptr<msg::GoodInfo>& data , bool isDiamond);
    
    void click_image_bg();
    
    void set_clicked(bool IsClicked);
private:
    ImageView *bg;
    Text *name;
    Text *descrip;
    Text *money;
    std::string ID = "com.chinarichinc.haopai.diamond_1";
    bool m_bIsdiamond;
    
    
};


#endif /* ShopCell_hpp */
