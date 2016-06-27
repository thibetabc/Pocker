//
//  MatchBaseInfo.cpp
//  pokerclient
//
//  Created by qqronaldz on 15/11/26.
//
//

#include "MatchBaseInfo.h"
#include "RoomCreateMatch.h"
#include "cocostudio/CocoStudio.h"
#include "cocos2d.h"
#include "GameTools.h"
#include "TipView.h"
#include "msg.pb.h"
#include "GameDataManager.h"
#include "utils.h"
#include <time.h>

#define TAG_NAME      10001
#define TAG_YEAR      10002
#define TAG_MONTH     10003
#define TAG_DAY       10004
#define TAG_REWARD    10005
#define TAG_BUYIN     10006
#define TAG_ADDON     10007
#define TAG_MINPEOPLE 10008
#define TAG_MAXPEOPLE 10009
#define TAG_BEGINCHIP 10010
#define TAG_HOUR      10011
#define TAG_MIN       10012
#define TAG_ACTIONTIM 10018
#define TAG_TABLE_MAXPEOPLE        10013

#define TAG_SIZE_1                 10050
#define TAG_SIZE_2                 10051
#define TAG_SIZE_3                 10052
#define TAG_SIZE_4                 10053
#define TAG_SPEED_1                10054
#define TAG_SPEED_2                10055
#define TAG_SPEED_3                10056
#define TAG_SPEED_4                10057

#define TAG_TEXT_SIZE_1                 10060
#define TAG_TEXT_SIZE_2                 10061
#define TAG_TEXT_SIZE_3                 10062
#define TAG_TEXT_SIZE_4                 10063
#define TAG_TEXT_SPEED_1                10064
#define TAG_TEXT_SPEED_2                10065
#define TAG_TEXT_SPEED_3                10066
#define TAG_TEXT_SPEED_4                10067

using namespace cocos2d;
using namespace cocos2d::ui;

bool MatchBaseInfo::m_bIsCreate;
bool MatchBaseInfo::m_bType;
//bool RoomCreateMatch::m_bIsFirstOpen ;
//int  RoomCreateMatch::m_nChoosedSizeModel ;
//int  RoomCreateMatch::m_nChoosedSpeedModel ;

bool MatchBaseInfo::init()
{
    if (!Layer::init())
    {
        return false;
    }
//    this->setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
//    this->setTouchEnabled(true);
//    auto listener = EventListenerTouchOneByOne::create();
//    listener->setSwallowTouches(true);
//    listener->onTouchBegan = CC_CALLBACK_2(MatchBaseInfo::onTouchBegan, this);
//    listener->onTouchMoved = CC_CALLBACK_2(MatchBaseInfo::onTouchMoved, this);
//    listener->onTouchEnded = CC_CALLBACK_2(MatchBaseInfo::onTouchEnded, this);
//    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
    if(!m_bType)
    {
        root = CSLoader::createNode("LayerBaseInfo.csb");
    }
    else
    {
        root = CSLoader::createNode("LayerMatchBaseInfoFK.csb");
    }
    this->addChild(root);
    text = dynamic_cast<Text *>(CSLoader::seekNodeByName(root, "Text_1"));
    //比赛名称
    auto text_name = dynamic_cast<TextField*>(CSLoader::seekNodeByName(root, "TextField_1"));
    auto editbox_name = GameTools::createEditBoxFromTextField(this, text_name);
    editbox_name->setTag(TAG_NAME);
    editbox_name->setDelegate(this);
    editbox_name->setReturnType(EditBox::KeyboardReturnType::DONE);
    editbox_name->setInputMode(EditBox::InputMode::SINGLE_LINE);
    //开始时间
    tools::time_string();
    time_t time = tools::date();
    tm* time_struct = localtime(&time);
    int n_year  = time_struct->tm_year + 1900;
    int n_month = time_struct->tm_mon + 1;
    int n_day   = time_struct->tm_mday;
    int n_hour  = time_struct->tm_hour;
    int n_min   = time_struct->tm_min;
    std::string str_year = tools::anything2string(n_year);
    std::string str_mon  = tools::anything2string(n_month);
    std::string str_day  = tools::anything2string(n_day);
    std::string str_hour = tools::anything2string(n_hour);
    std::string str_min  = tools::anything2string(n_min);
    auto text_year = dynamic_cast<TextField*>(CSLoader::seekNodeByName(root, "TextField_year"));
    auto editbox_year = GameTools::createEditBoxFromTextField(this, text_year);
    editbox_year->setTag(TAG_YEAR);
    editbox_year->setDelegate(this);
    editbox_year->setReturnType(EditBox::KeyboardReturnType::DONE);
    editbox_year->setInputMode(EditBox::InputMode::DECIMAL);
    editbox_year->setText(str_year.c_str());
    year = str_year;
    
    auto text_month = dynamic_cast<TextField*>(CSLoader::seekNodeByName(root, "TextField_month"));
    auto editbox_month = GameTools::createEditBoxFromTextField(this, text_month);
    editbox_month->setTag(TAG_MONTH);
    editbox_month->setDelegate(this);
    editbox_month->setReturnType(EditBox::KeyboardReturnType::DONE);
    editbox_month->setInputMode(EditBox::InputMode::NUMERIC);
    editbox_month->setText(str_mon.c_str());
    month = str_mon;
    
    auto text_day = dynamic_cast<TextField*>(CSLoader::seekNodeByName(root, "TextField_day"));
    auto editbox_day = GameTools::createEditBoxFromTextField(this, text_day);
    editbox_day->setTag(TAG_DAY);
    editbox_day->setDelegate(this);
    editbox_day->setReturnType(EditBox::KeyboardReturnType::DONE);
    editbox_day->setInputMode(EditBox::InputMode::NUMERIC);
    editbox_day->setText(str_day.c_str());
    day = str_day;
    
    auto text_hour = dynamic_cast<TextField*>(CSLoader::seekNodeByName(root, "TextField_hour"));
    auto editbox_hour = GameTools::createEditBoxFromTextField(this, text_hour);
    editbox_hour->setTag(TAG_HOUR);
    editbox_hour->setDelegate(this);
    editbox_hour->setReturnType(EditBox::KeyboardReturnType::DONE);
    editbox_hour->setInputMode(EditBox::InputMode::NUMERIC);
    editbox_hour->setText(str_hour.c_str());
    hour = str_hour;
    
    auto text_min = dynamic_cast<TextField*>(CSLoader::seekNodeByName(root, "TextField_min"));
    auto editbox_min = GameTools::createEditBoxFromTextField(this, text_min);
    editbox_min->setTag(TAG_MIN);
    editbox_min->setDelegate(this);
    editbox_min->setReturnType(EditBox::KeyboardReturnType::DONE);
    editbox_min->setInputMode(EditBox::InputMode::NUMERIC);
    min = str_min;
    editbox_min->setText(str_min.c_str());
    text->setAnchorPoint(Vec2(0,0.5));
    text->setPositionX(text->getPositionX() - text->getContentSize().width/2);
    if (!m_bType)
    {
        text->setString(tools::local_string("tournament_name", "锦标赛名称"));
        
        //最大人数
        auto text_max_people = dynamic_cast<TextField*>(CSLoader::seekNodeByName(root, "TextField_1_6"));
        auto editbox_max_people = GameTools::createEditBoxFromTextField(this, text_max_people);
        editbox_max_people->setTag(TAG_MAXPEOPLE);
        editbox_max_people->setDelegate(this);
        editbox_max_people->setReturnType(EditBox::KeyboardReturnType::DONE);
        editbox_max_people->setInputMode(EditBox::InputMode::NUMERIC);
        
    }
    else
    {

        text->setString(tools::local_string("SNG_name", "坐满即玩名称"));
    }
    
    //最少人数
    auto text_min_people = dynamic_cast<TextField*>(CSLoader::seekNodeByName(root, "TextField_1_5"));
    auto editbox_min_people = GameTools::createEditBoxFromTextField(this, text_min_people);
    editbox_min_people->setTag(TAG_MINPEOPLE);
    editbox_min_people->setDelegate(this);
    editbox_min_people->setReturnType(EditBox::KeyboardReturnType::DONE);
    editbox_min_people->setInputMode(EditBox::InputMode::NUMERIC);

    
    //奖池比例
    auto text_reward = dynamic_cast<TextField*>(CSLoader::seekNodeByName(root, "TextField_1_1"));
    auto editbox_reward = GameTools::createEditBoxFromTextField(this, text_reward);
    editbox_reward->setTag(TAG_REWARD);
    editbox_reward->setDelegate(this);
    editbox_reward->setReturnType(EditBox::KeyboardReturnType::DONE);
    editbox_reward->setInputMode(EditBox::InputMode::NUMERIC);
    //买入数量
    auto text_buyin = dynamic_cast<TextField*>(CSLoader::seekNodeByName(root, "TextField_1_2"));
    auto editbox_buyin = GameTools::createEditBoxFromTextField(this, text_buyin);
    editbox_buyin->setTag(TAG_BUYIN);
    editbox_buyin->setDelegate(this);
    editbox_buyin->setReturnType(EditBox::KeyboardReturnType::DONE);
    editbox_buyin->setInputMode(EditBox::InputMode::NUMERIC);
    //Text_1_3_3_0
    auto text_action_time = dynamic_cast<TextField*>(CSLoader::seekNodeByName(root, "TextField_1_7_0"));
    auto editbox_action_time = GameTools::createEditBoxFromTextField(this, text_action_time);
    editbox_action_time->setTag(TAG_ACTIONTIM);
    editbox_action_time->setDelegate(this);
    editbox_action_time->setReturnType(EditBox::KeyboardReturnType::DONE);
    editbox_action_time->setInputMode(EditBox::InputMode::NUMERIC);
    //addon规则
    auto text_addon = dynamic_cast<TextField*>(CSLoader::seekNodeByName(root, "TextField_1_4"));
    auto editbox_addon = GameTools::createEditBoxFromTextField(this, text_addon);
    editbox_addon->setTag(TAG_ADDON);
    editbox_addon->setDelegate(this);
    editbox_addon->setReturnType(EditBox::KeyboardReturnType::DONE);
    editbox_addon->setInputMode(EditBox::InputMode::NUMERIC);
    
    //每桌最大人数 》3
    auto text_max_people_table = dynamic_cast<TextField*>(CSLoader::seekNodeByName(root, "TextField_1_6_0"));
    auto max_people_table = GameTools::createEditBoxFromTextField(this, text_max_people_table);
    max_people_table->setTag(TAG_TABLE_MAXPEOPLE);
    max_people_table->setDelegate(this);
    max_people_table->setReturnType(EditBox::KeyboardReturnType::DONE);
    max_people_table->setInputMode(EditBox::InputMode::NUMERIC);
    //初始筹码
    auto text_begin_chips = dynamic_cast<TextField*>(CSLoader::seekNodeByName(root, "TextField_1_7"));
    auto editbox_begin_chips = GameTools::createEditBoxFromTextField(this, text_begin_chips);
    editbox_begin_chips->setTag(TAG_BEGINCHIP);
    editbox_begin_chips->setDelegate(this);
    editbox_begin_chips->setReturnType(EditBox::KeyboardReturnType::DONE);
    editbox_begin_chips->setInputMode(EditBox::InputMode::NUMERIC);
    //规模模版
    auto template_nomal = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root, "Image_14"));
    template_nomal->addClickEventListener(std::bind(&MatchBaseInfo::click_check_box, this,std::placeholders::_1));
    template_nomal->setTag(TAG_SIZE_1);
    
    
    auto template_fast = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root, "Image_15"));
    template_fast->addClickEventListener(std::bind(&MatchBaseInfo::click_check_box, this,std::placeholders::_1));
    template_fast->setTag(TAG_SIZE_2);
    
    auto template_faster = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root, "Image_16"));
    template_faster->addClickEventListener(std::bind(&MatchBaseInfo::click_check_box, this,std::placeholders::_1));
    template_faster->setTag(TAG_SIZE_3);
   
    auto template_custom = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root, "Image_17"));
    template_custom->addClickEventListener(std::bind(&MatchBaseInfo::click_check_box, this,std::placeholders::_1));
    template_custom->setTag(TAG_SIZE_4);
    
    auto size_template_text1 = dynamic_cast<Text *>(CSLoader::seekNodeByName(root, "Text_size_1"));
    size_template_text1->setTag(TAG_TEXT_SIZE_1);
    
    auto size_template_text2 = dynamic_cast<Text *>(CSLoader::seekNodeByName(root, "Text_size_2"));
    size_template_text2->setTag(TAG_TEXT_SIZE_2);
    
    auto size_template_text3 = dynamic_cast<Text *>(CSLoader::seekNodeByName(root, "Text_size_3"));
    size_template_text3->setTag(TAG_TEXT_SIZE_3);
    
    auto size_template_text4 = dynamic_cast<Text *>(CSLoader::seekNodeByName(root, "Text_size_4"));
    size_template_text4->setTag(TAG_TEXT_SIZE_4);
    
    //速度模版
    auto speed_template_nomal = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root, "Image_14_0"));
    speed_template_nomal->addClickEventListener(std::bind(&MatchBaseInfo::click_check_box, this,std::placeholders::_1));
    speed_template_nomal->setTag(TAG_SPEED_1);
    
    auto speed_template_fast = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root, "Image_15_0"));
    speed_template_fast->addClickEventListener(std::bind(&MatchBaseInfo::click_check_box, this,std::placeholders::_1));
    speed_template_fast->setTag(TAG_SPEED_2);
    
    auto speed_template_faster = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root, "Image_16_0"));
    speed_template_faster->addClickEventListener(std::bind(&MatchBaseInfo::click_check_box, this,std::placeholders::_1));
    speed_template_faster->setTag(TAG_SPEED_3);
    
    auto speed_template_custom = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root, "Image_17_0"));
    speed_template_custom->addClickEventListener(std::bind(&MatchBaseInfo::click_check_box, this,std::placeholders::_1));
    speed_template_custom->setTag(TAG_SPEED_4);
    
    auto speed_template_text1 = dynamic_cast<Text *>(CSLoader::seekNodeByName(root, "Text_speed_1"));
    speed_template_text1->setTag(TAG_TEXT_SPEED_1);
    
    auto speed_template_text2 = dynamic_cast<Text *>(CSLoader::seekNodeByName(root, "Text_speed_2"));
    speed_template_text2->setTag(TAG_TEXT_SPEED_2);
    
    auto speed_template_text3 = dynamic_cast<Text *>(CSLoader::seekNodeByName(root, "Text_speed_3"));
    speed_template_text3->setTag(TAG_TEXT_SPEED_3);
    
    auto speed_template_text4 = dynamic_cast<Text *>(CSLoader::seekNodeByName(root, "Text_speed_4"));
    speed_template_text4->setTag(TAG_TEXT_SPEED_4);
    //隐藏所有模版
    for (int i = TAG_SIZE_2; i <= TAG_SPEED_4; i++)
    {
        root->getChildByTag(i)->setVisible(false);
        root->getChildByTag(i+10)->setVisible(false);
    }
    root->getChildByTag(TAG_SPEED_1)->setVisible(true);
    root->getChildByTag(TAG_TEXT_SPEED_1)->setVisible(true);
    //显示服务器要求模版
    int size = (int)GDM->get_room_infomations().tournamentsizetemplate().size();
    for(int i = TAG_SIZE_2; i <TAG_SIZE_2 + size; i++)
    {
        ImageView *model_size = (ImageView *)root->getChildByTag(i);
        model_size->setVisible(true);
        Text * text = (Text *)root->getChildByTag(i+10);
        text->setVisible(true);
        text->setString(GDM->get_room_infomations().tournamentsizetemplate(i - TAG_SIZE_2).templatename());
        
    }
    int speed_size = (int)GDM->get_room_infomations().tournamentspeedtemplate().size();
    for(int i = TAG_SPEED_2; i <TAG_SPEED_2 + speed_size; i++)
    {
        root->getChildByTag(i)->setVisible(true);
        Text * text = (Text *)root->getChildByTag(i+10);
        text->setVisible(true);
        text->setString(GDM->get_room_infomations().tournamentspeedtemplate(i - TAG_SPEED_2).templatename());
    }
    if (m_bIsCreate)
    {
        if(RoomCreateMatch::m_bIsFirstOpen)
        {
            RoomCreateMatch::m_bIsFirstOpen = false;
            set_start_time();
            click_check_box(template_fast);
            click_check_box(speed_template_fast);
        }
        switch (RoomCreateMatch::m_nChoosedSizeModel)
        {
            case 0:
            {
                click_check_box(template_nomal);
                
            }
                break;
            case 1:
            {
                click_check_box(template_fast);
                
            }
                break;
            case 2:
            {
                click_check_box(template_faster);
            }
                break;
            case 3:
            {
                click_check_box(template_custom);
                
            }
                break;
                
            default:
                break;
        }
        switch (RoomCreateMatch::m_nChoosedSpeedModel)
        {
            case 0:
            {
                
                click_check_box(speed_template_nomal);
            }
                break;
            case 1:
            {
               
                click_check_box(speed_template_fast);
            }
                break;
            case 2:
            {
              
                click_check_box(speed_template_faster);
            }
                break;
            case 3:
            {
               
                click_check_box(speed_template_custom);
            }
                break;
                
            default:
                break;
        }
    }
    else
    {
       
        template_nomal->setTouchEnabled(false);
        template_fast->setTouchEnabled(false);
        template_faster->setTouchEnabled(false);
        template_custom->setTouchEnabled(false);
    }
     refresh();
    
    
    return true;
}

bool MatchBaseInfo::onTouchBegan(Touch *touch, Event *unused_event)
{
    GameTools::editBoxLoseInputFocus();
    return  true;
}

void MatchBaseInfo::click_check_box(Ref *sender)
{
    if(!m_bIsCreate)
        return;
    auto check_box = (ImageView *)sender;
    int tag = check_box->getTag();
    auto creat_layer = (RoomCreateMatch *)this->getParent();
    msg::TournamentInfo *info;
    switch (tag)
    {
        case TAG_SIZE_1:
        {
            RoomCreateMatch::m_nChoosedSizeModel = 0;
        }
            break;
        case TAG_SIZE_2:
        {
            RoomCreateMatch::m_nChoosedSizeModel = 1;
        }
            break;
        case TAG_SIZE_3:
        {
            RoomCreateMatch::m_nChoosedSizeModel = 2;
        }
            break;
        case TAG_SIZE_4:
        {
            RoomCreateMatch::m_nChoosedSizeModel = 3;
        }
            break;
        case TAG_SPEED_1:
        {
            RoomCreateMatch::m_nChoosedSpeedModel = 0;
        }
            break;
        case TAG_SPEED_2:
        {
            RoomCreateMatch::m_nChoosedSpeedModel = 1;
        }
            break;
        case TAG_SPEED_3:
        {
            RoomCreateMatch::m_nChoosedSpeedModel = 2;
        }
            break;
        case TAG_SPEED_4:
        {
            RoomCreateMatch::m_nChoosedSpeedModel = 3;
        }
            break;
            
        default:
            break;
    }
    if(tag < TAG_SPEED_1)
    {
        //规模模版
        for(int i = TAG_SIZE_1; i <= TAG_SIZE_4; i++)
        {
            if (tag == i)
            {
                continue;
            }
            auto pre_select = (ImageView*)root->getChildByTag(i);
            pre_select->loadTexture("res/login_res/wxz_05.png");
        }
        auto select = (ImageView*)root->getChildByTag(tag);
        select->loadTexture("res/login_res/xz_05.png");
        
        //数据
        int idx = tag - TAG_SIZE_2;
        if (idx < 0)
        {
            //自定义，数据不变
            return;
        }
        info->CopyFrom(GDM->get_room_infomations().tournamentsizetemplate(idx));
        
    }
    else
    {
        //速度模版
        for(int i = TAG_SPEED_1; i <= TAG_SPEED_4; i++)
        {
            if (tag == i)
            {
                continue;
            }
            auto pre_select = (ImageView*)root->getChildByTag(i);
            pre_select->loadTexture("res/login_res/wxz_05.png");
        }
        auto select = (ImageView*)root->getChildByTag(tag);
        select->loadTexture("res/login_res/xz_05.png");
        
        //数据
        int idx = tag - TAG_SPEED_2;
        if (idx < 0)
        {
            //自定义，数据不变
            return;
        }
        info->CopyFrom(GDM->get_room_infomations().tournamentspeedtemplate(idx));
        
    }
    
    
    msg::TournamentInfo *tourmentInfo;
    if(info->has_tournamentlevel())
        tourmentInfo->set_allocated_tournamentlevel(new msg::TournamentLevel(info->tournamentlevel()));
    if(info->has_tournamentprize())
        tourmentInfo->set_allocated_tournamentprize(new msg::TournamentPrize(info->tournamentprize()));
    if(info->has_maxaddonlevel())
        tourmentInfo->set_maxaddonlevel(info->maxaddonlevel());
    if(info->has_startbb())
        tourmentInfo->set_startbb(info->startbb());
    if(info->has_minplayernum())
        tourmentInfo->set_minplayernum(info->minplayernum());
    if(info->has_maxplayernum())
        tourmentInfo->set_maxplayernum(info->maxplayernum());
    if(info->has_playerpertable())
        tourmentInfo->set_playerpertable(info->playerpertable());
    
    
}

void MatchBaseInfo::refresh()
{
    msg::TournamentInfo *tourmentInfo;
    if(this->m_bIsCreate)
    {
        tourmentInfo;
        tourmentInfo->set_issitandgo(m_bType);
    }
    else
    {
        if(GDM->get_410_data()->has_tournamentinfo())
        {
            tourmentInfo = GDM->get_410_data()->mutable_tournamentinfo();
        }
        
    }
    auto name = (EditBox *)this->getChildByTag(TAG_NAME);
    name->setText(tourmentInfo->roomname().c_str());
    
    //时间
    std::string start_time = tourmentInfo->starttime();
    start_time = start_time.substr(0,start_time.length()-3);
    time_t timep = tools::string2digital<time_t>(start_time.c_str());
    tm* timeStuct = localtime(&timep);
    
    std::string str_year = tools::anything2string(timeStuct->tm_year + 1900);
    std::string str_month = tools::anything2string(timeStuct->tm_mon + 1);
    std::string str_day = tools::anything2string(timeStuct->tm_mday);
    std::string str_hour = tools::anything2string(timeStuct->tm_hour);
    std::string str_min = tools::anything2string(timeStuct->tm_min);
    
    auto year = (EditBox *)this->getChildByTag(TAG_YEAR);
    year->setText(str_year.c_str());
    auto month = (EditBox *)this->getChildByTag(TAG_MONTH);
    month->setText(str_month.c_str());
    auto day = (EditBox *)this->getChildByTag(TAG_DAY);
    day->setText(str_day.c_str());
    auto hour = (EditBox *)this->getChildByTag(TAG_HOUR);
    hour->setText(str_hour.c_str());
    auto min = (EditBox *)this->getChildByTag(TAG_MIN);
    min->setText(str_min.c_str());
//
    auto rate = (EditBox *)this->getChildByTag(TAG_REWARD);
    rate->setText(tools::digital2string<int>(tourmentInfo->prizepercent()).c_str());
    
//
    auto buyin = (EditBox *)this->getChildByTag(TAG_BUYIN);
    buyin->setText(tools::digital2string<int>(tourmentInfo->buyin()).c_str());
    //
    auto action_time = (EditBox *)this->getChildByTag(TAG_ACTIONTIM);
    action_time->setText(tools::digital2string<int>(tourmentInfo->actiontime()).c_str());
    
    
    auto addon = (EditBox *)this->getChildByTag(TAG_ADDON);
    addon->setText(tools::digital2string<int>(tourmentInfo->maxaddonlevel()).c_str());
    auto min_people = (EditBox *)this->getChildByTag(TAG_MINPEOPLE);
    min_people->setText(tools::digital2string<int>(tourmentInfo->minplayernum()).c_str());
    auto max_people = (EditBox *)this->getChildByTag(TAG_MAXPEOPLE);
    if (max_people   != NULL)
        max_people->setText(tools::digital2string<int>(tourmentInfo->maxplayernum()).c_str());
    auto table_max_people = (EditBox *)this->getChildByTag(TAG_TABLE_MAXPEOPLE);
    table_max_people->setText(tools::digital2string(tourmentInfo->playerpertable()).c_str());
    auto start_chip = (EditBox *)this->getChildByTag(TAG_BEGINCHIP);
    start_chip->setText(tools::digital2string<int>(tourmentInfo->startbb()).c_str());
    if(!this->m_bIsCreate)
    {
        
        name->setTouchEnabled(false);
        
        year->setTouchEnabled(false);
        month->setTouchEnabled(false);
        day->setTouchEnabled(false);
        hour->setTouchEnabled(false);
        min->setTouchEnabled(false);
        
        rate->setTouchEnabled(false);
        buyin->setTouchEnabled(false);
        addon->setTouchEnabled(false);
        min_people->setTouchEnabled(false);
        if(max_people)
            max_people->setTouchEnabled(false);
        table_max_people->setTouchEnabled(false);
        start_chip->setTouchEnabled(false);
        action_time->setTouchEnabled(false);
    }
}

void MatchBaseInfo::set_start_time()
{
    struct tm time ;
    time.tm_year = tools::string2digital<int>(year) - 1900;
    time.tm_mon = tools::string2digital<int>(month) - 1;
    time.tm_mday = tools::string2digital<int>(day);
    time.tm_hour = tools::string2digital<int>(hour);
    time.tm_min = tools::string2digital<int>(min);
    time.tm_sec = tools::string2digital<int>(sec);
    time.tm_zone = "CST";
    time.tm_gmtoff = 28800;
    time.tm_isdst = 0;
    
    time_t ft = mktime(&time);
    std::string startTime = tools::digital2string(ft);
    // *1000
    startTime += "000";
    //GDM->get_create_room_info()->mutable_tournamentinfo()->set_starttime(startTime);
    
}
void MatchBaseInfo::editBoxEditingDidBegin(EditBox* editBox)
{
    
}
void MatchBaseInfo::editBoxEditingDidEnd(EditBox* editBox)
{
    
}
void MatchBaseInfo::editBoxTextChanged(EditBox* editBox, const std::string& text)
{
    
}
void MatchBaseInfo::editBoxReturn(EditBox* editBox)
{
    msg::TournamentInfo *info;
    int tag = editBox->getTag();
    std::string s = editBox->getText();
    int length = (int)s.length();
    if (length == 0)
    {
        return;
    }
    switch (tag) {
        case TAG_NAME:
        {
//            GDM->get_create_room_info()->mutable_roominfo()->set_roomname(s);
//            GDM->get_create_room_info()->mutable_tournamentinfo()->set_roomname(s);
        }
            break;
        case TAG_YEAR:
        {
            if (length != 4)
            {
                //位数不对
                TipView::showAlertView("请输入四位数年份，如：2015");
                editBox->setText("");
                break;
            }
            for (auto i : s)
            {
                if (i < '0' || i > '9')
                {
                    //存在非法字符
                    TipView::showAlertView("请用0-9数字表示年份，如：2015");
                    editBox->setText("");
                    break;
                }
            }
            
            this->year = s;
            set_start_time();
            
        }
            break;
        case TAG_MONTH:
        {
            int month = atoi(s.c_str());
            if (month < 1 || month > 12)
            {
                TipView::showAlertView("请输入1-12之间的数字");
                editBox->setText("");
                break;
            }
            this->month = s;
            set_start_time();
        }
            break;
        case TAG_DAY:
        {
            int day = atoi(s.c_str());
            if (day < 1 || day > 31)
            {
                TipView::showAlertView("请输入1-31之间的数字");
                editBox->setText("");
                break;
            }
            this->day = s;
            set_start_time();
        }
            break;
        case TAG_HOUR:
        {
            int hour = atoi(s.c_str());
            if (hour < 0 || hour > 23)
            {
                TipView::showAlertView("请输入0-23之间的数字");
                editBox->setText("");
                break;
            }
            this->hour = s;
            set_start_time();
        }
            break;
        case TAG_MIN:
        {
            int min = atoi(s.c_str());
            if (min < 0 || min > 59)
            {
                TipView::showAlertView("请输入0-59之间的数字");
                editBox->setText("");
                break;
            }
            this->min = s;
            set_start_time();
        }
            break;
            
        case TAG_REWARD:
        {
            int reward = atoi(s.c_str());
            if (reward < 1 || reward > 100)
            {
                editBox->setText("");
                editBox->setPlaceHolder("");
                TipView::showAlertView("请输入1-100之间的数字");
                
                break;
            }
            info->set_prizepercent(reward);
        }
            
            break;
        case TAG_BUYIN:
        {
            for (auto i : s)
            {
                if (i < '0' || i > '9')
                {
                    //存在非法字符
                    TipView::showAlertView("存在非法字符，请输入数字");
                    editBox->setText("");
                    break;
                }
            }
            info->set_buyin(atoi(s.c_str()));
            
        }
            break;
        case TAG_ACTIONTIM:
        {
            for (auto i : s)
            {
                if (i < '0' || i > '9')
                {
                    //存在非法字符
                    TipView::showAlertView("存在非法字符，请输入数字");
                    editBox->setText("");
                    break;
                }
            }
            info->set_actiontime(atoi(s.c_str()));

        }
            break;
        case TAG_ADDON:
        {
            for (auto i : s)
            {
                if (i < '0' || i > '9')
                {
                    //存在非法字符
                    TipView::showAlertView("存在非法字符，请输入数字");
                    editBox->setText("");
                    break;
                }
            }
            info->set_maxaddonlevel(atoi(s.c_str()));
            auto model = (ImageView *)root->getChildByTag(TAG_SPEED_1);
            click_check_box(model);
            
        }
            break;
        case TAG_MINPEOPLE:
        {
            for (auto i : s)
            {
                if (i < '0' || i > '9')
                {
                    //存在非法字符
                    TipView::showAlertView("存在非法字符，请输入数字");
                    editBox->setText("");
                    break;
                }
            }
            info->set_minplayernum(atoi(s.c_str()));
            auto model = (ImageView *)root->getChildByTag(TAG_SIZE_1);
            click_check_box(model);
            if (m_bType)
            {
                info->set_maxplayernum(atoi(s.c_str()));
            }
            
            
        }
            break;
        case TAG_MAXPEOPLE:
        {
            for (auto i : s)
            {
                if (i < '0' || i > '9')
                {
                    //存在非法字符
                    TipView::showAlertView("存在非法字符，请输入数字");
                    editBox->setText("");
                    break;
                }
            }
            info->set_maxplayernum(atoi(s.c_str()));
            auto model = (ImageView *)root->getChildByTag(TAG_SIZE_1);
            click_check_box(model);
        }
            
            break;
        case TAG_TABLE_MAXPEOPLE:
        {
            for (auto i : s)
            {
                if (i < '0' || i > '9')
                {
                    //存在非法字符
                    TipView::showAlertView("存在非法字符，请输入数字");
                    editBox->setText("");
                    break;
                }
            }
            
            int i = atoi(s.c_str());
            
            if (i < 3 || i > 10)
            {
                editBox->setText("");
                TipView::showAlertView("每桌要求3-10人");
                break;
            }
            info->set_playerpertable(i);

        }
            break;
        case TAG_BEGINCHIP:
        {
            for (auto i : s)
            {
                if (i < '0' || i > '9')
                {
                    //存在非法字符
                    TipView::showAlertView("存在非法字符，请输入数字");
                    editBox->setText("");
                    break;
                }
            }
            info->set_startbb(atoi(s.c_str()));
            auto model = (ImageView *)root->getChildByTag(TAG_SPEED_1);
            click_check_box(model);

        }
            
            break;
            
        default:
            break;
    }
}