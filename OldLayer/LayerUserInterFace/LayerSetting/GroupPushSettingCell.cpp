//
//  GroupPushSettingCell.cpp
//  pokerclient
//
//  Created by ronaldz on 15/10/29.
//
//
#include "cocos2d.h"
#include "GroupPushSettingCell.h"
#include "utils.h"
#include "cocostudio/CocoStudio.h"
using namespace cocos2d;
bool GroupPushSettingCell::init()
{
    auto root = CSLoader::createNode("GroupPushSettingCell.csb");
    this->addChild(root);
    return true;
}
void GroupPushSettingCell::update(msg::Property *property)
{
    
}