//
//  AudioManager.h
//  pokerclient
//
//  Created by lvzhangle on 16/3/13.
//
//

#ifndef __pokerclient__AudioManager__
#define __pokerclient__AudioManager__

#include "cocos2d.h"
#include "AudioEngine.h"
//#include <string>
//#include <list>

#define AM AudioManager::getInstance()

using namespace cocos2d;
using namespace experimental;

class AudioManager
{
public:
    static AudioManager* getInstance()
    {
        if (!__audio_manager__)
        {
            __audio_manager__ = new AudioManager();
        }
        
        return __audio_manager__;
    }
    
    static void destroy()
    {
        delete __audio_manager__;
        __audio_manager__ = nullptr;
    }
    
public:
    void play_game_voice(const std::string file_path);
    void playEffect(const std::string file_path, bool isloop = false);
    void playBackgroundMusic(const std::string file_path, bool isloop = false);
    
    float getEffectsVolume();
    void setEffectsVolume(float volume);
    float getBackgroundMusicVolume();
    void setBackgroundMusicVolume(float volume);
    void stopAll();
    void begin_recording();
    void end_recording();
private:
    std::list<std::string> list_game_voice_;
    float BackgroundMusicVolume_;
    float EffectVolume_;
    void play_next_game_voice();
    int current_voice_id = 0;
    bool is_recording = false;
    
private:
    static AudioManager * __audio_manager__;
    
    AudioManager();
    AudioManager(const AudioManager&);
    AudioManager operator=(const AudioManager&);
};

#endif /* AudioManager_h */
