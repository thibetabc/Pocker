//
//  friend_data.h
//  pokerclient
//
//  Created by ll on 15/8/18.
//
//

#ifndef __pokerclient__friend_data__
#define __pokerclient__friend_data__

#include <string>

class friend_data
{
public:
    typedef enum {
        CELL_NEW_FRIEND= 1,
        CELL_INVAITATION_FRIEND = 2,
        CELL_FRIEND = 3,
        CELL_AUDIT_FRIEND = 4,
        CELL_FRIEND_ADDRESS = 5
    } CELL_TYPE;
    
    friend_data(const uint32_t userid,
                const std::string nick_name,
                const std::string pic_name,
                const bool isonline,
                const uint32_t cell_type);
    friend_data();
    
    void set_userid(const uint32_t id){userid_ = id;}
    void set_nick_name(const std::string nick_name){nick_name_ = nick_name;}
    void set_pic_name(const std::string pic_name){pic_name_ = pic_name;}
    void set_isonline(const bool isonline){isonline_ = isonline;}
    void set_cell_type(const uint32_t cell_type){cell_type_ = cell_type;}
    
    const uint32_t get_userid() {return userid_;}
    const std::string& get_nick_name() {return nick_name_;}
    const std::string& get_pic_name() {return pic_name_;}
    const bool get_isonline() {return isonline_;}
    const uint32_t get_cell_type() {return cell_type_;}
        
private:
    uint32_t userid_;
    std::string nick_name_;
    std::string pic_name_;
    bool isonline_;
    
    // 1 NewFriendCell  2 InvaitationFriendCell  3 FriendCell 4 AuditFriendCell
    uint32_t cell_type_;
};

#endif /* defined(__pokerclient__friend_data__) */
