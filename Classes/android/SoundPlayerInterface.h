#pragma once

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include <string>

class SoundPlayerInterface {
public:
    
    static void play_sound(const std::string& path);
    static void stop_play();
    static bool is_playing();
    
};

#endif