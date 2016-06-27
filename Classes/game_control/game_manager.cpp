//
//  game_manager.cpp
//  pokerclient
//
//  Created by MirrorMiko on 15/5/3.
//
//
#include "FileManager.h"
#include "game_manager.h"
#include "threads.h"
#include "events.h"
#include "net_manager.h"
#include "cocos2d.h"

void game_manager::init(){
    _managers.push_back(&threads::thread_manager::shared_instence());
    _managers.push_back(&events::event_manager::shared_instence());
    _managers.push_back(&net_manager::shared_instence());
    _managers.push_back(&FileManager::shared_instence());

    for (auto iter = _managers.begin(); iter != _managers.end(); iter++) {
        (*iter)->init();
    }
    cocos2d::Director::getInstance()->getScheduler()->schedule(std::bind(&game_manager::coco_update, this, std::placeholders::_1), this, 0, kRepeatForever, 0, false, "handle_data");
}

void game_manager::update(){
    for (auto iter = _managers.begin(); iter != _managers.end(); iter++) {
        (*iter)->update();
    }
}

void game_manager::coco_update(float dt){
    update();
}