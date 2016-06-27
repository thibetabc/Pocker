//
//  ProcessorManager.cpp
//  pokerclient
//
//  Created by zhongyukeji on 16/4/11.
//
//

#include "ProcessorManager.h"
#include "utils.h"

ProcessorManager* ProcessorManager::__Processor_manager__ = nullptr;

msg::Response ProcessorManager::null_up()
{
    msg::Response processor;
    return processor;
}

msg::Response ProcessorManager::response_down(std::shared_ptr<net_data_recv_package> rec)
{
    msg::Response processor;
    processor.ParseFromArray(rec->body, rec->bodysize);
    return processor;
}

msg::Processor_97_SendVerificationByUserID ProcessorManager::send_verification_by_user(std::string account)
{
    msg::Processor_97_SendVerificationByUserID processor;
    processor.set_username(account);
    return processor;
}

msg::Processor_98_ResetPassword ProcessorManager::reset_password(std::string account, std::string password, std::string verification)
{
    msg::Processor_98_ResetPassword processor;
    msg::UserLoginInfo* userinfo = processor.mutable_logininfo();
    userinfo->set_username(account);
    userinfo->set_password(password);
    processor.set_verification(verification);
    return processor;
}

msg::Processor_99_Login_UP ProcessorManager::login_up(std::string account, std::string password, bool is_user_phone)
{
    msg::Processor_99_Login_UP processor;
    msg::UserLoginInfo* userinfo = processor.mutable_logininfo();
    if (is_user_phone)
    {
        userinfo->set_phonenum(account);
    }
    else
    {
        userinfo->set_username(account);
    }
    userinfo->set_password(password);
    return processor;
}

msg::Processor_99_Login_DOWN ProcessorManager::login_down(std::shared_ptr<net_data_recv_package> rec)
{
    msg::Processor_99_Login_DOWN processor;
    processor.ParseFromArray(rec->body, rec->bodysize);
    return processor;
}

msg::Processor_100_LoginResource ProcessorManager::login_resource_up(std::string key)
{
    msg::Processor_100_LoginResource processor;
    processor.set_key(key);
    return processor;
}

msg::Processor_100_LoginResource_Down ProcessorManager::login_resoucre_down(std::shared_ptr<net_data_recv_package> rec)
{
    msg::Processor_100_LoginResource_Down processor;
    processor.ParseFromArray(rec->body, rec->bodysize);
    return processor;
}

msg::Processor_103_FeedBack ProcessorManager::feedback_up(const std::string& description, const std::string& descrip_type, const std::string& pic_name)
{
    msg::Processor_103_FeedBack processor;
    processor.mutable_feedback()->set_describe(description);
//    processor.mutable_feedback()->set_type(descrip_type);
//    processor.mutable_feedback()->add_picname(pic_name);
    return processor;
}

msg::Processor_104_Register ProcessorManager::register_up(const std::string& username, const std::string& password, const std::string& bind_phone, const std::string& verification, const std::string& referee)
{
    msg::Processor_104_Register processor;
    msg::UserLoginInfo* userinfo = processor.mutable_logininfo();
    userinfo->set_username(username);
    userinfo->set_password(password);
    
    if (!bind_phone.empty() && !verification.empty())
    {
        processor.set_phonenumber(bind_phone);
        processor.set_verification(verification);
    }
    
    if (!referee.empty())
        processor.set_referee(referee);
    return processor;
}

msg::Processor_105_Device ProcessorManager::devicetoken_up(const std::string& devicetoken)
{
    msg::Processor_105_Device processor;
    processor.set_devicetoken(devicetoken);
    return processor;
}

msg::Processor_106_ReceiveIOSPayReceipt ProcessorManager::iospay_up(const std::string& receipt, uint32_t userid)
{
    msg::Processor_106_ReceiveIOSPayReceipt processor;
    processor.set_receipt(receipt);
    processor.set_userid(userid);
    return processor;
}

msg::Processor_106_ReceiveIOSPayReceipt_Down ProcessorManager::iospay_down(std::shared_ptr<net_data_recv_package> rec)
{
    msg::Processor_106_ReceiveIOSPayReceipt_Down processor;
    processor.ParseFromArray(rec->body, rec->bodysize);
    return processor;
}

msg::Processor_107_GetGoodsInfo ProcessorManager::getGoodsInfo_down(std::shared_ptr<net_data_recv_package> rec)
{
    msg::Processor_107_GetGoodsInfo processor;
    processor.ParseFromArray(rec->body, rec->bodysize);
    return processor;
}

msg::Processor_108_BuyGood ProcessorManager::buygood_up(uint32_t goodid,uint32_t userid)
{
    msg::Processor_108_BuyGood processor;
    processor.set_goodid(goodid);
    processor.set_userid(userid);
    return processor;
}

msg::Processor_109_AndroidPay ProcessorManager::androidpay_up(const std::string& id, uint32_t userid)
{
    msg::Processor_109_AndroidPay processor;
    processor.set_userid(userid);
    processor.set_productid(id);
    return processor;
}

msg::Processor_109_AndroidPayDown ProcessorManager::androidpay_down(std::shared_ptr<net_data_recv_package> rec)
{
    msg::Processor_109_AndroidPayDown processor;
    processor.ParseFromArray(rec->body, rec->bodysize);
    return processor;
}

msg::Processor_111_GetUserSetting ProcessorManager::getUserSetting_down(std::shared_ptr<net_data_recv_package> rec)
{
    msg::Processor_111_GetUserSetting processor;
    processor.ParseFromArray(rec->body, rec->bodysize);
    return processor;
}

msg::Processor_112_ChangeUserSetting ProcessorManager::changeUserSetting_up(const msg::UserDetailInfo& info)
{
    msg::Processor_112_ChangeUserSetting processor;
    processor.mutable_userdetailinfo()->CopyFrom(info);
    return processor;
}

msg::Processor_112_ChangeUserSetting ProcessorManager::changeUserSetting_up(const msg::UserLoginInfo& info)
{
    msg::Processor_112_ChangeUserSetting processor;
    processor.mutable_userlogininfo()->CopyFrom(info);
    return processor;
}

msg::Processor_112_ChangeUserSetting ProcessorManager::changeUserSetting_up(const msg::UserGameSettings& settings)
{
    msg::Processor_112_ChangeUserSetting processor;
    processor.mutable_gamesettings()->CopyFrom(settings);
    return processor;
}

msg::Processor_113_GetUserPermission_UP ProcessorManager::GetUserPermission_up(uint32_t groupid, uint32_t userid)
{
    msg::Processor_113_GetUserPermission_UP processor;
    processor.set_groupid(groupid);
    processor.set_userid(userid); 
    return processor;
}

//msg::Processor_113_GetUserPermission_UP ProcessorManager::GetUserPermission_up(std::vector<uint32_t> group_ids, uint32_t userid)
//{
//    msg::Processor_113_GetUserPermission_UP processor;
//    for (auto & it : group_ids)
//    {
//        processor.add_groupid(it);
//    }
//    processor.set_userid(userid);
//    return processor;
//}

msg::Processor_113_GetUserPermission_DOWN ProcessorManager::GetUserPermission_down(std::shared_ptr<net_data_recv_package> rec)
{
    msg::Processor_113_GetUserPermission_DOWN processor;
    processor.ParseFromArray(rec->body, rec->bodysize);
    return processor;
}

msg::Processor_114_ChangePermission ProcessorManager::changePermission_up(uint32_t userID, const msg:: GroupPermission& permission)
{
    msg::Processor_114_ChangePermission processor;
    processor.set_userid(userID);
    processor.mutable_permission()->CopyFrom(permission);
    return processor;
}

msg::Processor_115_GetGroupSetting_UP ProcessorManager::GetGroupSetting_up(uint32_t groupid)
{
    msg::Processor_115_GetGroupSetting_UP processor;
    processor.set_groupid(groupid);
    return processor;
}

msg::Processor_115_GetGroupSetting_DOWN ProcessorManager::GetGroupSetting_down(std::shared_ptr<net_data_recv_package> rec)
{
    msg::Processor_115_GetGroupSetting_DOWN processor;
    processor.ParseFromArray(rec->body, rec->bodysize);
    return processor;
}

msg::Processor_116_ChangeGroupSetting ProcessorManager::changeGroupSetting_up(msg::GroupSetting setting)
{
    msg::Processor_116_ChangeGroupSetting processor;
    processor.mutable_setting()->CopyFrom(setting);
    return processor;
}

msg::Processor_118_SendVerification ProcessorManager::SendVerification_up(const std::string& phoneNumber)
{
    msg::Processor_118_SendVerification processor;
    processor.set_phonenumber(phoneNumber);
    return processor;
}

msg::Processor_119_BindPhone ProcessorManager::BindPhone_up(const std::string& phoneNumber, const std::string& verification)
{
    msg::Processor_119_BindPhone processor;
    processor.set_phonenumber(phoneNumber);
    processor.set_verification(verification);
    return processor;
}

msg::Processor_120_GetFriendList ProcessorManager::getFriendList_down(std::shared_ptr<net_data_recv_package> rec)
{
    msg::Processor_120_GetFriendList processor;
    processor.ParseFromArray(rec->body, rec->bodysize);
    return processor;
}

msg::Processor_121_SearchUser_UP ProcessorManager::SearchUser_up(const std::string& key_word)
{
    msg::Processor_121_SearchUser_UP processor;
    processor.set_searchname(key_word);
    return processor;
}

msg::Processor_121_SearchUser_DOWN ProcessorManager::SearchUser_down(std::shared_ptr<net_data_recv_package> rec)
{
    msg::Processor_121_SearchUser_DOWN processor;
    processor.ParseFromArray(rec->body, rec->bodysize);
    return processor;
}

msg::Processor_122_SendRequest ProcessorManager::SendRequest_up(const msg::Request& request)
{
    msg::Processor_122_SendRequest processor;
    processor.mutable_request()->Add()->CopyFrom(request);
    return processor;
}

msg::Processor_123_ReceiveRequestList ProcessorManager::ReceiveRequestList_down(std::shared_ptr<net_data_recv_package> rec)
{
    msg::Processor_123_ReceiveRequestList processor;
    processor.ParseFromArray(rec->body, rec->bodysize);
    return processor;
}

msg::Processor_124_DealRequest ProcessorManager::DealRequest_up(uint32_t RequestID, bool IsAccepted)
{
    msg::Processor_124_DealRequest processor;
    processor.set_requestid(RequestID);
    processor.set_accepted(IsAccepted);
    return processor;
}

msg::Processor_124_DealRequest_DOWN ProcessorManager::DealRequest_down(std::shared_ptr<net_data_recv_package> rec)
{
    msg::Processor_124_DealRequest_DOWN processor;
    processor.ParseFromArray(rec->body, rec->bodysize);
    return processor;
}

msg::Processor_125_DeleteFriend ProcessorManager::DeleteFriend_up(uint32_t userid)
{
    msg::Processor_125_DeleteFriend processor;
    processor.set_userid(userid);
    return processor;
}

msg::Processor_126_Set_MomentBlock ProcessorManager::BlockUser_up(uint32_t userid, bool IsBlock)
{
    msg::Processor_126_Set_MomentBlock processor;
    processor.set_userid(userid);
    processor.set_isblock(IsBlock);
    return processor;
}

msg::Processor_127_SearchFriendByPhoneNumber_UP ProcessorManager::SearchFriendByPhoneNumber_up(const std::vector<std::string>& numbers)
{
    msg::Processor_127_SearchFriendByPhoneNumber_UP processor;
    for (auto iter: numbers)
        processor.add_phonenumber(iter);
    return processor;
}

msg::Processor_127_SearchFriendByPhoneNumber_DOWN ProcessorManager::SearchFriendByPhoneNumber_down(std::shared_ptr<net_data_recv_package> rec)
{
    msg::Processor_127_SearchFriendByPhoneNumber_DOWN processor;
    processor.ParseFromArray(rec->body, rec->bodysize);
    return processor;
}

msg::Processor_128_GetMomentBlockUsers_DOWN ProcessorManager::GetMomentBlockUsers_down(std::shared_ptr<net_data_recv_package> rec)
{
    msg::Processor_128_GetMomentBlockUsers_DOWN processor;
    processor.ParseFromArray(rec->body, rec->bodysize);
    return processor;
}

msg::Processor_130_Send_Diamond ProcessorManager::Send_Diamond_UP(const uint32_t userId, const uint32_t amount)
{
    msg::Processor_130_Send_Diamond processor;
    processor.set_userid(userId);
    processor.set_amount(amount);
    return processor;
}

msg::Processor_140_CreateGroup ProcessorManager::CreateGroup_UP(const msg::GroupInfo& groupinfo)
{
    msg::Processor_140_CreateGroup processor;
    processor.mutable_groupinfo()->CopyFrom(groupinfo);
    return processor;
}

msg::Processor_140_CreateGroup_Down ProcessorManager::CreateGroup_Down(std::shared_ptr<net_data_recv_package> rec)
{
    msg::Processor_140_CreateGroup_Down processor;
    processor.ParseFromArray(rec->body, rec->bodysize);
    return processor;
}

msg::Processor_141_EditGroup ProcessorManager::EditGroup_UP(const msg::GroupInfo& groupinfo)
{
    msg::Processor_141_EditGroup processor;
    processor.mutable_groupinfo()->CopyFrom(groupinfo);
    return processor;
}

msg::Processor_142_SearchGroup_UP ProcessorManager::SearchGroup_UP(msg::GroupSearchType type, const std::string& name)
{
    msg::Processor_142_SearchGroup_UP processor;
    processor.set_type(type);
    processor.set_searchname(name);
    return processor;
}

msg::Processor_142_SearchGroup_DOWN ProcessorManager::SearchGroup_Down(std::shared_ptr<net_data_recv_package> rec)
{
    msg::Processor_142_SearchGroup_DOWN processor;
    processor.ParseFromArray(rec->body, rec->bodysize);
    return processor;
}

msg::Processor_144_KickUserFromGroup ProcessorManager::KickUserFromGroup_UP(uint32_t groupid, uint32_t userid)
{
    msg::Processor_144_KickUserFromGroup processor;
    processor.set_groupid(groupid);
    processor.set_userid(userid);
    return processor;
}

msg::Processor_145_GetGroupInfo_UP ProcessorManager::GetGroupInfo_UP(uint32_t groupid)
{
    msg::Processor_145_GetGroupInfo_UP processor;
    processor.set_groupid(groupid);
    return processor;
}

msg::Processor_145_GetGroupInfo_DOWN ProcessorManager::GetGroupInfo_Down(std::shared_ptr<net_data_recv_package> rec)
{
    msg::Processor_145_GetGroupInfo_DOWN processor;
    processor.ParseFromArray(rec->body, rec->bodysize);
    return processor;
}

msg::Processor_146_GetGroupDetailInfo_UP ProcessorManager::GetGroupDetailInfo_UP(uint32_t groupid)
{
    msg::Processor_146_GetGroupDetailInfo_UP processor;
    processor.set_groupid(groupid);
    return processor;
}

msg::Processor_146_GetGroupDetailInfo_DOWN ProcessorManager::GetGroupDetailInfo_Down(std::shared_ptr<net_data_recv_package> rec)
{
    msg::Processor_146_GetGroupDetailInfo_DOWN processor;
    processor.ParseFromArray(rec->body, rec->bodysize);
    return processor;
}

msg::Processor_147_SetGroupManageSetting ProcessorManager::SetGroupManageSetting_UP(uint32_t groupid, const msg::GroupManageSetting& permission)
{
    msg::Processor_147_SetGroupManageSetting processor;
    processor.set_groupid(groupid);
    processor.mutable_setting()->CopyFrom(permission);
    return processor;
}

msg::Processor_148_SetGroupAdmin ProcessorManager::SetGroupAdmin_UP(uint32_t groupid, uint32_t userid, bool isAdmin)
{
    msg::Processor_148_SetGroupAdmin processor;
    processor.set_groupid(groupid);
    processor.set_userid(userid);
    processor.set_isadmin(isAdmin);
    return processor;
}

msg::Processor_150_GetRoomInfo_UP ProcessorManager::GetRoomInfo_UP(uint32_t groupid)
{
    msg::Processor_150_GetRoomInfo_UP processor;
    if (groupid != 0)
        processor.set_groupid(groupid);
    return processor;
}

msg::Processor_150_GetRoomInfo_DOWN ProcessorManager::GetRoomInfo_Down(std::shared_ptr<net_data_recv_package> rec)
{
    msg::Processor_150_GetRoomInfo_DOWN processor;
    processor.ParseFromArray(rec->body, rec->bodysize);
    return processor;
}

msg::Processor_152_CreateRoom ProcessorManager::CreateTournament_UP(const msg::RoomInfo& roominfo, const msg::TournamentInfo tournamentinfo)
{
    msg::Processor_152_CreateRoom processor;
    processor.mutable_roominfo()->CopyFrom(roominfo);
    processor.mutable_tournamentinfo()->CopyFrom(tournamentinfo);
    return processor;
}

msg::Processor_152_CreateRoom ProcessorManager::CreateRoom_UP(const msg::RoomInfo& roominfo, const msg::RoomProp& roomprop)
{
    msg::Processor_152_CreateRoom processor;
    processor.mutable_roominfo()->CopyFrom(roominfo);
    processor.mutable_roomprop()->CopyFrom(roomprop);
    return processor;
}

msg::Processor_154_CancelTournament ProcessorManager::CancelTournament_UP(uint32_t roomid)
{
    msg::Processor_154_CancelTournament processor;
    processor.set_roomid(roomid);
    return processor;
}

msg::Processor_160_SendChatMessage ProcessorManager::SendChatMessage_UP(const msg::ChatMessage& message)
{
    msg::Processor_160_SendChatMessage processor;
    processor.mutable_chatmessage()->CopyFrom(message);
    return processor;
}

msg::Processor_160_SendChatMessage_Down ProcessorManager::SendChatMessage_Down(std::shared_ptr<net_data_recv_package> rec)
{
    msg::Processor_160_SendChatMessage_Down processor;
    processor.ParseFromArray(rec->body, rec->bodysize);
    return processor;
}

msg::Processor_161_ReceiveChatMessage_UP ProcessorManager::ReceiveChatMessage_UP(uint32_t last_msg_id)
{
    msg::Processor_161_ReceiveChatMessage_UP processor;
    processor.set_lastmsgid(last_msg_id);
    return processor;
}

msg::Processor_161_ReceiveChatMessage ProcessorManager::ReceiveChatMessage_Down(std::shared_ptr<net_data_recv_package> rec)
{
    msg::Processor_161_ReceiveChatMessage processor;
    processor.ParseFromArray(rec->body, rec->bodysize);
    return processor;
}

msg::Processor_170_GetHistoryScore_Up ProcessorManager::GetHistoryScore_Up(const msg::RoomType &roomtype, bool isTournament, const std::string &startTime, const std::string &endTime, uint32_t groupid)
{
    msg::Processor_170_GetHistoryScore_Up processor;
    if (roomtype)
    {
        processor.set_roomtype(roomtype);
    }
    if (isTournament) {
        processor.set_istournament(isTournament);
    }
    std::string EndTime;
    if (endTime!="")
    {
        processor.set_endtime(endTime);
        EndTime=endTime;
    }
    else
    {
        long endtime = tools::date();
        processor.set_endtime(tools::to_string(endtime));
        EndTime=tools::to_string(endtime);
    }
    if (startTime!="")
    {
        processor.set_starttime(startTime);
    }
    else
    {
        long starttime=tools::stol(EndTime)-10*365*24*60*60;
        processor.set_starttime(tools::to_string(starttime));
    }
    
    if (groupid!=0) {
        processor.set_groupid(groupid);
    }
    return processor;
}

msg::Processor_170_GetHistoryScore_Down ProcessorManager::GetHistoryScore_Down(std::shared_ptr<net_data_recv_package> rec)
{
    msg::Processor_170_GetHistoryScore_Down processor;
    processor.ParseFromArray(rec->body, rec->bodysize);
    return processor;
}

msg::Processor_171_GetGroupUserRank_UP ProcessorManager::GetGroupUserRank_Up(const msg::RoomType &roomtype,bool isTournament, uint32_t groupid)
{
    msg::Processor_171_GetGroupUserRank_UP processor;
    processor.set_groupid(groupid);
    processor.set_roomtype(roomtype);
    processor.set_istournament(isTournament);
    return processor;
}

msg::Processor_171_GetGroupUserRank_DOWN ProcessorManager::GetGroupUserRank_Down(std::shared_ptr<net_data_recv_package> rec)
{
    msg::Processor_171_GetGroupUserRank_DOWN processor;
    processor.ParseFromArray(rec->body, rec->bodysize);
    return processor;
}

msg::Processor_181_JoinTournament ProcessorManager::JoinTournament_UP(uint32_t roomid, bool IsJoin)
{
    msg::Processor_181_JoinTournament processor;
    processor.set_roomid(roomid);
    processor.set_isjoin(IsJoin);
    return processor;
}

msg::Processor_182_TournamentStart ProcessorManager::TournamentStart_Down(std::shared_ptr<net_data_recv_package> rec)
{
    msg::Processor_182_TournamentStart processor;
    processor.ParseFromArray(rec->body, rec->bodysize);
    return processor;
}

msg::Processor_190_ReceiveMessage ProcessorManager::ReceiveMessage_Down(std::shared_ptr<net_data_recv_package> rec)
{
    msg::Processor_190_ReceiveMessage processor;
    processor.ParseFromArray(rec->body, rec->bodysize);
    return processor;
}

msg::Processor_190_ReceiveMessage_UP ProcessorManager::ReceiveMessage_Up(bool result, uint32_t id)
{
    msg::Processor_190_ReceiveMessage_UP processor;
    processor.set_id(id);
    processor.set_choice(result);
    return processor;
}

msg::Processor_191_NotifyChange ProcessorManager::NotifyChange_Down(std::shared_ptr<net_data_recv_package> rec)
{
    msg::Processor_191_NotifyChange processor;
    processor.ParseFromArray(rec->body, rec->bodysize);
    return processor;
}

msg::Processor_200_PublishMoment ProcessorManager::PublishMoment_UP(const msg::Moment& moment)
{
    msg::Processor_200_PublishMoment processor;
    processor.mutable_moment()->CopyFrom(moment);
    return processor;
}

msg::Processor_201_GetMoment_UP ProcessorManager::GetMoment_UP(const msg::MomentDisplayType& type, uint32_t MomentID, bool IsDown)
{
    msg::Processor_201_GetMoment_UP processor;
    processor.set_momentid(MomentID);
    processor.set_isdown(IsDown);
    processor.set_type(type);
    return processor;
}

msg::Processor_201_GetMoment_DOWN ProcessorManager::GetMoment_Down(std::shared_ptr<net_data_recv_package> rec)
{
    msg::Processor_201_GetMoment_DOWN processor;
    processor.ParseFromArray(rec->body, rec->bodysize);
    return processor;
}

msg::Processor_202_DeleteMoment ProcessorManager::DeleteMoment_UP(uint32_t MomentID)
{
    msg::Processor_202_DeleteMoment processor;
    processor.set_momentid(MomentID);
    return processor;
}

msg::Processor_203_PublishComment ProcessorManager::PublishComment_UP(const msg::CommentInfo comment)
{
    msg::Processor_203_PublishComment processor;
    processor.mutable_commentinfo()->CopyFrom(comment);
    return processor;
}

msg::Processor_204_GetComment_UP ProcessorManager::GetComment_UP(uint32_t MomentID)
{
    msg::Processor_204_GetComment_UP processor;
    processor.set_momentid(MomentID);
    return processor;
}

msg::Processor_205_DeleteMoment ProcessorManager::DeleteComent_UP(uint32_t ComentID,uint32_t MomentID)
{
    msg::Processor_205_DeleteMoment processor;
    processor.set_commentid(ComentID);
    processor.set_momentid(MomentID);
    return processor;
}

msg::Processor_204_GetComment_DOWN ProcessorManager::GetComment_Down(std::shared_ptr<net_data_recv_package> rec)
{
    msg::Processor_204_GetComment_DOWN processor;
    processor.ParseFromArray(rec->body, rec->bodysize);
    return processor;
}
msg::Processor_420_PokerGetScore_UP ProcessorManager::PokerGetScore_UP(int room_id)
{
    msg::Processor_420_PokerGetScore_UP processor;
    processor.set_roomid(room_id);
    return processor;
}

msg::Processor_420_PokerGetScore_DOWN ProcessorManager::PokerGetScore_DOWN(std::shared_ptr<net_data_recv_package> rec)
{
    msg::Processor_420_PokerGetScore_DOWN processor;
    processor.ParseFromArray(rec->body, rec->bodysize);
    return processor;
}
