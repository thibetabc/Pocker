//
//  GroupShareLayer.cpp
//  pokerclient
//
//  Created by admin on 15-8-26.
//
//

#include "GroupShareLayer.h"
#include "GameDataManager.h"
#include "utils.h"
#include "cmd_def.h"
//#include "GameTools.h"
#include "ActionSheet.h"
#include "TipView.h"
#include "PokerDataManager.h"
#include "FileManager.h"

#define FRIEND_CIRCLE_ID 0
#define RECORD_LAYER_TAG 123

bool GroupShareLayer::init()
{
    if(!BaseLayer::init()) {
        return false;
    }
    
    
    auto size = Director::getInstance()->getWinSize();
    
    m_pLayer = Layer::create();
    this->addChild(m_pLayer,255);
    
    auto root = CSLoader::createNode("LayerUserContent.csb");
    this->addChild(root);
    auto btn_publish = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_confirm"));
    auto btn_cancel = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "btn_back"));
    
    image_check_handcard_bg_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root, "image_check_handcard_bg"));
    image_check_publiccard_bg_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root, "image_check_publiccard_bg"));
    image_check_visible_bg_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root, "image_check_visible_bg"));
    image_check_need_point_bg_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root, "image_check_need_point_bg"));
    image_check_reward_bg_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root, "image_check_reward_bg"));
    
    image_check_handcard_1_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root, "image_check_handcard_1"));
    image_check_publiccard_1_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root, "image_check_publicard_1"));
    image_check_visible_1_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root, "image_check_visible_1"));
    image_check_need_point_1_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root, "image_check_need_point_1"));
    image_check_reward_1_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root, "image_check_reward_1"));
    
    text_check_publicard_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_check_publicard"));
    text_check_visible_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_check_visible"));
    text_check_need_point_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_check_need_point"));
    text_check_reward_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_check_reward"));
    
    text_check_publicard_note_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_check_publicard_note"));
    text_check_visible_note_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_check_visible_note"));
    text_check_need_point_note_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_check_need_point_note"));
    text_check_reward_note_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_check_reward_note"));
    
    sprite_check_handcard_ = dynamic_cast<Sprite*>(CSLoader::seekNodeByName(root, "sprite_check_handcard"));
    
    btn_add_pic = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "btn_add_pic"));
    
    image_check_visible_bg_->setTouchEnabled(true);
    image_check_need_point_bg_->setTouchEnabled(true);
    image_check_reward_bg_->setTouchEnabled(true);
    image_check_handcard_bg_->setTouchEnabled(true);
    
    image_check_handcard_bg_->setTouchEnabled(true);
    image_check_publiccard_bg_->setTouchEnabled(true);
    image_check_visible_bg_->setTouchEnabled(true);
    image_check_need_point_bg_->setTouchEnabled(true);
    image_check_reward_bg_->setTouchEnabled(true);
    
    btn_publish->addClickEventListener(std::bind(&GroupShareLayer::click_btn_publish, this, std::placeholders::_1));
    btn_cancel->addClickEventListener(std::bind(&GroupShareLayer::click_btn_cancel, this, std::placeholders::_1));
    btn_add_pic->addClickEventListener(std::bind(&GroupShareLayer::click_btn_add_pic, this, std::placeholders::_1));
    
    image_check_handcard_bg_->addClickEventListener(std::bind(&GroupShareLayer::click_image_handcard_select, this, std::placeholders::_1));
    image_check_publiccard_bg_->addClickEventListener(std::bind(&GroupShareLayer::click_image_public_card, this, std::placeholders::_1));
    image_check_visible_bg_->addClickEventListener(std::bind(&GroupShareLayer::click_image_visible, this, std::placeholders::_1));
    image_check_need_point_bg_->addClickEventListener(std::bind(&GroupShareLayer::click_image_need_point, this, std::placeholders::_1));
    image_check_reward_bg_->addClickEventListener(std::bind(&GroupShareLayer::click_image_reward, this, std::placeholders::_1));
    
    image_check_need_point_bg_->setVisible(false);
    image_check_reward_bg_->setVisible(false);
    
    auto textfield_reward_points = dynamic_cast<TextField*>(CSLoader::seekNodeByName(root, "TextField_reward_points"));
    
    text_title_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_title"));
    text_title_->setString("手牌分享");
    for (auto& iter : root->getChildren()) {
        log("child'name:%s", iter->getName().c_str());
    }
    
    auto tf_descrip = dynamic_cast<TextField*>(CSLoader::seekNodeByName(root, "tf_descrip"));
    
    auto contentSize = tf_descrip->getContentSize();
    
    
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    tf_descrip_ = GameTools::createMultiEditBoxFormTextField(root, tf_descrip, __FILE__, __LINE__);
    tf_descrip_->setBackgroundColor(Color4B::WHITE);
    tf_descrip_->setPlaceHolder(tools::local_string("decrip_moment", "分享是个好习惯..........").c_str());
    tf_descrip_->setMultLine(true);
    tf_descrip_->setReturnType(MultiEditBox::KeyboardReturnType::DONE);
    tf_descrip_->setContentSize(Size(tf_descrip_->getContentSize().width, tf_descrip_->getContentSize().height * 1));
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    tf_descrip_ = GameTools::createEditBoxFromTextField(root, tf_descrip);
    tf_descrip_->setPlaceHolder(tools::local_string("decrip_moment", "分享是个好习惯..........").c_str());
    tf_descrip_->setReturnType(EditBox::KeyboardReturnType::DONE);
#endif
    
    /*
     //tf_descrip_ = GameTools::createMultiEditBoxFromTextField(root, tf_descrip, "", 1);
     tf_descrip_ = GameTools::createMultiEditBoxFormTextField(root, tf_descrip, __FILE__, __LINE__);
     
     //tf_descrip_->setBackgroundColor(Color4B::WHITE);
     
     tf_descrip_->setDelegate(this);
     tf_descrip_->ignoreContentAdaptWithSize(true);
     tf_descrip_->setInputMode(EditBox::InputMode::ANY);
     //    tf_descrip_->setPlaceHolder("说出你此刻的想法.......");
     tf_descrip_->setReturnType(EditBox::KeyboardReturnType::DONE);
     */
    
    
    //tf_descrip_->setText("");
    
    dispatch_.register_event(EVENT_SEND_SUCCESS_COMMUNITY, BIND_FUNC(this, GroupShareLayer::handle_send_success_community));
    dispatch_.register_event(EVENT_SELECT_HANDCARD, BIND_FUNC(this, GroupShareLayer::handle_select_handcard));
    dispatch_.register_event(EVENT_TODO_WHO_SCENE, BIND_FUNC(this, GroupShareLayer::handle_who_scene));
    dispatch_.register_event(EVENT_TODO_PUBLIC_HD, BIND_FUNC(this, GroupShareLayer::handle_public_hd));
    dispatch_.register_event(EVENT_UPLOAD_FILE_SUCCESS,BIND_FUNC(this, GroupShareLayer::handle_upload_succeed));
    Moment = processor.mutable_moment();
    MomentInfo = Moment->mutable_momentinfo();
    NormalMomentInfo = MomentInfo->mutable_normalmomentinfo();
    
    //默认值
    Moment->set_displaytype(msg::MomentDisplayType::FRIEND);
    MomentInfo->set_momenttype(1);
    
    picname_number = 0;
    
    
    return true;
}

void GroupShareLayer::set_type(const int type)
{
    type_ = type;
    switch (type_) {
        case 1://zongheneirong
        {
            image_check_handcard_bg_->setVisible(false);
            image_check_publiccard_bg_->setVisible(false);
            text_title_->setString(tools::local_string("综合内容", "zongheneirong"));
            break;
        }
        case 2://shoupai
        {
            btn_add_pic->setVisible(false);
            text_title_->setString("手牌分享");
            break;
        }
        default:
            break;
    }
}

void GroupShareLayer::click_btn_publish(Ref* sender){
    GameTools::editBoxLoseInputFocus();//
    bool bFinished = true;
    bool bNoFaild = true;
    for (auto name : NormalMomentInfo->picname())
    {
        auto strVec = FM.m_pUploadedFiles;
        if (std::find(strVec.begin(), strVec.end(), name) == strVec.end())
        {
            bFinished = false;
            break;
        }
    }
    for (auto name : NormalMomentInfo->picname())
    {
        auto strVec = FM.m_pUploadFaidFiles;
        if (std::find(strVec.begin(), strVec.end(), name) != strVec.end())
        {
            bNoFaild = false;
            break;
        }
    }
    //    if (!bNoFaild)
    //    {
    //        TipView::showAlertView("图片上传失败，请重新上传");
    //        return;
    //    }
    //    if (!bFinished)
    //    {
    //        TipView::showAlertView("图片上传未完成，请稍候");
    //        return;
    //    }
    RecordLayer* prl = (RecordLayer*)getChildByTag(RECORD_LAYER_TAG);
    if(prl != NULL)
    {
        prl->click_btn_close(sender);
    }
    if (type_ == 2 ) {
        log("%d",processor.mutable_moment()->momentinfo().normalmomentinfo().handid());
        if (!processor.mutable_moment()->momentinfo().normalmomentinfo().has_handid()) {
            TipView::showAlertView("请选择手牌");
            return;
        }
    }
    else
        processor.mutable_moment()->mutable_momentinfo()->mutable_normalmomentinfo()->clear_handid();
    log("%s",tf_descrip_->getText());
    std::string content = tf_descrip_->getText();
    content.size();
    log("%s, %lu",content.c_str(), content.size());
    if (content.size() <= 0) {
        TipView::showAlertView("请填写内容！");
    }else{
        NormalMomentInfo->set_wordcontent(tf_descrip_->getText());
        GDM->send_publish_momment(processor);
    }
}

void GroupShareLayer::click_btn_cancel(Ref* sender)
{
    removeFromParent();
}
void GroupShareLayer::click_btn_add_pic(Ref* sender){
    
    GameTools::editBoxLoseInputFocus();//
    
    ActionSheet* as = ActionSheet::create();
    as->setIsChat(true);
    
    as->set_after_choose_pic_callback(std::bind(&GroupShareLayer::after_choose_pic, this, std::placeholders::_1));
    BaseLayer* parent1 = static_cast<BaseLayer*>(this->getParent());
    parent1->addChild(as);
}

void GroupShareLayer::after_choose_pic(const std::string& filename)
{
    const std::string pic_names_ = filename;
    FM.upload_file(pic_names_);
    NormalMomentInfo->add_picname(pic_names_);
    
    cocos2d::log("%s\n%s",pic_names_.c_str(),NormalMomentInfo->picname(picname_number++).c_str());
    cocos2d::log("%d\n%d",picname_number,NormalMomentInfo->picname().size());
    
    processor.moment().momentinfo().normalmomentinfo().picname();//::google_public::protobuf::RepeatedPtrField< ::std::string>
    
    ImageView* add_pic = ImageView::create(filename);
    add_pic->setPosition(Vec2(btn_add_pic->getPositionX(), btn_add_pic->getPositionY()));
    Size s(120, 120);
    Size contentSize = add_pic->getContentSize();
    if(contentSize.width > s.width || contentSize.height > s.height) {
        add_pic->ignoreContentAdaptWithSize(false);
        add_pic->setContentSize(s);
    }
    add_pic->setContentSize(s);
    add_pic->setTouchEnabled(true);
    add_pic->addClickEventListener([=](Ref*){
        auto imageview = ImageView::create(filename);
        auto scene = Director::getInstance()->getRunningScene();
        auto& size = Director::getInstance()->getWinSize();
        auto layout = Layout::create();
        layout->setContentSize(size);
        float scale = 1.0f;
        if(imageview->getContentSize().width > size.width) {
            scale = std::min(scale, size.width / imageview->getContentSize().width);
        }
        if(imageview->getContentSize().height > size.height) {
            scale = std::min(scale, size.height / imageview->getContentSize().height);
        }
        imageview->setScale(scale);
        LayerColor* layer = LayerColor::create(Color4B(240, 239, 245, 255), size.width, size.height);
        layout->addChild(layer, -2);
        layout->setTouchEnabled(true);
        layout->addClickEventListener([=](Ref*){
            layout->removeFromParent();
        });
        layout->addChild(imageview, 0);
        imageview->setPosition(Point(size) * 0.5);
        scene->addChild(layout, 10);
    });
    this->addChild(add_pic);
    
    
    LayerColor *black = LayerColor::create(Color4B(100, 100, 100, 100), 120, 120);
    //    black->setAnchorPoint(add_pic->getAnchorPoint());
    black->setPosition(add_pic->getPosition() - Point(black->getContentSize()/2));
    m_pLayer->addChild(black);
    LabelTTF *label = LabelTTF::create("正在上传","Arial", 18);
    label->setColor(cocos2d::Color3B::WHITE);
    label->setPosition(Point(black->getContentSize()/2));
    black->addChild(label);
    
    cocos2d::log("x:%f, y:%f",add_pic->getPositionX(),add_pic->getPositionY());
    if (add_pic->getPositionX() >= 500) {
        btn_add_pic->setPosition(Vec2(95, add_pic->getPositionY() - 150));
        image_check_visible_bg_->setPosition(Vec2(image_check_visible_bg_->getPositionX(), image_check_visible_bg_->getPositionY() - 150));
        image_check_need_point_bg_->setPosition(Vec2(image_check_need_point_bg_->getPositionX(), image_check_need_point_bg_->getPositionY() - 150));
        image_check_reward_bg_->setPosition(Vec2(image_check_reward_bg_->getPositionX(), image_check_reward_bg_->getPositionY() - 150));
    }else{
        btn_add_pic->setPosition(Vec2(add_pic->getPositionX() + 150, add_pic->getPositionY()));
    }
    
    if (btn_add_pic->getPositionY() < 450 && btn_add_pic->getPositionX() > 110) {
        btn_add_pic->setVisible(false);
    }
}

void GroupShareLayer::click_image_handcard_select(Ref* sender)
{
    
    PDM->read_records_fri();
    auto prl = static_cast<RecordLayer*>(this->getChildByTag(RECORD_LAYER_TAG));
    if(prl)
    {
        prl->setVisible(true);
    }
    else
    {
        prl = RecordLayer::create();
    }
    
    
    //prl->setPosition(Vec2(-DESIGN_WIDTH, 250));
    prl->update_data();
    prl->set_isFriHandCard(true);
    this->addChild(prl, 0, RECORD_LAYER_TAG);
    
}
void GroupShareLayer::click_image_public_card(Ref* sender)
{
    /*if((prl != NULL)&&prl->isShow)
     {
     prl->click_btn_close(sender);
     }
     GameTools::editBoxLoseInputFocus();//
     PublicHD* ph = PublicHD::create();
     BaseLayer* parent = static_cast<BaseLayer*>(this->getParent());
     parent->addChild(ph);*/
    
}
void GroupShareLayer::click_image_visible(Ref* sender){//谁能看到
    GameTools::editBoxLoseInputFocus();//
    RecordLayer* prl = (RecordLayer*)getChildByTag(RECORD_LAYER_TAG);
    if(prl != NULL)
    {
        prl->click_btn_close(sender);
    }
    
    SelectWhoScene* as = SelectWhoScene::create();
    BaseLayer* parent1 = static_cast<BaseLayer*>(this->getParent());
    parent1->addChild(as);
}
void GroupShareLayer::click_image_need_point(Ref* sender){
    GameTools::editBoxLoseInputFocus();//
}
void GroupShareLayer::click_image_reward(Ref* sender){
    GameTools::editBoxLoseInputFocus();//
}
void GroupShareLayer::handle_who_scene(cmd_data_pointer data)
{
    auto recv = std::dynamic_pointer_cast<cmd_data_string>(data);
    if (recv) {
        const std::string str_scene = recv->get_value();
        text_check_visible_note_->setString(str_scene);
        
        if (str_scene == "自己") {
            Moment->set_displaytype(msg::MomentDisplayType::MINE);
        }else if (str_scene == "约友")
        {
            Moment->set_displaytype(msg::MomentDisplayType::FRIEND);
        }else if (str_scene == "所有人")
        {
            Moment->set_displaytype(msg::MomentDisplayType::SQUARE);
        }
        
    }
}
void GroupShareLayer::handle_public_hd(cmd_data_pointer data)
{
    auto recv = std::dynamic_pointer_cast<cmd_data_string>(data);
    if (recv) {
        const std::string str_scene = recv->get_value();
        text_check_publicard_note_->setString(str_scene);
    }
}
void GroupShareLayer::handle_select_handcard(cmd_data_pointer data)
{
    GameTools::editBoxLoseInputFocus();//
    RecordLayer* prl = (RecordLayer*)getChildByTag(RECORD_LAYER_TAG);
    NormalMomentInfo->set_handid(prl->get_gamerecord()->handid());
    
    sprite_check_handcard_->removeAllChildren();
    
    int32_t idx = 0;
    for (auto& iter : prl->get_gamerecord()->holecard()) {
        const std::string image_name = PDM->get_card_image(iter);
        Sprite* card = Sprite::create(image_name);
        //        card->setScale(0.4);
        card->setScaleX(0.5);
        card->setScaleY(0.4);
        card->setPosition(Point(card->getContentSize().width * 0.5 * ((idx + 1) * 0.3), sprite_check_handcard_->getContentSize().height * 0.5));
        sprite_check_handcard_->addChild(card, 1);
        idx++;
    }
    
    prl->setVisible(false);
    prl->set_select_handcard(true);
}

void GroupShareLayer::handle_upload_succeed(cmd_data_pointer data)
{
    bool bFinished = true;
    bool bNoFaild = true;
    for (auto name : NormalMomentInfo->picname())
    {
        auto strVec = FM.m_pUploadedFiles;
        if (std::find(strVec.begin(), strVec.end(), name) == strVec.end())
        {
            bFinished = false;
            break;
        }
    }
    for (auto name : NormalMomentInfo->picname())
    {
        auto strVec = FM.m_pUploadFaidFiles;
        if (std::find(strVec.begin(), strVec.end(), name) != strVec.end())
        {
            bNoFaild = false;
            break;
        }
    }
    if (bFinished && bNoFaild)
    {
        m_pLayer->removeAllChildren();
    }
}

#pragma mark EditBox delegate
void GroupShareLayer::editBoxEditingDidBegin(EditBox* editBox)
{
    RecordLayer* prl = (RecordLayer*)getChildByTag(RECORD_LAYER_TAG);
    if((prl != NULL)&&prl->isShow)
    {
        prl->removeFromParent();
    }
}
void GroupShareLayer::editBoxEditingDidEnd(EditBox* editBox)
{}
void GroupShareLayer::editBoxTextChanged(EditBox* editBox, const std::string& text)
{}
void GroupShareLayer::editBoxReturn(EditBox* editBox)
{
    
}


void GroupShareLayer::handle_send_success_community(cmd_data_pointer){
    
    //    auto tip = TipView::showAlertView("发布朋友圈成功");
    //    tip->set_tipview_close_callback([this](){
    //
    //        this->removeFromParentAndCleanup(true);
    //        GDM->send_get_moment(FRIEND_CIRCLE_ID,0,msg::MomentDisplayType::FRIEND);
    //    });
    auto tip = TipView::create();
    tip->update_note("发布朋友圈成功");
    tip->show_cancel(false);
    this->getParent()->addChild(tip);
    tip->set_tipview_close_callback([this](){
        
        this->removeFromParentAndCleanup(true);
        switch(GDM->circletype)
        {
            case GameDataManager::TYPE_SQUARE:
                GDM->send_get_moment(FRIEND_CIRCLE_ID,0,msg::MomentDisplayType::SQUARE);
                break;
            case GameDataManager::TYPE_RECOMMEND:
                GDM->send_get_moment(FRIEND_CIRCLE_ID,0,msg::MomentDisplayType::RECOMMEND);
                break;
            case GameDataManager::TYPE_FRIEND:
                GDM->send_get_moment(FRIEND_CIRCLE_ID,0,msg::MomentDisplayType::FRIEND);
                break;
            case GameDataManager::TYPE_MINE:
                GDM->send_get_moment(FRIEND_CIRCLE_ID,0,msg::MomentDisplayType::MINE);
                break;
            default:
                break;
        }
    });
}
//废弃
void GroupShareLayer::handle_recv_success_community(cmd_data_pointer){
    
    auto tip = TipView::showAlertView("发布朋友圈成功");
    tip->set_tipview_close_callback([this](){
        this->removeFromParent();
    });
}

bool SelectWhoScene::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    auto layerColor = LayerColor::create(Color4B(100, 100, 100, 80));
    this->addChild(layerColor, -1);
    
    auto size_ = Director::getInstance()->getWinSize();
    size_ = Size(std::min(size_.width, size_.height), std::max(size_.width, size_.height));
    
    auto layout_= ui::Layout::create();
    layout_->setContentSize(size_);
    layout_->setTouchEnabled(true);
    layout_->setSwallowTouches(true);
    layout_->setAnchorPoint(Point::ZERO);
    layout_->addClickEventListener([=](Ref* sender){
        SelectWhoScene::click_image_cancel(sender);
    });
    this->addChild(layout_, -9);
    
    origin_friend = Point(0, -189);
    
    root_friend = CSLoader::createNode("LayerFriendIssue.csb");
    root_friend->setPosition(origin_friend);
    this->addChild(root_friend);
    
    
    auto image_issue_event= dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_friend, "image_issue_event"));
    image_issue_event->setVisible(false);
    
    auto text_content= dynamic_cast<Text*>(CSLoader::seekNodeByName(root_friend, "text_content"));
    auto text_cancel = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_friend, "text_cancel"));
    auto text_brand = dynamic_cast<Text*>(CSLoader::seekNodeByName(root_friend, "text_brand"));
    auto text_report= dynamic_cast<Text*>(CSLoader::seekNodeByName(root_friend, "text_report"));
    auto text_issue_event= dynamic_cast<Text*>(CSLoader::seekNodeByName(root_friend, "text_issue_event"));
    
    text_content->setString(tools::local_string("to_self", "仅自己可见"));
    text_brand->setString(tools::local_string("to_all", "所有人可见"));
    text_report->setString(tools::local_string("to_friends", "约友可见"));
    text_issue_event->setVisible(false);
    
    auto image_all= dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_friend, "image_brand"));
    auto image_fri= dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_friend, "image_report"));
    auto image_self= dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_friend, "image_content"));
    auto image_cancel = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_friend, "image_cancel"));
    
    
    image_all->addClickEventListener(std::bind(&SelectWhoScene::click_image_all, this, std::placeholders::_1));
    image_cancel->addClickEventListener(std::bind(&SelectWhoScene::click_image_cancel, this, std::placeholders::_1));
    image_fri->addTouchCallback(std::bind(&SelectWhoScene::click_image_fri, this, std::placeholders::_1));
    image_self->addClickEventListener(std::bind(&SelectWhoScene::click_image_self, this, std::placeholders::_1));
    return true;
}

void SelectWhoScene::click_image_all(Ref* sender)
{
    SelectWhoScene::click_image_cancel(sender);
    cmd_data_pointer data(new cmd_data_string("所有人") );
    events::global_dispatch.raise_event(EVENT_TODO_WHO_SCENE, data);
}
void SelectWhoScene::click_image_fri(Ref* sender)
{
    SelectWhoScene::click_image_cancel(sender);
    cmd_data_pointer data(new cmd_data_string("约友") );
    events::global_dispatch.raise_event(EVENT_TODO_WHO_SCENE, data);
}
void SelectWhoScene::click_image_self(Ref* sender)
{
    SelectWhoScene::click_image_cancel(sender);
    cmd_data_pointer data(new cmd_data_string("自己") );
    events::global_dispatch.raise_event(EVENT_TODO_WHO_SCENE, data);
}
void SelectWhoScene::click_image_cancel(Ref* sender)
{
    MoveTo* move = MoveTo::create(0.2f, origin_friend);
    Sequence* seq = Sequence::create(move, CallFunc::create([this]() {
        this->removeFromParentAndCleanup(true);
    }), NULL);
    
    this->runAction(seq);
}

void SelectWhoScene::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();
    root_friend->runAction(MoveTo::create(0.2f, Point::ZERO));
}

bool PublicHD::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    auto layerColor = LayerColor::create(Color4B(100, 100, 100, 80));
    this->addChild(layerColor, -1);
    
    auto size_ = Director::getInstance()->getWinSize();
    size_ = Size(std::min(size_.width, size_.height), std::max(size_.width, size_.height));
    
    auto layout_= ui::Layout::create();
    layout_->setContentSize(size_);
    layout_->setTouchEnabled(true);
    layout_->setSwallowTouches(true);
    layout_->setAnchorPoint(Point::ZERO);
    layout_->addClickEventListener([=](Ref* sender){
        PublicHD::click_image_cancel(sender);
    });
    this->addChild(layout_, -9);
    
    origin_friend = Point(0, -189);
    
    root_friend = CSLoader::createNode("LayerPublicHD.csb");
    root_friend->setPosition(origin_friend);
    this->addChild(root_friend);
    
    auto image_yes= dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_friend, "image_yes"));
    auto image_no= dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_friend, "image_no"));
    auto image_cancel = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root_friend, "image_cancel"));
    
    
    image_yes->addClickEventListener(std::bind(&PublicHD::click_image_yes, this, std::placeholders::_1));
    image_cancel->addClickEventListener(std::bind(&PublicHD::click_image_cancel, this, std::placeholders::_1));
    image_no->addTouchCallback(std::bind(&PublicHD::click_image_no, this, std::placeholders::_1));
    
    
    
    return true;
}

void PublicHD::click_image_yes(Ref* sender)
{
    PublicHD::click_image_cancel(sender);
    cmd_data_pointer data(new cmd_data_string("公开") );
    events::global_dispatch.raise_event(EVENT_TODO_PUBLIC_HD, data);
}
void PublicHD::click_image_no(Ref* sender)
{
    PublicHD::click_image_cancel(sender);
    cmd_data_pointer data(new cmd_data_string("不公开") );
    events::global_dispatch.raise_event(EVENT_TODO_PUBLIC_HD, data);
}
void PublicHD::click_image_cancel(Ref* sender)
{
    MoveTo* move = MoveTo::create(0.2f, origin_friend);
    Sequence* seq = Sequence::create(move, CallFunc::create([this]() {
        this->removeFromParentAndCleanup(true);
    }), NULL);
    
    this->runAction(seq);
}

void PublicHD::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();
    root_friend->runAction(MoveTo::create(0.2f, Point::ZERO));
}
