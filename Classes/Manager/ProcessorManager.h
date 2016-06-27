//
//  ProcessorManager.hpp
//  pokerclient
//
//  Created by zhongyukeji on 16/4/11.
//
//

#ifndef ProcessorManager_h
#define ProcessorManager_h

#include <stdio.h>
#include "msg.pb.h"
#include "net_data_recv.h"

using namespace std;

#define PM ProcessorManager::getInstance()

class ProcessorManager {
public:
    static ProcessorManager* getInstance() {
        if(!__Processor_manager__) {
            __Processor_manager__ = new ProcessorManager();
        }
        return __Processor_manager__;
    }
    
    static void destroy() {
        delete __Processor_manager__;
        __Processor_manager__ = nullptr;
    }
    
    msg::Response response_down(std::shared_ptr<net_data_recv_package> rec);
    msg::Response null_up();
    msg::Processor_97_SendVerificationByUserID send_verification_by_user(std::string account);
    msg::Processor_98_ResetPassword reset_password(std::string account, std::string password, std::string verification);
    msg::Processor_99_Login_UP login_up(std::string account, std::string password, bool is_user_phone);
    msg::Processor_99_Login_DOWN login_down(std::shared_ptr<net_data_recv_package> rec);
    msg::Processor_100_LoginResource login_resource_up(std::string key);
    msg::Processor_100_LoginResource_Down login_resoucre_down(std::shared_ptr<net_data_recv_package> rec);
    msg::Processor_103_FeedBack feedback_up(const std::string& description, const std::string& descrip_type, const std::string& pic_name);
    msg::Processor_104_Register register_up(const std::string& username, const std::string& password, const std::string& bind_phone, const std::string& verification, const std::string& referee);
    msg::Processor_105_Device devicetoken_up(const std::string& devicetoken);
    msg::Processor_106_ReceiveIOSPayReceipt iospay_up(const std::string& receipt, uint32_t userid);
    msg::Processor_106_ReceiveIOSPayReceipt_Down iospay_down(std::shared_ptr<net_data_recv_package> rec);
    msg::Processor_107_GetGoodsInfo getGoodsInfo_down(std::shared_ptr<net_data_recv_package> rec);
    msg::Processor_108_BuyGood buygood_up(uint32_t goodid,uint32_t userid);
    msg::Processor_109_AndroidPay androidpay_up(const std::string& id, uint32_t userid);
    msg::Processor_109_AndroidPayDown androidpay_down(std::shared_ptr<net_data_recv_package> rec);
    msg::Processor_111_GetUserSetting getUserSetting_down(std::shared_ptr<net_data_recv_package> rec);
    msg::Processor_112_ChangeUserSetting changeUserSetting_up(const msg::UserDetailInfo& info);
    msg::Processor_112_ChangeUserSetting changeUserSetting_up(const msg::UserLoginInfo& info);
    msg::Processor_112_ChangeUserSetting changeUserSetting_up(const msg::UserGameSettings& settings);
    msg::Processor_113_GetUserPermission_UP GetUserPermission_up(uint32_t groupid, uint32_t userid);
    //msg::Processor_113_GetUserPermission_UP GetUserPermission_up(std::vector<uint32_t> group_ids, uint32_t userid);
    msg::Processor_113_GetUserPermission_DOWN GetUserPermission_down(std::shared_ptr<net_data_recv_package> rec);
    msg::Processor_114_ChangePermission changePermission_up(uint32_t userID, const msg::GroupPermission& permission);
    msg::Processor_115_GetGroupSetting_UP GetGroupSetting_up(uint32_t groupid);
    msg::Processor_115_GetGroupSetting_DOWN GetGroupSetting_down(std::shared_ptr<net_data_recv_package> rec);
    msg::Processor_116_ChangeGroupSetting changeGroupSetting_up(msg::GroupSetting setting);
    msg::Processor_118_SendVerification SendVerification_up(const std::string& phoneNumber);
    msg::Processor_119_BindPhone BindPhone_up(const std::string& phoneNumber, const std::string& verification);
    msg::Processor_120_GetFriendList getFriendList_down(std::shared_ptr<net_data_recv_package> rec);
    msg::Processor_121_SearchUser_UP SearchUser_up(const std::string& key_word);
    msg::Processor_121_SearchUser_DOWN SearchUser_down(std::shared_ptr<net_data_recv_package> rec);
    msg::Processor_122_SendRequest SendRequest_up(const msg::Request& request);
    msg::Processor_123_ReceiveRequestList ReceiveRequestList_down(std::shared_ptr<net_data_recv_package> rec);
    msg::Processor_124_DealRequest DealRequest_up(uint32_t RequestID, bool IsAccepted);
    msg::Processor_124_DealRequest_DOWN DealRequest_down(std::shared_ptr<net_data_recv_package> rec);
    msg::Processor_125_DeleteFriend DeleteFriend_up(uint32_t userid);
    msg::Processor_126_Set_MomentBlock BlockUser_up(uint32_t userid, bool IsBlock);
    msg::Processor_127_SearchFriendByPhoneNumber_UP SearchFriendByPhoneNumber_up(const std::vector<std::string>& numbers);
    msg::Processor_127_SearchFriendByPhoneNumber_DOWN SearchFriendByPhoneNumber_down(std::shared_ptr<net_data_recv_package> rec);
    msg::Processor_128_GetMomentBlockUsers_DOWN GetMomentBlockUsers_down(std::shared_ptr<net_data_recv_package> rec);
    msg::Processor_130_Send_Diamond Send_Diamond_UP(const uint32_t userId, const uint32_t amount);
    msg::Processor_140_CreateGroup CreateGroup_UP(const msg::GroupInfo& groupinfo);
    msg::Processor_140_CreateGroup_Down CreateGroup_Down(std::shared_ptr<net_data_recv_package> rec);
    msg::Processor_141_EditGroup EditGroup_UP(const msg::GroupInfo& groupinfo);
    msg::Processor_142_SearchGroup_UP SearchGroup_UP(msg::GroupSearchType type, const std::string& name);
    msg::Processor_142_SearchGroup_DOWN SearchGroup_Down(std::shared_ptr<net_data_recv_package> rec);
    msg::Processor_144_KickUserFromGroup KickUserFromGroup_UP(uint32_t groupid, uint32_t userid);
    msg::Processor_145_GetGroupInfo_UP GetGroupInfo_UP(uint32_t groupid);
    msg::Processor_145_GetGroupInfo_DOWN GetGroupInfo_Down(std::shared_ptr<net_data_recv_package> rec);
    msg::Processor_146_GetGroupDetailInfo_UP GetGroupDetailInfo_UP(uint32_t groupid);
    msg::Processor_146_GetGroupDetailInfo_DOWN GetGroupDetailInfo_Down(std::shared_ptr<net_data_recv_package> rec);
    msg::Processor_147_SetGroupManageSetting SetGroupManageSetting_UP(uint32_t groupid, const msg::GroupManageSetting& permission);
    msg::Processor_148_SetGroupAdmin SetGroupAdmin_UP(uint32_t groupid, uint32_t userid, bool isAdmin);
    msg::Processor_150_GetRoomInfo_UP GetRoomInfo_UP(uint32_t groupid);
    msg::Processor_150_GetRoomInfo_DOWN GetRoomInfo_Down(std::shared_ptr<net_data_recv_package> rec);
    msg::Processor_152_CreateRoom CreateTournament_UP(const msg::RoomInfo& roominfo, const msg::TournamentInfo tournamentinfo);
    msg::Processor_152_CreateRoom CreateRoom_UP(const msg::RoomInfo& roominfo, const msg::RoomProp& roomprop);
    msg::Processor_154_CancelTournament CancelTournament_UP(uint32_t roomid);
    msg::Processor_160_SendChatMessage SendChatMessage_UP(const msg::ChatMessage& message);
    msg::Processor_160_SendChatMessage_Down SendChatMessage_Down(std::shared_ptr<net_data_recv_package> rec);
    msg::Processor_161_ReceiveChatMessage_UP ReceiveChatMessage_UP(uint32_t last_msg_id);
    msg::Processor_161_ReceiveChatMessage ReceiveChatMessage_Down(std::shared_ptr<net_data_recv_package> rec);
    msg::Processor_170_GetHistoryScore_Up GetHistoryScore_Up(const msg::RoomType &roomtype,bool isTournament,const std::string &startTime,const std::string &endTime,uint32_t groupid);
    msg::Processor_170_GetHistoryScore_Down GetHistoryScore_Down(std::shared_ptr<net_data_recv_package> rec);
    msg::Processor_171_GetGroupUserRank_UP GetGroupUserRank_Up(const msg::RoomType &roomtype,bool isTournament, uint32_t groupid);
    msg::Processor_171_GetGroupUserRank_DOWN GetGroupUserRank_Down(std::shared_ptr<net_data_recv_package> rec);
    msg::Processor_181_JoinTournament JoinTournament_UP(uint32_t roomid, bool IsJoin);
    msg::Processor_182_TournamentStart TournamentStart_Down(std::shared_ptr<net_data_recv_package> rec);
    msg::Processor_190_ReceiveMessage ReceiveMessage_Down(std::shared_ptr<net_data_recv_package> rec);
    msg::Processor_190_ReceiveMessage_UP ReceiveMessage_Up(bool result, uint32_t id);
    msg::Processor_191_NotifyChange NotifyChange_Down(std::shared_ptr<net_data_recv_package> rec);
    msg::Processor_200_PublishMoment PublishMoment_UP(const msg::Moment& moment);
    msg::Processor_201_GetMoment_UP GetMoment_UP(const msg::MomentDisplayType& type, uint32_t MomentID, bool IsDown);
    msg::Processor_201_GetMoment_DOWN GetMoment_Down(std::shared_ptr<net_data_recv_package> rec);
    msg::Processor_202_DeleteMoment DeleteMoment_UP(uint32_t MomentID);
    msg::Processor_203_PublishComment PublishComment_UP(const msg::CommentInfo comment);
    msg::Processor_204_GetComment_UP GetComment_UP(uint32_t MomentID);
    msg::Processor_204_GetComment_DOWN GetComment_Down(std::shared_ptr<net_data_recv_package> rec);
    msg::Processor_205_DeleteMoment DeleteComent_UP(uint32_t ComentID, uint32_t MomentID);
    msg::Processor_420_PokerGetScore_UP PokerGetScore_UP(int room_id);
    msg::Processor_420_PokerGetScore_DOWN PokerGetScore_DOWN(std::shared_ptr<net_data_recv_package> rec);
private:
    static ProcessorManager* __Processor_manager__;
};
#endif /* ProcessorManager_h */
