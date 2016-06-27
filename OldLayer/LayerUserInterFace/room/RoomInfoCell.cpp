#include "RoomInfoCell.h"
#include "utils.h"
// 引入相关的头文件
#include "CCUMSocialSDK.h"
#include "UMShareButton.h"
#include "net_manager.h"
#include "NET_CMD.h"
#include "GameDataManager.h"
// 使用友盟命令空间
USING_NS_UM_SOCIAL;
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#include "PayManager.h"

#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

#endif

bool RoomInfoCell::init()
{
    if(!TableViewCell::init()) {
        return false;
    }
    
    auto root = CSLoader::createNode("NodeRoomInfo.csb");
    this->addChild(root);
    
    image_bg_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root, "image_bg"));
    image_icon_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root, "image_icon"));
    text_name_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_name"));
    text_group_name_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_group_name"));
    text_status_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_status"));
    text_player_number_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_player_number"));
    text_share_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_share"));
    
    text_share_->addClickEventListener(std::bind(&RoomInfoCell::share, this, std::placeholders::_1));
    
    text_group_name_->setVisible(false);
    
    return true;
}


void RoomInfoCell::update_data(const msg::RoomInfo& data)
{
    data_ = data;
//    image_icon_->loadTexture("");
    text_name_->setString(data_.roomname());
    std::string key;
    switch (data_.roomstatus()) {
        case 0:
            key = "open";
            break;
        case 1:
            key = "close";
            break;
        case 2:
            key = "waiting";
            break;
            
        default:
            break;
    }
    std::string status = tools::local_string(key, "关闭还是开启");
    text_status_->setString(status);
    std::string number = tools::to_string(data.playernum()) + "/" + tools::to_string(data.maxplayernum());
    text_player_number_->setString(number);
    
    //关闭和开启都使用开启图标 --bylvhznagle 20151207
    //std::string image_name = data_->get_status() == 0 ? "login_res/roomlist_res/image_icon_room.png" : "login_res/roomlist_res/image_icon_room_closed.png";
    std::string image_name = "login_res/roomlist_res/image_icon_room.png";


    image_icon_->loadTexture(image_name);
    image_icon_->cocos2d::Node::setContentSize(Size(90, 95));
    
    bool is_tournament = data_.istournament();
    if (is_tournament)
    {
        double db_time = data_.starttime();
        db_time /= 1000;
        std::string text_start_time = tools::time_display(db_time, tools::TIMEDISPLAYTYPE::ALLTIME);
        text_group_name_->setString(tools::local_string("starting_at", "开始时间：") + text_start_time);
        text_group_name_->setVisible(true);
    }
    
    
    std::string image_bg_name = "login_res/group_res/group_cell_bg.png";
    if (data_.isofficalgroup())
    {
        image_bg_name = "login_res/group_res/group_cell_bg_2.png";
    }
    image_bg_->loadTexture(image_bg_name);
}

void RoomInfoCell::set_is_touch(bool is_touch)
{
    std::string image_bg_name;
    if (is_touch)
    {
        image_bg_name = "login_res/group_res/group_cell_bg_select.png";
        
    }else {
        image_bg_name = "login_res/group_res/group_cell_bg.png";
        if (data_.isofficalgroup())
        {
            image_bg_name = "login_res/group_res/group_cell_bg_2.png";
        }
    }
    image_bg_->loadTexture(image_bg_name);
}

void RoomInfoCell::share(Ref* sender)
{
    CCUMSocialSDK *sdk;
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    sdk = CCUMSocialSDK::create("556d5eaa67e58e5fcd001173");//友盟APP key
#else
    sdk = CCUMSocialSDK::create("557f7d5767e58e9cc9001d65");//友盟APP key
#endif

    sdk->setWeiXinAppInfo("wx659249d10a35848b","5b5b7b4b90c656a426d71c4f1e2bca32");//微信id，key（secret）
    
    vector<int>* platforms = new vector<int>();
    platforms->push_back(umeng::social::WEIXIN);
    platforms->push_back(umeng::social::WEIXIN_CIRCLE);
    
    sdk->setPlatforms(platforms);
    
    char  a[256];
    sprintf(a,"http://%s:8080/poker/share.jsp?userid=%s&groupid=%s&roomid=%s",
            net_manager::shared_instence().FileServerIP.c_str(),
            StringUtils::format("%d", GDM->get_user_id()).c_str(),
            StringUtils::format("%d", get_room_data().groupid()).c_str(),
            StringUtils::format("%d", data_.roomid()).c_str());
    sdk->setTargetUrl(a);
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    sdk->openShare ("我在好牌里创建了一个房间，快来一起玩啊！", "icon108.png", nullptr);
#else
    sdk->openShare ("我在好牌里创建了一个房间，快来一起玩啊！", "assets/Icon108.png", nullptr);
#endif

}