//
//  GameDataManager.cpp
//  pokerclient
//
//  Created by duanmt on 15/5/11.
//
//
#include "PushManager.h"
#include "GameDataManager.h"
#include "user_data.h"
#include "TipView.h"
#include "SceneManager.h"
#include "NET_CMD.h"
#include "net_manager.h"
#include "utils.h"
#include "cmd_def.h"
#include "FileManager.h"
#include "file_utils.h"
#include "PokerDataManager.h"
#include "LoginManager.h"
#include "PayManager.h"
#include "ProcessorManager.h"
#include "ChatMessageManager.h"
#include "InvitationManager.h"
#include "MomentsManager.h"
#include "LayerMain.h"
#include "LayerChat.h"
#include "LayerChooseGameRoom.h"
#include "LayerFriendList.h"
#include "LayerGroupList.h"
#include "LayerInvitationList.h"
#include "Toast.h"

#define TEST_DATA 0


GameDataManager* GameDataManager::__game_data_manager__ = nullptr;
//PokerRoomLayer* SceneManager::PokerRoom;
GameDataManager::GameDataManager()
{
    register_events();
    //开始时间
    tools::time_string();
    time_t time = tools::date();
    double t = time * 1000;
    
    //是否使用四色牌初始化
    is_newcard_ = UserDefault::getInstance()->getBoolForKey("is_newcard");
    show_group_tutor_ = UserDefault::getInstance()->getBoolForKey("show_group_tutor");
    show_friend_tutor_ = UserDefault::getInstance()->getBoolForKey("show_friend_tutor");
    show_game_tutor_ = UserDefault::getInstance()->getBoolForKey("show_game_tutor");
    show_moment_tutor_ = UserDefault::getInstance()->getBoolForKey("show_moment_tutor");
    show_setting_tutor_ = UserDefault::getInstance()->getBoolForKey("show_setting_tutor");
}

void GameDataManager::set_is_newcard(bool isnewcard){
    is_newcard_ = isnewcard;
    UserDefault::getInstance()->setBoolForKey("is_newcard", isnewcard);
    UserDefault::getInstance()->flush();
}

void GameDataManager::set_last_game_info(msg::RoomType type, bool is_tournament, int groupid, int roomid){
    client_user_setting_data.set_lastgroupid(groupid);
    client_user_setting_data.set_lastroomtype(type);
    client_user_setting_data.set_lastistournament(is_tournament);
    client_user_setting_data.set_lastroomid(roomid);
    
    std::string result = "";
    bool succ = client_user_setting_data.SerializeToString(&result);
    if(succ) {
        std::string user_cache = GDM->get_user_directory();
        file_utils::write_file(user_cache + "client_user_setting.a", result);
    }
}


void GameDataManager::set_show_tutorial(int index, bool show) {
    switch (index){
        case 1:
            show_group_tutor_ =show;
            UserDefault::getInstance()->setBoolForKey("show_group_tutor", show);
            break;
        case 2:
            show_friend_tutor_ =show;
            UserDefault::getInstance()->setBoolForKey("show_friend_tutor", show);
            break;
        case 3:
            show_game_tutor_ =show;
            UserDefault::getInstance()->setBoolForKey("show_game_tutor", show);
            break;
        case 4:
            show_moment_tutor_ =show;
            UserDefault::getInstance()->setBoolForKey("show_moment_tutor", show);
            break;
        case 5:
            show_setting_tutor_ =show;
            UserDefault::getInstance()->setBoolForKey("show_setting_tutor", show);
            break;
    }
    
    UserDefault::getInstance()->flush();
}

const bool GameDataManager::get_show_tutorial(int index) {
    switch (index){
        case 1:
            return show_group_tutor_;
            break;
        case 2:
            return show_friend_tutor_;
            break;
        case 3:
            return show_game_tutor_;
            break;
        case 4:
            return show_moment_tutor_;
            break;
        case 5:
            return show_setting_tutor_;
            break;
        default:
            return true;
            
    }
}

const msg::RoomProp& GameDataManager::get_Property_by_game_id(const msg::RoomType game_id)
{
    for (auto& iter : room_infomations_.createroominfo()) {
        if(iter.roomtype().roomtype() == game_id) {
            return iter.roomprop();
            break;
        }
    }
    assert(false);
}

void GameDataManager::login_resource_server()
{
    auto processor = PM->login_resource_up(session_key_);
    processor.set_key(session_key_);
    net_manager::shared_instence().send_data(processor, REQ_LOGIN_RESOURCES);
}

bool GameDataManager::has_session_key_(){
    return !session_key_.empty();
}

void GameDataManager::handle_login(cmd_data_pointer data)
{
    std::shared_ptr<net_data_recv_package> rec = dynamic_pointer_cast<net_data_recv_package>(data);
    auto processor = PM->login_down(rec);
    if(processor.response().status() == msg::Status::SUCCESS) {
        userinfo_.CopyFrom(processor.user());
        CM->init_chat_records();
        IM->init_invitation_records();
        MM->init_moment_records();
        
        client_user_setting_data.set_lastroomid(0);
        client_user_setting_data.set_lastgroupid(0);
        client_user_setting_data.set_lastroomtype(msg::RoomType::NLHOLDEM);
        client_user_setting_data.set_lastistournament(false);
        std::string file_path = GDM->get_user_directory() + "client_user_setting.a";
        if(file_utils::file_exists(file_path)) {
            auto content = file_utils::read_file(file_path);
            client_user_setting_data.ParseFromString(content);
        }
        
        net_manager::shared_instence().user_id = userinfo_.userid();
        session_key_ = processor.key();
        net_manager::shared_instence().FileServerIPv4 = processor.fileserverinfo().ip();
        net_manager::shared_instence().FileServerIPv6 = processor.fileserverinfo().ipv6();
        LM->get_default_account()->set_status_login(true);
        LM->save_account();
        
        login_resource_server();
    }
    else {
        TipView* tip = TipView::showAlertView(processor.response().message());
        tip->set_tipview_close_callback([=]{
            SceneManager::addLoginLayer();
        });
    }
}

const std::string GameDataManager::get_user_directory()
{
    std::string path = cocos2d::FileUtils::getInstance()->getWritablePath() + tools::to_string(userinfo_.userid());
    return path + "/";
}

const std::string GameDataManager::get_session_key()
{
    return session_key_;
}

void GameDataManager::handle_offline(cmd_data_pointer data)
{
    std::shared_ptr<net_data_recv_package> rec = dynamic_pointer_cast<net_data_recv_package>(data);
    auto processor = PM->response_down(rec);
    
    PDM->leave_all_room();
    SceneManager::addLoginLayer();
    net_manager::shared_instence().disconnect_all();
    GameDataManager::destroy();
    PokerDataManager::destroy();
    LM->get_default_account()->set_status_login(false);
    LM->save_account();
    TipView::showAlertView(tools::local_string(processor.message()));
}

void GameDataManager::handle_login_resource_server(cmd_data_pointer data)
{
    std::shared_ptr<net_data_recv_package> rec = dynamic_pointer_cast<net_data_recv_package>(data);
    auto processor = PM->login_resoucre_down(rec);
    switch(processor.response().status())
    {
        case msg::Status::SUCCESS:{
            PushManager::getInstance()->m_bIsLogin = true;
            PushManager::getInstance()->SendDeviceToken();
            if (!SceneManager::GetMainLayer())
                SceneManager::addMainLayer();
            room_infomations_.CopyFrom(processor.roominfomation());
            std::string path = cocos2d::FileUtils::getInstance()->getWritablePath() + tools::to_string(userinfo_.userid());
            file_utils::create_directory_if_missing(path);
            std::vector<std::string> vecSearchPaths;
            vecSearchPaths.push_back("res");
            vecSearchPaths.push_back("res/login_res");
            vecSearchPaths.push_back("res/login_res/game_res/effect");
            vecSearchPaths.push_back("res/image");
            vecSearchPaths.push_back("res/csb");
            vecSearchPaths.push_back("res/Default");
            vecSearchPaths.push_back("res/mahjong");
            vecSearchPaths.push_back("res/image/Mahjong_game_res");
            
            vecSearchPaths.push_back(path);

            cocos2d::FileUtils::getInstance()->setSearchPaths(vecSearchPaths);
            
            //cocos2d::FileUtils::getInstance()->addSearchPath(path);
            
            for (int i=0; i<processor.roomserverinfo().serverinfo().size(); i++){
                add_room_server(processor.roomserverinfo().serverinfo(i).ip(), processor.roomserverinfo().serverinfo(i).ipv6(), processor.roomserverinfo().serverinfo(i).port());
                net_manager::shared_instence().connect(net_manager::SERVER_ROOM);
                break;
            }
            if(processor.has_offlinemessage())
            {
                CM->receive_chat_message(processor.offlinemessage());
            }
            if(processor.has_offlinerequest())
            {
                IM->add_request(processor.offlinerequest());
            }
            if(PushManager::getInstance()->m_bPushLogin)
            {
                PushManager::getInstance()->GoToPokerRoom();
            }
            //发送未收到返回的交易凭证
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
            PayManager::getInstance()->SendFaildTrans();
#endif
            break;
        }
        case msg::Status::FAILED:
        {
            TipView* tip = TipView::showAlertView(processor.response().message());
            tip->set_tipview_close_callback([=]{
                SceneManager::addLoginLayer();
            });
            break;
        }
        default:
            break;
    }
    
}

void GameDataManager::add_room_server(std::string serverIPv4, std::string serverIPv6, uint32_t port)
{
    std::shared_ptr<server_data> roomserver(new server_data(net_manager::SERVER_ROOM, serverIPv4, serverIPv6, port));
    net_manager::shared_instence().add_server(roomserver);
    roomserver->signaldelay = 1.0f;
    roomserver->signalCountToBreak = 4;
    roomserver->signalID = REQ_SIGNAL_ROOM;
}

void GameDataManager::handle_notifychange(cmd_data_pointer data){
    std::shared_ptr<net_data_recv_package> rec = dynamic_pointer_cast<net_data_recv_package>(data);
    auto processor = PM->NotifyChange_Down(rec);
    if (processor.has_roomchange()){
        LayerMain* lm = dynamic_cast<LayerMain*>(SceneManager::GetMainLayer());
        if (lm && SceneManager::is_layer_on_top(lm))
            lm->send_get_room_info();
        
        LayerChat * lc = dynamic_cast<LayerChat*>(SceneManager::get_layer_by_name(typeid(LayerChat).name()));
        if (lc && SceneManager::is_layer_on_top(lc))
            lc->send_get_room_info();
        
        LayerChooseGameRoom * lg = dynamic_cast<LayerChooseGameRoom*>(SceneManager::get_layer_by_name(typeid(LayerChooseGameRoom).name()));
        if (lg && SceneManager::is_layer_on_top(lg))
            lg->send_get_room_info();
    }
    if (processor.has_groupchange()){
        LayerGroupList* lg = dynamic_cast<LayerGroupList*>(SceneManager::get_layer_by_name(typeid(LayerGroupList).name()));
        if (lg && SceneManager::is_layer_on_top(lg))
            lg->requestGroupList();
    }
    if (processor.has_friendchange()){
        LayerFriendList* lf = dynamic_cast<LayerFriendList*>(SceneManager::get_layer_by_name(typeid(LayerFriendList).name()));
        if (lf && SceneManager::is_layer_on_top(lf))
            lf->send_msg();
    }
}

void GameDataManager::request_change_language_room()
{
    
    msg::Response processor;
    processor.set_status(msg::Status::SUCCESS);
    net_manager::shared_instence().send_data(processor, REQ_CHANGE_LANGUAGE_ROOM);
}

void GameDataManager::handle_change_language_room(cmd_data_pointer data)
{
    std::shared_ptr<net_data_recv_package> rec = dynamic_pointer_cast<net_data_recv_package>(data);
    msg::Processor_151_GetRoomInfomation processor;
    processor.ParseFromArray(rec->body, rec->bodysize);
    room_infomations_.CopyFrom(processor.roominfomation());
}

void GameDataManager::handle_get_roommessage(cmd_data_pointer data)
{
    std::shared_ptr<net_data_recv_package> rec = dynamic_pointer_cast<net_data_recv_package>(data);
    msg::Processor_190_ReceiveMessage processor;
    processor.ParseFromArray(rec->body, rec->bodysize);
    
    
    //    // add by zhaoxuewei
    //    // 弹出message淡出
    //    auto runningscene = Director::getInstance()->getRunningScene();
    //    auto size = Director::getInstance()->getWinSize();
    //
    //    Text* text_message = Text::create(processor.message(), "Thonburi", 36);
    //    text_message->setTextColor(Color4B(181, 181, 181, 255));
    //    text_message->setAnchorPoint(Vec2(0.5, 0.5));
    //    LayerColor* message_bg = LayerColor::create(Color4B(0, 0, 0, 125), text_message->getContentSize().width + 20, 50);
    //    //message_bg->setAnchorPoint(Vec2(0.5, 0.5));
    //
    //    message_bg->addChild(text_message);
    //    text_message->setPosition(Point(message_bg->getContentSize().width/2, message_bg->getContentSize().height/2));
    //
    //    runningscene->addChild(message_bg,256);
    //    message_bg->setPosition(Point(Director::getInstance()->getWinSize().width/2, message_bg->getContentSize().height/2 + 20) - Point(message_bg->getContentSize().width/2, message_bg->getContentSize().height/2));
    //    Action * message_action = Sequence::create(DelayTime::create(4.0f), FadeOut::create(2.0f), NULL);
    //    Action * message_action_cp = message_action->clone();
    //    text_message->runAction(message_action_cp);
    //    message_bg->runAction(Sequence::create(DelayTime::create(4.0f), FadeOut::create(2.0f), NULL));
    
    Toast::ToastShow(processor.message(), 4.0f);
}

#pragma mark private
void GameDataManager::register_events()
{
    dispatch_.register_event(REQ_LOGIN_RESOURCES, BIND_FUNC(this, GameDataManager::handle_login_resource_server));
    dispatch_.register_event(REQ_LOGIN, BIND_FUNC(this, GameDataManager::handle_login));
    dispatch_.register_event(REQ_190,
                             BIND_FUNC(this, GameDataManager::handle_get_roommessage));
    dispatch_.register_event(REQ_OFFLINE,
                             BIND_FUNC(this, GameDataManager::handle_offline));
    dispatch_.register_event(REQ_NOTIFY_CHANGE,
                             BIND_FUNC(this, GameDataManager::handle_notifychange));
    dispatch_.register_event(REQ_CHANGE_LANGUAGE_ROOM,
                             BIND_FUNC(this, GameDataManager::handle_change_language_room));
}

const msg::CreateRoomInfoForSave* GameDataManager::get_create_room_info(msg::RoomType type, bool is_tournament)
{
    for (int iterCount = 0; iterCount < client_user_setting_data.createroominfoforsave().size(); iterCount++)
    {
        if (client_user_setting_data.createroominfoforsave(iterCount).istournament() == is_tournament && client_user_setting_data.createroominfoforsave(iterCount).roomtype() == type)
        {
            return &client_user_setting_data.createroominfoforsave(iterCount);
        }
    }
    
    return nullptr;
}

void GameDataManager::set_createroom_info(msg::RoomType type, bool is_tournament, const msg::RoomInfo& roominfo, const msg::RoomProp& roomprop)
{
    bool isSaveFind = false;
    for (int iterCount = 0; iterCount < client_user_setting_data.createroominfoforsave().size(); iterCount++)
    {
        if (client_user_setting_data.createroominfoforsave(iterCount).istournament() == is_tournament && client_user_setting_data.createroominfoforsave(iterCount).roomtype() == type)
        {
            client_user_setting_data.mutable_createroominfoforsave(iterCount)->mutable_roominfo()->CopyFrom(roominfo);
            client_user_setting_data.mutable_createroominfoforsave(iterCount)->mutable_roomprop()->CopyFrom(roomprop);
            isSaveFind = true;
            break;
        }
    }
    if (!isSaveFind)
    {
        msg::CreateRoomInfoForSave * createRoomInfo = client_user_setting_data.add_createroominfoforsave();
        createRoomInfo->set_roomtype(type);
        createRoomInfo->set_istournament(is_tournament);
        createRoomInfo->mutable_roomprop()->CopyFrom(roomprop);
        createRoomInfo->mutable_roominfo()->CopyFrom(roominfo);
    }
    
    std::string result = "";
    bool succ = client_user_setting_data.SerializeToString(&result);
    if(succ) {
        std::string user_cache = GDM->get_user_directory();
        file_utils::write_file(user_cache + "client_user_setting.a", result);
    }
}

void GameDataManager::set_createroom_info(msg::RoomType type, bool is_tournament, const msg::RoomInfo& roominfo, const std::string & template_size_name,  const std::string & template_speed_name,const msg::TournamentInfo& tournament_info)
{
    bool isSaveFind = false;
    for (int iterCount = 0; iterCount < client_user_setting_data.createroominfoforsave().size(); iterCount++)
    {
        if (client_user_setting_data.createroominfoforsave(iterCount).istournament() == is_tournament && client_user_setting_data.createroominfoforsave(iterCount).roomtype() == type)
        {
            client_user_setting_data.mutable_createroominfoforsave(iterCount)->mutable_roominfo()->CopyFrom(roominfo);
            client_user_setting_data.mutable_createroominfoforsave(iterCount)->set_templatesizename(template_size_name);
            client_user_setting_data.mutable_createroominfoforsave(iterCount)->set_templatespeedname(template_speed_name);
            client_user_setting_data.mutable_createroominfoforsave(iterCount)->mutable_tournamentinfo()->CopyFrom(tournament_info);
            isSaveFind = true;
            break;
        }
    }
    if (!isSaveFind)
    {
        msg::CreateRoomInfoForSave * createRoomInfo = client_user_setting_data.add_createroominfoforsave();
        createRoomInfo->set_roomtype(type);
        createRoomInfo->set_istournament(is_tournament);
        createRoomInfo->mutable_roominfo()->CopyFrom(roominfo);
        createRoomInfo->set_templatesizename(template_size_name);
        createRoomInfo->set_templatespeedname(template_speed_name);
        createRoomInfo->mutable_tournamentinfo()->CopyFrom(tournament_info);
    }
    
    std::string result = "";
    bool succ = client_user_setting_data.SerializeToString(&result);
    if(succ) {
        std::string user_cache = GDM->get_user_directory();
        file_utils::write_file(user_cache + "client_user_setting.a", result);
    }
}

bool GameDataManager::is_file_exist(const std::string& filename)
{
    std::string file_name = FM.get_file_name(filename);
    if(cocos2d::FileUtils::getInstance()->isFileExist(file_name)) {
        return true;
    }
    auto user_path = get_user_directory() + file_name;
    if(cocos2d::FileUtils::getInstance()->isFileExist(user_path)) {
        return true;
    }
    return false;
}
std::vector<float> GameDataManager::get_self_score(msg::Processor_170_GetHistoryScore_Down processor)
{
    std::vector<float> score;
    float allscore = 0;
    float allscore_time = 0;
    float allscore_hands = 0;
    float alltime = 0;
    float allhands = 0;
    for(int i=processor.score().size()-1;i>=0;i--)
    {
        if (processor.score(i).playtime()!= 0)
        {
            allscore_time += processor.score(i).score();
            alltime += (1.0 * processor.score(i).playtime() / (1000 * 60 * 60));
        }
        if (processor.score(i).playhands() != 0)
        {
            allscore_hands += processor.score(i).score();
            allhands += processor.score(i).playhands();
        }
        allscore += processor.score(i).score();
    }
    score.push_back(allscore);
    score.push_back(allhands);
    score.push_back(allscore_hands);
    score.push_back(alltime);
    score.push_back(allscore_time);
    return score;
}
