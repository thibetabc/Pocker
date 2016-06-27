//
//  MatchJoinListNode.cpp
//  pokerclient
//
//  Created by qqronaldz on 15/12/2.
//
//

#include "MatchJoinListNode.h"
#include "GameDataManager.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "cocos-ext.h"
#include "cocos2d.h"
#include "msg.pb.h"
#include "GameTools.h"
#include "utils.h"

bool MatchJoinListNode::init()
{
    if (!TableViewCell::init())
    {
        return false;
    }
    root = CSLoader::createNode("NodeJoinList.csb");
    this->addChild(root);
    num = dynamic_cast<Text *>(CSLoader::seekNodeByName(root, "Text_num"));
    name = dynamic_cast<Text *>(CSLoader::seekNodeByName(root, "Text_name"));
    return true;
}
void MatchJoinListNode::update_data(msg::UserDetailInfo data,int idx)
{
    num->setString(tools::digital2string(idx + 1));
    name->setString(data.nickname());
}