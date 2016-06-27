//
//  LayerPublichPriendsCircle.cpp
//  pokerclient
//
//  Created by zhongyu on 16/6/6.
//
//

#include "LayerPublichPriendsCircle.h"
#include "LayerFriendList.h"

#define ZORDER_NEW_LAYER 9
#define RECORDLAYERTAG 1
#define IMAGEBASETAG 100

LayerPublichPriendsCircle* LayerPublichPriendsCircle::create()
{
    LayerPublichPriendsCircle* layerCircleInfo=new LayerPublichPriendsCircle;
    if (layerCircleInfo->init())
    {
        layerCircleInfo->autorelease();
        return layerCircleInfo;
    }
    return nullptr;
    
    delete layerCircleInfo;
    layerCircleInfo = nullptr;
    return nullptr;

    
}

bool LayerPublichPriendsCircle::init()
{
    if(!BaseLayer::init())
    {
        return false;
    }
    ifChooseImage=false;
    ifChooseHandID=false;
    presentImageNUM=1;
    chooseType=msg::MomentDisplayType::FRIEND;
    updateImageName="Nopic";
    updateImageName2="Nopic";
    updateImageName3="Nopic";
    updateImageName4="Nopic";
    root=CSLoader::createNode("LayerPublishFriendsCircle.csb");
    this->addChild(root);
    btn_back=dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "Button_back"));
    btn_publish=dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "Button_publish"));
    title=dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_publish_friends"));
    textfield_input=dynamic_cast<TextField*>(CSLoader::seekNodeByName(root, "textfield_input"));
    Image_pic_bg=dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root, "Image_pic_bg"));
    Image_pic_bg_1=dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root, "Image_pic_bg_1"));
    Image_pic_bg_2=dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root, "Image_pic_bg_2"));
    Image_pic_bg_3=dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root, "Image_pic_bg_3"));
    Text_choose_handid=dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "Text_choose_handid"));
    Button_go=dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "Button_go"));
    Image_if_open=dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root, "Image_if_open"));
    layer_public=dynamic_cast<Layout*>(CSLoader::seekNodeByName(root, "layer_public"));
    layer_simi=dynamic_cast<Layout*>(CSLoader::seekNodeByName(root, "layer_simi"));
    layer_choose_handid=dynamic_cast<Layout*>(CSLoader::seekNodeByName(root, "Panel_4"));
    Text_public=dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "Text_public"));
    Text_simi=dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "Text_simi"));
    Image_pub=dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root, "Image_pub"));
    Image_sic=dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root, "Image_sic"));
    Image_if_open=dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root, "Image_if_open"));
    auto Image_input_bg =dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root, "Image_input_bg"));
    chooseNode_=dynamic_cast<Node*>(CSLoader::seekNodeByName(root, "Node_1"));
    btn_publish->setTitleText(tools::local_string("send","发送"));
    textfield_input->setPlaceHolder(tools::local_string("input_info","请输入"));
    chooseNode_->setVisible(false);
    
    ed_property_value_ = GameTools::createEditBoxFromTextField(Image_input_bg, textfield_input);
    ed_property_value_->setReturnType(EditBox::KeyboardReturnType::DONE);
    ed_property_value_->setMaxLength(textfield_input->getContentSize().width);
    //ed_property_value_->setInputMode(cocos2d::ui::EditBox::InputMode::NUMERIC);
    ed_property_value_->setInputFlag(cocos2d::ui::EditBox::InputFlag::SENSITIVE);
    ed_property_value_->setFontColor(Color3B(255, 255, 255));
    ed_property_value_->setPlaceholderFontColor(Color3B(151, 151, 151));
    
    
    ed_property_value_->setDelegate(this);
    is_edit_=true;
    
    if (!is_edit_)
    {
        ed_property_value_->setTouchEnabled(false);
    }
    
    
    title->setString(tools::local_string("published_circle","发布朋友圈"));
    Text_choose_handid->setString(tools::local_string("choose_handID","选择手牌"));
    Text_public->setString(tools::local_string("gongkai","公开（所有朋友可见）"));
    Text_simi->setString(tools::local_string("simi","私密（仅自己可见）"));
    
    Button_go->addClickEventListener([&](Ref *ref)
                                   {
                                       this->goHandIDPage();
                                   });
    layer_public->setTouchEnabled(true);
    layer_public->addClickEventListener([&](Ref *ref)
                                   {
                                       this->setPublicCircle(true);
                                   });
    
    layer_simi->setTouchEnabled(true);
    layer_simi->addClickEventListener([&](Ref *ref)
                                        {
                                            this->setPublicCircle(false);
                                        });
    btn_publish->addClickEventListener([&](Ref *ref)
                                       {
                                           //this->publishCircle();
                                           this->upload_pictures();//先上传图片再传输协议
                                       });
    
    btn_back->addClickEventListener([&](Ref *ref)
                                    {
                                        this->removeFromParent();
                                    });
    
    Image_pic_bg->setTouchEnabled(true);
    Image_pic_bg->addClickEventListener(std::bind(&LayerPublichPriendsCircle::click_btn_add, this, std::placeholders::_1));
    layer_choose_handid->setTouchEnabled(true);
    layer_choose_handid->addClickEventListener(std::bind(&LayerPublichPriendsCircle::goToChooseHandIDPage, this, std::placeholders::_1));
    this->setPublicCircle(true);
    this->setHandIDOrImageChoose();
    
    Image_pic_bg_1->setVisible(false);
    Image_pic_bg_2->setVisible(false);
    Image_pic_bg_3->setVisible(false);
    Image_pic_bg_1->setTouchEnabled(true);
    Image_pic_bg_2->setTouchEnabled(true);
    Image_pic_bg_3->setTouchEnabled(true);
    Image_pic_bg->setTag(IMAGEBASETAG);
    Image_pic_bg_1->setTag(IMAGEBASETAG+1);
    Image_pic_bg_2->setTag(IMAGEBASETAG+2);
    Image_pic_bg_3->setTag(IMAGEBASETAG+3);
    Image_pic_bg_1->addClickEventListener(std::bind(&LayerPublichPriendsCircle::click_btn_add, this, std::placeholders::_1));
    Image_pic_bg_2->addClickEventListener(std::bind(&LayerPublichPriendsCircle::click_btn_add, this, std::placeholders::_1));
    Image_pic_bg_3->addClickEventListener(std::bind(&LayerPublichPriendsCircle::click_btn_add, this, std::placeholders::_1));
    
    
    
    auto& size = Director::getInstance()->getWinSize();
    auto layout = Layout::create();
    layout->setContentSize(size);
    layout->setTouchEnabled(true);
    layout->setAnchorPoint(Point::ZERO);
    layout->setSwallowTouches(false);
    layout->addClickEventListener([this](Ref*){
        GameTools::editBoxLoseInputFocus();
    });
    this->addChild(layout, 10);
    
    return true;

}

void LayerPublichPriendsCircle::setHandID(int handid)
{
    ifChooseHandID=true;
    ifChooseImage=false;
    chooseHandID=handid;
    this->setHandIDOrImageChoose();
}

void LayerPublichPriendsCircle::setHandIDOrImageChoose()
{
    if(ifChooseHandID)
    {
        layer_choose_handid->setTouchEnabled(true);
        Text_choose_handid->setColor(Color3B(255,255,255));
        Image_pic_bg->setTouchEnabled(false);
        Image_pic_bg->setVisible(false);
    }
    else if(ifChooseImage)
    {
        layer_choose_handid->setTouchEnabled(false);
        Text_choose_handid->setColor(Color3B(151,151,151));
        Image_pic_bg->setTouchEnabled(true);
        Image_pic_bg->setVisible(true);
    }
    else if((!ifChooseHandID)&&(!ifChooseImage))
    {
        layer_choose_handid->setTouchEnabled(true);
        Text_choose_handid->setColor(Color3B(255,255,255));
        Image_pic_bg->setTouchEnabled(true);
        Image_pic_bg->setVisible(true);
    }
}

void LayerPublichPriendsCircle::goToChooseHandIDPage(Ref* sender)
{
    auto prl = RecordLayer::create();
    prl->set_chooseHandID(true);
    prl->set_isFriHandCard(false);
    prl->update_data();
    this->addChild(prl, 0, RECORDLAYERTAG);
}

void LayerPublichPriendsCircle::setPublicCircle(bool ifpublic)
{
    if(ifpublic)
    {
        Image_pub->loadTexture("friend_circle/publish_friend_circle/sign-s.png");
        Image_sic->loadTexture("friend_circle/publish_friend_circle/sign-ds.png");
        Text_simi->setColor(Color3B(76,96,115));
        Text_public->setColor(Color3B(255,255,255));
        chooseType=msg::MomentDisplayType::FRIEND;
    }
    else
    {
        Image_pub->loadTexture("friend_circle/publish_friend_circle/sign-ds.png");
        Image_sic->loadTexture("friend_circle/publish_friend_circle/sign-s.png");
        Text_simi->setColor(Color3B(255,255,255));
        Text_public->setColor(Color3B(76,96,115));
        chooseType=msg::MomentDisplayType::MINE;
    }
}

void LayerPublichPriendsCircle::goHandIDPage()
{
    
}

void LayerPublichPriendsCircle::click_btn_add(Ref * sender)
{
    //editbox_input_->setVisible(false);
    LayerChoosePhoto* as = LayerChoosePhoto::create();
    as->setIsChat(true);
    int tag = dynamic_cast<ImageView*>(sender)->getTag();
    switch(tag)
    {
        case IMAGEBASETAG:
            as->set_choose_photo_callback(std::bind(&LayerPublichPriendsCircle::send_chatmessage_picture, this, std::placeholders::_1));
            break;
        case IMAGEBASETAG+1:
            as->set_choose_photo_callback(std::bind(&LayerPublichPriendsCircle::send_chatmessage_picture1, this, std::placeholders::_1));
            break;
        case IMAGEBASETAG+2:
            as->set_choose_photo_callback(std::bind(&LayerPublichPriendsCircle::send_chatmessage_picture2, this, std::placeholders::_1));
            break;
        case IMAGEBASETAG+3:
            as->set_choose_photo_callback(std::bind(&LayerPublichPriendsCircle::send_chatmessage_picture3, this, std::placeholders::_1));
            break;
        default:
            break;
    }
    as->set_dismiss_callback([this](){
        GameTools::editBoxLoseInputFocus();
    });
    this->addChild(as, ZORDER_NEW_LAYER);
}

void LayerPublichPriendsCircle::send_chatmessage_picture(std::string pic_file_name)
{
    printf("pic name is %s",pic_file_name.c_str());
    Image_pic_bg_1->setVisible(true);
    Image_pic_bg->loadTexture(pic_file_name);
    updateImageName=pic_file_name;
    ifChooseImage=true;
    ifChooseHandID=false;
    this->setHandIDOrImageChoose();
}

void LayerPublichPriendsCircle::send_chatmessage_picture1(std::string pic_file_name)
{
    printf("pic name is %s",pic_file_name.c_str());
    Image_pic_bg_2->setVisible(true);
    Image_pic_bg_1->loadTexture(pic_file_name);
    updateImageName2=pic_file_name;
    ifChooseImage=true;
    ifChooseHandID=false;
    this->setHandIDOrImageChoose();
}

void LayerPublichPriendsCircle::send_chatmessage_picture2(std::string pic_file_name)
{
    printf("pic name is %s",pic_file_name.c_str());
    Image_pic_bg_3->setVisible(true);
    Image_pic_bg_2->loadTexture(pic_file_name);
    updateImageName3=pic_file_name;
    ifChooseImage=true;
    ifChooseHandID=false;
    this->setHandIDOrImageChoose();
}

void LayerPublichPriendsCircle::send_chatmessage_picture3(std::string pic_file_name)
{
    printf("pic name is %s",pic_file_name.c_str());
    Image_pic_bg_3->loadTexture(pic_file_name);
    updateImageName4=pic_file_name;
    ifChooseImage=true;
    ifChooseHandID=false;
    this->setHandIDOrImageChoose();
}

void LayerPublichPriendsCircle::publishCircle()//发送请求
{
   
    
    msg::Moment moment;
    msg::Processor_200_PublishMoment processor;
    msg::Request request;
    msg::NormalMomentInfo *nmInfo=processor.mutable_moment()->mutable_momentinfo()->mutable_normalmomentinfo();
    if(updateImageName.compare("Nopic")!=0)
    {
        nmInfo->add_picname(updateImageName);
    }
    if(updateImageName2.compare("Nopic")!=0)
    {
        nmInfo->add_picname(updateImageName2);
    }
    if(updateImageName3.compare("Nopic")!=0)
    {
        nmInfo->add_picname(updateImageName3);
    }
    if(updateImageName4.compare("Nopic")!=0)
    {
        nmInfo->add_picname(updateImageName4);
    }
    if(ifChooseHandID)
    {
        nmInfo->set_handid(chooseHandID);//设置handid
    }
    processor.mutable_moment()->set_displaytype(chooseType);
    processor.mutable_moment()->mutable_momentinfo()->set_momenttype(1);
    if(!commentInfo.empty())
    {
        nmInfo->set_wordcontent(commentInfo);//设置评论
    }
    send_data(processor, REQ_PUBLISH_MOMENT);
}

void LayerPublichPriendsCircle::register_events()
{
    dispatch_.register_event(REQ_PUBLISH_MOMENT,BIND_FUNC(this,LayerPublichPriendsCircle::handle_get_publish_circle_info));
    dispatch_.register_event(EVENT_UPLOAD_FILE_SUCCESS,
                             BIND_FUNC(this, LayerPublichPriendsCircle::handle_upload_event));//上传文件
}

void LayerPublichPriendsCircle::upload_pictures()
{
    this->editBoxEditingDidEnd(ed_property_value_);
    if(updateImageName.compare("Nopic")==0&&(!ifChooseHandID)&&commentInfo.empty())
    {
        TipView::showAlertView(tools::local_string("empty_punishifo","朋友圈信息不能为空"));
        return;
    }
    updateImageArray.clear();//清空上传列表
    updateSuccessImageNum=0;
    if(updateImageName.compare("Nopic")!=0)
    {
        updateImageArray.push_back(updateImageName);
    }
    if(updateImageName2.compare("Nopic")!=0)
    {
        updateImageArray.push_back(updateImageName2);
    }
    if(updateImageName3.compare("Nopic")!=0)
    {
        updateImageArray.push_back(updateImageName3);
    }
    if(updateImageName4.compare("Nopic")!=0)
    {
        updateImageArray.push_back(updateImageName3);
    }
    
    if(updateImageName.compare("Nopic")!=0)
    {
        FM.upload_file(updateImageName);
    }
    if(updateImageName2.compare("Nopic")!=0)
    {
        FM.upload_file(updateImageName2);
    }
    if(updateImageName3.compare("Nopic")!=0)
    {
        FM.upload_file(updateImageName3);
    }
    if(updateImageName4.compare("Nopic")!=0)
    {
        FM.upload_file(updateImageName4);
    }
    
    if(updateImageArray.size()==0)//没有传图片
    {
        this->publishCircle();
    }
}

void LayerPublichPriendsCircle::handle_upload_event(cmd_data_pointer data)//图片上传成功开始请求发布协议
{
    auto s = std::dynamic_pointer_cast<cmd_data_string>(data);
    std::string imgeName ;
    bool ifhasPic=false;
    for(auto athor:updateImageArray)
    {
        if(s->get_value() ==athor)
        {
            ifhasPic=true;
        }
        updateSuccessImageNum++;
    }
    if(!ifhasPic)
    {
        log("图片上传有问题 图片名 %s",s->get_value().c_str());
        return;
    }
    if(updateSuccessImageNum==updateImageArray.size())//全部图片上传成功
    {
        this->publishCircle();//请求协议
    }
}

void LayerPublichPriendsCircle::setChooseNode(const std::shared_ptr<msg::GameRecord>& obj)
{
    chooseNode_->setVisible(true);
    chooseNode_->removeAllChildren();
    int32_t idx = 0;
    for (auto& iter : obj->holecard()) {
        const std::string image_name = PDM->get_card_image(iter);
        Sprite* card = Sprite::create(image_name);
        card->setScale(0.4);
        card->setPosition(Point(card->getContentSize().width * 0.5 * ((idx + 1) * 0.3), chooseNode_->getContentSize().height * 0.5));
        chooseNode_->addChild(card, 1);
        idx++;
    }
    
}


void LayerPublichPriendsCircle::handle_get_publish_circle_info(cmd_data_pointer data)//网络返回
{
    ShowTipMessage(false);
    std::shared_ptr<net_data_recv_package> rec=dynamic_pointer_cast<net_data_recv_package>(data);
    if(!check_request(rec)) return;
    auto processor = PM->response_down(rec);
    switch (processor.status()){
        case msg::Status::SUCCESS:
            Toast::ToastShow(tools::local_string("operate_successed","操作成功"));
            dynamic_cast<LayerFriendList*>(this->getParent())->getOnlineData(false);
            this->removeFromParent();
            break;
        case msg::Status::FAILED:
            Toast::ToastShow(tools::local_string("operate_failed","操作失败"));
            break;
        default:
            break;
    }

}

#pragma mark EditBox delegate
void LayerPublichPriendsCircle::editBoxEditingDidBegin(EditBox* editBox)
{
    
}

void LayerPublichPriendsCircle::editBoxEditingDidEnd(EditBox* editBox)
{
    
}

void LayerPublichPriendsCircle::editBoxTextChanged(EditBox* editBox, const std::string& text)
{
    
}

void LayerPublichPriendsCircle::editBoxReturn(EditBox* editBox)
{
    if (strlen(editBox->getText()) == 0) {
        
    } else {
        std::string key_word = editBox->getText();
        if(key_word.empty()) {
            
        }
        else {
            commentInfo=key_word;
        }
    }
    //editBox->setText("");
}




