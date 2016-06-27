//
//  MatchChipLevelNode.cpp
//  pokerclient
//
//  Created by qqronaldz on 15/11/27.
//
//

#include "MatchChipLevelNode.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "cocos-ext.h"
#include "cocos2d.h"
#include "msg.pb.h"
#include "utils.h"
#include "GameTools.h"
#include "GameDataManager.h"
#include "RoomCreateMatch.h"
#define TAG_LEVEL                 10001
#define TAG_BLIND_CHIP            10002
#define TAG_ANTE                  10003
#define TAG_RAISE_CHIP_TIME       10004
#define TAG_REST_TIME             10005
#define TAG_ADD_BTN               10006
using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;
using namespace cocos2d;
//bool RoomCreateMatch::m_bIsFirstOpen ;
//int  RoomCreateMatch::m_nChoosedSizeModel ;
//int  RoomCreateMatch::m_nChoosedSpeedModel ;

bool MatchChipLevelNode::init()
{
    if (!TableViewCell::init())
    {
        return false;
    }
    m_bEditing = false;
    root = CSLoader::createNode("NodeChipLevel.csb");
    this->addChild(root);
    level = dynamic_cast<Text *>(CSLoader::seekNodeByName(root, "Text_1"));
    blind_chip = dynamic_cast<Text *>(CSLoader::seekNodeByName(root, "Text_2"));
    ante = dynamic_cast<Text *>(CSLoader::seekNodeByName(root, "Text_3"));
    raise_chip_time = dynamic_cast<Text *>(CSLoader::seekNodeByName(root, "Text_4"));
    rest_time = dynamic_cast<Text *>(CSLoader::seekNodeByName(root, "Text_5"));
    bg = dynamic_cast<ImageView *>(CSLoader::seekNodeByName(root, "image_shared"));
    level->setTag(TAG_LEVEL);
    blind_chip ->setTag(TAG_BLIND_CHIP);
    ante ->setTag(TAG_ANTE);
    raise_chip_time->setTag(TAG_RAISE_CHIP_TIME);
    rest_time->setTag(TAG_REST_TIME);
    rest_time = dynamic_cast<Text *>(CSLoader::seekNodeByName(root, "Text_5"));
    
    delete_node = dynamic_cast<ImageView *>(CSLoader::seekNodeByName(root, "Image_6"));
    delete_node->addClickEventListener(std::bind(&MatchChipLevelNode::DeleteNode, this));
    m_pData = NULL;
    delete_node->setVisible(false);
    
    add_node = dynamic_cast<ImageView *>(CSLoader::seekNodeByName(root, "Image_5"));
    add_node->setTag(TAG_ADD_BTN);
    add_node->addClickEventListener(std::bind(&MatchChipLevelNode::AddNode, this));
    return true;
}
void MatchChipLevelNode::set_custom()
{
    
}
void MatchChipLevelNode::AddNode()
{
    msg::BlindLevel *value = new msg::BlindLevel(*m_pData);
//    GDM->get_create_room_info()->mutable_tournamentinfo()->mutable_tournamentlevel()->mutable_level()->AddAllocated(value);
//    //排序
//    for (int i = GDM->get_create_room_info()->mutable_tournamentinfo()->mutable_tournamentlevel()->mutable_level()->size()-1; i > idx; i--)
//    {
//        GDM->get_create_room_info()->mutable_tournamentinfo()->mutable_tournamentlevel()->mutable_level()->SwapElements(i, i-1);
//    }
    
    auto table = (TableView *)this->getParent()->getParent();
    if (table)
    {
        table->reloadData();
    }
    RoomCreateMatch::m_nChoosedSpeedModel = 0;
}

void MatchChipLevelNode::DeleteNode()
{
//    if(GDM->get_create_room_info()->mutable_tournamentinfo()->mutable_tournamentlevel()->mutable_level()->size() == 1)
//    {
//        return;
//    }
//    GDM->get_create_room_info()->mutable_tournamentinfo()->mutable_tournamentlevel()->mutable_level()->DeleteSubrange(idx, 1);
    auto table = (TableView *)this->getParent()->getParent();
    this->removeFromParent();
    if (table)
    {
        table->reloadData();
    }
    RoomCreateMatch::m_nChoosedSpeedModel = 0;
    
}

void MatchChipLevelNode::set_edit(bool b)
{
    delete_node->setVisible(b);
    add_node->setVisible(!b);
    std::string image_bg_name;
    if (b)
    {
        image_bg_name = "login_res/group_res/group_cell_bg_select.png";
        
    }else {
        image_bg_name = "login_res/group_res/group_cell_bg.png";
    }
    bg->loadTexture(image_bg_name);
    if (b)
    {
//        auto level = (Text *)root->getChildByTag(TAG_LEVEL);
//        if ( level != NULL)
//        {

//        }
        auto blind_chip = (Text *)root->getChildByTag(TAG_BLIND_CHIP);
        if (blind_chip)
        {
            auto editbox  = GameTools::createEditBoxFromText(root, blind_chip, blind_chip->getString(),Size(155,104));
            editbox->setReturnType(EditBox::KeyboardReturnType::DONE);
            editbox->setInputMode(cocos2d::ui::EditBox::InputMode::NUMERIC);
            editbox->setDelegate(this);
            
        }
        auto ante = (Text *)root->getChildByTag(TAG_ANTE);
        if (ante)
        {
            auto editbox  = GameTools::createEditBoxFromText(root, ante, ante->getString(),Size(110,104));
            editbox->setReturnType(EditBox::KeyboardReturnType::DONE);
            editbox->setInputMode(cocos2d::ui::EditBox::InputMode::NUMERIC);
            editbox->setDelegate(this);
            
        }
        auto raise_chip_time = (Text *)root->getChildByTag(TAG_RAISE_CHIP_TIME);
        if (raise_chip_time)
        {
            auto editbox  = GameTools::createEditBoxFromText(root, raise_chip_time, raise_chip_time->getString(),Size(132,104));
            editbox->setInputMode(cocos2d::ui::EditBox::InputMode::NUMERIC);
            editbox->setDelegate(this);
            
        }
        auto rest_time = (Text *)root->getChildByTag(TAG_REST_TIME);
        if (rest_time)
        {
            auto editbox  = GameTools::createEditBoxFromText(root, rest_time, rest_time->getString(),Size(130,104));
            editbox->setInputMode(cocos2d::ui::EditBox::InputMode::NUMERIC);
            editbox->setDelegate(this);
        }
        m_bEditing = true;
    }
    else
    {
//        auto level = (EditBox *)root->getChildByTag(TAG_LEVEL);
//        if ( level != NULL)
//        {
//
//        }
        auto blind_chip = (EditBox *)root->getChildByTag(TAG_BLIND_CHIP);
        blind_chip->setText(tools::anything2string(m_pData->bigblind()).c_str());
        if (blind_chip)
        {
            auto text = GameTools::createTextFromEditBox(root, blind_chip, "Arial", 36);
            if (m_pData)
            {
                std::stringstream s1;
                s1<<m_pData->smallblind();
                s1<<'/';
                s1<<m_pData->bigblind();
                text->setString(s1.str().c_str());
                
            }
        }
        auto ante = (EditBox *)root->getChildByTag(TAG_ANTE);
        if (ante)
        {
            GameTools::createTextFromEditBox(root, ante, "Arial", 36);
        }
        auto raise_chip_time = (EditBox *)root->getChildByTag(TAG_RAISE_CHIP_TIME);
        if (raise_chip_time)
        {
            GameTools::createTextFromEditBox(root, raise_chip_time, "Arial", 36);
        }
        auto rest_time = (EditBox *)root->getChildByTag(TAG_REST_TIME);
        if (rest_time)
        {
            GameTools::createTextFromEditBox(root, rest_time, "Arial", 36);
        }
        m_bEditing = false;
    }
    
}

void MatchChipLevelNode::update_data(msg::BlindLevel *data,int idx,bool isCreate)
{
    this->idx = idx - 1;
    m_pData = data;
    std::stringstream s1;
    s1<<data->smallblind();
    s1<<'/';
    s1<<data->bigblind();
    
    std::stringstream s2;
    s2<<data->ante();
    
    std::stringstream s3;
    s3<<data->lasttime();
    
    std::stringstream s4;
    s4<<data->resttime();
    if (m_bEditing)
    {
        set_edit(false);
        m_bEditing = false;
    }
    auto num = (Text *)root->getChildByTag(TAG_LEVEL);
    if (num)
    {
        num->setString(tools::anything2string(idx));
    }
    auto blind_chip = (Text *)root->getChildByTag(TAG_BLIND_CHIP);
    if (blind_chip)
    {
        blind_chip->setString(s1.str());
    }
    auto ante = (Text *)root->getChildByTag(TAG_ANTE);
    if (ante)
    {
        ante->setString(s2.str());
    }
    auto raise_chip_time = (Text *)root->getChildByTag(TAG_RAISE_CHIP_TIME);
    if (raise_chip_time)
    {
        raise_chip_time->setString(s3.str());
    }
    auto rest_time = (Text *)root->getChildByTag(TAG_REST_TIME);
    if (rest_time)
    {
        rest_time->setString(s4.str());
    }
    auto btn_add = (ImageView *)root->getChildByTag(TAG_ADD_BTN);
    if (!isCreate && btn_add != NULL)
    {
        btn_add->setVisible(false);
        num->setPositionX(num->getPositionX() - 15);
    }
}

void MatchChipLevelNode::set_is_touch(bool is_touch)
{
    std::string image_bg_name;
    if (is_touch)
    {
        image_bg_name = "login_res/group_res/group_cell_bg_select.png";
        
    }else {
        image_bg_name = "login_res/group_res/group_cell_bg.png";
    }
    bg->loadTexture(image_bg_name);
}

void MatchChipLevelNode::editBoxEditingDidBegin(EditBox* editBox)
{
    
}
void MatchChipLevelNode::editBoxEditingDidEnd(EditBox* editBox)
{
    
}
void MatchChipLevelNode::editBoxTextChanged(EditBox* editBox, const std::string& text)
{
    int tag = editBox->getTag();
    switch (tag) {
        case TAG_BLIND_CHIP:
        {
            std::string text = editBox->getText();
            if (m_pData)
            {
                
                std::vector<std::string> vec_s = tools::split(text, "/");
                
                if (vec_s.size() == 2)
                {
                    text = vec_s[1];
                }
                m_pData->set_smallblind(atoi(text.c_str())/2);
                m_pData->set_bigblind(atoi(text.c_str()));
                std::stringstream s1;
                s1<<m_pData->smallblind();
                s1<<'/';
                s1<<m_pData->bigblind();
                //editBox->setText(s1.str().c_str());
                
            }
        }
            break;
        case TAG_ANTE:
        {
            std::string text = editBox->getText();
            if (m_pData)
            {
                m_pData->set_ante(atoi(text.c_str()));
            }
        }
            break;
        case TAG_RAISE_CHIP_TIME:
        {
            std::string text = editBox->getText();
            if (m_pData)
            {
                m_pData->set_lasttime(atoi(text.c_str()));
            }
        }
            break;
        case TAG_REST_TIME:
        {
            std::string text = editBox->getText();
            if (m_pData)
            {
                m_pData->set_resttime(atoi(text.c_str()));
            }
        }
            break;
            
        default:
            break;
    }
    RoomCreateMatch::m_nChoosedSpeedModel = 0;
}
void MatchChipLevelNode::editBoxReturn(EditBox* editBox)
{
    
//    update_data(m_pData,this->idx+1);
}
