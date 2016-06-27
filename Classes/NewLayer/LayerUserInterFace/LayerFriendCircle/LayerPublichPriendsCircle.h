//
//  LayerPublichPriendsCircle.hpp
//  pokerclient
//
//  Created by zhongyu on 16/6/6.
//
//

#ifndef LayerPublichPriendsCircle_h
#define LayerPublichPriendsCircle_h
#include "SceneManager.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "cocos-ext.h"
#include "utils.h"
#include <stdio.h>
#include "ProcessorManager.h"
#include "GameDataManager.h"
#include "NET_CMD.h"
#include "GameTools.h"
#include "TipView.h"
#include "LayerSearchUser.h"
#include "LayerInvitationList.h"
#include "LayerUserInfo.h"
#include "InvitationManager.h"
#include "LayerChat.h"
#include "ChatMessageManager.h"
#include "ShaderSprite.h"
#include "LayerChoosePhoto.h"
#include "Toast.h"
#include "RecordLayer.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace extension;

class  LayerPublichPriendsCircle:public BaseLayer,public EditBoxDelegate
{
public:
    static LayerPublichPriendsCircle* create();
    virtual bool init();
    void setPublicCircle(bool ifpublic);
    void goHandIDPage();
    void publishCircle();
    void register_events();
    void handle_get_publish_circle_info(cmd_data_pointer data);
    void click_btn_add(Ref * sender);
    void send_chatmessage_picture(std::string pic_file_name);
    void send_chatmessage_picture1(std::string pic_file_name);
    void send_chatmessage_picture2(std::string pic_file_name);
    void send_chatmessage_picture3(std::string pic_file_name);
    void goToChooseHandIDPage(Ref* sender);
    void setHandID(int handid);
    void setChooseNode(const std::shared_ptr<msg::GameRecord>& obj);
    void setHandIDOrImageChoose();
    void handle_upload_event(cmd_data_pointer data);
    void upload_pictures();
#pragma mark EditBox delegate
    virtual void editBoxEditingDidBegin(EditBox* editBox);
    virtual void editBoxEditingDidEnd(EditBox* editBox);
    virtual void editBoxTextChanged(EditBox* editBox, const std::string& text);
    virtual void editBoxReturn(EditBox* editBox);
private:
    Node* root;
    Node* chooseNode_;
    Button* btn_back;
    Button* btn_publish;
    Text* title;
    TextField* textfield_input;
    EditBox* ed_property_value_;
    ImageView* Image_pic_bg;
    ImageView* Image_pic_bg_1;
    ImageView* Image_pic_bg_2;
    ImageView* Image_pic_bg_3;
    Text* Text_choose_handid;
    Button* Button_go;
    ImageView* Image_if_open;
    Layout* layer_public;
    Layout* layer_simi;
    Text* Text_public;
    Text* Text_simi;
    ImageView* Image_pub;
    ImageView* Image_sic;
    std::string commentInfo;
    std::string updateImageName;
    std::string updateImageName2;
    std::string updateImageName3;
    std::string updateImageName4;
    msg::MomentDisplayType chooseType;
    Layout* layer_choose_handid;
    std::vector<std::string> updateImageArray;
    int updateSuccessImageNum;
    int presentImageNUM;
    
    
    bool is_edit_;
    bool ifChooseImage;
    bool ifChooseHandID;
    int chooseHandID;
    

    
    
    
    
};

#endif /* LayerPublichPriendsCircle_hpp */
