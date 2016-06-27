//
//  NotificationObject.h
//  OceanFish
//
//  Created by mingt on 11/18/13.
//
//

#ifndef __OceanFish__NotificationObject__
#define __OceanFish__NotificationObject__

#include <iostream>
#include "cocos2d.h"
//#include "CSContentJsonDictionary.h"
#include "json/rapidjson.h"
#include "json/document.h"

#include "message_block.hpp"


USING_NS_CC;

class NotificationObject : public Ref {
public:
    static NotificationObject* create(const rapidjson::Document& doc);
    virtual bool init(const rapidjson::Document& doc);
    
    const rapidjson::Document& get_doc() {return _doc;}
    
    void set_data(const char* data, uint32_t length);
    std::shared_ptr<message_block> get_data(){ return _data;}
private:
    rapidjson::Document _doc;
    std::shared_ptr<message_block> _data;
};

#endif /* defined(__OceanFish__NotificationObject__) */
