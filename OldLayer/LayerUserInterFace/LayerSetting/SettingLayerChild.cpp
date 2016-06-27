#include "SettingLayerChild.h"
#include "utils.h"
#include "LanguageLayer.h"
#include "cmd_def.h"
#include "SettingLayer.h"
#include "TipView.h"
#include "AudioManager.h"

bool SettingLayerChild::init()
{
    if(!BaseLayer::init()) {
        return false;
    }
    
    set_show_dir(RIGHT_TO_LEFT);

    auto root = CSLoader::createNode("LayerSettingChild.csb");
    this->addChild(root);
    
    
    text_title_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_title"));
    text_title_->setString(tools::local_string("setting", "设置"));
    
    btn_back_ = dynamic_cast<Button*>(CSLoader::seekNodeByName(root, "btn_back"));
    btn_back_->setTitleText("    " + tools::local_string("back", "返回"));
    btn_back_->addClickEventListener(std::bind(&SettingLayerChild::click_btn_back, this, std::placeholders::_1));
    
    text_language_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_language"));
    text_language_->setString(tools::local_string("language", "多语言"));
    
    auto image_language = dynamic_cast<ImageView*>(CSLoader::seekNodeByName(root, "image_language"));
    image_language->addClickEventListener(std::bind(&SettingLayerChild::click_image_language, this, std::placeholders::_1));
    
    checkbox_newcard_ = dynamic_cast<CheckBox*>(CSLoader::seekNodeByName(root, "checkbox_card_suit"));
    if(GDM->get_is_newcard()){
        checkbox_newcard_->setSelected(true);
    }
    else{
        checkbox_newcard_->setSelected(false);
    }
    checkbox_newcard_->addEventListener([=](Ref* sender, CheckBox::EventType type){
        switch(type)
        {
            case CheckBox::EventType::SELECTED:
                GDM->set_is_newcard(true);
                break;
            case CheckBox::EventType::UNSELECTED:
                GDM->set_is_newcard(false);
                break;
            default:
                break;
        }
    });
    
    text_music_volume_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_music_volume"));
    text_music_volume_->setString(tools::local_string("music_volume", "音乐"));
    slider_music_volume_ = dynamic_cast<Slider*>(CSLoader::seekNodeByName(root, "slider_music_volume"));
    slider_music_volume_->setPercent(AM->getBackgroundMusicVolume() * 100.0);
    slider_music_volume_->addEventListener(std::bind(&SettingLayerChild::slider_slider_music_volume, this, std::placeholders::_1, std::placeholders::_2));
    
    text_effects_volume_ = dynamic_cast<Text*>(CSLoader::seekNodeByName(root, "text_effects_volume"));
    text_effects_volume_->setString(tools::local_string("effects_volume", "音效"));
    slider_effects_volume_ = dynamic_cast<Slider*>(CSLoader::seekNodeByName(root, "slider_effects_volume"));
    slider_effects_volume_->setPercent(AM->getEffectsVolume() * 100.0);
    slider_effects_volume_->addEventListener(std::bind(&SettingLayerChild::slider_slider_effects_volume, this, std::placeholders::_1, std::placeholders::_2));
    
    dispatch_.register_event(EVENT_CHANGE_LANGUAGE,
                             BIND_FUNC(this, SettingLayerChild::update_ui));
    
    return true;
}

void SettingLayerChild::click_btn_back(Ref* sender)
{
    removeFromParent();
}

void SettingLayerChild::click_image_language(Ref* sender)
{
    TipView::showAlertView("此功能暂未开通");
    //LanguageLayer* ll = LanguageLayer::create();
    //this->addChild(ll);
}

void SettingLayerChild::slider_slider_music_volume(Ref * sender, Slider::EventType)
{
    float music_volume = slider_music_volume_->getPercent() / 100.0;
    
    cocos2d::log("SettingLayerChild::slider_slider_music_volume %f", music_volume);
    
    AM->setBackgroundMusicVolume(music_volume);
}

void SettingLayerChild::slider_slider_effects_volume(Ref * sender, Slider::EventType)
{
    float effects_volume = slider_effects_volume_->getPercent() / 100.0;
    
    cocos2d::log("SettingLayerChild::slider_slider_effects_volume %f", effects_volume);
    
    AM->setEffectsVolume(effects_volume);
    
}

void SettingLayerChild::update_ui(cmd_data_pointer)
{
    text_title_->setString(tools::local_string("setting", "设置"));
    btn_back_->setTitleText("    " + tools::local_string("back", "返回"));
    text_language_->setString(tools::local_string("language", "多语言"));
}