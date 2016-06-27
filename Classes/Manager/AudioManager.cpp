//
//  AudioManager.cpp
//  pokerclient
//
//  Created by zhongyu on 16/3/13.
//
//

#include "AudioManager.h"

AudioManager* AudioManager::__audio_manager__ = nullptr;

AudioManager::AudioManager()
{
    setBackgroundMusicVolume(UserDefault::getInstance()->getFloatForKey("music_volume", 0.5));
    setEffectsVolume(UserDefault::getInstance()->getFloatForKey("effects_volume", 0.5));
}

void AudioManager::play_game_voice(const std::string file_path)
{
    list_game_voice_.push_back(file_path);
    if (!is_recording && current_voice_id == 0)
        play_next_game_voice();
}

void AudioManager::play_next_game_voice()
{
    if (list_game_voice_.size() == 0)
    {
        return;
    }else
    {
        std::string game_voice_file_path = list_game_voice_.front();
        list_game_voice_.pop_front();
        if (current_voice_id != 0)
            cocos2d::experimental::AudioEngine::stop(current_voice_id);
        current_voice_id = cocos2d::experimental::AudioEngine::play2d(game_voice_file_path);
        cocos2d::experimental::AudioEngine::setVolume(current_voice_id, BackgroundMusicVolume_);
        cocos2d::experimental::AudioEngine::setFinishCallback(current_voice_id,
            [&](int, const std::string &){
                current_voice_id = 0;
                if (!is_recording)
                    play_next_game_voice();
        });
    }
}

void AudioManager::stopAll()
{
    cocos2d::experimental::AudioEngine::stopAll();
    list_game_voice_.clear();
    is_recording = false;
    current_voice_id = 0;
}

void AudioManager::begin_recording()
{
    is_recording = true;
    cocos2d::experimental::AudioEngine::stopAll();
    current_voice_id = 0;
}

void AudioManager::end_recording()
{
    is_recording = false;
    play_next_game_voice();
}

void AudioManager::playEffect(const std::string file_path, bool isloop)
{
    if (is_recording) return;
    int current_game_voice_id_ = cocos2d::experimental::AudioEngine::play2d(file_path);
    cocos2d::experimental::AudioEngine::setVolume(current_game_voice_id_, EffectVolume_);
}

void AudioManager::playBackgroundMusic(const std::string file_path, bool isloop)
{
    if (is_recording) return;
    if (current_voice_id != 0)
        cocos2d::experimental::AudioEngine::stop(current_voice_id);
    current_voice_id = cocos2d::experimental::AudioEngine::play2d(file_path);
    cocos2d::experimental::AudioEngine::setVolume(current_voice_id, BackgroundMusicVolume_);
}

float AudioManager::getEffectsVolume()
{
    return EffectVolume_;
}

void AudioManager::setEffectsVolume(float volume)
{
    EffectVolume_ = volume;
    
    UserDefault::getInstance()->setFloatForKey("music_volume", EffectVolume_);
    
    UserDefault::getInstance()->flush();
}

float AudioManager::getBackgroundMusicVolume()
{
    return BackgroundMusicVolume_;
}

void AudioManager::setBackgroundMusicVolume(float volume)
{
    BackgroundMusicVolume_ = volume;
    
    UserDefault::getInstance()->setFloatForKey("effects_volume", BackgroundMusicVolume_);
    
    UserDefault::getInstance()->flush();
}