#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "BaseLayer.h"
#include "cocos-ext.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;


class LanguageCell : public TableViewCell {
public:
    CREATE_FUNC(LanguageCell);
    
    virtual bool init();
    
    void update_ui(const std::pair<std::string, std::pair<std::string, std::string> >& data);
    
    void click_image_language(Ref* sender);
    
    const std::pair<std::string, std::pair<std::string, std::string> >& get_data() { return data_;}
    
private:
    
    std::pair<std::string, std::pair<std::string, std::string> > data_;
    Text* text_language_;
    
    std::function<void(const std::string&)> callback_;
};