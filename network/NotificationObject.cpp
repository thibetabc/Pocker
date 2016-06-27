//
//  NotificationObject.cpp
//  OceanFish
//
//  Created by mingt on 11/18/13.
//
//

#include "NotificationObject.h"

NotificationObject* NotificationObject::create(const rapidjson::Document& doc)
{
    NotificationObject *pRet = new NotificationObject();
    if (pRet && pRet->init(doc))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    } 
}

bool NotificationObject::init(const rapidjson::Document& doc)
{
//    _doc = doc.;
    _data = std::shared_ptr<message_block>(new message_block(1));
    return true;
}

void NotificationObject::set_data(const char* data, uint32_t length)
{
    _data = std::shared_ptr<message_block>(new message_block(length));
    _data->copy(data, length);
}