//
//  LayerEditName.cpp
//  pokerclient
//
//  群信息编辑
//  用户个人信息编辑也会导入到这里
//  Created by duanmt on 15/5/14.
//
//

#include "LayerEditName.h"
#include "GameTools.h"
#include "utils.h"
#include "GameDataManager.h"
#include "TipView.h"

LayerEditName* LayerEditName::create(const int type)
{
    LayerEditName* layer = new LayerEditName();
    layer->init(type);
    layer->autorelease();
    return layer;
}

bool LayerEditName::init(const int type)
{
    if(!BaseLayer::init()) {
        return false;
    }
    
    tv_note_ = nullptr;
    tf_name_ = nullptr;
    tf_name_2 = nullptr;
    
    edit_type_ = static_cast<LAYER_EDIT_TYPE>(type);
    
    auto root = CSLoader::createNode("LayerEditName.csb");
    this->addChild(root);
    
    auto text_cancel = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "btn_back"));
    auto text_save = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_save"));
    text_title_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_title"));
    image_bg_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root, "image_bg"));
    
    sprite_male_select_ = dynamic_cast<Sprite*>(CSLoader::seekNodeByName(root, "sprite_male_select"));
    sprite_female_select_ = dynamic_cast<Sprite*>(CSLoader::seekNodeByName(root, "sprite_female_select"));
    text_male_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_male"));
    text_female_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_female"));
    image_bg_bottom_ = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root, "image_bg_bottom"));
    
    image_bg_->addClickEventListener(std::bind(&LayerEditName::click_image_male, this, std::placeholders::_1));
    image_bg_bottom_->addClickEventListener(std::bind(&LayerEditName::click_image_female, this, std::placeholders::_1));
    
    sprite_male_select_->setVisible(false);
    sprite_female_select_->setVisible(false);
    text_male_->setVisible(false);
    text_female_->setVisible(false);
    //    image_bg_bottom_->setVisible(false);
    
    
    //text_cancel->setString(tools::local_string("cancel", "取消"));
    text_save->setString(tools::local_string("save", "保存"));
    
    text_cancel->addClickEventListener(std::bind(&LayerEditName::text_cancel_click, this, std::placeholders::_1));
    text_save->addClickEventListener(std::bind(&LayerEditName::text_save_click, this, std::placeholders::_1));
    
    auto tf_name = dynamic_cast<TextField*>(CSLoader::seekNodeByName(root, "tf_name"));
    auto tf_name2 = dynamic_cast<TextField*>(CSLoader::seekNodeByName(root, "tf_name_2"));
    
    auto init_edit_box = [=]() {
        tf_name_ = GameTools::createEditBoxFromTextField(this, tf_name);
        tf_name_->setReturnType(EditBox::KeyboardReturnType::DONE);
        //
        tf_name_2 = GameTools::createEditBoxFromTextField(this, tf_name2);
        tf_name_2->setReturnType(EditBox::KeyboardReturnType::DONE);
        
        tf_name_->setMaxLength(tf_name_->getContentSize().width);
        tf_name_->setFontName("Thonburi");
        tf_name_->setOpacity(0);
        //
        tf_name_2->setMaxLength(tf_name_2->getContentSize().width);
        tf_name_2->setFontName("Thonburi");
        tf_name_2->setOpacity(0);
    };
    
    auto init_multi_line_edit_box = [=]() {
        tv_note_ = GameTools::createEditBoxFromTextField(root, tf_name);
        tv_note_->setMaxLength(30);
/*#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
        auto contentSize = tf_name->getContentSize();
        
//        tv_note_->setBackgroundColor(Color4B::WHITE);
        tv_note_->setReturnType(EditBox::KeyboardReturnType::DONE);
        LayerColor* lc = LayerColor::create(Color4B::WHITE, contentSize.width, contentSize.height * 3);
        lc->setAnchorPoint(Point::ZERO);
        tv_note_->addChild(lc);
        image_bg_->setContentSize(Size(image_bg_->getContentSize().width, image_bg_->getContentSize().height * 3));
        tv_note_->setContentSize(Size(tv_note_->getContentSize().width, tv_note_->getContentSize().height * 3));
 
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID*/
        image_bg_->setContentSize(Size(image_bg_->getContentSize().width, image_bg_->getContentSize().height * 4));
        tv_note_->setContentSize(Size(tv_note_->getContentSize().width, tv_note_->getContentSize().height * 4));
//#endif
    };
    
    bool editable = true;
    switch (edit_type_) {
        case EDIT_USER_NAME:
            init_edit_box();
            text_title_->setString(tools::local_string("name", "名字"));
            tf_name_2->setVisible(false);
            image_bg_bottom_->setVisible(false);
            tf_name_->setInputMode(cocos2d::ui::EditBox::InputMode::ANY);
            break;
        case EDIT_USER_PASSWORD:
            init_edit_box();
            text_title_->setString(tools::local_string("password", "密码"));
            tf_name_->setPlaceHolder("密码：");
            tf_name_2->setPlaceHolder("密码确认：");
            
            tf_name_->setInputFlag(cocos2d::ui::EditBox::InputFlag::PASSWORD);
            tf_name_->setInputMode(EditBox::InputMode::SINGLE_LINE);
            tf_name_2->setInputFlag(cocos2d::ui::EditBox::InputFlag::PASSWORD);
            tf_name_2->setInputMode(EditBox::InputMode::SINGLE_LINE);
            break;
        case EDIT_USER_SEX: {
            text_save->removeFromParent();
            tf_name->removeFromParent();
            tf_name2->removeFromParent();
            text_title_->setString(tools::local_string("sex", "性别"));
            sprite_male_select_->setVisible(userdetailinfo_.sex() == msg::Sex::MALE);
            sprite_female_select_->setVisible(userdetailinfo_.sex() == msg::Sex::FEMALE);
            text_male_->setVisible(true);
            text_female_->setVisible(true);
            image_bg_bottom_->setVisible(true);
            break;
        }
        case EDIT_GROUP_NAME:
            editable = group_data_->groupownerid() == userdetailinfo_.userid();
            init_edit_box();
            tf_name_->setEnabled(editable);
            text_title_->setString(tools::local_string("group_name", "名字"));
            tf_name_2->setVisible(false);
            image_bg_bottom_->setVisible(false);
            break;
        case EDIT_GROUP_NOTE:
            tf_name2->removeFromParent();
            editable = group_data_->groupownerid() == userdetailinfo_.userid();
            init_multi_line_edit_box();
//            tv_note_->setEnabled(editable);
            text_title_->setString(tools::local_string("group_note", "群备注"));
            image_bg_bottom_->setVisible(false);
            break;
        case EDIT_GROUP_PASSWORD:
            editable = group_data_->groupownerid() == userdetailinfo_.userid();
            init_edit_box();
            tf_name_->setEnabled(editable);
            text_title_->setString(tools::local_string("group_password", "群密码"));
            tf_name_->setInputFlag(cocos2d::ui::EditBox::InputFlag::PASSWORD);
            tf_name_2->setVisible(false);
            image_bg_bottom_->setVisible(false);
            break;
        case EDIT_OTHER:
            init_edit_box();
            tf_name_->setEnabled(true);
            tf_name_2->setVisible(false);
            image_bg_bottom_->setVisible(false);
            break;
        default:
            break;
    }
    
    
    return true;
}

const bool LayerEditName:: GetValueConfirmed(){
    return ValueConfirmed;
}
void LayerEditName::text_cancel_click(Ref* sender)
{
    removeFromParent();
}
void LayerEditName::text_save_click(Ref* sender)
{
    std::string text;
    if(tf_name_)
        text = tf_name_->getText();
    else if(tv_note_) {
        text = tv_note_->getText();
    }
    else {
        assert(0);
    }
    if(text.empty()) {
        switch (edit_type_) {
            case EDIT_USER_NAME:
                TipView::showAlertView(tools::local_string("nickname_empty", "昵称不能为空"));
                return;
            case EDIT_USER_PASSWORD:
                TipView::showAlertView(tools::local_string("password_empty", "密码不能为空"));
                return;
            case EDIT_GROUP_NAME:
                TipView::showAlertView(tools::local_string("groupname_empty", "群名称不能为空！"));
                return;
            case EDIT_GROUP_NOTE:
                break;
            case EDIT_GROUP_PASSWORD:
                break;
            default:
                break;
        }
    }
    if (edit_type_ == EDIT_USER_PASSWORD) {
        password1 = tf_name_->getText();
        password2 = tf_name_2->getText();
    }
    
    switch (edit_type_) {
        case EDIT_USER_NAME:
            
            //GDM->change_person_name(text);
            break;
        case EDIT_USER_PASSWORD:
            if(password1 != password2) {
                TipView::showAlertView(tools::local_string("passowrd_not_same", "密码不一致，请重新输入！"));
                return;
            }
            //GDM->change_person_password(text);
            break;
        case EDIT_USER_SEX:
            //            GDM->change_person_sex(<#const uint8_t sex#>)
            break;
        case EDIT_GROUP_NAME:
            //GDM->change_group_name(group_data_->groupid(), text);
            break;
        case EDIT_GROUP_NOTE:
            //GDM->change_group_note(group_data_->groupid(), text);
            break;
        case EDIT_GROUP_PASSWORD:
            //GDM->change_group_password(group_data_->groupid(), text);
            break;
        default:
            break;
    }
    if (tf_name_) {
        tf_name_->setVisible(false);
    }
    ValueConfirmed = true;
    this->removeFromParent();
}

void LayerEditName::click_image_male(Ref* sender)
{
    if(edit_type_ != EDIT_USER_SEX)
        return;
    sprite_male_select_->setVisible(true);
    sprite_female_select_->setVisible(false);
    //GDM->change_person_sex(msg::Sex::MALE);
    if(tf_name_)
        tf_name_->setVisible(false);
    this->removeFromParent();
}
void LayerEditName::click_image_female(Ref* sender)
{
    if(edit_type_ != EDIT_USER_SEX)
        return;
    sprite_male_select_->setVisible(false);
    sprite_female_select_->setVisible(true);
    //GDM->change_person_sex(msg::Sex::FEMALE);
    if(tf_name_)
        tf_name_->setVisible(false);
    this->removeFromParent();
}

void LayerEditName::set_title(const std::string& title)
{
    text_title_->setString(title);
}
void LayerEditName::set_defalut_value(const std::string& value)
{
    tf_name_->setText(value.c_str());
}
void LayerEditName::set_number_keyboard()
{
    tf_name_->setInputMode(cocos2d::ui::EditBox::InputMode::NUMERIC);
}
const std::string LayerEditName::get_value()
{
    std::string str = tf_name_->getText();
    if(str.empty()) {
        str = "0";
    }
    return str;
}

void LayerEditName::onExit()
{
    if(tf_name_)
        tf_name_->setVisible(false);
    LayerPop::onExit();
}

#pragma mark EditBox delegate
//void LayerEditName::editBoxEditingDidBegin(EditBox* editBox)
//{
//    cocos2d::log("editBoxEditingDidBegin");
//}
//void LayerEditName::editBoxEditingDidEnd(EditBox* editBox)
//{
//    cocos2d::log("editBoxEditingDidEnd");
//}
//void LayerEditName::editBoxTextChanged(EditBox* editBox, const std::string& text)
//{
//    cocos2d::log("editBoxTextChanged:%s", text.c_str());
//}
//void LayerEditName::editBoxReturn(EditBox* editBox)
//{
//    cocos2d::log("editBoxReturn");
//}