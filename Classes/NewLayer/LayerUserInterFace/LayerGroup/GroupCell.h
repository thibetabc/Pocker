//
//  GroupCell.h
//  pokerclient
//
//  Created by duanmt on 15/5/6.
//
//

#ifndef __pokerclient__MessageCell__
#define __pokerclient__MessageCell__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "SceneManager.h"
#include "cocos-ext.h"
#include "events.h"
#include "ShaderSprite.h"


using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;

class group_data;

class GroupCell : public TableViewCell {
public:
    CREATE_FUNC(GroupCell);
    
    virtual bool init();
    
    void update_property(const msg::GroupInfo& group_data);
    void update_ui();
    void update_unread_message_count();
    const msg::GroupInfo& get_group_data() { return group_data_;}
    
    void handle_download_events(cmd_data_pointer);
    void set_is_touch(bool is_touch = true);
    
    void share(Ref* sender);
    void setShareSpriteTexture(const std::string img);//传入头像url
    #pragma mark - setter and getter
    CC_SYNTHESIZE(ImageView*, imageBg, ImageBg);
    CC_SYNTHESIZE(ShaderSprite*, imgIcon, ImgageIcon);
    CC_SYNTHESIZE(Sprite*, spriteSex, SpriteSex);
    CC_SYNTHESIZE(Text*, textName, TextName);
    CC_SYNTHESIZE(Text*, textRemark, TextRemark);
    CC_SYNTHESIZE(Vec2, remarkPosition, RemarkPosition);
    CC_SYNTHESIZE(Text*, textLastestChat, TextLastextChat);
    CC_SYNTHESIZE(Text*, textTime, TextTime);
    CC_SYNTHESIZE(Sprite*, spriteChat, SpriteChat);
    CC_SYNTHESIZE(Sprite*, spriteRedBall, SpriteRedBall);
    CC_SYNTHESIZE(Text*, textUnreadCount, TextUnreadCount);
    CC_SYNTHESIZE(Button*, buttonOperate, ButtonOperate);
    CC_SYNTHESIZE(Sprite*, spriteSelect, SpriteSelect);
    
private:
    void resetShareSpriteTexture(const std::string img); //下载完后调整头像大小
    
private:
    events::event_dispatch dispatch_;
    msg::GroupInfo group_data_;
    //
    const std::string cellBgNormal = "image_club/club_group_bg.png";
    const std::string cellBgPress  = "image_club/club_group_bg_press.png";
    Size headImgSize;
};


#endif /* defined(__pokerclient__MessageCell__) */
