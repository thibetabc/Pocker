//
//  GroupCell.cpp
//  pokerclient
//
//  Created by duanmt on 15/5/6.
//
//

#include "GroupCell.h"
#include "GameDataManager.h"
#include "utils.h"
#include "cmd_def.h"
#include "FileManager.h"
#include "ChatMessageManager.h"


#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#include "PayManager.h"
// 引入相关的头文件

#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

#endif

bool GroupCell::init()
{
    if(!TableViewCell::init()) {
        return false;
    }
    
    auto root = CSLoader::createNode("NodeGroupCell_0.csb");
   
    
    this->addChild(root);
    
    imageBg = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root, "img_group_cell"));
    auto tmpImgIcon = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root, "img_group_head"));
    tmpImgIcon->setVisible(false);
    headImgSize = tmpImgIcon->getContentSize();
    spriteSex = dynamic_cast<Sprite*>(CSLoader::seekNodeByName(root, "sprite_sex"));
    spriteSex->setVisible(false);//性别 默认隐藏
    spriteChat = dynamic_cast<Sprite*>(CSLoader::seekNodeByName(root, "img_chat"));
    spriteRedBall = dynamic_cast<Sprite*>(CSLoader::seekNodeByName(root, "img_msg_tip"));
    textUnreadCount = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "txt_msg_tip"));
    textName = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "txt_group_name"));
    textRemark = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_group_remark"));
    remarkPosition = textRemark->getPosition();
    textLastestChat = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "txt_group_latest_chat"));
    textTime = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_time"));
    buttonOperate = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "btn_operate"));
    buttonOperate->setVisible(false);//默认隐藏
    spriteSelect = dynamic_cast<Sprite*>(CSLoader::seekNodeByName(root,"sprite_select"));
    spriteSelect->setVisible(false);//多选项 默认隐藏
    //头像
    imgIcon = ShaderSprite::create("Default/image_defualt_group_icon_large.png", "Default/image_mask_circle.png");
    imgIcon->setPosition(tmpImgIcon->getPosition());
    root->addChild(imgIcon, tmpImgIcon->getLocalZOrder());
    
    //调整cell的width
    root->setPositionX((DESIGN_WIDTH - imageBg->getContentSize().width)/2);
    // register events;
    dispatch_.register_event(EVENT_DOWLOAD_FILE,
                             BIND_FUNC(this, GroupCell::handle_download_events));//EVENT_DOWLOAD_FILE
    
    return true;
}

void GroupCell::handle_download_events(cmd_data_pointer data)
{
    auto str = std::dynamic_pointer_cast<cmd_data_string>(data);
    auto url = str->get_value();
    if (url != group_data_.grouppic()) return;
    assert (GDM->is_file_exist(url));
    resetShareSpriteTexture(url);
}

void GroupCell::setShareSpriteTexture(const std::string imgUrl)
{
    std::string img = imgUrl;
    if(img.empty() || img == "")
    {
        img = "Default/image_defualt_group_icon_large.png";
    }
    group_data_.set_grouppic(img);
    if(FileUtils::getInstance()->isFileExist(img) || GDM->is_file_exist(img))
    {
        resetShareSpriteTexture(img);
    }
    else
    {
        FM.download_file(img);  //下载完成前的默认头像
        resetShareSpriteTexture("Default/image_defualt_group_icon_large.png");
    }
}
void GroupCell::resetShareSpriteTexture(const std::string img)
{
    
    imgIcon->setTexture(img);
    imgIcon->setScale(headImgSize.width / imgIcon->getContentSize().width,headImgSize.height / imgIcon->getContentSize().height);
}

void GroupCell::update_property(const msg::GroupInfo& group_data)
{
    group_data_.CopyFrom(group_data);
    
//    std::string image_bg_name = "image_club/club_group_bg.png";
//    if (group_data.type() == msg::GroupType::OFFICALGROUP)
//    {
//        image_bg_name = "login_res/group_res/group_cell_bg_2.png";
//    }
    imageBg->loadTexture(cellBgNormal);
    
    update_ui();
}


void GroupCell::update_ui()
{  
    setShareSpriteTexture(group_data_.grouppic());
    
    textName->setString(group_data_.groupname().empty() ? " " : group_data_.groupname());//俱乐部名
    textRemark->setString(group_data_.groupcomment());//备注
    
    auto lastmessage = CM->get_last_message(group_data_.groupid(), ChatMessageManager::MESSAGETYPE::GROUPCHAT);
    if(lastmessage)
    {
        
        std::string str ="";
        if (lastmessage->has_userdetailinfo()){
            str.append(lastmessage->userdetailinfo().nickname().c_str());
            str.append(":");
        }
        if (lastmessage->messageinfo().type() == msg::MessageType::Text)
            str.append(lastmessage->messageinfo().textinfo().text().c_str());
        else if (lastmessage->messageinfo().type() == msg::MessageType::Picture)
            str.append(tools::local_string("Received_picture", "收到图片"));
        else if (lastmessage->messageinfo().type() == msg::MessageType::Voice)
            str.append(tools::local_string("Received_voice", "收到语音"));
        
        //最后聊天信息
        textLastestChat->setString(str.empty() ? " " : str);
        
        //最后聊天时间
        const std::string submit_time = lastmessage->subtime();
        std::string text = "";
        if(!submit_time.empty()) {
            double time = tools::stod(submit_time);
            time /= 1000;
            text = tools::time_display(time);
        }
        textTime->setString(text);
    }
    else
    {
        
        textTime->setString(" ");
        textLastestChat->setString(" ");
    }
    
    //未读消息
    uint32_t unread_message_count = CM->get_unread_message_count(group_data_.groupid(), ChatMessageManager::MESSAGETYPE::GROUPCHAT);
    spriteRedBall->setVisible(unread_message_count != 0);
    textUnreadCount->setString(tools::to_string(unread_message_count));
    
    
}

void GroupCell::set_is_touch(bool is_touch)
{
    imageBg->loadTexture(is_touch ? cellBgPress : cellBgNormal);
}

void GroupCell::share(Ref* sender)
{
}

void GroupCell::update_unread_message_count()
{
    uint32_t unread_message_count = CM->get_unread_message_count(group_data_.groupid(), ChatMessageManager::MESSAGETYPE::GROUPCHAT);
    spriteRedBall->setVisible(unread_message_count != 0);
    textUnreadCount->setString(tools::to_string(unread_message_count));
}
