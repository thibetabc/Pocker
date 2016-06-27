#ifndef __pokerclient__SettingLayerChild__
#define __pokerclient__SettingLayerChild__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "BaseLayer.h"
#include "events.h"
#include "LayerManager.h"
#include "GameDataManager.h"

using namespace cocos2d;
using namespace cocos2d::ui;

class SettingLayerChild : public BaseLayer {
public:
    CREATE_FUNC(SettingLayerChild);
    virtual bool init();
    
    void click_btn_back(Ref* sender);
    void click_image_language(Ref* sender);

    void slider_slider_music_volume(Ref * sender, Slider::EventType);
    void slider_slider_effects_volume(Ref * sender, Slider::EventType);
    
    void update_ui(cmd_data_pointer);
private:
    events::event_dispatch dispatch_;
    bool isEnglish;
    Text* text_title_;
    Text* text_language_;
    CheckBox* checkbox_newcard_;
    
    Text* text_music_volume_;
    Slider* slider_music_volume_;
    
    Text* text_effects_volume_;
    Slider* slider_effects_volume_;
    
    Button* btn_back_;
};

#endif /* defined(__pokerclient__SettingLayerChild__) */
